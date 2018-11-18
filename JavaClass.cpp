/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  JavaClass.cpp   
*  Created by zhufeifei(34008081@qq.com) on 2018/06/28 
*  https://github.com/zhenfei2016/FFL-v2.git
*  android jni帮助函数，通过java包名获取java层的类的一些信息
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

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		JavaClass::JavaClass(JNIEnv &env,ExportConfig *config) :
				JavaClassBase(env, config->mJavaClassSignName) {
			mJniBaseId=NULL;
			mConfig = config;
			init(env);
		}

		JavaClass::~JavaClass() {
			JNIEnv *env = 0;
			getJNIEnv(&env);
			unInit(*env);
		}

		//
		// 初始化，反初始化，用这个类的其他函数前需要执行的
		//
		bool JavaClass::init(JNIEnv &env) {
			if (mClass) {
				return true;
			}

			if (mClassSign == NULL || mClassSign[0] == 0) {
				return false;
			}

			jclass clssz = JNIfindClass(env, mClassSign);
			if (clssz) {
				mClass = (jclass) JNInewGlobalRef(env, clssz);
				JNIdeleteLocalRef(env, clssz);
			}

			//
			// 获取保存jnibase的java层地址
			//
			if(mConfig!=NULL &&
					mConfig->mJniBaseName !=NULL &&
					mConfig->createNativeObject) {

				mJniBaseId = getFieldId(env, mConfig->mJniBaseName,JNIBASE_SIGN_NAME);
				JNIclearException(env);
			}
			return mClass != NULL;
		}

		void JavaClass::unInit(JNIEnv &env) {
			if (mClass) {
				JNIdeleteGlobalRef(env, mClass);
				mClass = NULL;
			}
		}

		//
		//  thiz      ： java层对象
		//  weakparam : 弱引用参数
		//
		NativeObject *JavaClass::createNativeInstace(JNIEnv &env, jobject thiz, jobject weakparam) {
			if (mConfig==NULL||mConfig->createNativeObject == NULL || mJniBaseId ==NULL) {
				return NULL;
			}

			//
			//  获取java层 thiz对象保存的 JNiBase对象
			//
			jobject jnibaseJava=env.GetObjectField(thiz,mJniBaseId);
			JniBase* jniBase=new JniBase(&env,jnibaseJava);
			//
			//  创建native层对象
			//
			NativeObject *obj = mConfig->createNativeObject (&env, thiz, weakparam, this);
			obj->mJniBase=jniBase;
			jniBase->nativePtrSaveToJava(&env,(obj));
			return obj;
		}

		void JavaClass::destroyNativeInstace(JNIEnv &env,jobject thiz) {
			if (mConfig==NULL||mConfig->destroyNativeObject == NULL || mJniBaseId ==NULL) {
				return ;
			}

			//
			//  jnibase中获取保存的指针
			//
			jobject jnibaseJava=env.GetObjectField(thiz,mJniBaseId);
			JNIclearException(env);
			JniBase jniBase(&env,jnibaseJava);

			NativeObject *obj = jniBase.nativePtrLoadFromJava(&env);
			if (obj != NULL) {
				mConfig->destroyNativeObject(&env, obj, this);
			}
		}
		//
		//  通过thiz获取java层的native指针
		//
		NativeObject* JavaClass::getNativePtr(JNIEnv &env, jobject thiz){
			if (NULL== mJniBaseId) {
				return NULL;
			}

			//
			//  jnibase中获取保存的指针
			//
			jobject jnibaseJava=env.GetObjectField(thiz,mJniBaseId);
			JNIclearException(env);
			JniBase jniBase(&env,jnibaseJava);

			return  jniBase.nativePtrLoadFromJava(&env);
		}
		//
		//  导出一系列native层函数
		//
		bool JavaClass::exportNativeMethodOrField(JNIEnv &env, JNINativeMethod *methodList, int count) {
			if (mClass == NULL) {
				JNI_LOG_WARNING("Field to importJavaMethod, class is null ");
				return false;
			}

			if (methodList == NULL || count<=0) {
				return false;
			}

			env.RegisterNatives(mClass, methodList, count);
			return true;
		}

		//
		//  导入java层函数，成员
		//
		bool JavaClass::importJavaMethodOrField(JNIEnv &env, JNIJavaMethod *methodOrFieldList,
												int count) {
			if (mClass == NULL) {
				JNI_LOG_WARNING("Field to importJavaMethod, class is null ");
				return false;
			}

			if (methodOrFieldList == NULL || count<=0) {
				return false;
			}

			for (int i = 0; i < count; i++) {
				JNIJavaMethod *mth = methodOrFieldList + i;
				if (mth->mJavaMethodOrFieldName == NULL || mth->mJavaMethodOrFieldSign == NULL) {
					continue;
				}

				switch (mth->mType) {
					case STATIC_FIELD: {
						mth->mFieldId = getStaticFieldId(env, mth->mJavaMethodOrFieldName,
														 mth->mJavaMethodOrFieldSign);
						JNIclearException(env);
						if (mth->mFieldId == NULL) {
							JNI_LOG_WARNING("static field: %s %s %s not find. ", mClassSign,
											mth->mJavaMethodOrFieldName,
											mth->mJavaMethodOrFieldSign);
						}
						methodOrFieldList[i].mFieldId = mth->mFieldId;
					}
						break;
					case INSTANCE_FIELD: {
						mth->mFieldId = getFieldId(env, mth->mJavaMethodOrFieldName,
												   mth->mJavaMethodOrFieldSign);
						JNIclearException(env);
						if (mth->mFieldId == NULL) {
							JNI_LOG_WARNING("instance field: %s %s %s not find. ", mClassSign,
											mth->mJavaMethodOrFieldName,
											mth->mJavaMethodOrFieldSign);
						}
						methodOrFieldList[i].mFieldId = mth->mFieldId;
					}
						break;
					case STATIC_METHOD: {
						mth->mMethodId = getStaticMethodId(env, mth->mJavaMethodOrFieldName,
														   mth->mJavaMethodOrFieldSign);
						JNIclearException(env);
						if (mth->mMethodId == NULL) {
							JNI_LOG_WARNING("static method: %s %s %s not find. ", mClassSign,
											mth->mJavaMethodOrFieldName,
											mth->mJavaMethodOrFieldSign);
						}
						methodOrFieldList[i].mMethodId = mth->mMethodId;
					}
						break;
					case INSTANCE_METHOD: {
						mth->mMethodId = getMethodId(env, mth->mJavaMethodOrFieldName,
													 mth->mJavaMethodOrFieldSign);
						JNIclearException(env);
						if (mth->mMethodId == NULL) {
							JNI_LOG_WARNING("instance method: %s %s %s not find. ", mClassSign,
											mth->mJavaMethodOrFieldName,
											mth->mJavaMethodOrFieldSign);
						}
						methodOrFieldList[i].mMethodId = mth->mMethodId;
					}
						break;
					default:
						break;
				}
			}
			return true;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		NativeObject::NativeObject() :  mJavaObject(NULL) {
			mJniBase=NULL;
		}
		NativeObject::~NativeObject() {
		}

		//
		//  保存一个跟这个对象关联的java层对象
		//
		JavaObject* NativeObject::setJavaObject(JavaObject *obj) {
            JavaObject* ob=mJavaObject;
			mJavaObject = obj;
            return ob;
		}
		JavaObject *NativeObject::getJavaObject() const {
			return mJavaObject;
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//
		//  javaObj:java层对象，golbal对象
		//   autoDel: 是否当这个JavaFFLPlayer对象删除的时候，需要自动删除java层对象
		//
		JavaObject::JavaObject(jobject javaObj, bool autoDel, JNIJavaMethod *methodList) :
		 	mJavaObj(javaObj),
				mAutoDel(autoDel) {
			mMethodList = methodList;
		}

		JavaObject::~JavaObject() {
			if (mAutoDel && mJavaObj != NULL) {
				JNIEnv *env = 0;
				getJNIEnv(&env);
				if (env) {
					JNIdeleteGlobalRef(*env, mJavaObj);
				}
			}
		}

		JNIJavaMethod *JavaObject::getMethodInfo(int index) {
			return mMethodList + index;
		}

		//
		//  java层的对象
		//
		jobject JavaObject::getJavaObject() const {
			return mJavaObj;
		}
	}
}

