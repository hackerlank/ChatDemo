//
//  RTChatSdkAndroidImpl.h
//  RTChat
//
//  Created by yibin on 14-8-14.
//  Copyright (c) 2014骞� yunwei. All rights reserved.
//
#include "RTChatSdkAndroidIpml.h"
#include "RTChatCommonTypes.h"

#include <android/log.h>
#include <unordered_map>
#include <stdlib.h>
#include <string>
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "platform/CCFileUtils.h"

#define MAX_BUFFER_SIZE 1024

template <class T>
inline void constructDynamic(T *ptr) {
    new ((void*)ptr) T();
};

#define BUFFER_CMD(cmd, name, len) char buffer##name[len]; \
    cmd* name = (cmd*)buffer##name; \
    constructDynamic(name); \



 #define LOGD(msg) \
     __android_log_print(ANDROID_LOG_ERROR, "RTChatSDK", "%s:%d: %s", __FILE__, \
                        __LINE__, msg); \

#define JVOEOBSERVER(rettype, name)                                             \
  extern "C" rettype JNIEXPORT JNICALL Java_com_lw_RecordImage_GameVoiceManager_##name

#define JAVATAROBSERVER(rettype, name)                                             \
  extern "C" rettype JNIEXPORT JNICALL Java_com_lw_RecordImage_GameAvatar_##name

#define  CLASS_NAME "com/lw/RecordImage/GameVoiceManager"
#define  AVATAR_CLASS_NAME "com/lw/RecordImage/GameAvatar"

USING_NS_CC;

namespace rtchatsdk {
    static  RTChatSDKMain* s_RTChatSDKMain = NULL;
    static jobject evoeObject = NULL;


    RTChatSDKMain& RTChatSDKMain::sharedInstance()
    {
        if (!s_RTChatSDKMain) {
            s_RTChatSDKMain = new RTChatSDKMain();
            
        }
        
        return *s_RTChatSDKMain;
    }

    jobject RTChatSDKMain::GetEVOEObject()
    {
        cocos2d::JniMethodInfo voemethodInfo;
        if (! cocos2d::JniHelper::getStaticMethodInfo(voemethodInfo,CLASS_NAME, "GetIntance", "()Lcom/ztgame/embededvoice/VoiceChannelEngine;"))
        {
          return NULL;
        }
        
        jobject evoe = voemethodInfo.env->CallStaticObjectMethod(voemethodInfo.classID, voemethodInfo.methodID);

        return evoe;
    }


    SdkErrorCode RTChatSDKMain::initSDK(const std::string& appid, const std::string& key,const char* uniqueid)
    {
//        CCLOG(">>>>>>>initSDK start");
//         cocos2d::JniMethodInfo voemethodInfo;
//         if (! cocos2d::JniHelper::getStaticMethodInfo(voemethodInfo,CLASS_NAME,"Init","(Ljava/lang/String;)V"))
//         {
//           return OPERATION_FAILED;
//         }
//
//         jstring jappid = voemethodInfo.env->NewStringUTF(appid.c_str());
//        voemethodInfo.env->CallStaticObjectMethod(voemethodInfo.classID, voemethodInfo.methodID,jappid);
//
//        CCLOG(">>>>>>>initSDK stop");

        return OPERATION_OK;
    }

    void RTChatSDKMain::registerMsgCallback(const pMsgCallFunc& func)
    {
       callbackfunc = func;
    }


    void RTChatSDKMain::setParams(const std::string& voiceUploadUrl, const char* xunfeiAppID)
    {
        CCLOG(">>>>>setParams1 %s, %s", voiceUploadUrl.c_str(), xunfeiAppID);

        cocos2d::JniMethodInfo voemethodInfo;
        if (cocos2d::JniHelper::getStaticMethodInfo(voemethodInfo,CLASS_NAME, "SetParams", "(Ljava/lang/String;Ljava/lang/String;)V"))
        {
            jstring jurl = voemethodInfo.env->NewStringUTF(voiceUploadUrl.c_str());

            jstring jXunfei;
            if (xunfeiAppID != NULL) {
                jXunfei = voemethodInfo.env->NewStringUTF(xunfeiAppID);
            }

            CCLOG(">>>>>setParams2");
            voemethodInfo.env->CallStaticVoidMethod(voemethodInfo.classID, voemethodInfo.methodID, jurl, jXunfei);

            voemethodInfo.env->DeleteLocalRef(jurl);

            if (xunfeiAppID != NULL) {
                voemethodInfo.env->DeleteLocalRef(jXunfei);
            }

            voemethodInfo.env->DeleteLocalRef(voemethodInfo.classID);

            CCLOG(">>>>>setParams3");
        }
    }

    bool RTChatSDKMain::startRecordVoice(unsigned int labelid, bool needTranslate)
    {
        cocos2d::JniMethodInfo voemethodInfo;
        if (! cocos2d::JniHelper::getStaticMethodInfo(voemethodInfo,CLASS_NAME, "startRecordWithIndex", "(IZ)V"))
        {
          return NULL;
        }
        
        voemethodInfo.env->CallStaticVoidMethod(voemethodInfo.classID, voemethodInfo.methodID,labelid,needTranslate);
        voemethodInfo.env->DeleteLocalRef(voemethodInfo.classID);

        return true;
    }

    bool RTChatSDKMain::stopRecordVoice()
    {
        cocos2d::JniMethodInfo voemethodInfo;
        if (! cocos2d::JniHelper::getStaticMethodInfo(voemethodInfo,CLASS_NAME, "stopRecording", "()V"))
        {
          return NULL;
        }
        
        voemethodInfo.env->CallStaticVoidMethod(voemethodInfo.classID, voemethodInfo.methodID);

        return true;
    }

    bool RTChatSDKMain::startPlayLocalVoice(unsigned int labelid, const char* voiceUrl)
    {
        JniMethodInfo methodInfo;
        if (!JniHelper::getStaticMethodInfo(methodInfo,CLASS_NAME, "startPlayingWithIndex", "(ILjava/lang/String;)V"))
        {
          return NULL;
        }
        
        jstring jurl = methodInfo.env->NewStringUTF(voiceUrl);
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,labelid,jurl);


        methodInfo.env->DeleteLocalRef(jurl);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);

        CCLOG(">>>>>startPlayLocalVoice2 %d, %s", labelid, voiceUrl);
        return true;
    }

    bool RTChatSDKMain::stopPlayLocalVoice()
    {
    	cocos2d::JniMethodInfo voemethodInfo;

		if (! cocos2d::JniHelper::getStaticMethodInfo(voemethodInfo,CLASS_NAME, "stopPlayingFile", "()V"))
		{
		  return NULL;
		}

		voemethodInfo.env->CallStaticVoidMethod(voemethodInfo.classID, voemethodInfo.methodID);

		return true;
    }

    bool RTChatSDKMain::cancelRecordedVoice()
    {

        cocos2d::JniMethodInfo voemethodInfo;
        if (! cocos2d::JniHelper::getStaticMethodInfo(voemethodInfo,CLASS_NAME, "cancelRecordingVoice", "()V"))
        {
          return NULL;
        }
        
        voemethodInfo.env->CallStaticVoidMethod(voemethodInfo.classID, voemethodInfo.methodID);

        return true;
    }

    bool RTChatSDKMain::setAvater(unsigned int uid,int type)
    {   
      
      cocos2d::JniMethodInfo imgmethodInfo;
      if (! cocos2d::JniHelper::getStaticMethodInfo(imgmethodInfo,AVATAR_CLASS_NAME, "setAvaterWithUid", "(II)V"))
        {
          return NULL;
        }
        
        imgmethodInfo.env->CallStaticVoidMethod(imgmethodInfo.classID, imgmethodInfo.methodID,uid,type);
      return true;
    }

    bool RTChatSDKMain::getAvater(unsigned int uid,int type,const char* imageUrl)
    {

        cocos2d::JniMethodInfo imgmethodInfo;
        if (! cocos2d::JniHelper::getStaticMethodInfo(imgmethodInfo,AVATAR_CLASS_NAME, "GetAvaterWithUid", "(IILjava/lang/String;)V"))
        {
          return NULL;
        }
        
        jstring jurl;
        if(imageUrl != NULL)
            jurl = imgmethodInfo.env->NewStringUTF(imageUrl);
        imgmethodInfo.env->CallStaticVoidMethod(imgmethodInfo.classID, imgmethodInfo.methodID,uid,type,jurl);

        return true;
    }

    bool RTChatSDKMain::startVoiceToText()
    {
        
        cocos2d::JniMethodInfo imgmethodInfo;
        if (! cocos2d::JniHelper::getStaticMethodInfo(imgmethodInfo,CLASS_NAME, "startVoiceToText", "()V"))
        {
            return NULL;
        }
        
        imgmethodInfo.env->CallStaticVoidMethod(imgmethodInfo.classID, imgmethodInfo.methodID);
        return true;
    }

    bool RTChatSDKMain::stopVoiceToText()
    {
        
        cocos2d::JniMethodInfo imgmethodInfo;
        if (! cocos2d::JniHelper::getStaticMethodInfo(imgmethodInfo,CLASS_NAME, "stopVoiceToText", "()V"))
        {
            return NULL;
        }
        
        imgmethodInfo.env->CallStaticVoidMethod(imgmethodInfo.classID, imgmethodInfo.methodID);
        return true;
    }

    bool RTChatSDKMain::startQueryGeoInfo()
    {
        cocos2d::JniMethodInfo imgmethodInfo;
        if (! cocos2d::JniHelper::getStaticMethodInfo(imgmethodInfo, CLASS_NAME, "startQueryGeoInfo", "()V"))
        {
            return NULL;
        }
        
        imgmethodInfo.env->CallStaticVoidMethod(imgmethodInfo.classID, imgmethodInfo.methodID);
        return true;
    }
    
    uint64_t RTChatSDKMain::convertJstring2Uint64(jstring longstring)
    {
        std::string sTempID = cocos2d::JniHelper::jstring2string(longstring);
        uint64_t  temp64id = strtoull(sTempID.c_str(),NULL,10);
        return temp64id;
    }


    JVOEOBSERVER(int,RecordingStop)(JNIEnv* jni,jobject thiz ,jboolean isok,jstring result)
    {
      LOGD("---------------------");
       std::string cResult = cocos2d::JniHelper::jstring2string(result);
      return 0;
    }

    JVOEOBSERVER(int,RecordingUploadEnd)(JNIEnv* jni,jobject thiz ,jboolean isok,jstring result)
    {
      LOGD("---------------------");
      std::string cResult = cocos2d::JniHelper::jstring2string(result);
      if(isok)
      {
        RTChatSDKMain::sharedInstance().callbackfunc(enRequestRec, OPERATION_OK,cResult);

      }else
      {
        RTChatSDKMain::sharedInstance().callbackfunc(enRequestRec, OPERATION_FAILED, cResult);
      }
      return 0;
    }

    JVOEOBSERVER(int,RecordingVoiceDownloadBegin)(JNIEnv* jni,jobject thiz,jboolean isok,jstring result)
    {
      LOGD("---------------------");
      return 0;
    }

    JVOEOBSERVER(int,RecordingVoiceDownloadEnd)(JNIEnv* jni,jobject thiz ,jboolean isok,jstring result)
    {
      LOGD("---------------------");
       std::string cResult = cocos2d::JniHelper::jstring2string(result);
      if(isok)
      {
        RTChatSDKMain::sharedInstance().callbackfunc(enRequestPlay, OPERATION_OK,cResult);

      }else
      {
        RTChatSDKMain::sharedInstance().callbackfunc(enRequestPlay, OPERATION_FAILED, cResult);
      }
      return 0;
    }

    JVOEOBSERVER(int,RecordingPlayVoiceOnStop)(JNIEnv* jni,jobject thiz ,jboolean isok,jstring result)
    {
      LOGD("---------------------");
       std::string cResult = cocos2d::JniHelper::jstring2string(result);
      if(isok)
      {
        RTChatSDKMain::sharedInstance().callbackfunc(enNotifyPlayOver, OPERATION_OK,cResult);

      }else
      {
        RTChatSDKMain::sharedInstance().callbackfunc(enNotifyPlayOver, OPERATION_FAILED, cResult);
      }
      return 0;
    }


    JVOEOBSERVER(int,RecordingVoiceOnVolume)(JNIEnv* jni,jobject thiz ,jboolean isok,jstring result)
    {
      LOGD("---------------------");
       std::string cResult = cocos2d::JniHelper::jstring2string(result);
      if(isok)
      {
        RTChatSDKMain::sharedInstance().callbackfunc(enNotifyRecordPower, OPERATION_OK,cResult);

      }else
      {
        RTChatSDKMain::sharedInstance().callbackfunc(enNotifyRecordPower, OPERATION_FAILED, cResult);
      }
      return 0;
    }



    JAVATAROBSERVER(void,setAvaterCallback)(JNIEnv* jni,jobject thiz ,jboolean isok,jstring result)
    {
      LOGD("---------------------");
       std::string cResult = cocos2d::JniHelper::jstring2string(result);
      if(isok)
      {
        RTChatSDKMain::sharedInstance().callbackfunc(enReqSetAvaterResult, OPERATION_OK,cResult);

      }else
      {
        RTChatSDKMain::sharedInstance().callbackfunc(enReqSetAvaterResult, OPERATION_FAILED, cResult);
      }
      
    }

    JAVATAROBSERVER(void,getAvaterCallback)(JNIEnv* jni,jobject thiz ,jboolean isok,jstring result)
    {
      LOGD("---------------------");
       std::string cResult = cocos2d::JniHelper::jstring2string(result);
      if(isok)
      {
        RTChatSDKMain::sharedInstance().callbackfunc(enReqGetAvaterResult, OPERATION_OK,cResult);

      }else
      {
        RTChatSDKMain::sharedInstance().callbackfunc(enReqGetAvaterResult, OPERATION_FAILED, cResult);
      }
      
    }
        
    JVOEOBSERVER(void,OnReceiveVoiceText)(JNIEnv* jni,jobject thiz ,jboolean isok,jstring result)
    {
//        LOGD("---------------------");
        std::string cResult = cocos2d::JniHelper::jstring2string(result);
        if(isok)
        {
            RTChatSDKMain::sharedInstance().callbackfunc(enNotifyVoiceTextResult, OPERATION_OK,cResult);
            
        }else
        {
            RTChatSDKMain::sharedInstance().callbackfunc(enNotifyVoiceTextResult, OPERATION_FAILED, cResult);
        }
        
    }

    JVOEOBSERVER(void,OnReceiveGeoInfoText)(JNIEnv* jni,jobject thiz ,jboolean isok,jstring result)
    {
        LOGD("---------------------");
        std::string cResult = cocos2d::JniHelper::jstring2string(result);
        if(isok)
        {
            RTChatSDKMain::sharedInstance().callbackfunc(enNotifyCoodinateInfo, OPERATION_OK,cResult);
            
        }else
        {
            RTChatSDKMain::sharedInstance().callbackfunc(enNotifyCoodinateInfo, OPERATION_FAILED, cResult);
        }
        
    }
}

