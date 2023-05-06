#pragma once
#include <string>
#include <iostream>
#include "Library.h"
#include <gst/gst.h>
#include <gst/audio/streamvolume.h>
#include <thread>

class Player
{
private:
	std::thread m_GstThread;
	GstElement *m_Playbin = nullptr;
	GMainLoop *m_GstLoop = nullptr;

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
	void Stop();
	bool GetEOS();
	static void GstThread(GstElement *pipeline, GMainLoop *loop);

	bool m_Startup = false;
    bool m_IsPaused = true;
    bool m_IsMuted = false;
    float m_Volume = 1;
    Library m_Library;
    Playlist m_CurrentPlaylist;
    int m_CurrentSongIndex = 0;
    std::string m_CurrentSongTitle;
};

