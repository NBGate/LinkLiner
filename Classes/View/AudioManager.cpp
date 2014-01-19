//
//  AudioManager.cpp
//  LinkLiner
//
//  Created by 周巍然 on 19/1/14.
//  refer http://blog.csdn.net/ym19860303/article/details/8534200
//

#include "AudioManager.h"
#include <SimpleAudioEngine.h>


void AudioManager::init() {
    //preLoadBgMusic(bgMusic);
    //preLoadSound(testSound);
}

void AudioManager::preLoadBgMusic(Music& music) {
    music.musicFilePath = CCFileUtils::sharedFileUtils()->fullPathForFilename(music.musicFilePath).c_str();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(music.musicFilePath);
}

void AudioManager::preLoadSound(Sound& sound)
{
    sound.soundFilePath = CCFileUtils::sharedFileUtils()->fullPathForFilename(sound.soundFilePath).c_str();
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(sound.soundFilePath);
}

void AudioManager::setBgMusicVolume(float volume) {
    CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(volume);
}

void AudioManager::playMusic(const Music& music, bool loop/* default = true */)
{
    /* 先判断是否已经在播放,如果在播放，则直接返回 */
    if (CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()) {
        return;
    }
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(music.musicFilePath, loop);
}

void AudioManager::playSound(Sound& sound,bool loop/* default is false */)
{
    sound.soundId = CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(sound.soundFilePath, loop);
}

void AudioManager::pauseMusic(Music& music)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

void AudioManager::pauseSound(Sound& sound)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseEffect(sound.soundId);
}


void AudioManager::stopMusic(Music& music, bool release/* default is true */)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(release);
}

void AudioManager::stopSound(Sound& sound)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopEffect(sound.soundId);
}

void AudioManager::stopAllSound()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
}

void AudioManager::resumeMusic()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

void AudioManager::end()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->end();
}





