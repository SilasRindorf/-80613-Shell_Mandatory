cmake_minimum_required(VERSION 3.20)
project(Shell_Mandatory C)

set(CMAKE_C_STANDARD 11)

add_executable(Shell_Mandatory main.c oldMain.c oldMain.h given-files/Test.cpp given-files/Test.h)
