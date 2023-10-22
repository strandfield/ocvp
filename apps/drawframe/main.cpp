// Copyright (C) 2023 Vincent Chambrin
// This file is part of the 'ocv-playground' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "ocvp/cli.h"
#include "ocvp/contour.h"
#include "ocvp/drawframe.h"
#include "ocvp/image.h"
#include "ocvp/pnp.h"

#include <iostream>

struct Params
{
    std::string input_image_path;
    std::string camera_json_path;
    std::string distortion_json_path;
    std::string pnpresult_json_path;
    std::string output_image_path;
};

void print_help()
{
    std::cout << "drawframe: draws the world frame axes onto an image" << std::endl;
    std::cout << "usage: drawframe <input_image> <camera.json> <distortion.json> <pnpresult.json> "
                 "<output_image>"
              << std::endl;

    std::exit(0);
}

Params parse_cli(int argc, char* argv[])
{
    if (argc != 6)
    {
        std::cerr << "Invalid arguments" << std::endl;
        std::exit(1);
    }

    Params params;

    params.input_image_path = argv[1];
    params.camera_json_path = argv[2];
    params.distortion_json_path = argv[3];
    params.pnpresult_json_path = argv[4];
    params.output_image_path = argv[5];

    return params;
}

int main(int argc, char* argv[])
{
    if (argc <= 1 || ocvp::cli::is_help(argv[1]))
        print_help();

    Params params = parse_cli(argc, argv);

    ocvp::CameraIntrinsics intrinsics = ocvp::load_camera_intrinsics(params.camera_json_path);
    ocvp::DistortionCoefficients distortion
      = ocvp::load_distortion_coeffs(params.distortion_json_path);
    ocvp::PnPResult result = ocvp::load_pnp_result(params.pnpresult_json_path);

    cv::Mat image;

    try
    {
        image = ocvp::load_image(params.input_image_path);
    }
    catch (const std::runtime_error& ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    constexpr float length = 0.1;
    constexpr int thickness = 6;

    ocvp::draw_frame_axes(
      image, intrinsics, distortion, result.rvec, result.tvec, length, thickness);

    bool ok = ocvp::save_image(image, params.output_image_path);

    if (!ok)
    {
        std::cerr << "Failed to save output image..." << std::endl;
        return 1;
    }

    return 0;
}
