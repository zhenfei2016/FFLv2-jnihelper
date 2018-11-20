/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  JavaClassManager.hpp
*  Created by zhufeifei(34008081@qq.com) on 2018/10/02
*  https://github.com/zhenfei2016/FFL-v2.git
*  jni导出，导入的管理类
*
*/
#ifndef _JNIJAVA_CLASS_MANAGER_HPP_
#define _JNIJAVA_CLASS_MANAGER_HPP_

#include <jni.h>

/*
 *   使用方法：
 *   
 *   ExportConfig* exampleConfig={};
 *   JniDeclareClass(exampleConfig);
 *
 *
 *   JNI_Onload(){
 *          JniInitialize(jvm);
 *          JniRegisterAllClass();
 *   }
 *
 *   JNI_Onunload(){
 *          JniTerminate();
 *   }
 */

namespace FFL{
	namespace jni {
		class JavaClass;
		class ExportConfig;
		//
		//  声明需要导出，导入的类
		//
		int JniDeclareClass(ExportConfig *javaClass);
		//
		//  初始化
		//
		void JniInitialize(JavaVM* jvm) ;
        void JniTerminate();
		//
		//  注册所有的导出类
		//
		void JniRegisterAllClass(JNIEnv *env);
        
        ////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////
        //
        //  通过签名获取对应的已注册过的类
        //
        JavaClass *JniFindRegisteredClassByName(const char *signClassName);
        //
        //  通过签名获取对应的已注册过的类
        //
        ExportConfig *JniFindRegisteredClassConfigByName(const char *signClassName);
        

	}
}
#endif
