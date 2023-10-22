// Copyright (C) 2023 Vincent Chambrin
// This file is part of the 'ocv-playground' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "ocvp/cli.h"
#include "ocvp/contour.h"
#include "ocvp/image.h"

#include <iostream>

struct Params
{
    std::string input_image_path;
    std::vector<cv::Point> points;
    std::string output_image_path;
};

void print_help()
{
    std::cout << "drawcontour: draws the outline of a polygon on an image" << std::endl;
    std::cout << "usage: drawcontour <input_image> [x1:y1 x2:y2 x3:y3 ...] <output_image>"
              << std::endl;

    std::exit(0);
}

Params parse_cli(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cerr << "Not enough arguments" << std::endl;
        std::exit(1);
    }

    Params params;

    params.input_image_path = argv[1];
    params.output_image_path = argv[argc - 1];

    for (int i(2); i < argc - 1; ++i)
    {
        std::string arg = argv[i];
        size_t separator_index = arg.find(':');

        if (separator_index == std::string::npos)
        {
            std::cerr << "Malformed 2D point: " << arg << std::endl;
            std::exit(1);
        }

        cv::Point p;
        p.x = std::stoi(arg.substr(0, separator_index));
        p.y = std::stoi(arg.substr(separator_index + 1));
        params.points.push_back(p);
    }

    return params;
}

int main(int argc, char* argv[])
{
    if (argc <= 1 || ocvp::cli::is_help(argv[1]))
        print_help();

    Params params = parse_cli(argc, argv);

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

    ocvp::draw_contour(image, params.points, cv::Scalar(0, 0, 255), 8);

    bool ok = ocvp::save_image(image, params.output_image_path);

    if (!ok)
    {
        std::cerr << "Failed to save output image..." << std::endl;
        return 1;
    }

    return 0;
}
