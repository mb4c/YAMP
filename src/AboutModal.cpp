#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include "AboutModal.hpp"
#include "Texture.hpp"
#include <Utils.hpp>
#include <Config.h>
void AboutModal::Open()
{
	Modal::Open();

	if (!m_Logo.IsValid())
		m_Logo = Texture::LoadTextureFromFile("res/yamp.png");
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

		ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 128) * 0.5f);
		ImGui::Image((void*) (intptr_t) m_Logo.id, ImVec2(128, 128));
		ImGui::NewLine();
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
