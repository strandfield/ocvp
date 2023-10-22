// Copyright (C) 2023 Vincent Chambrin
// This file is part of the 'ocv-playground' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef DRAWINGSURFACE_H
#define DRAWINGSURFACE_H

#include <QWidget>

#include <QImage>

#include <memory>
#include <vector>

/**
 * @brief a widget in which the user can draw the contour of an object
 */
class DrawingSurface : public QWidget
{
    Q_OBJECT
public:
    explicit DrawingSurface(QWidget* parent = nullptr);
    ~DrawingSurface();

    const QImage& backgroundImage() const;
    void setBackgroungImage(const QImage& img);

    int nbControlPoints() const;
    QPoint controlPointPosition(int index) const;
    void setControlPointPosition(int index, const QPoint& pos);
    std::vector<QPoint> controlPoints() const;

    QImage pictureWithContour() const;

Q_SIGNALS:
    /**
     * @brief this signal is emitted when a control point has been created
     */
    void controlPointCreated();

    /**
     * @brief this signal is emitted when a control point has been created or edited
     */
    void controlPointsModified();

protected:
    void enterEvent(QEvent* ev) override;
    void leaveEvent(QEvent* ev) override;

    void mousePressEvent(QMouseEvent* ev) override;
    void mouseMoveEvent(QMouseEvent* ev) override;
    void mouseReleaseEvent(QMouseEvent* ev) override;

    void paintEvent(QPaintEvent* ev) override;

private:
    /**
     * @brief a point of the contour
     */
    struct ControlPoint
    {
        QPoint pos; ///< 2D position of the point (in pixels)
        int radius = 8; ///< radius of the circle used to visualize the point
        bool under_mouse = false; ///< whether the circle is under the mouse
    };

    struct ControlPointCreateOperation
    {
        QPoint press_pos; ///< position of mouse cursor when left button was pressed
    };

    struct ControlPointDragOperation
    {
        ControlPoint* controlpoint = nullptr;
    };

private:
    void updateControlPointsUnderMouseState(const QPoint& mousePos);
    void dragControlPoint(ControlPoint* cp, const QPoint& mousePos);
    void drawContour(QPainter& painter, bool drawControlPoints = false) const;

private:
    QImage m_background_image;
    bool m_under_mouse = false; ///< whether the mouse cursor is over the widget
    std::vector<std::unique_ptr<ControlPoint>> m_controlpoints;

    // In C++17, using std::optional might be more adequate than unique_ptr,
    // or std::variant<std::monostate, ControlPointCreateOperation, ControlPointDragOperation>
    std::unique_ptr<ControlPointCreateOperation> m_create_operation;
    std::unique_ptr<ControlPointDragOperation> m_drag_operation;
};

#endif // DRAWINGSURFACE_H
