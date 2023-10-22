// Copyright (C) 2023 Vincent Chambrin
// This file is part of the 'ocv-playground' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "pnp.h"

#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>

#include <stdexcept>

namespace ocvp
{

/**
 * @brief solves a PnP pose computation problem given the coordinates of a sheet of paper
 * @param a4sheet     coordinates of a A4 sheet on a picture
 * @param intrinsics  camera intrinsic parameters
 * @param distortion  distortion coefficients
 * @return a struct containing the rotation and translation vectors
 * @throw std::runtime_error if OpenCV fails at solving the problem
 *
 * @sa cv::solvePnP
 * (https://docs.opencv.org/4.x/d9/d0c/group__calib3d.html#ga549c2075fac14829ff4a58bc931c033d)
 */
PnPResult solve_pnp(const A4SheetOfPaper& a4sheet,
                    const CameraIntrinsics& intrinsics,
                    const DistortionCoefficients& distortion)
{
    std::vector<cv::Point3d> object_points{ cv::Point3d(0, 0, 0),
                                            cv::Point3d(0.21, 0, 0),
                                            cv::Point3d(0.21, 0.297, 0),
                                            cv::Point3d(0, 0.297, 0) };

    std::vector<cv::Point2d> image_points{ cv::Point2d(a4sheet.bottom_left),
                                           cv::Point2d(a4sheet.bottom_right),
                                           cv::Point2d(a4sheet.top_right),
                                           cv::Point2d(a4sheet.top_left) };

    cv::Mat camera_matrix = make_camera_matrix(intrinsics);
    std::vector<double> dist_coeffs = make_distcoeffs_vector(distortion);

    PnPResult result;
    result.rvec = cv::Mat::zeros(3, 1, CV_64FC1);
    result.tvec = cv::Mat::zeros(3, 1, CV_64FC1);

    bool problem_solved = cv::solvePnP(
      object_points, image_points, camera_matrix, dist_coeffs, result.rvec, result.tvec);

    if (!problem_solved)
    {
        throw std::runtime_error("cv::solvePnP() failed");
    }

    return result;
}

/**
 * @brief saves the result of solve_pnp() in a json file
 * @param filepath  path to the json file (must include the .json extension)
 * @param result    results of a call to solve_pnp()
 */
void save_pnp_result(const std::string& filepath, const PnPResult& result)
{
    cv::FileStorage fs{ filepath, cv::FileStorage::WRITE };
    fs << "rvec" << result.rvec;
    fs << "tvec" << result.tvec;
}

/**
 * @brief loads the result of a call to solve_pnp(), saved as a json file
 * @param filepath  path to the json file
 */
PnPResult load_pnp_result(const std::string& filepath)
{
    cv::FileStorage fs{ filepath, cv::FileStorage::READ };

    if (!fs.isOpened())
    {
        throw std::runtime_error("Could not open " + filepath);
    }

    PnPResult result;
    fs["rvec"] >> result.rvec;
    fs["tvec"] >> result.tvec;
    return result;
}

/**
 * @brief returns a rotation matrix from the rvec vector
 * @param rvec
 *
 * @sa cv::Rodrigues()
 * (https://docs.opencv.org/4.x/d9/d0c/group__calib3d.html#ga61585db663d9da06b68e70cfbf6a1eac)
 */
cv::Mat get_rotation_matrix(const cv::Mat& rvec)
{
    cv::Mat result;
    cv::Rodrigues(rvec, result);
    return result;
}

/**
 * @brief compute the camera position in the world coordinate system
 * @param rvec
 * @param tvec
 */
cv::Mat compute_camera_position(const cv::Mat& rvec, const cv::Mat& tvec)
{
    cv::Mat rot = ocvp::get_rotation_matrix(rvec);
    cv::Mat rott = rot.clone();
    cv::transpose(rot, rott);
    cv::Mat pos = rott * (-tvec);
    return pos;
}

} // namespace ocvp
