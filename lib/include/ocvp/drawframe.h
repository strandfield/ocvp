// Copyright (C) 2023 Vincent Chambrin
// This file is part of the 'ocv-playground' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef DRAWFRAME_H
#define DRAWFRAME_H

#include "camera.h"

namespace ocvp
{

PLAYGROUND_API void draw_frame_axes(cv::Mat& image,
                                    const CameraIntrinsics& camera_intrinsics,
                                    const DistortionCoefficients& dist_coeffs,
                                    const cv::Mat& rvec,
                                    const cv::Mat& tvec,
                                    float length = 1.f,
                                    int thickness = 6);

} // namespace ocvp

#endif // DRAWFRAME_H
