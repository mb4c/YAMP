#pragma once


#include <cstring>
#include <imgui.h>
#include "Playlist.h"

class Compare
{
public:

	enum ColumnID
	{
		ColumnID_Track,
		ColumnID_Name,
		ColumnID_Artist,
		ColumnID_Album,
		ColumnID_Genre,
		ColumnID_Year
	};
	static int ComparePlaylist(const Playlist& lhs, const Playlist& rhs);
	static bool CompareSong(const Song &lhs, const Song &rhs, ImGuiTableSortSpecs *specs);
	static bool CompareString(const std::string &lhs, const std::string &rhs, bool descending);
};
