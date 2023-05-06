#include "Player.h"

void Player::Init(const std::string& filePath, bool startup)
{
	if (m_Playbin != nullptr)
	{
		Stop();
	}

	/* Initialize GStreamer */
	gst_init(nullptr, nullptr);
	std::string file;
	file = "file://";
	file.append(filePath);

	std::cout << filePath << std::endl;
	/* Create the pipeline */
	m_Playbin = gst_element_factory_make ("playbin", "playbin");
	g_object_set (m_Playbin, "uri", file.c_str(), NULL);

	/* Create a new main loop with a new context */
	m_GstLoop = g_main_loop_new(NULL, FALSE);

	/* Set the pipeline to play */
	gst_element_set_state(m_Playbin, GST_STATE_PLAYING);

	/* Start the GStreamer thread */
	m_GstThread = std::thread(GstThread, m_Playbin, m_GstLoop);
	m_Startup = startup;

    UpdateTitle();

    SetVolume(m_Volume);
    m_IsPaused = false;
}

void Player::Pause()
{
	gst_element_set_state(m_Playbin, GST_STATE_PAUSED);
	m_IsPaused = true;
}

void Player::Play()
{
	gst_element_set_state(m_Playbin, GST_STATE_PLAYING);
    m_IsPaused = false;
}

void Player::SetVolume(float volume)
{
	if (m_IsMuted)
		g_object_set(m_Playbin, "volume", 0.0, NULL);
	else
		g_object_set(m_Playbin, "volume", volume, NULL);
}

float Player::GetVolume()
{
    float vol;
	g_object_get(m_Playbin, "volume", &vol, NULL);
	return vol;
}

float Player::GetLengthInSeconds()
{
	return static_cast<double>(GetLength()) / 1000000000.0;
}

uint64_t Player::GetLength()
{
    gint64 length;
	gst_element_query_duration(m_Playbin, GST_FORMAT_TIME, &length);
	return length;
}

float Player::GetCursorInSeconds()
{
	return static_cast<double>(GetCursor()) / 1000000000.0;
}

uint64_t Player::GetCursor()
{
	gint64 cursor;
	gst_element_query_position(m_Playbin, GST_FORMAT_TIME, &cursor);
	return cursor;
}

void Player::SetCursor(uint64_t cursor)
{
	gst_element_seek_simple(m_Playbin, GST_FORMAT_TIME,
							static_cast<GstSeekFlags>(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT), cursor);
}

void Player::SetPlaylist(Playlist playlist)
{
	m_CurrentPlaylist = std::move(playlist);
	m_CurrentSongIndex = 0;
	Init(m_CurrentPlaylist.songs[m_CurrentSongIndex].path);
}

void Player::SetPlaylist(Playlist playlist, int index)
{
    m_CurrentPlaylist = std::move(playlist);
    m_CurrentSongIndex = index;
    Init(m_CurrentPlaylist.songs[m_CurrentSongIndex].path);
}

void Player::Next()
{
	if (m_Startup)
	{
		Pause();
		SetCursor(0);
		return;
	}

    if (m_CurrentSongIndex < m_CurrentPlaylist.songs.size() - 1)
    {
        m_CurrentSongIndex++;
        Init(m_CurrentPlaylist.songs[m_CurrentSongIndex].path);
    }
}

void Player::Prev()
{
	if (m_Startup)
	{
		Pause();
		SetCursor(0);
		return;
	}

    if (m_CurrentSongIndex > 0)
    {
        m_CurrentSongIndex--;
        Init(m_CurrentPlaylist.songs[m_CurrentSongIndex].path);
    }
}

void Player::UpdateTitle()
{
    if (!m_CurrentPlaylist.songs.empty())
    {
		m_CurrentSongTitle = m_CurrentPlaylist.songs[m_CurrentSongIndex].artist;
		m_CurrentSongTitle += " - ";
		m_CurrentSongTitle += m_CurrentPlaylist.songs[m_CurrentSongIndex].title;
    }
 }

void Player::Stop()
{
	g_main_loop_quit(m_GstLoop);
	m_GstThread.join();
	gst_element_set_state(m_Playbin, GST_STATE_NULL);
	g_main_loop_unref (m_GstLoop);
	gst_object_unref (m_Playbin);
}

bool Player::GetEOS()
{
	GstFormat fmt = GST_FORMAT_TIME;
	gint64 pos, duration;

// Query the position and duration of the element
	if (!gst_element_query_position(m_Playbin, fmt, &pos) ||
		!gst_element_query_duration(m_Playbin, fmt, &duration)) {
		g_warning("Could not query position or duration of element");
		return false;
	}

// Check if the element has reached the end of the stream
	if (pos >= duration) {
		g_message("Element has reached end of stream");
		return true;

	} else {
		return false;
	}
}

void Player::GstThread(GstElement *pipeline, GMainLoop *loop)
{
	GstBus *bus;
	GstMessage *msg;
	/* Add a bus watch to the main context */
	bus = gst_element_get_bus(pipeline);
	gst_bus_add_watch(bus, [](GstBus *bus, GstMessage *message, gpointer data) -> gboolean {
		GError *err;
		gchar *debug_info;
		switch (GST_MESSAGE_TYPE (message)) {
			case GST_MESSAGE_ERROR:
				gst_message_parse_error (message, &err, &debug_info);
				g_printerr ("Error received from element %s: %s\n", GST_OBJECT_NAME (message->src), err->message);
				g_printerr ("Debugging information: %s\n", debug_info ? debug_info : "none");
				g_clear_error (&err);
				g_free (debug_info);
//				g_main_loop_quit (data->main_loop);
				break;
			case GST_MESSAGE_EOS:
				g_print ("End-Of-Stream reached.\n");
//				g_main_loop_quit (data->main_loop);
				break;
			case GST_MESSAGE_STATE_CHANGED: {
				GstState old_state, new_state, pending_state;
				gst_message_parse_state_changed (message, &old_state, &new_state, &pending_state);
//				if (GST_MESSAGE_SRC (message) == GST_OBJECT (data->playbin)) {
//					if (new_state == GST_STATE_PLAYING) {
//						/* Once we are in the playing state, analyze the streams */
////						analyze_streams (data);
//					}
//				}
			} break;
		}

		/* We want to keep receiving messages */
		return TRUE;
	}, loop);

	/* Start the main loop */
	g_main_loop_run(loop);

	/* Free resources */
	gst_object_unref(bus);
}
