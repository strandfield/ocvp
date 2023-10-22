// Copyright (C) 2023 Vincent Chambrin
// This file is part of the 'ocv-playground' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class DrawingSurface;
class CameraIntrinsicsGroupBox;
class DistortionCoefficientsGroupBox;

class QPushButton;
class QTabWidget;

/**
 * @brief the application main window 
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow();
    ~MainWindow();

protected Q_SLOTS: // slots for the "File" menu
    void open();
    void openImage();
    void openCameraJson();
    void openDistortionJson();
    void exportContour();
    void exit();

protected Q_SLOTS: // other slots
    void updateUserInterface();
    void solvePnP();

private: // initialization functions, called by the constructor
    void fillMenuBar();
    void createCentralWidget();

private:
    QAction* m_exportcontour_action = nullptr;
    QTabWidget* m_tab_widget = nullptr;
    DrawingSurface* m_drawingsurface = nullptr;
    CameraIntrinsicsGroupBox* m_cameraintrinsics_groupbox = nullptr;
    DistortionCoefficientsGroupBox* m_distortioncoeffs_groupbox = nullptr;
    QPushButton* m_solvepnp_button = nullptr;
};

#endif // MAINWINDOW_H
