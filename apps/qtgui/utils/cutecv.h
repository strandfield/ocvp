// Copyright (C) 2023 Vincent Chambrin
// This file is part of the 'ocv-playground' project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

/**
 * @file cutecv.h
 * @brief provides conversion functions between Qt and OpenCV
 */

#include <QImage>
#include <QPoint>

#include <opencv2/core.hpp>

#include <vector>

inline cv::Point to_opencv(const QPoint& pt)
{
    return cv::Point(pt.x(), pt.y());
}

inline cv::Mat to_opencv(const QImage& image)
{
    auto result = cv::Mat(image.height(), image.width(), CV_8UC3);

    for (int x(0); x < image.width(); ++x)
    {
        for (int y(0); y < image.height(); ++y)
        {
            QColor color = image.pixelColor(x, y);
            result.at<cv::Vec3b>(y, x) = cv::Vec3b(color.blue(), color.green(), color.red());
        }
    }

    return result;
}

inline QImage to_qimage(const cv::Mat& img)
{
    QImage result{ img.cols, img.rows, QImage::Format_RGB32 };

    for (int x(0); x < result.width(); ++x)
    {
        for (int y(0); y < result.height(); ++y)
        {
            cv::Vec3b color = img.at<cv::Vec3b>(y, x);
            result.setPixelColor(x, y, QColor(color[2], color[1], color[0]));
        }
    }

    return result;
}
