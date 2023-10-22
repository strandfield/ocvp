// Copyright (C) 2023 Vincent Chambrin
// This file is part of the 'ocv-playground' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "distortioncoefficientsgroupbox.h"

#include <QFormLayout>

DistortionCoefficientsGroupBox::DistortionCoefficientsGroupBox(QWidget* parent)
  : QGroupBox("Distortion coefficients", parent)
{
    auto setup_spinbox = [](QDoubleSpinBox* spinbox)
    {
        spinbox->setDecimals(18);
        spinbox->setMinimum(-10000000);
        spinbox->setMaximum(10000000);
    };

    m_k1_spinbox = new QDoubleSpinBox;
    m_k2_spinbox = new QDoubleSpinBox;
    m_k3_spinbox = new QDoubleSpinBox;
    m_k4_spinbox = new QDoubleSpinBox;
    m_k5_spinbox = new QDoubleSpinBox;
    m_k6_spinbox = new QDoubleSpinBox;
    m_p1_spinbox = new QDoubleSpinBox;
    m_p2_spinbox = new QDoubleSpinBox;

    for (auto* spinbox : { m_k1_spinbox,
                           m_k2_spinbox,
                           m_k3_spinbox,
                           m_k4_spinbox,
                           m_k5_spinbox,
                           m_k6_spinbox,
                           m_p1_spinbox,
                           m_p2_spinbox })
    {
        setup_spinbox(spinbox);
    }

    auto* l = new QFormLayout;
    l->addRow("k1", m_k1_spinbox);
    l->addRow("k2", m_k2_spinbox);
    l->addRow("k3", m_k3_spinbox);
    l->addRow("k4", m_k4_spinbox);
    l->addRow("k5", m_k5_spinbox);
    l->addRow("k6", m_k6_spinbox);
    l->addRow("p1", m_p1_spinbox);
    l->addRow("p2", m_p2_spinbox);

    setLayout(l);
}

DistortionCoefficientsGroupBox::~DistortionCoefficientsGroupBox()
{
}

ocvp::DistortionCoefficients DistortionCoefficientsGroupBox::getDistortionCoefficients() const
{
    ocvp::DistortionCoefficients result;
    result.k1 = m_k1_spinbox->value();
    result.k2 = m_k2_spinbox->value();
    result.k3 = m_k3_spinbox->value();
    result.k4 = m_k4_spinbox->value();
    result.k5 = m_k5_spinbox->value();
    result.k6 = m_k6_spinbox->value();
    result.p1 = m_p1_spinbox->value();
    result.p2 = m_p2_spinbox->value();
    return result;
}

void DistortionCoefficientsGroupBox::setDistortionCoefficients(
  const ocvp::DistortionCoefficients& params)
{
    m_k1_spinbox->setValue(params.k1);
    m_k2_spinbox->setValue(params.k2);
    m_k3_spinbox->setValue(params.k3);
    m_k4_spinbox->setValue(params.k4);
    m_k5_spinbox->setValue(params.k5);
    m_k6_spinbox->setValue(params.k6);
    m_p1_spinbox->setValue(params.p1);
    m_p2_spinbox->setValue(params.p2);
}
