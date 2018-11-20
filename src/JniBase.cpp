/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  JniBase.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/06/28 
*  https://github.com/zhenfei2016/FFL-v2.git
*  保存java层的一些东西
*
*/
#include <string.h>
#include <malloc.h>
#include <jni.h>
#include "JavaClass.hpp"
#include "JavaClassManager.hpp"
#include "JniBase.hpp"

namespace FFL {
	namespace jni {
		const char* JNIBASE_SIGN_NAME="ffl/jnihelper/JniBase";
		static JNINativeMethod gJnibaseNativeMethodTables[] = {
				{ "native_setup","(Ljava/lang/String;Ljava/lang/Object;Ljava/lang/Object;)V",(void *)JniBase::setup },
				{ "native_release","(Ljava/lang/String;Ljava/lang/Object;)V",(void *)JniBase::release },
				{0,0,0}
		};

		static JNIJavaMethod gJnibaseJavaMethodTables[]={
				{"mNativeInstancePtr","J",INSTANCE_FIELD,NULL},
				{0,0,UNKNOWN,0}
		};

		static  ExportConfig gJnibaseConfig={
				//
				//  java层的JniBase对应的名称
				//
				.mJniBaseName=NULL,
				//
				//  创建，删除nativeobject接口函数指针
				//
				.createNativeObject=NULL,
				.destroyNativeObject=NULL,
				//
				//  导出的native函数列表
				//
				.mNativeMethodList=gJnibaseNativeMethodTables,
				//
				//  导入的java层函数个数和列表
				//
				.mJavaMethodList=gJnibaseJavaMethodTables,
				//
				//  native层class
				//
				.mJavaClass=NULL,
				.mJavaClassSignName=JNIBASE_SIGN_NAME,
		};

		//JNI_REGISTER(Jnibase)
		void JNIExportRegisterClassJnibase(){
			JniDeclareClass(&gJnibaseConfig);
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//  hostJavaObj:宿主对象，包含这个对象的
		//
		JniBase::JniBase(JNIEnv* env,jobject javaJnibase):
				mNativeInstancePtrFieldId(0),
				mJavaJniBase(javaJnibase){
			init(env,mJavaJniBase);
		}
		JniBase::~JniBase() {
		}
		void JniBase::init(JNIEnv* env,jobject javaJnibase){
			mNativeInstancePtrFieldId=gJnibaseJavaMethodTables[0].mFieldId;
		}
		//
		//  保存native指针到java层
		//
		void JniBase::nativePtrSaveToJava(JNIEnv* env,NativeObject* obj){
			if(mJavaJniBase==NULL || mNativeInstancePtrFieldId ==NULL ){
				return ;
			}

			env->SetLongField(mJavaJniBase,
							  mNativeInstancePtrFieldId,
							 (jlong)obj);
		}
		//
		//  从java获取保存的native指针
		//
		NativeObject* JniBase::nativePtrLoadFromJava(JNIEnv* env){
			if(mJavaJniBase==NULL || mNativeInstancePtrFieldId ==NULL ){
				return NULL;
			}

			jlong ptr = env->GetLongField(mJavaJniBase, mNativeInstancePtrFieldId);
			return  (NativeObject*)(ptr);
		}
		//
		//  java层使用的native_setup函数
		//   objSign  ：java层对象对应的类签名
		//   obj      :java层对象指针，这个对象将于native层的对象关联上去
		//  weakparam :  java层传递的弱引用指针
		//
		void JniBase::setup(JNIEnv *env,jclass thiclass, jstring objSign, jobject obj, jobject weakparam){
            JniString jniString(*env, objSign);
            JavaClass *pThis=JniFindRegisteredClassByName(jniString.NString());
			if (pThis) {
				//
				//  创建native层对象
				//
				NativeObject *ptr = pThis->createNativeInstace(*env, obj, weakparam);
			}
		}

		//
		//  java层使用的native_release函数
		//  java层对象不用前了，调用此函数，进行一些释放操作
		//
		void JniBase::release(JNIEnv *env,jclass thiclass, jstring objSign, jobject obj){
			JniString jniString(*env,objSign);
			JavaClass *pThis = JniFindRegisteredClassByName(jniString.NString());
			if (pThis) {
				pThis->destroyNativeInstace(*env, obj);
			}
		}
	}
}

