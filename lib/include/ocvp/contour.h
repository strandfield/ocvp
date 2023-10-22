// Copyright (C) 2023 Vincent Chambrin
// This file is part of the 'ocv-playground' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef CONTOUR_H
#define CONTOUR_H

#include "defs.h"

#include <opencv2/core/mat.hpp>

#include <vector>

namespace ocvp
{

PLAYGROUND_API void draw_contour(cv::Mat& image,
                                 const std::vector<cv::Point>& points,
                                 const cv::Scalar& color,
                                 int thickness = 6);

} // namespace ocvp

#endif // CONTOUR_H
