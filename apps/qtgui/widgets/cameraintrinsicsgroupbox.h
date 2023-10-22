// Copyright (C) 2023 Vincent Chambrin
// This file is part of the 'ocv-playground' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef CAMERAINTRINSICSGROUPBOX_H
#define CAMERAINTRINSICSGROUPBOX_H

#include "ocvp/camera.h"

#include <QDoubleSpinBox>
#include <QGroupBox>

/**
 * @brief a widget that allows the user to view and edit the camera intrinsic parameters
 */
class CameraIntrinsicsGroupBox : public QGroupBox
{
    Q_OBJECT
public:
    explicit CameraIntrinsicsGroupBox(QWidget* parent = nullptr);
    ~CameraIntrinsicsGroupBox();

    ocvp::CameraIntrinsics getCameraIntrinsics() const;
    void setCameraIntrinsics(const ocvp::CameraIntrinsics& params);

private:
    QDoubleSpinBox* m_fx_spinbox;
    QDoubleSpinBox* m_fy_spinbox;
    QDoubleSpinBox* m_cx_spinbox;
    QDoubleSpinBox* m_cy_spinbox;
};

#endif // CAMERAINTRINSICSGROUPBOX_H
