// Copyright (C) 2023 Vincent Chambrin
// This file is part of the 'ocv-playground' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "controlpointsgroupbox.h"

#include <QVBoxLayout>

ControlPointsGroupBox::ControlPointsGroupBox(QWidget* parent)
  : QGroupBox("Control points", parent)
{
    m_list_widget = new QListWidget;

    auto* l = new QVBoxLayout;
    l->addWidget(m_list_widget);

    setLayout(l);
}

ControlPointsGroupBox::~ControlPointsGroupBox()
{
}

void ControlPointsGroupBox::setControlPoints(const std::vector<QPoint>& positions)
{
    if (m_list_widget->count() > positions.size())
    {
        m_list_widget->clear();
    }

    while (m_list_widget->count() < positions.size())
    {
        m_list_widget->addItem(QString());
    }

    for (int i(0); i < m_list_widget->count(); ++i)
    {
        const QPoint& pos = positions.at(i);

        m_list_widget->item(i)->setText(
          QString("(%1, %2)").arg(QString::number(pos.x()), QString::number(pos.y())));
    }
}
