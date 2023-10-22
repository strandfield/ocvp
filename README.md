
# OpenCV playground

Working setup (hopefully) for experimenting with Qt/OpenCV. 😇

## Building the code

Note: there is a "CMake Build" GitHub workflow in `.github/workflows` that 
you can use for help if you are building on Linux.

### Prerequisites

- Build system: CMake
- a C++ compiler with C++14 support
- OpenCV
- Qt5 (for the GUI, optional)

**Getting OpenCV on Windows**

- Download a Windows release of OpenCV: https://opencv.org/releases/
- Extract the archive
- Copy the content of the `build` directory to an easily accessible location (e.g. `C\OpenCV`)

**Getting Qt5 on Windows**

Get Qt5 binaries from [Qt for Open Source Development](https://www.qt.io/download-open-source).

**Getting OpenCV on Linux**

```
sudo apt-get install libopencv-dev
```

**Getting Qt5 on Linux**

```
sudo apt install qtbase5-dev
```

### Step-by-step build instructions

Create and move to a build directory:

```
mkdir build && cd build
```

Generate the project using CMake:

```
cmake ..
```

Optional: pass the `BUILD_QT_GUI` to build the Qt GUI.

```
cmake -DBUILD_QT_GUI=ON ..
```

**Note (Windows):** you will probably need to define the following variables:
- `OpenCV_DIR`: OpenCV root directory (contains `OpenCVConfig.cmake`)
- (if `BUILD_QT_GUI=ON`) `Qt5_DIR`: directory containing `Qt5Config.cmake`


Example:
```
cmake -DOpenCV_DIR=C:\OpenCV -DBUILD_QT_GUI=ON -DQt5_DIR=C:\Qt\Qt5.12.12\5.12.12\msvc2017_64\lib\cmake\Qt5 ..
```

**Building (linux)**:

```
make
```

**Building (Windows)**:

```
cmake --build . --config Release --target ALL_BUILD
```

Note: to run the executables on Windows, you may also need to add the `bin` directories 
of OpenCV and Qt5 to the `PATH` environment variable.

## Overview of the programs

The `apps` directory contains the source code for several C++ applications:

`drawcontour` takes as input an image and a list of 2D points (syntax `x:y`) 
and draws in red the outline polygon defined by the points.
This program can be used to produce a visual representation of the 
detected contour of an object.

`solvepnp` solves a perspective-n-point pose estimation problem using 
the `cv::solvePnP()` function. 
It takes as input the 2D contour of a sheet of paper, the camera 
intrinsic parameters (as a `camera.json` file) and the distortion 
coefficients (`distortion.json`) and produces the `rvec` and `tvec`
vectors describing the world-to-camera transformation.

`drawframe` draws the frame axes on the 2D image given the results 
of `solvepnp`.

`qtgui` is a graphical user interface (GUI) that can be used to 
perform all of the above without using the command-line.

All these programs rely on shared code in `lib` that is built 
as a static library.

## Estimation of camera pose from a sheet of A4 paper

The above programs can be used to estimate the camera pose given 
a picture containing a A4 sheet of paper.

What you will need:
- a picture with an A4 sheet of paper
- the camera intrinsic parameters
- the distortion coefficients

The camera intrinsic parameters and distortion coefficients can 
be specified directly in the Qt GUI or using JSON files.

Step-by-step pose estimation using the Qt GUI:
- In the menu bar, use `File > Open... > Calibration image` to open the image
- Specify the camera intrinsics and distortion coefficients in the right column 
- Draw the contour of the sheet of paper with 4 mouse clicks, in the following order: bottom left corner, then bottom right, top right and top left
- Click the `Solve PnP` button.
- The results are displayed in a new tab: image with the frame axes, `rvec` and `tvec`, rotation matrix, position of the camera wrt. the sheet of paper.


