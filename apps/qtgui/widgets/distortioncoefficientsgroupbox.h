// Copyright (C) 2023 Vincent Chambrin
// This file is part of the 'ocv-playground' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef DISTORTIONCOEFFICIENTSGROUPBOX_H
#define DISTORTIONCOEFFICIENTSGROUPBOX_H

#include "ocvp/camera.h"

#include <QDoubleSpinBox>
#include <QGroupBox>

/**
 * @brief a widget that allows the user to view and edit the distortion coefficients
 */
class DistortionCoefficientsGroupBox : public QGroupBox
{
    Q_OBJECT
public:
    explicit DistortionCoefficientsGroupBox(QWidget* parent = nullptr);
    ~DistortionCoefficientsGroupBox();

    ocvp::DistortionCoefficients getDistortionCoefficients() const;
    void setDistortionCoefficients(const ocvp::DistortionCoefficients& params);

private:
    QDoubleSpinBox* m_k1_spinbox;
    QDoubleSpinBox* m_k2_spinbox;
    QDoubleSpinBox* m_k3_spinbox;
    QDoubleSpinBox* m_k4_spinbox;
    QDoubleSpinBox* m_k5_spinbox;
    QDoubleSpinBox* m_k6_spinbox;
    QDoubleSpinBox* m_p1_spinbox;
    QDoubleSpinBox* m_p2_spinbox;
};

#endif // DISTORTIONCOEFFICIENTSGROUPBOX_H
