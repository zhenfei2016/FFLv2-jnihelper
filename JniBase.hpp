/*
 *  This file is part of FFL project.
 *
 *  The MIT License (MIT)
 *  Copyright (C) 2017-2018 zhufeifei All rights reserved.
 *
 *  JniBase.hpp
 *  Created by zhufeifei(34008081@qq.com) on 2018/06/28
 *  https://github.com/zhenfei2016/FFL-v2.git
 *  保存java层的一些东西
 *
 */
#ifndef _JNI_BASE_HPP_
#define _JNI_BASE_HPP_
#include <jni.h>
#include "JavaClassBase.hpp"

namespace FFL {
	namespace  jni {
		class NativeObject;
		class JniBase {
		public:
			//
			//  jniJavaObj:java层的对象，用户保存一些native的基本信息
			//
			JniBase(JNIEnv* env,jobject  jniJavaObj);
			~JniBase();
		private:
			void init(JNIEnv* env,jobject hostJavaObj);
		public:
			//
			//  保存native指针到java层
			//
			void nativePtrSaveToJava(JNIEnv* env,NativeObject* obj);
			//
			//  从java获取保存的native指针
			//
			NativeObject* nativePtrLoadFromJava(JNIEnv* env);
		public:
			//
			//  java层使用的native_setup函数
			//   objSign  ：java层对象对应的类签名
			//   obj      :java层对象指针，这个对象将于native层的对象关联上去
			//  weakparam :  java层传递的弱引用指针
			//
			static void setup(JNIEnv *env,jclass thiclass, jstring objSign, jobject obj, jobject weakparam);

			//
			//  java层使用的native_release函数
			//  java层对象不用前了，调用此函数，进行一些释放操作
			//
			static void release(JNIEnv *env,jclass thiclass, jstring objSign, jobject obj);
		private:
			//
			//  java层对象保存native对象的id,对应setup函数输入的 jobject obj,第三参数
			//
			jfieldID mNativeInstancePtrFieldId;
            //
            //  java层jni对象具柄
            //
			jobject  mJavaJniBase;
		};

		extern const char* JNIBASE_SIGN_NAME;
	}
}
#endif
