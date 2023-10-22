// Copyright (C) 2023 Vincent Chambrin
// This file is part of the 'ocv-playground' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "pnpresultwidget.h"

#include "../utils/cutecv.h"

#include "ocvp/drawframe.h"

#include <QFileDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QTableWidget>
#include <QVBoxLayout>

PnPResultWidget::PnPResultWidget(QImage image,
                                 const ocvp::CameraIntrinsics& intrinsics,
                                 const ocvp::DistortionCoefficients& distcoeffs,
                                 const ocvp::PnPResult& result,
                                 QWidget* parent)
  : QWidget(parent)
{
    cv::Mat cvimage = to_opencv(image);
    ocvp::draw_frame_axes(cvimage, intrinsics, distcoeffs, result.rvec, result.tvec, 0.1);

    QLabel* label = new QLabel;
    m_image = to_qimage(cvimage);
    label->setPixmap(QPixmap::fromImage(m_image));
    QScrollArea* scrollarea = new QScrollArea;
    scrollarea->setWidget(label);

    auto* layout = new QHBoxLayout(this);
    constexpr int stretch = 1;
    layout->addWidget(scrollarea, stretch);

    {
        auto* rightcolumn = new QWidget();

        {
            auto* export_button = new QPushButton("Export image");
            connect(export_button, &QPushButton::clicked, this, &PnPResultWidget::exportImage);

            auto* sublayout = new QVBoxLayout(rightcolumn);
            sublayout->addWidget(createPnPResultGroupBox(result));
            sublayout->addWidget(createPnPRotationMatrixGroupBox(result));
            sublayout->addWidget(createCameraPositionGroupBox(result));
            sublayout->addStretch(1);
            sublayout->addWidget(export_button);
        }

        layout->addWidget(rightcolumn);
    }
}

PnPResultWidget::~PnPResultWidget()
{
}

void PnPResultWidget::exportImage()
{
    auto directory = QString();
    auto filters = QString("Images (*.jpg *.jpeg *.png)");
    QString path
      = QFileDialog::getSaveFileName(nullptr, "Save image with frame axes", directory, filters);

    if (path.isEmpty())
    {
        return;
    }

    m_image.save(path);
}

QGroupBox* PnPResultWidget::createPnPResultGroupBox(const ocvp::PnPResult& pnp)
{
    auto* box = new QGroupBox("solvePnP results");

    auto* table = new QTableWidget(3, 2);
    table->setHorizontalHeaderLabels(QStringList() << "rvec"
                                                   << "tvec");

    {
        auto* vh = new QHeaderView(Qt::Vertical);
        vh->hide();
        table->setVerticalHeader(vh);
    }

    table->setItem(0, 0, new QTableWidgetItem(QString::number(pnp.rvec.at<double>(0))));
    table->setItem(1, 0, new QTableWidgetItem(QString::number(pnp.rvec.at<double>(1))));
    table->setItem(2, 0, new QTableWidgetItem(QString::number(pnp.rvec.at<double>(2))));

    table->setItem(0, 1, new QTableWidgetItem(QString::number(pnp.tvec.at<double>(0))));
    table->setItem(1, 1, new QTableWidgetItem(QString::number(pnp.tvec.at<double>(1))));
    table->setItem(2, 1, new QTableWidgetItem(QString::number(pnp.tvec.at<double>(2))));

    auto* layout = new QVBoxLayout();
    layout->addWidget(table);
    box->setLayout(layout);

    return box;
}

QGroupBox* PnPResultWidget::createPnPRotationMatrixGroupBox(const ocvp::PnPResult& pnp)
{
    auto* box = new QGroupBox("Rotation matrix");

    auto* table = new QTableWidget(3, 3);

    {
        auto* hh = new QHeaderView(Qt::Horizontal);
        hh->hide();
        table->setHorizontalHeader(hh);
    }

    {
        auto* vh = new QHeaderView(Qt::Vertical);
        vh->hide();
        table->setVerticalHeader(vh);
    }

    cv::Mat rot = ocvp::get_rotation_matrix(pnp.rvec);

    for (int r(0); r < 3; ++r)
    {
        for (int c(0); c < 3; ++c)
        {
            table->setItem(r, c, new QTableWidgetItem(QString::number(rot.at<double>(r, c))));
        }
    }

    auto* layout = new QVBoxLayout();
    layout->addWidget(table);
    box->setLayout(layout);

    return box;
}

QGroupBox* PnPResultWidget::createCameraPositionGroupBox(const ocvp::PnPResult& pnp)
{
    auto* box = new QGroupBox("Camera position");

    auto* table = new QTableWidget(3, 1);
    table->setHorizontalHeaderLabels(QStringList() << "Position");
    table->setVerticalHeaderLabels(QStringList() << "x"
                                                 << "y"
                                                 << "z");

    cv::Mat pos = ocvp::compute_camera_position(pnp.rvec, pnp.tvec);
    double dist = cv::norm(pos);

    table->setItem(0, 0, new QTableWidgetItem(QString::number(pos.at<double>(0))));
    table->setItem(1, 0, new QTableWidgetItem(QString::number(pos.at<double>(1))));
    table->setItem(2, 0, new QTableWidgetItem(QString::number(pos.at<double>(2))));

    auto* layout = new QVBoxLayout();
    layout->addWidget(table);
    layout->addWidget(new QLabel(QString("distance = %1").arg(QString::number(dist))));
    box->setLayout(layout);

    return box;
}
