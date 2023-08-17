#include "StatusPanel.hpp"

void StatusPanel::RenderPanel(Player& player)
{
	ImGui::Begin("Status");
	TextCentered(player.m_CurrentSongTitle);

	if(ImGui::Button(ICON_FA_BACKWARD_STEP))
	{
		player.Prev();
	}
	ImGui::SameLine();

	if(ImGui::Button(player.m_IsPaused ? ICON_FA_PLAY : ICON_FA_PAUSE))
	{
		if(player.m_IsPaused)
			player.Play();
		else
			player.Pause();

	}

	ImGui::SameLine();
	if(ImGui::Button(ICON_FA_FORWARD_STEP))
	{
		player.Next();
	}
	ImGui::SameLine();
	std::string timeStr = SecondsToTime(player.GetCursorInSeconds());
	timeStr += " / " + SecondsToTime(player.GetLengthInSeconds());
	ImGui::Text("%s", timeStr.c_str());
	ImGui::SameLine();
	ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.75f);

	double cursor = (double)player.GetCursor() / (double)player.GetLength();
	static double min = 0;
	static double max = 1;

	if (player.GetEOS())
	{
		if (m_Repeat)
			player.SetCursor(0);
		else
			player.Next();
	}

	if(ImGui::SliderScalar("##Progress", ImGuiDataType_Double, &cursor, &min, &max, ""))
	{
		m_Holding = true;
		player.Pause();
		player.SetCursor(cursor * player.GetLength());
	}

	if (m_Holding && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		player.Play();
		m_Holding = false;
	}

	ImGui::SameLine();

	if (ImGui::Button(player.m_IsMuted ? ICON_FA_VOLUME_XMARK : ICON_FA_VOLUME_HIGH))
	{
		if (player.m_IsMuted)
		{
			player.m_IsMuted = false;
		}
		else
		{
			player.m_IsMuted = true;
		}
		player.SetVolume(player.m_Volume);
	}


	ImGui::SameLine();
	ImGui::PushItemWidth(-(ImGui::GetFontSize() * 2.0f + ImGui::GetStyle().ItemSpacing.x));
	std::stringstream ss;
	ss << std::fixed << std::setprecision(0) << round(player.m_Volume * 100) << "%%";

	if(ImGui::SliderFloat("##volume", &player.m_Volume, 0.0f, 1.0f, ss.str().c_str()))
	{
		player.SetVolume(player.m_Volume);
		m_Preferences->m_Volume = player.m_Volume;

	}
	ImGui::SetItemUsingMouseWheel();
	if (ImGui::IsItemHovered())
	{
		float wheel = ImGui::GetIO().MouseWheel;
		if (wheel)
		{
			if (ImGui::IsItemActive())
			{
				ImGui::ClearActiveID();
			}
			else
			{
				player.m_Volume += wheel * 0.05f;
				player.m_Volume = std::clamp(player.m_Volume, 0.0f, 1.0f);
				player.SetVolume(player.m_Volume);
				m_Preferences->m_Volume = player.m_Volume;
			}
		}
	}

	ImGui::SameLine();
	if (ImGui::Button(m_Repeat ? ICON_FA_ARROW_RIGHT : ICON_FA_REPEAT))
	{
		m_Repeat = !m_Repeat;
	}

	ImGui::End();
}

StatusPanel::StatusPanel(Preferences* preferences)
{
	m_Preferences = preferences;
}
