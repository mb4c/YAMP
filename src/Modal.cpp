#include <imgui.h>
#include "Modal.hpp"

void Modal::Open()
{
	m_ShouldOpen = true;
}

void Modal::Close()
{
	ImGui::CloseCurrentPopup();
}
