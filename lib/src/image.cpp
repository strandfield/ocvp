// Copyright (C) 2023 Vincent Chambrin
// This file is part of the 'ocv-playground' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "image.h"

#include <opencv2/imgcodecs.hpp>

#include <stdexcept>

namespace ocvp
{

/**
 * @brief loads an image into memory
 * @param filepath  path to the image
 * @return the image as a cv::Mat object
 * @throw std::runtime_error on failure
 *
 * @warning BGR format is used for colored images
 */
cv::Mat load_image(const std::string& filepath)
{
    cv::Mat img = cv::imread(filepath);

    if (img.data == nullptr)
    {
        if (!cv::haveImageReader(filepath))
        {
            throw std::runtime_error("Image format is not supported by OpenCV");
        }

        throw std::runtime_error("Image could not be loaded");
    }

    return img;
}

/**
 * @brief saves an image onto the disk
 * @param image     the image
 * @param filepath  save path
 * @return whether the image was successfully saved
 *
 * @note the image format is deduced from the file extension
 */
bool save_image(const cv::Mat& image, const std::string& filepath)
{
    return cv::imwrite(filepath, image);
}

} // namespace ocvp
