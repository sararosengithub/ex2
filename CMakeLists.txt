cmake_minimum_required(VERSION 3.17)
project(ex2)

set(CMAKE_CXX_STANDARD 14)

add_executable(ex2 main.cpp anomaly_detection_util.cpp SimpleAnomalyDetector.cpp timeseries.cpp)