#pragma once

#include <Player.h>
#include <Preferences.h>

class StatusPanel
{
public:
	StatusPanel(Preferences* preferences);
	bool m_Holding = false;
	bool m_Repeat = false;
	Preferences* m_Preferences;
	void RenderPanel(Player& player);
};
