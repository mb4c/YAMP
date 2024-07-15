#include <imgui.h>
#include "AboutModal.hpp"
#include <Utils.hpp>
#include <Config.h>

void AboutModal::Open()
{
	Modal::Open();
}

void AboutModal::Render()
{
	if (m_ShouldOpen)
	{
		ImGui::OpenPopup(m_Name.c_str());
		m_ShouldOpen = false;
	}

	ImGui::SetNextWindowSize({200, 0});

	if (ImGui::BeginPopupModal(m_Name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{

		//TODO: show logo above app name
		TextCentered("YAMP");
		ImGui::NewLine();
		std::string version;
		version.append("Version: ")
				.append(std::to_string(YAMP_VERSION_MAJOR)).append(".")
				.append(std::to_string(YAMP_VERSION_MINOR)).append(".")
				.append(std::to_string(YAMP_VERSION_PATCH));
		TextCentered(version);

		if (ImGui::Button("Close"))
		{
			Close();
		}

		ImGui::EndPopup();
	}

}
