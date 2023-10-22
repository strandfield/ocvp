// Copyright (C) 2023 Vincent Chambrin
// This file is part of the 'ocv-playground' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef IMAGE_H
#define IMAGE_H

#include "defs.h"

#include <opencv2/core/mat.hpp>

#include <string>

namespace ocvp
{

PLAYGROUND_API cv::Mat load_image(const std::string& filepath);
PLAYGROUND_API bool save_image(const cv::Mat& image, const std::string& filepath);

} // namespace ocvp

#endif // IMAGE_H
