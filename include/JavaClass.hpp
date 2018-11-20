/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  JavaClass.hpp
*  Created by zhufeifei(34008081@qq.com) on 2018/06/28
*  https://github.com/zhenfei2016/FFL-v2.git
*  android jni帮助函数，通过java包名获取java层的类的一些信息
*
*/
#ifndef _JNIJAVA_CLASS_HPP_
#define _JNIJAVA_CLASS_HPP_
#include <jni.h>
#include "JavaClassBase.hpp"

namespace FFL {
    
	namespace  jni {
		class JavaClass;
        class NativeObject;
		class JavaObject;
		class JniBase;
		//
		//  创建，释放native实例
		//
		typedef NativeObject *(*Fun_createNativeObject)(JNIEnv *env, jobject thiz,
														jobject weakParam, JavaClass *ctx);

		typedef void (*Fun_destroyNativeObject)(JNIEnv *env, NativeObject *, JavaClass *ctx);

        //
        //  导出native层信息，导入的java层信息
        //
		class ExportConfig {
		public:
			//
			//  java层的对象JniBase成员变量名称
			//
			const char *mJniBaseName;
			//
			//  创建，删除nativeobject接口函数指针
			//
			Fun_createNativeObject createNativeObject;
			Fun_destroyNativeObject destroyNativeObject;
			//
			//  导出的native函数列表
			//
			JNINativeMethod *mNativeMethodList;
            
           
            /////////////////////////////////////////////////
            //  java层导入的信息定义
            ///////////////////////////////////////////////////
            //
            //  当前这个java类的签名字符串
            //
            const char* mJavaClassSignName;
			//
			//  导入的java层函数个数和列表
			//
			JNIJavaMethod *mJavaMethodList;
			//
			//  native层class，对象由内部进行创建
			//
			JavaClass *mJavaClass;
        };
		
        //
		//  一个java类，获取java层先关函数，成员变量对应native的句柄
		//
		class JavaClass : public JavaClassBase {
		public:
			JavaClass(JNIEnv &env,ExportConfig *config);
			virtual ~JavaClass();
		protected:
			//
			// 初始化，反初始化，用这个类的其他函数前需要执行的
			//
			bool init(JNIEnv &env);
			void unInit(JNIEnv &env);
		public:
			//
			//  创建native层对象
			//  thiz: java层的对应对象
			//  weakparam: java层的输入参数，弱引用
			//  此函数的调用对应java层的native_setup
			//
			NativeObject *createNativeInstace(JNIEnv &env, jobject thiz, jobject weakparam);
			//
			//  java层对象释放的时候执行
			//  此函数的调用对应java层的native_release
			//
			void destroyNativeInstace(JNIEnv &env,jobject thiz);
			//
			//  通过thiz获取java层的native指针
			//
			NativeObject* getNativePtr(JNIEnv &env, jobject thiz);
		public:
			//
			//  导出一系列native层函数
			//
			bool exportNativeMethodOrField(JNIEnv &env,
										   JNINativeMethod *methodList, int count);

			//
			//  导入java层函数，成员
			//
			bool importJavaMethodOrField(JNIEnv &env, JNIJavaMethod *methodOrFieldList, int count);
		protected:
			friend class JavaObject;
			friend class NativeObject;

			ExportConfig *mConfig;
			jfieldID  mJniBaseId;
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //  native层对象的基类
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class NativeObject {
		public:
			NativeObject();
			virtual ~NativeObject();
			//
			//  保存一个跟这个对象关联的java层对象
			//
			JavaObject* setJavaObject(JavaObject *obj);
            //
            // 获取内部关联的 java层对象
            //
			JavaObject *getJavaObject() const;
		protected:
			friend class JavaClass;
            //
            //  java层的Jnibase成员在native层的句柄
            //
			JniBase* mJniBase;
            //
            //  当前native层对象关联上一个java层的对象
            //
			JavaObject *mJavaObject;
		};

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //  java层对象，可以通过这个对象操作java层的函数等
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		class JavaObject {
		public:
			//
			//  javaObj:java层对象，golbal对象
			//   autoDel: 是否当这个JavaFFLPlayer对象删除的时候，需要自动删除java层对象
			//
			JavaObject(jobject javaObj,
                       bool autoDel,
                       JNIJavaMethod *methodList);
			~JavaObject();
            //
            //  获取java层对象的基本信息
            //
			JNIJavaMethod *getMethodInfo(int index);
			//
			//  java层的对象
			//
			jobject getJavaObject() const;
        protected:
            //
            //  java层句柄指针列表
            //
			JNIJavaMethod *mMethodList;
            //
            // 保存java层对象
            //
			jobject mJavaObj;
			bool mAutoDel;
		};
	}
}
#endif
