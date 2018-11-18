/*
 *  This file is part of FFL project.
 *
 *  The MIT License (MIT)
 *  Copyright (C) 2017-2018 zhufeifei All rights reserved.
 *
 *  JavaClassManager.cpp
 *  Created by zhufeifei(34008081@qq.com) on 2018/10/02
 *  https://github.com/zhenfei2016/FFL-v2.git
 *  jni导出，导入的管理类
 *
 */
#include "JavaClassManager.hpp"
#include <vector>
#include <memory.h>
#include <jni.h>
#include "JavaClass.hpp"


namespace FFL {
	namespace jni {
		class JavaClassManagerImpl {
		public:
			JavaClassManagerImpl() {
			}

			~JavaClassManagerImpl() {
			}

			int registerClass(ExportConfig *javaClass) {
				for (size_t i = 0; i < mRegistedClassList.size(); i++) {
					ExportConfig *c = mRegistedClassList[i];

				}
				mRegistedClassList.push_back(javaClass);
				return mRegistedClassList.size() - 1;
			}

			ExportConfig *findClassByName(const char *name) {
				for (size_t i = 0; i < mRegistedClassList.size(); i++) {
					if(mRegistedClassList[i]->mJavaClass==NULL){
						continue;
					}
					const char *sign = mRegistedClassList[i]->mJavaClass->getClassSign();
					if (strcmp(sign, name) == 0) {
						return mRegistedClassList[i];
					}
				}
				return NULL;
			}

			ExportConfig *findClassById(int id) {
				if (id < 0 || id >= mRegistedClassList.size()) {
					return NULL;
				}
				return mRegistedClassList[id];
			}

            void clear(){
            }
		protected:
			std::vector<ExportConfig *> mRegistedClassList;
		};

		static JavaClassManagerImpl gClassmanager;

		//
		//  注册等待导出，导入的类
		//
		int JniRegisterClass(ExportConfig *javaClass) {
			return gClassmanager.registerClass(javaClass);
		}

		//
		//  通过签名获取对应的已注册过的类
		//
		JavaClass *JniFindRegisteredClassByName(const char *signClassName) {
			ExportConfig* config= JniFindRegisteredClassConfigByName(signClassName);
			if(config){
				return  config->mJavaClass;
			}
			return NULL;
		}
        //
		//  通过签名获取对应的已注册过的类
		//
		ExportConfig *JniFindRegisteredClassConfigByName(const char *signClassName){
			return gClassmanager.findClassByName(signClassName);
		}
		//
		//  初始化
		//
		void JniInitialize(JavaVM* jvm) {
            setJavaVM(jvm);
			extern void JNIExportRegisterClassJnibase();
			JNIExportRegisterClassJnibase();
		}

        void JniTerminate(){
            gClassmanager.clear();
        }
        
		//
		//  注册所有的导出类
		//
		void JniRegisterAllClass(JNIEnv *env){
			for(int id=0;id<100;id++){
				ExportConfig* config=gClassmanager.findClassById(id);
				if(config==NULL){
					break;
				}

				config->mJavaClass=new JavaClass(*env,config->mJavaClassSignName,config);

				//导出native函数
				{
					int methodCount=0;
					JNINativeMethod *entry = config->mNativeMethodList;
					while (entry) {
						if (entry->name == NULL || entry->fnPtr == NULL) {
							break;
						}
						methodCount++;
                        entry++;
					}

					if(methodCount) {
						config->mJavaClass->exportNativeMethodOrField(*env,
																	  config->mNativeMethodList,
																	  methodCount);
					}
				}

				//导入java函数
				{
					int methodCount=0;
					JNIJavaMethod *entry = config->mJavaMethodList;
					while (entry) {
						if (entry->mJavaMethodOrFieldSign == NULL || entry->mJavaMethodOrFieldName == NULL) {
							break;
						}
						methodCount++;
                        entry++;
					}

					if(methodCount) {
						config->mJavaClass->importJavaMethodOrField(*env,
																	config->mJavaMethodList,
																	methodCount);
					}
				}
			}
		}
	}
}
