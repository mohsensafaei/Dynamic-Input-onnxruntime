cmake_minimum_required(VERSION 3.10)

project(ONNX_Runtime VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

SET(CMAKE_CXX_FLAGS "-O2")
SET(CMAKE_C_FLAGS "-O2")

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/point_cloud_data.pcd.bin
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
 file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/pointpillars_refined.onnx
      DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

INCLUDE_DIRECTORIES(./include)

####### change path to your onnxruntime ###################
find_path(ONNX_RUNTIME_SESSION_INCLUDE_DIRS onnxruntime_cxx_api.h HINTS ./onnxruntime-linux-x64-gpu-1.12.0/include)

find_library(ONNX_RUNTIME_LIB onnxruntime HINTS ./onnxruntime-linux-x64-gpu-1.12.0/lib)
find_library(ONNX_RUNTIME_LIB2 onnxruntime_providers_cuda HINTS ./onnxruntime-linux-x64-gpu-1.12.0/lib)

add_executable(ONNX_Runtime ./src/main.cpp)
target_include_directories(ONNX_Runtime PRIVATE ${ONNX_RUNTIME_SESSION_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})
message(${ONNX_RUNTIME_LIB2})
target_link_libraries(ONNX_Runtime PRIVATE ${ONNX_RUNTIME_LIB} ${OpenCV_LIBRARIES} ${ONNX_RUNTIME_LIB2} m)
