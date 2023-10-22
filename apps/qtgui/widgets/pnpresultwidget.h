// Copyright (C) 2023 Vincent Chambrin
// This file is part of the 'ocv-playground' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef PNPRESULTWIDGET_H
#define PNPRESULTWIDGET_H

#include "ocvp/pnp.h"

#include <QWidget>

#include <QImage>

class QGroupBox;

/**
 * @brief a widget that displays the result of a PnP pose estimation problem
 * 
 * This widget contains the following:
 * - the calibration picture with the world frame drawn (in a scroll area)
 * - the rvec and tvec (in a table)
 * - the rotation matrix (in a table) 
 * - the position of the camera wrt. the world frame (in a table)
 */
class PnPResultWidget : public QWidget
{
    Q_OBJECT
public:
    PnPResultWidget(QImage image,
                    const ocvp::CameraIntrinsics& intrinsics,
                    const ocvp::DistortionCoefficients& distcoeffs,
                    const ocvp::PnPResult& result,
                    QWidget* parent = nullptr);
    ~PnPResultWidget();

protected Q_SLOTS:
    void exportImage();

private:
    QGroupBox* createPnPResultGroupBox(const ocvp::PnPResult& pnp);
    QGroupBox* createPnPRotationMatrixGroupBox(const ocvp::PnPResult& pnp);
    QGroupBox* createCameraPositionGroupBox(const ocvp::PnPResult& pnp);

private:
    QImage m_image;
};

#endif // PNPRESULTWIDGET_H
