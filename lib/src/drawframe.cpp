// Copyright (C) 2023 Vincent Chambrin
// This file is part of the 'ocv-playground' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "drawframe.h"

#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>

#include <stdexcept>

namespace ocvp
{

/**
 * @brief draws the axes of the world frame on an image
 * @param image              input/output image on which the axes are drawn
 * @param camera_intrinsics  the camera intrinsic parameters
 * @param dist_coeffs        the distortion coefficients
 * @param rvec               screen-to-world rotation vector
 * @param tvec               screen-to-world translation vector
 * @param length             3D-world length of the axes (defaults to 1)
 * @param thickness          thickness (in pixels) of the axes
 */
void draw_frame_axes(cv::Mat& image,
                     const CameraIntrinsics& camera_intrinsics,
                     const DistortionCoefficients& dist_coeffs,
                     const cv::Mat& rvec,
                     const cv::Mat& tvec,
                     float length,
                     int thickness)
{
    cv::drawFrameAxes(image,
                      ocvp::make_camera_matrix(camera_intrinsics),
                      ocvp::make_distcoeffs_vector(dist_coeffs),
                      rvec,
                      tvec,
                      length,
                      thickness);
}

} // namespace ocvp
