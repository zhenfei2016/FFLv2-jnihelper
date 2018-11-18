cmake_minimum_required(VERSION 3.4.1)

PROJECT(jniHelper)


SET(SRC_LIST
   JniBase.hpp;JniBase.cpp
   JavaClassBase.hpp;JavaClassBase.cpp
   JavaClass.hpp;JavaClass.cpp
   JavaClassManager.hpp;JavaClassManager.cpp
   JniHelper.hpp
   )
add_library(jniHelper STATIC ${SRC_LIST})








