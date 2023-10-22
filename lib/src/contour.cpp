// Copyright (C) 2023 Vincent Chambrin
// This file is part of the 'ocv-playground' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "contour.h"

#include <opencv2/imgproc.hpp>

#include <stdexcept>

namespace ocvp
{

/**
 * @brief draws the outline of a polygon on an image
 * @param image      the image on which the polygon is drawn
 * @param points     points of the polygon (need not be closed)
 * @param color      BGR color
 * @param thickness  thickness of the lines in pixels
 */
void draw_contour(cv::Mat& image,
                  const std::vector<cv::Point>& points,
                  const cv::Scalar& color,
                  int thickness)
{
    for (size_t i(0); i < points.size(); ++i)
    {
        cv::Point first = points.at(i);
        cv::Point second = points.at((i + 1) % points.size());

        cv::line(image, first, second, color, thickness);
    }
}

} // namespace ocvp
