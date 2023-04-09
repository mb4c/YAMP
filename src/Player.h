#pragma once
#include <string>
#include <iostream>
#include "miniaudio.h"
#include "Library.h"

class Player
{
private:
	ma_result m_Result;
	ma_engine* m_Engine = nullptr;
	ma_sound m_Sound;
	ma_engine_config engineConfig;

public:
    void Init(const std::string& filePath, bool startup = false);
    void Pause();
    void Play();
    void SetVolume(float volume);
    float GetVolume();
	float GetLengthInSeconds();
    uint64_t GetLength();
    float GetCursorInSeconds();
    uint64_t GetCursor();
    void SetCursor(uint64_t cursor);
    void SetPlaylist(Playlist playlist);
    void SetPlaylist(Playlist playlist, int index);
    void Next();
    void Prev();
    void UpdateTitle();

	bool m_Startup = false;
    bool m_IsPaused = true;
    bool m_IsMuted = false;
    float m_Volume = 1;
    Library m_Library;
    Playlist m_CurrentPlaylist;
    int m_CurrentSongIndex = 0;
    std::string m_CurrentSongTitle;
};

