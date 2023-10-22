// Copyright (C) 2023 Vincent Chambrin
// This file is part of the 'ocv-playground' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "drawingsurface.h"

#include <QMouseEvent>

#include <QBrush>
#include <QPainter>
#include <QPen>

#include <QVector2D>

#include <algorithm>
#include <iterator>
#include <utility>

DrawingSurface::DrawingSurface(QWidget* parent)
  : QWidget(parent)
{
    setMouseTracking(true); // we want to receive mouse "hover" events
}

DrawingSurface::~DrawingSurface()
{
}

const QImage& DrawingSurface::backgroundImage() const
{
    return m_background_image;
}

void DrawingSurface::setBackgroungImage(const QImage& img)
{
    m_background_image = img;

    if (!img.isNull())
    {
        setFixedSize(img.size());
    }

    update();
}

int DrawingSurface::nbControlPoints() const
{
    return static_cast<int>(m_controlpoints.size());
}

QPoint DrawingSurface::controlPointPosition(int index) const
{
    return m_controlpoints.at(index)->pos;
}

void DrawingSurface::setControlPointPosition(int index, const QPoint& pos)
{
    dragControlPoint(m_controlpoints[index].get(), pos);
}

std::vector<QPoint> DrawingSurface::controlPoints() const
{
    std::vector<QPoint> result;
    result.reserve(m_controlpoints.size());

    using ControlPointPtr = std::unique_ptr<ControlPoint>;

    std::transform(m_controlpoints.begin(),
                   m_controlpoints.end(),
                   std::back_inserter(result),
                   [](const ControlPointPtr& cp) { return cp->pos; });

    return result;
}

QImage DrawingSurface::pictureWithContour() const
{
    QImage img = backgroundImage().copy();

    {
        QPainter painter{ &img };
        drawContour(painter);
    }

    return img;
}

void DrawingSurface::enterEvent(QEvent* ev)
{
    QWidget::enterEvent(ev);
    m_under_mouse = true;
    update();
}

void DrawingSurface::leaveEvent(QEvent* ev)
{
    QWidget::leaveEvent(ev);
    m_under_mouse = false;
    update();
}

void DrawingSurface::mousePressEvent(QMouseEvent* ev)
{
    updateControlPointsUnderMouseState(ev->pos());

    // Find if any control point is under the mouse
    using ControlPointPtr = std::unique_ptr<ControlPoint>;
    auto it = std::find_if(m_controlpoints.begin(),
                           m_controlpoints.end(),
                           [](const ControlPointPtr& cp) { return cp->under_mouse; });

    if (it != m_controlpoints.end())
    {
        // Start a "drag" operation
        ControlPointDragOperation op;
        op.controlpoint = it->get();
        m_drag_operation = std::make_unique<ControlPointDragOperation>(op);
    }
    else
    {
        // Start a "create" operation
        if (m_controlpoints.size() < 4)
        {
            ControlPointCreateOperation op;
            op.press_pos = ev->pos();
            m_create_operation = std::make_unique<ControlPointCreateOperation>(op);
        }
    }
}

void DrawingSurface::mouseMoveEvent(QMouseEvent* ev)
{
    if (m_create_operation)
    {
        // no-op
    }
    else if (m_drag_operation)
    {
        dragControlPoint(m_drag_operation->controlpoint, ev->pos());
    }
    else
    {
        if (ev->button() == Qt::NoButton)
        {
            updateControlPointsUnderMouseState(ev->pos());
        }
    }
}

void DrawingSurface::mouseReleaseEvent(QMouseEvent* ev)
{
    if (m_create_operation)
    {
        constexpr int jitter_threshold = 6;

        if ((ev->pos() - m_create_operation->press_pos).manhattanLength() <= jitter_threshold)
        {
            ControlPoint cp;
            cp.pos = ev->pos();

            m_controlpoints.push_back(std::make_unique<ControlPoint>(cp));

            Q_EMIT controlPointCreated();
            Q_EMIT controlPointsModified();

            update();
        }

        m_create_operation.reset();
    }
    else if (m_drag_operation)
    {
        dragControlPoint(m_drag_operation->controlpoint, ev->pos());
        m_drag_operation.reset();
    }
}

void DrawingSurface::paintEvent(QPaintEvent* /* ev */)
{
    QPainter painter{ this };

    painter.drawImage(0, 0, backgroundImage());

    if (!m_controlpoints.empty())
    {
        drawContour(painter, m_under_mouse);
    }
}

void DrawingSurface::updateControlPointsUnderMouseState(const QPoint& mousePos)
{
    for (auto& cp : m_controlpoints)
    {
        bool under_mouse = QVector2D(cp->pos - mousePos).length() <= cp->radius;

        if (std::exchange(cp->under_mouse, under_mouse) != under_mouse)
        {
            // schedule a redraw if the state has changed
            update();
        }
    }
}

void DrawingSurface::dragControlPoint(ControlPoint* cp, const QPoint& mousePos)
{
    if (cp->pos != mousePos)
    {
        cp->pos = mousePos;
        Q_EMIT controlPointsModified();
        update();
    }
}

void DrawingSurface::drawContour(QPainter& painter, bool drawControlPoints) const
{
    painter.save();

    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen{ Qt::red };
    pen.setWidth(6);
    painter.setPen(pen);

    for (size_t i(0); i < m_controlpoints.size(); ++i)
    {
        const ControlPoint& first = *m_controlpoints.at(i);
        const ControlPoint& second = *m_controlpoints.at((i + 1) % m_controlpoints.size());

        painter.drawLine(first.pos, second.pos);
    }

    if (drawControlPoints)
    {
        painter.setPen(QPen());

        for (auto& cp : m_controlpoints)
        {
            painter.setBrush(QBrush(cp->under_mouse ? Qt::cyan : Qt::blue));
            painter.drawEllipse(cp->pos, cp->radius, cp->radius);
        }
    }

    painter.restore();
}
