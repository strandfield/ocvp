// Copyright (C) 2023 Vincent Chambrin
// This file is part of the 'ocv-playground' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "mainwindow.h"

#include "utils/cutecv.h"
#include "widgets/cameraintrinsicsgroupbox.h"
#include "widgets/controlpointsgroupbox.h"
#include "widgets/distortioncoefficientsgroupbox.h"
#include "widgets/drawingsurface.h"
#include "widgets/pnpresultwidget.h"

#include "ocvp/drawframe.h"
#include "ocvp/image.h"
#include "ocvp/pnp.h"

#include <QAction>
#include <QKeySequence>
#include <QMenu>
#include <QMenuBar>

#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>

MainWindow::MainWindow()
{
    setWindowTitle("OpenCV Playground");

    fillMenuBar();
    createCentralWidget();

    updateUserInterface();

    connect(m_drawingsurface,
            &DrawingSurface::controlPointsModified,
            this,
            &MainWindow::updateUserInterface);
}

MainWindow::~MainWindow()
{
}

void MainWindow::open()
{
    auto directory = QString();
    QString path = QFileDialog::getExistingDirectory(this, "Open working folder", directory);

    if (path.isEmpty())
    {
        return;
    }

    if (QFileInfo::exists(path + "/camera.json"))
    {
        auto values = ocvp::load_camera_intrinsics((path + "/camera.json").toStdString());
        m_cameraintrinsics_groupbox->setCameraIntrinsics(values);
    }

    if (QFileInfo::exists(path + "/distortion.json"))
    {
        auto values = ocvp::load_distortion_coeffs((path + "/distortion.json").toStdString());
        m_distortioncoeffs_groupbox->setDistortionCoefficients(values);
    }

    if (QFileInfo::exists(path + "/calibration.jpg"))
    {
        m_drawingsurface->setBackgroungImage(QImage(path + "/calibration.jpg"));
    }
}

void MainWindow::openImage()
{
    auto directory = QString();
    auto filters = QString("Images (*.jpg *.jpeg *.png)");
    QString path = QFileDialog::getOpenFileName(this, "Open", directory, filters);

    if (path.isEmpty())
    {
        return;
    }

    m_drawingsurface->setBackgroungImage(QImage(path));
}

void MainWindow::openCameraJson()
{
    auto directory = QString();
    auto filters = QString("Camera intrinsics (camera.json)");
    QString path = QFileDialog::getOpenFileName(this, "Open camera.json", directory, filters);

    if (path.isEmpty())
    {
        return;
    }

    auto values = ocvp::load_camera_intrinsics(path.toStdString());
    m_cameraintrinsics_groupbox->setCameraIntrinsics(values);
}

void MainWindow::openDistortionJson()
{
    auto directory = QString();
    auto filters = QString("Distortion coefficients (distortion.json)");
    QString path = QFileDialog::getOpenFileName(this, "Open distortion.json", directory, filters);

    if (path.isEmpty())
    {
        return;
    }

    auto values = ocvp::load_distortion_coeffs(path.toStdString());
    m_distortioncoeffs_groupbox->setDistortionCoefficients(values);
}

void MainWindow::exportContour()
{
    auto directory = QString();
    auto filters = QString("Images (*.jpg *.jpeg *.png)");
    QString path = QFileDialog::getSaveFileName(this, "Save contour", directory, filters);

    if (path.isEmpty())
    {
        return;
    }

    QImage img = m_drawingsurface->pictureWithContour();
    img.save(path);
}

void MainWindow::exit()
{
    close();
}

/**
 * @brief updates the user interface
 * 
 * This function enables or disables some UI elements depending on the state 
 * of the application. 
 */
void MainWindow::updateUserInterface()
{
    m_exportcontour_action->setEnabled(m_drawingsurface
                                       && m_drawingsurface->controlPoints().size() > 2);

    m_solvepnp_button->setEnabled(m_drawingsurface
                                  && m_drawingsurface->controlPoints().size() == 4);
}

void MainWindow::solvePnP()
{
    std::vector<QPoint> points = m_drawingsurface->controlPoints();

    ocvp::A4SheetOfPaper sheet;
    sheet.bottom_left = to_opencv(points.at(0));
    sheet.bottom_right = to_opencv(points.at(1));
    sheet.top_right = to_opencv(points.at(2));
    sheet.top_left = to_opencv(points.at(3));

    ocvp::CameraIntrinsics intrinsics = m_cameraintrinsics_groupbox->getCameraIntrinsics();
    ocvp::DistortionCoefficients distcoeffs
      = m_distortioncoeffs_groupbox->getDistortionCoefficients();

    try
    {
        ocvp::PnPResult result = ocvp::solve_pnp(sheet, intrinsics, distcoeffs);
        auto* widget = new PnPResultWidget(
          m_drawingsurface->backgroundImage(), intrinsics, distcoeffs, result);
        int tab_index = m_tab_widget->addTab(widget, "Result");
        m_tab_widget->setCurrentIndex(tab_index);
    }
    catch (const std::exception& ex)
    {
        QMessageBox::warning(
          this,
          "Solve PnP failed",
          QString("Solve PnP failed with the following error: \n%1").arg(ex.what()));
    }
}

void MainWindow::fillMenuBar()
{
    QMenu* file = menuBar()->addMenu("File");

    file->addAction("Open folder", this, &MainWindow::open, QKeySequence("Ctrl+O"));
    QMenu* open_menu = file->addMenu("Open...");
    open_menu->addAction("Calibration image", this, &MainWindow::openImage);
    open_menu->addAction("camera.json", this, &MainWindow::openCameraJson);
    open_menu->addAction("distortion.json", this, &MainWindow::openDistortionJson);
    m_exportcontour_action = file->addAction("Export contour", this, &MainWindow::exportContour);
    file->addSeparator();
    file->addAction("Exit", this, &MainWindow::exit, QKeySequence("Alt+F4"));
}

void MainWindow::createCentralWidget()
{
    m_tab_widget = new QTabWidget;

    auto* w = new QWidget(this);

    {
        auto* layout = new QHBoxLayout;

        {
            auto* scrollarea = new QScrollArea();

            {
                m_drawingsurface = new DrawingSurface();
                scrollarea->setWidget(m_drawingsurface);
            }

            constexpr int stretch = 1;
            layout->addWidget(scrollarea, stretch);
        }

        {
            auto* rightcolumn = new QWidget();

            {
                auto* sublayout = new QVBoxLayout(rightcolumn);
                auto* cpgroup = new ControlPointsGroupBox();
                m_cameraintrinsics_groupbox = new CameraIntrinsicsGroupBox();
                m_distortioncoeffs_groupbox = new DistortionCoefficientsGroupBox();
                m_solvepnp_button = new QPushButton("Solve PnP");

                connect(m_drawingsurface,
                        &DrawingSurface::controlPointsModified,
                        cpgroup,
                        [cpgroup, this]()
                        { cpgroup->setControlPoints(m_drawingsurface->controlPoints()); });

                connect(m_solvepnp_button, &QPushButton::clicked, this, &MainWindow::solvePnP);

                sublayout->addWidget(cpgroup);
                sublayout->addWidget(m_cameraintrinsics_groupbox);
                sublayout->addWidget(m_distortioncoeffs_groupbox);
                sublayout->addStretch(1);
                sublayout->addWidget(m_solvepnp_button);
            }

            layout->addWidget(rightcolumn);
        }

        w->setLayout(layout);
    }

    m_tab_widget->addTab(w, "Calibration");

    setCentralWidget(m_tab_widget);
}
