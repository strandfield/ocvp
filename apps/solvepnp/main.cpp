// Copyright (C) 2023 Vincent Chambrin
// This file is part of the 'ocv-playground' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "ocvp/cli.h"
#include "ocvp/contour.h"
#include "ocvp/image.h"
#include "ocvp/pnp.h"

#include <opencv2/calib3d.hpp>

#include <iostream>

struct Params
{
    ocvp::A4SheetOfPaper corner_coordinates;
    std::string camera_json_path;
    std::string distortion_json_path;
    std::string result_json_path;
};

void print_help()
{
    std::cout << "solvepnp: solves a  Perspective-n-Point (PnP) pose computation problem using A4 "
                 "sheet of paper"
              << std::endl;
    std::cout
      << "usage: solvepnp x1:y1 x2:y2 x3:y3 x4:y4 <camera.json> <distortion.json> [result.json]"
      << std::endl;
    std::cout << "description: " << std::endl;
    std::cout << "  points must be specified counter-clockwise starting at the bottom left corner"
              << std::endl;
    std::cout << "  <camera.json> specifies the camera intrinsic parameters" << std::endl;
    std::cout << "  <distortion.json> specifies the distortion coefficients" << std::endl;
    std::cout << "  [result.json] optional output file in which results are saved" << std::endl;
    std::exit(0);
}

cv::Point2d parse_point2d(const std::string& arg)
{
    size_t separator_index = arg.find(':');

    if (separator_index == std::string::npos)
    {
        std::cerr << "Malformed 2D point: " << arg << std::endl;
        std::exit(1);
    }

    cv::Point2d p;
    p.x = std::stoi(arg.substr(0, separator_index));
    p.y = std::stoi(arg.substr(separator_index + 1));

    return p;
}

Params parse_cli(int argc, char* argv[])
{
    if (argc > 8 || argc < 7)
    {
        std::cerr << "Incorrect number of arguments" << std::endl;
        std::exit(1);
    }

    Params params;
    params.corner_coordinates.bottom_left = parse_point2d(argv[1]);
    params.corner_coordinates.bottom_right = parse_point2d(argv[2]);
    params.corner_coordinates.top_right = parse_point2d(argv[3]);
    params.corner_coordinates.top_left = parse_point2d(argv[4]);
    params.camera_json_path = argv[5];
    params.distortion_json_path = argv[6];

    if (argc == 8)
    {
        params.result_json_path = argv[7];
    }

    return params;
}

int main(int argc, char* argv[])
{
    if (argc <= 1 || ocvp::cli::is_help(argv[1]))
        print_help();

    Params params = parse_cli(argc, argv);

    ocvp::A4SheetOfPaper a4sheet = params.corner_coordinates;

    ocvp::CameraIntrinsics intrinsics = ocvp::load_camera_intrinsics(params.camera_json_path);

    ocvp::DistortionCoefficients distortion
      = ocvp::load_distortion_coeffs(params.distortion_json_path);

    ocvp::PnPResult result;

    try
    {
        result = ocvp::solve_pnp(a4sheet, intrinsics, distortion);
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Failed at solving PnP problem..." << std::endl;
        return 1;
    }

    std::cout << "rvec = \n" << result.rvec << std::endl;
    std::cout << "tvec = \n" << result.tvec << std::endl;

    {
        cv::Mat rotation_matrix = ocvp::get_rotation_matrix(result.rvec);
        std::cout << "rotation_matrix = \n" << rotation_matrix << std::endl;
    }

    {
        cv::Mat camera_position = ocvp::compute_camera_position(result.rvec, result.tvec);
        std::cout << "camera_position = \n" << camera_position << std::endl;
    }

    if (!params.result_json_path.empty())
    {
        ocvp::save_pnp_result(params.result_json_path, result);
        std::cout << "Results saved into " << params.result_json_path << std::endl;
    }

    return 0;
}
