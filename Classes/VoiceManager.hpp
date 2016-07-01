//
//  VoiceManager.hpp
//  MyCppGame
//
//  Created by wangxin on 15/12/1.
//
//

#ifndef VoiceManager_hpp
#define VoiceManager_hpp

#include <stdio.h>
#include "RTChatSdk.h"

using namespace rtchatsdk;

class VoiceManager {
public:
    VoiceManager();
    virtual ~VoiceManager();

    void callBack(SdkResponseCmd cmdType, SdkErrorCode error, const std::string& msgStr);
    
    void startRecord();
    
    void stopRecord();
    
    void startPlay();
private:
    struct VoiceInfo
    {
        std::string url;
        int duration;
    };

    VoiceInfo* info;
};

#endif /* VoiceManager_hpp */
