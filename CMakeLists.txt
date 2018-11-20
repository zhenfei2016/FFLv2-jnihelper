cmake_minimum_required(VERSION 3.4.1)

PROJECT(jniHelper)

include_directories(./include)

SET(SRC_LIST
   src/JniBase.hpp;src/JniBase.cpp
   include/JavaClassBase.hpp;src/JavaClassBase.cpp
   include/JavaClass.hpp;src/JavaClass.cpp
   include/JavaClassManager.hpp;src/JavaClassManager.cpp
   include/JniHelper.hpp
   )
add_library(jniHelper STATIC ${SRC_LIST})








