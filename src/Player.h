#pragma once
#include <string>
#include <iostream>
#include "Library.h"
#include <gst/gst.h>
#include <gst/audio/streamvolume.h>
#include <thread>

typedef struct _GstData
{
	GstElement *playbin;
	GMainLoop *main_loop;
	bool eos;
} GstData;

class Player
{
private:
	std::thread m_GstThread;
	GstElement *m_Playbin = nullptr;
	GMainLoop *m_GstLoop = nullptr;
	static gboolean HandleMessages (GstBus *bus, GstMessage *msg, GstData *data);


public:
    void Init(const std::string& filePath, bool startup = false);
    void Pause();
    void Play();
    void SetVolume(float volume);
    float GetVolume();
	double GetLengthInSeconds();
    uint64_t GetLength();
    double GetCursorInSeconds();
    uint64_t GetCursor();
    void SetCursor(uint64_t cursor);
    void SetPlaylist(Playlist playlist);
    void SetPlaylist(Playlist playlist, int index);
    void Next();
    void Prev();
    void UpdateTitle();
	void Stop();
	bool GetEOS();
	static void GstThread(GstElement *pipeline, GMainLoop *loop, GstData* data);
	bool m_Startup = false;
    bool m_IsPaused = true;
    bool m_IsMuted = false;
    float m_Volume = 1;
    Library m_Library;
    Playlist m_CurrentPlaylist;
    int m_CurrentSongIndex = 0;
    std::string m_CurrentSongTitle;
	GstData m_Data;
};


