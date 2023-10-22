
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC TRUE)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HDR_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_executable(qtgui ${SRC_FILES} ${HDR_FILES})

target_link_libraries(qtgui playgroundlib)
target_link_libraries(qtgui Qt5::Core Qt5::Gui Qt5::Widgets)
