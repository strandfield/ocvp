
file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HDR_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

add_library(playgroundlib STATIC ${HDR_FILES} ${SRC_FILES})
target_include_directories(playgroundlib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(playgroundlib PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/ocvp")
target_link_libraries(playgroundlib ${OpenCV_LIBS})

get_target_property(target_type playgroundlib TYPE)
message("target_type=${target_type}")
if (target_type STREQUAL STATIC_LIBRARY)
    target_compile_definitions(playgroundlib PUBLIC -DPLAYGROUND_STATIC_LINKING)
endif()
