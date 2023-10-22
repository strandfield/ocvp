// Copyright (C) 2023 Vincent Chambrin
// This file is part of the 'ocv-playground' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef CAMERA_H
#define CAMERA_H

#include "defs.h"

#include <opencv2/core/mat.hpp>

#include <string>
#include <vector>

namespace ocvp
{

/**
 * @brief raw struct containing camera intrinsic parameters
 */
struct CameraIntrinsics
{
    double cx;
    double cy;
    double fx;
    double fy;
};

PLAYGROUND_API void save_camera_intrinsics(const std::string& filepath,
                                           const CameraIntrinsics& params);
PLAYGROUND_API CameraIntrinsics load_camera_intrinsics(const std::string& filepath);

PLAYGROUND_API cv::Mat make_camera_matrix(const CameraIntrinsics& intrinsics);

/**
 * @brief raw struct containing distortion coefficients
 */
struct DistortionCoefficients
{
    double k1;
    double k2;
    double p1;
    double p2;
    double k3;
    double k4;
    double k5;
    double k6;
};

PLAYGROUND_API void save_distortion_coeffs(const std::string& filepath,
                                           const DistortionCoefficients& coeffs);
PLAYGROUND_API DistortionCoefficients load_distortion_coeffs(const std::string& filepath);

PLAYGROUND_API std::vector<double> make_distcoeffs_vector(const DistortionCoefficients& coeffs);

} // namespace ocvp

#endif // CAMERA_H
