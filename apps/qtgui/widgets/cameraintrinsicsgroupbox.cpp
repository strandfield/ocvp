// Copyright (C) 2023 Vincent Chambrin
// This file is part of the 'ocv-playground' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "cameraintrinsicsgroupbox.h"

#include <QFormLayout>

CameraIntrinsicsGroupBox::CameraIntrinsicsGroupBox(QWidget* parent)
  : QGroupBox("Camera intrinsics", parent)
{
    auto setup_spinbox = [](QDoubleSpinBox* spinbox)
    {
        spinbox->setDecimals(12);
        spinbox->setMinimum(0);
        spinbox->setMaximum(10000000);
    };

    m_fx_spinbox = new QDoubleSpinBox;
    m_fy_spinbox = new QDoubleSpinBox;
    m_cx_spinbox = new QDoubleSpinBox;
    m_cy_spinbox = new QDoubleSpinBox;

    for (auto* spinbox : { m_fx_spinbox, m_fy_spinbox, m_cx_spinbox, m_cy_spinbox })
        setup_spinbox(spinbox);

    auto* l = new QFormLayout;
    l->addRow("fx", m_fx_spinbox);
    l->addRow("fy", m_fy_spinbox);
    l->addRow("cx", m_cx_spinbox);
    l->addRow("cy", m_cy_spinbox);

    setLayout(l);
}

CameraIntrinsicsGroupBox::~CameraIntrinsicsGroupBox()
{
}

ocvp::CameraIntrinsics CameraIntrinsicsGroupBox::getCameraIntrinsics() const
{
    ocvp::CameraIntrinsics result;
    result.fx = m_fx_spinbox->value();
    result.fy = m_fy_spinbox->value();
    result.cx = m_cx_spinbox->value();
    result.cy = m_cy_spinbox->value();
    return result;
}

void CameraIntrinsicsGroupBox::setCameraIntrinsics(const ocvp::CameraIntrinsics& params)
{
    m_fx_spinbox->setValue(params.fx);
    m_fy_spinbox->setValue(params.fy);
    m_cx_spinbox->setValue(params.cx);
    m_cy_spinbox->setValue(params.cy);
}
