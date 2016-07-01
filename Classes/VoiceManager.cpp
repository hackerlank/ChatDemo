//
//  VoiceManager.cpp
//  MyCppGame
//
//  Created by wangxin on 15/12/1.
//
//

#include "VoiceManager.hpp"
#include "cocos2d.h"

#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

using namespace  rapidjson;

VoiceManager::VoiceManager()
:info(nullptr)
{
    RTChatSDKMain::sharedInstance().initSDK("", "");
    RTChatSDKMain::sharedInstance().registerMsgCallback(std::bind(&VoiceManager::callBack, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

#ifdef __APPLE__
    RTChatSDKMain::sharedInstance().setParams("http://sdk.audio.mztgame.com/wangpan.php", "54acafe7");
#else
    RTChatSDKMain::sharedInstance().setParams("http://sdk.audio.mztgame.com/wangpan.php", "566503f0");
#endif
}

VoiceManager::~VoiceManager()
{
    
}



void VoiceManager::callBack(SdkResponseCmd cmdType, SdkErrorCode error, const std::string& msgStr)
{
    if (error == !OPERATION_OK) {
        return;
    }
    switch (cmdType) {
        case enRequestRec:
        {
            rapidjson::Document readdoc;
            readdoc.Parse<0>(msgStr.c_str());

            if (info == nullptr)
                info = new VoiceInfo();

            info->url = readdoc["url"].GetString();
            info->duration = std::atoi(readdoc["duration"].GetString());

            break;
        }
        case enNotifyVoiceTextResult:
            CCLOG("voicetext = %s", msgStr.c_str());
        default:
            break;
    }

    CCLOG("voicetext = %s", msgStr.c_str());
}

void VoiceManager::startRecord()
{
    if (1) {
        RTChatSDKMain::sharedInstance().startRecordVoice(1, false);
    }
    else {
        if (info)
            RTChatSDKMain::sharedInstance().startPlayLocalVoice(1, info->url.c_str());
    }
}

void VoiceManager::stopRecord()
{
    if (1) {
        RTChatSDKMain::sharedInstance().stopRecordVoice();
    }
    else {
        RTChatSDKMain::sharedInstance().stopPlayLocalVoice();
    }
}

void VoiceManager::startPlay()
{
    if (info)
        RTChatSDKMain::sharedInstance().startPlayLocalVoice(1, info->url.c_str());
}

