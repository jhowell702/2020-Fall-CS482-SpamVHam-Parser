cmake_minimum_required (VERSION 2.6)
project (2-spamhamspam)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(classify src/classify.cc)
add_executable(training src/training.cc)
