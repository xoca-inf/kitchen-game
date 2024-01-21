#include "AudioManager.h"

AudioManager::AudioManager()
{
    W4_LOG_DEBUG("[AudioManager] Loaded");
}

AudioManager::~AudioManager() {}

sptr<Audio> AudioManager::playSound(std::string path) {
    auto audio = make::sptr<Audio>(path);
    if(!isMuted)
        audio->play();

    return audio;
}

void AudioManager::playFailSound() {
    playSound(failSoundPath);
}

void AudioManager::playWinSound() {
    playSound(winSoundPath);
}

void AudioManager::playMergeSound() {
    playSound(mergeSoundPath);
}

void AudioManager::playClickSound() {
    playSound(clickSoundPath);
}

void AudioManager::playWater() {
    waterRunning = playSound(waterSoundPath);
    waterRunning->setGain(0.1f);
    waterRunning->setLooped(true);
    isWaterPlaying = true;
}

void AudioManager::playTransform() {
    playSound(objectTransformPath);
}

void AudioManager::stopWater() {
    waterRunning->stop();
    isWaterPlaying = false;
}

void AudioManager::unMute() {
    isMuted = false;
    if(isWaterPlaying){
        waterRunning->play();
    }
}

void AudioManager::mute() {
    isMuted = true;
    waterRunning->stop();
}


