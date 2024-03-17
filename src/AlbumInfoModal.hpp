#pragma once
#include <imgui.h>
#include "Modal.hpp"
#include "Playlist.hpp"

class AlbumInfoModal : Modal
{
public:
	AlbumInfoModal(std::string name) : Modal(name)
	{
	};

	Playlist m_Playlist;
	void Open(Playlist playlist);
	void Render() override;
};
