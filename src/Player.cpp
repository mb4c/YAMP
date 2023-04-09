#include "Player.h"

void Player::Init(const std::string& filePath, bool startup)
{

	m_Startup = startup;

	if(m_Engine != nullptr)
	{
		ma_engine_stop(m_Engine);
		ma_engine_uninit(m_Engine);
	}
	else
	{
	}
	m_Engine = new ma_engine;

    UpdateTitle();
    if(!m_IsPaused)
	{
		Pause();
	}

	m_Result = ma_engine_init(nullptr, m_Engine);
	if (m_Result != MA_SUCCESS)
	{
		std::cout << "ma_engine_init failed: " << ma_result_description(m_Result) <<  std::endl;
		return;
	}

	m_Result = ma_sound_init_from_file(m_Engine, filePath.c_str(), 0, nullptr, nullptr, &m_Sound);
	if (m_Result != MA_SUCCESS)
	{
		std::cout << ma_result_description(m_Result) << std::endl;
		return;
	}

	ma_sound_start(&m_Sound);
    SetVolume(m_Volume);
    m_IsPaused = false;
}

void Player::Pause()
{

	ma_sound_stop(&m_Sound);
    m_IsPaused = true;
}

void Player::Play()
{
	if (ma_sound_start(&m_Sound) != MA_SUCCESS)
    {
		printf("Failed to start playback device.\n");
	}
    m_IsPaused = false;

}

void Player::SetVolume(float volume)
{
    if (m_IsMuted)
		ma_sound_set_volume(&m_Sound, 0.0f);
    else
		ma_sound_set_volume(&m_Sound, volume);

}

float Player::GetVolume()
{
    float vol;
	vol = ma_sound_get_volume(&m_Sound);
	return vol;
}

float Player::GetLengthInSeconds()
{
    float length;
    ma_sound_get_length_in_seconds(&m_Sound, &length);
    return length;
}

uint64_t Player::GetLength()
{
    ma_uint64 length;
	ma_sound_get_length_in_pcm_frames(&m_Sound, &length);
	return length;
}

float Player::GetCursorInSeconds()
{
    float cursor;
	ma_sound_get_cursor_in_seconds(&m_Sound, &cursor);
	return cursor;
}

uint64_t Player::GetCursor()
{
	ma_uint64 cursor;
	ma_sound_get_cursor_in_pcm_frames(&m_Sound, &cursor);
	return cursor;
}

void Player::SetCursor(uint64_t cursor)
{
	ma_sound_seek_to_pcm_frame(&m_Sound,cursor);
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