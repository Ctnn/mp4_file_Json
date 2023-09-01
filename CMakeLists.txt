cmake_minimum_required(VERSION 3.0)

project(mp4_Json)

# C++17 standardını etkinleştir
set(CMAKE_CXX_STANDARD 17)

# Include klasörünü projeye ekle
include_directories(include)

# Projenizin kaynak dosyalarını ekle
add_executable(mp4_Json mp4_Json.cpp)
