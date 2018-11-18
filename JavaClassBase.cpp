/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  JavaClassBase.cpp   
*  Created by zhufeifei(34008081@qq.com) on 2018/06/28 
*  https://github.com/zhenfei2016/FFL-v2.git
*  android jni帮助函数，通过java包名获取java层的类的一些信息
*
*/
#include <string.h>
#include <malloc.h>
#include <jni.h>
#include "JavaClassBase.hpp"

namespace FFL {
	namespace jni {
		//
		//  保存一系列的全局变量，java虚拟机JavaVM，和线程上下文环境 JNIEnv
		//
		static JavaVM *gJVM;

		JavaVM *getJavaVM() {
			return gJVM;
		}

		void setJavaVM(JavaVM *jvm) {
			gJVM = jvm;
		}

		void getJNIEnv(JNIEnv **env) {
			gJVM->GetEnv((void **) env, JNI_VERSION_1_4);
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		JavaClassBase::JavaClassBase(JNIEnv &env, const char *sign) : mClass(NULL) {
			mClassSign = strdup(sign);
		}

		JavaClassBase::~JavaClassBase() {
			JNIEnv *env = 0;
			getJNIEnv(&env);

			JNI_LOG_INFO("JavaClassBase %s not Free.", mClassSign);
			free((void *) mClassSign);
			mClassSign = NULL;
		}

		jclass JavaClassBase::getJClass() const {
			return mClass;
		}

		const char *JavaClassBase::getClassSign() const {
			return mClassSign;
		}

		//
		//  丢出一个异常
		//
		void JavaClassBase::throwException(JNIEnv &env, const char *msg) {
			if (env.ThrowNew(mClass, msg) != JNI_OK) {
				JNI_LOG_ERROR("%s: Failed: msg:%s", __func__, msg);
			}
		}

		//
		//查找类中函数
		//
		jmethodID JavaClassBase::getMethodId(JNIEnv &env, const char *name, const char *sign) {
			jmethodID methodID = env.GetMethodID(mClass, name, sign);
			if (!methodID) {
				JNI_LOG_WARNING("Failed to getMethodId: %s ,%s", name, sign);
			}
			return methodID;
		}

		//
		//  查找静态函数
		//
		jmethodID
		JavaClassBase::getStaticMethodId(JNIEnv &env, const char *name, const char *sign) {
			jmethodID methodID = env.GetStaticMethodID(mClass, name, sign);
			if (!methodID) {
				JNI_LOG_WARNING("Failed to getStaticMethodId: %s ,%s", name, sign);
			}
			return methodID;
		}

		//
		//  查找成员变量
		//
		jfieldID JavaClassBase::getFieldId(JNIEnv &env, const char *name, const char *sign) {
			jfieldID fieldID = env.GetFieldID(mClass, name, sign);
			if (!fieldID) {
				JNI_LOG_WARNING("Failed to getFieldId: %s ,%s", name, sign);
			}
			return fieldID;
		}

		//
		//  查找静态成员变量
		//
		jfieldID JavaClassBase::getStaticFieldId(JNIEnv &env, const char *name, const char *sign) {
			jfieldID fieldID = env.GetStaticFieldID(mClass, name, sign);
			if (!fieldID) {
				JNI_LOG_WARNING("Failed to getStaticFieldId: %s ,%s", name, sign);
			}
			return fieldID;
		}

		//
		//  查找对应签名的类
		//
		jclass JNIfindClass(JNIEnv &env, const char *sign) {
			jclass clazz = env.FindClass(sign);
			JNIclearException(env);
			return clazz;
		}

		//
		//  清空当前的异常
		//
		bool JNIclearException(JNIEnv &env) {
			if (env.ExceptionCheck()) {
				env.ExceptionDescribe();
				env.ExceptionClear();
				return true;
			}
			return false;
		}

		//
		//  创建全局引用
		//
		jobject JNInewGlobalRef(JNIEnv &env, jobject obj) {
			if (obj) {
				jobject globalObj = env.NewGlobalRef(obj);
				JNIclearException(env);
				return globalObj;
			}

			return NULL;
		}

		void JNIdeleteGlobalRef(JNIEnv &env, jobject obj) {
			if (obj) {
				env.DeleteGlobalRef(obj);
			}
		}

		//
		//  减少局部引用
		//
		void JNIdeleteLocalRef(JNIEnv &env, jobject obj) {
			if (obj) {
				env.DeleteLocalRef(obj);
			}
		}

		//
		// 获取java层数组数据指针
		//
		jbyte *JNIgetPtrFromByteArray(JNIEnv &env, jbyteArray bytearray) {
			return (jbyte *) env.GetPrimitiveArrayCritical(bytearray, 0);
		}

		//
		// 释放局部引用
		//
		void JNIreleasePtrFromByteArray(JNIEnv &env, jbyteArray bytearray, void *data) {
			env.ReleasePrimitiveArrayCritical(bytearray, data, 0);
		}

		jbyteArray JNInewByteArray(JNIEnv &env, jsize capacity) {
			jbyteArray local = env.NewByteArray(capacity);
			JNIclearException(env);
			if (!local) {
				return NULL;
			}

			jbyteArray global = (jbyteArray) JNInewGlobalRef(env, local);
			env.DeleteLocalRef(local);
			return global;
		}

		//
		// 写数据到bytearray中
		//
		void JNIwriteByteArray(JNIEnv &env, jbyteArray byteArray, jsize capacity, uint8_t *data,
							   uint32_t dataLen) {
			if (dataLen <= capacity) {
				env.SetByteArrayRegion(byteArray, 0, dataLen, (jbyte *) data);
			} else {
				JNI_LOG_WARNING("JNIwriteByteArray len=%d,capcaity=%d", dataLen, capacity);
			}
		}


		//FFLv2/media/OVHPlayer

	}

}

