#pragma once
#include "W4Framework.h"

W4_USE_UNSTRICT_INTERFACE

class AudioManager {
public:
    static AudioManager& instance() {
        static AudioManager instance;
        return instance;
    }

    AudioManager();
    virtual ~AudioManager();

    void playClickSound();
    void playFailSound();
    void playWinSound();
    void playMergeSound();
    void playWater();
    void stopWater();
    void playTransform();
    void mute();
    void unMute();

private:
    bool isMuted = false;
    std::string clickSoundPath = "sounds/Click.mp3";
    std::string failSoundPath = "sounds/Fail.mp3";
    std::string winSoundPath = "sounds/Win.mp3";
    std::string mergeSoundPath = "sounds/Item_Merges.mp3";
    std::string waterSoundPath = "sounds/WaterRunning.mp3";
    std::string objectTransformPath = "sounds/ObjectTransform.mp3";

    sptr<Audio> playSound(std::string path);

    sptr<Audio> waterRunning;
    bool isWaterPlaying;
};