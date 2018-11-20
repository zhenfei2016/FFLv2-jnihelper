1. include： 目录位其他库引用的头文件
1. lib ：    已经编译了一个arm-v7a的库文件
2. java：    本库需要的java层对象


使用方法：
#include "JniHelper.hpp"
/*
 *   使用方法：
 *   
 *   声明需要导出的native函数，或导入的java对象
 *   ExportConfig* exampleConfig={};
 *   JniDeclareClass(exampleConfig);
 *
 *   初始化导出，导入
 *   JNI_OnLoad(){
 *      JniInitialize(jvm);
 *      JniRegisterAllClass();
 *   }
 *
 *   JNI_OnUnLoad(){
 *      JniTerminate();
 *   }
 */