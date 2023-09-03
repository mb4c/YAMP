#include <imgui.h>
#include "TrackInfoModal.hpp"

void TrackInfoModal::Render()
{
	if (m_ShouldOpen)
	{
		ImGui::OpenPopup(m_Name.c_str());
		m_ShouldOpen = false;
	}

	if (ImGui::BeginPopupModal(m_Name.c_str(),  nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Title: %s", m_Song.title.c_str());
		ImGui::Text("Artist: %s", m_Song.artist.c_str());
		ImGui::Text("Album: %s", m_Song.album.c_str());
		ImGui::Text("Year: %u", m_Song.year);
		ImGui::Text("Track: %u", m_Song.track);
		ImGui::Text("Genre: %s", m_Song.genre.c_str());
		ImGui::Text("Duration: %d", m_Song.duration);
		ImGui::Text("Path: %s", m_Song.path.string().c_str());

		if (ImGui::Button("Close"))
		{
			Close();
		}

		ImGui::EndPopup();
	}
}

void TrackInfoModal::Open(const Song& song)
{
	m_Song = song;
	Modal::Open();
}
