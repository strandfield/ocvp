// Copyright (C) 2023 Vincent Chambrin
// This file is part of the 'ocv-playground' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef PNP_H
#define PNP_H

#include "camera.h"

namespace ocvp
{

/**
 * @brief stores the 2D coordinates on a picture of a A4 sheet of paper
 */
struct A4SheetOfPaper
{
    cv::Point bottom_left;
    cv::Point bottom_right;
    cv::Point top_right;
    cv::Point top_left;
};

/**
 * @brief stores the result of Perspective-n-Point (PnP) pose computation problem
 */
struct PnPResult
{
    cv::Mat rvec;
    cv::Mat tvec;
};

PLAYGROUND_API PnPResult solve_pnp(const A4SheetOfPaper& a4sheet,
                                   const CameraIntrinsics& intrinsics,
                                   const DistortionCoefficients& distortion);

PLAYGROUND_API void save_pnp_result(const std::string& filepath, const PnPResult& result);
PLAYGROUND_API PnPResult load_pnp_result(const std::string& filepath);

PLAYGROUND_API cv::Mat get_rotation_matrix(const cv::Mat& rvec);

PLAYGROUND_API cv::Mat compute_camera_position(const cv::Mat& rvec, const cv::Mat& tvec);

} // namespace ocvp

#endif // PNP_H
