/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  JavaClassBase.hpp
*  Created by zhufeifei(34008081@qq.com) on 2018/06/28
*  https://github.com/zhenfei2016/FFL-v2.git
*  android jni帮助函数，通过java包名获取java层的类的一些信息
*
*/
#ifndef _JNIJAVA_CLASS_BASE_HPP_
#define _JNIJAVA_CLASS_BASE_HPP_
#include <jni.h>


#define  JNI_LOG_INFO(...)
#define  JNI_LOG_ERROR(...)
#define  JNI_LOG_WARNING(...)

namespace FFL {
	namespace jni {
		//
		//  获取，设置虚拟机
		//
		extern JavaVM *getJavaVM();

		extern void setJavaVM(JavaVM *jvm);

		//
		//  获取当前线程的env
		//
		extern void getJNIEnv(JNIEnv **env);

		enum JNIJavaMethodType {
			UNKNOWN = 0,
			//类静态方法
					STATIC_METHOD = 1,
			//成员，实例的方法
					INSTANCE_METHOD = 2,
			// 类中静态的field
					STATIC_FIELD = 3,
			//成员，实例的field
					INSTANCE_FIELD = 4,
		};
		typedef struct JNIJavaMethod {
			//  java层方法
			//  void postEventFromNative(String ,int ,int int);
			//参数签名" Ljava/lang/Object;III)V";，函数名postEventFromNative
			//
			const char *mJavaMethodOrFieldName;
			const char *mJavaMethodOrFieldSign;
			//
			//  native层保存的java层的句柄
			//
			JNIJavaMethodType mType;
			union {
				jfieldID mFieldId;
				jmethodID mMethodId;
			};
		} JNIJavaMethod;

		//
		//  一个java类，获取java层先关函数，成员变量对应native的句柄
		//
		class JavaClassBase {
		public:
			JavaClassBase(JNIEnv &env, const char *sign);

			virtual ~JavaClassBase();

		public:
			jclass getJClass() const;

			const char *getClassSign() const;

			//
			//  调用java层函数
			//
#define callJavaStaticMethod(env, ...) env.CallStaticVoidMethod(env,mClass,__VA_ARGS__)
#define callJavaMethod(env, obj, ...) env.CallVoidMethod(env,mClass,obj, __VA_ARGS__)
		public:
			//
			//  丢出一个异常
			//
			void throwException(JNIEnv &env, const char *msg);

			//
			//查找类中函数
			//
			jmethodID getMethodId(JNIEnv &env, const char *name, const char *sign);

			//
			//  查找静态函数
			//
			jmethodID getStaticMethodId(JNIEnv &env, const char *name, const char *sign);

			//
			//  查找成员变量
			//
			jfieldID getFieldId(JNIEnv &env, const char *name, const char *sign);

			//
			//  查找静态成员变量
			//
			jfieldID getStaticFieldId(JNIEnv &env, const char *name, const char *sign);

		protected:
			//
			//  java类句柄
			//
			jclass mClass;
			//
			//类的签名
			//
			const char *mClassSign;
		};

		//
		//  查找对应签名的类
		//
		jclass JNIfindClass(JNIEnv &env, const char *sign);

		//
		//  清空当前的异常,返回是否存在异常的
		//
		bool JNIclearException(JNIEnv &env);

		//
		//  创建全局引用
		//
		jobject JNInewGlobalRef(JNIEnv &env, jobject obj);

		void JNIdeleteGlobalRef(JNIEnv &env, jobject obj);

		//
		//  减少局部引用
		//
		void JNIdeleteLocalRef(JNIEnv &env, jobject obj);

		//
		// 获取java层数组数据指针
		//
		jbyte *JNIgetPtrFromByteArray(JNIEnv &env, jbyteArray bytearray);

		//
		// 释放局部引用
		//
		void JNIreleasePtrFromByteArray(JNIEnv &env, jbyteArray bytearray, void *data);

		//
		//  创建java层bytearray ,不用以后需要 JNIdeleteGlobalRef
		//
		jbyteArray JNInewByteArray(JNIEnv &env, jsize capacity);

		//
		// 写数据到bytearray中
		//
		void JNIwriteByteArray(JNIEnv &env, jbyteArray byteArray, jsize capacity, uint8_t *data,
							   uint32_t dataLen);


		class JniString{
		public:
			explicit JniString(JNIEnv &env, jstring& str):mEnv(env),
														 mJString(str){
				mNString=mEnv.GetStringUTFChars(mJString, 0);
			}
			~JniString(){
				//mEnv.ReleaseStringUTFChars(mJString, mNString);
			}
			const char * NString() const{
				return mNString;
			}
		private:
			//JniString(const JniString& );
			//JniString&operator=();
		private:
			JNIEnv &mEnv;
			jstring& mJString;
			const char* mNString;
		};
	}
}
#endif
