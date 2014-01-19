//
//  AudioManager.h
//  LinkLiner
//
//  Created by 周巍然 on 19/1/14.
//
//

#ifndef LinkLiner_AudioManager_h
#define LinkLiner_AudioManager_h

#include "cocos2d.h"
USING_NS_CC;

struct Sound {
    unsigned int soundId;
    const char * soundFilePath;
};

struct Music{
    const char * musicFilePath;
};

//static Music bgMusic =
//{
//    "music/back1.mp3"
//};
//
//static Sound testSound =
//{
//    0,
//    "music/win.mp3"
//};

class AudioManager
{
public:
    static void init();
    
public:
    static void preLoadBgMusic(Music& music);
    static void preLoadSound(Sound& sound);
    
    static void setBgMusicVolume(float volume);
    
    static void playMusic(const Music& music, bool loop = true);
    static void playSound(Sound& sound, bool loop = false);
    
    static void pauseMusic(Music& music);
    static void pauseSound(Sound& sound);
    
    static void stopMusic(Music& music, bool release = true);
    static void stopSound(Sound& sound);
    static void stopAllSound();
    
    static void resumeMusic();
    static void end();
    
};

#endif
