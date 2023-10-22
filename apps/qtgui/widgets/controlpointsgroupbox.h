// Copyright (C) 2023 Vincent Chambrin
// This file is part of the 'ocv-playground' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef CONTROLPOINTSGROUPBOX_H
#define CONTROLPOINTSGROUPBOX_H

#include <QGroupBox>
#include <QListWidget>

/**
 * @brief a widget that displays the coordinates of a set of points
 * 
 * This widget is used to display the coordinates of the control points 
 * of the DrawingSurface. 
 */
class ControlPointsGroupBox : public QGroupBox
{
    Q_OBJECT
public:
    explicit ControlPointsGroupBox(QWidget* parent = nullptr);
    ~ControlPointsGroupBox();

    void setControlPoints(const std::vector<QPoint>& positions);

private:
    QListWidget* m_list_widget;
};

#endif // CONTROLPOINTSGROUPBOX_H
