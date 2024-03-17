#pragma once
#include "Modal.hpp"
#include "Song.hpp"

class TrackInfoModal : Modal
{
public:
	TrackInfoModal(std::string name) : Modal(name)
	{
	};

	Song m_Song;
	void Open(const Song& song);
	void Render() override;
};
