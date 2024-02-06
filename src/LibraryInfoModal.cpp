#include "LibraryInfoModal.hpp"

void LibraryInfoModal::Open(std::shared_ptr<Library> library)
{
	m_Library = library;
	Modal::Open();
}

void LibraryInfoModal::Render()
{
	if (m_ShouldOpen)
	{
		ImGui::OpenPopup(m_Name.c_str());
		m_ShouldOpen = false;
	}

	if (ImGui::BeginPopupModal(m_Name.c_str(),  nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Albums: %zu", m_Library->m_Albums.size());
		ImGui::Text("Songs: %zu", m_Library->m_Songs.size());
		ImGui::Text("Playlists: %zu", m_Library->m_Playlists.size());
		ImGui::Text("Duration: %s", SecondsToTimeHMS(m_Library->m_TotalSongTime).c_str());

		if (ImGui::Button("Close"))
		{
			Close();
		}

		ImGui::EndPopup();
	}
}


