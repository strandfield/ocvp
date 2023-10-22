// Copyright (C) 2023 Vincent Chambrin
// This file is part of the 'ocv-playground' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "pnp.h"

#include <opencv2/core.hpp>

#include <stdexcept>

namespace ocvp
{

void save_camera_intrinsics(const std::string& filepath, const CameraIntrinsics& params)
{
    cv::FileStorage fs{ filepath, cv::FileStorage::WRITE };
    fs << "cx" << params.cx;
    fs << "cy" << params.cy;
    fs << "fx" << params.fx;
    fs << "fy" << params.fy;
}

CameraIntrinsics load_camera_intrinsics(const std::string& filepath)
{
    cv::FileStorage fs{ filepath, cv::FileStorage::READ };

    if (!fs.isOpened())
    {
        throw std::runtime_error("Could not open " + filepath);
    }

    CameraIntrinsics params;
    fs["cx"] >> params.cx;
    fs["cy"] >> params.cy;
    fs["fx"] >> params.fx;
    fs["fy"] >> params.fy;

    return params;
}

/**
 * @brief builds the camera intrinsic matrix from intrinsic parameters
 * @param intrinsics
 *
 * The resulting matrix has the following shape:
 * [ fx   0  cx ]
 * [  0  fy  cy ]
 * [  0   0   1 ]
 */
cv::Mat make_camera_matrix(const CameraIntrinsics& intrinsics)
{
    return (cv::Mat_<double>(3, 3) << intrinsics.fx,
            0,
            intrinsics.cx,
            0,
            intrinsics.fy,
            intrinsics.cy,
            0,
            0,
            1);
}

void save_distortion_coeffs(const std::string& filepath, const DistortionCoefficients& coeffs)
{
    cv::FileStorage fs{ filepath, cv::FileStorage::WRITE };
    fs << "k1" << coeffs.k1;
    fs << "k2" << coeffs.k2;
    fs << "k3" << coeffs.k3;
    fs << "k4" << coeffs.k4;
    fs << "k5" << coeffs.k5;
    fs << "k6" << coeffs.k6;
    fs << "p1" << coeffs.p1;
    fs << "p2" << coeffs.p2;
}

DistortionCoefficients load_distortion_coeffs(const std::string& filepath)
{
    cv::FileStorage fs{ filepath, cv::FileStorage::READ };

    if (!fs.isOpened())
    {
        throw std::runtime_error("Could not open " + filepath);
    }

    DistortionCoefficients coeffs;
    fs["k1"] >> coeffs.k1;
    fs["k2"] >> coeffs.k2;
    fs["k3"] >> coeffs.k3;
    fs["k4"] >> coeffs.k4;
    fs["k5"] >> coeffs.k5;
    fs["k6"] >> coeffs.k6;
    fs["p1"] >> coeffs.p1;
    fs["p2"] >> coeffs.p2;

    return coeffs;
}

/**
 * @brief builds a vector for a given set of distortion coefficients
 * @param coeffs
 * @return the vector [ k1, k2, p1, p2, k3, k4, k5, k6 ]
 */
std::vector<double> make_distcoeffs_vector(const DistortionCoefficients& coeffs)
{
    return {
        coeffs.k1, coeffs.k2, coeffs.p1, coeffs.p2, coeffs.k3, coeffs.k4, coeffs.k5, coeffs.k6
    };
}

} // namespace ocvp
