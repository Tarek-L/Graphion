// Note use the Absolute Path

#pragma once

#include <log/logging.hpp>

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio/miniaudio.h> 

#define STREAM              0x00000001
#define DECODE              0x00000002
#define ASYNC               0x00000004
#define LOOPING             0x00000008
#define NO_SPATIALIZATION   0x00000010
#define NO_PITCH            0x00000020
#define NO_GAIN             0x00000040
#define DECODE_ONLY         0x00000080
#define SUCCESS MA_SUCCESS

namespace Graphion::Audio{

class Audio {
public:
    static void Init() {
        if (s_Initialized) return;

        ma_result result = ma_engine_init(nullptr, &s_Engine);
        ASSERT(result == SUCCESS, "Failed to initialize miniaudio engine");

        s_Initialized = (result == SUCCESS);
        LOG_INFO("Audio System Initialized");
    }

    static void Shutdown() {
        if (!s_Initialized) return;

        ma_engine_uninit(&s_Engine);
        s_Initialized = false;
        LOG_INFO("Audio System Shutdown");
    }

    static ma_engine* getEngine() { return &s_Engine; }
    static bool IsInitialized() { return s_Initialized; }

private:
    static inline ma_engine s_Engine;
    static inline bool s_Initialized = false;

    Audio() = delete;
};

class Sound {
public:
    static std::unique_ptr<Sound> create(const std::string& filePath, uint32_t flags = 0) {
        ASSERT(Audio::IsInitialized(), "Cannot create sound: Audio engine not initialized");
        
        auto soundInstance = std::unique_ptr<Sound>(new Sound());
        
        ma_result result = ma_sound_init_from_file(
            Audio::getEngine(), 
            filePath.c_str(), 
            flags, 
            nullptr, 
            nullptr, 
            &soundInstance->m_Handle
        );

        if (result != SUCCESS) {
            LOG_ERROR("Failed to load sound file: {}", filePath);
            return nullptr;
        }

        soundInstance->m_Valid = true;
        return soundInstance;
    }

    ~Sound() {
        if (m_Valid) {
            ma_sound_uninit(&m_Handle);
        }
    }

    void play() {
        ASSERT(m_Valid, "Attempted to Play an invalid sound");
        ma_sound_start(&m_Handle);
    }

    void pause() {
        ASSERT(m_Valid, "Attempted to Pause an invalid sound");
        ma_sound_stop(&m_Handle);
    }

    void stop() {
        ASSERT(m_Valid, "Attempted to Stop an invalid sound");
        ma_sound_stop(&m_Handle);
        ma_sound_seek_to_pcm_frame(&m_Handle, 0);
    }

    void restart() {
        stop();
        play();
    }

    bool isPlaying() const {
        if (!m_Valid) return false;
        return ma_sound_is_playing(&m_Handle);
    }

    void setVolume(float volume) {
        ASSERT(m_Valid, "Attempted to SetVolume on an invalid sound");
        ma_sound_set_volume(&m_Handle, volume);
    }

    float getVolume() const {
        if (!m_Valid) return 0.0f;
        return ma_sound_get_volume(&m_Handle);
    }

    bool isValid() const { return m_Valid; }

private:
    Sound() : m_Valid(false) {}

    ma_sound m_Handle;
    bool m_Valid;

    Sound(const Sound&) = delete;
    Sound& operator=(const Sound&) = delete;
};
}
