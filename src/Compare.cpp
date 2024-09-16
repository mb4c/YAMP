#include "Compare.hpp"

int Compare::ComparePlaylist(const Playlist &lhs, const Playlist &rhs)
{
	return strcmp(lhs.name.c_str(), rhs.name.c_str());
}

bool Compare::CompareString(const std::string &lhs, const std::string &rhs, bool descending)
{
	int result = lhs.compare(rhs);
	return descending ? (result > 0) : (result < 0);
}

bool Compare::CompareSong(const Song &lhs, const Song &rhs, ImGuiTableSortSpecs *specs)
{

	if (lhs.title.empty() || rhs.title.empty())
		return false;

	for (int n = 0; n < specs->SpecsCount; n++)
	{
		const ImGuiTableColumnSortSpecs* sort_spec = &specs->Specs[n];
		int delta = 0;
		switch (sort_spec->ColumnUserID)
		{
			case ColumnID_Track:        delta = (lhs.track - rhs.track);                			break;
			case ColumnID_Title:        delta = (strcmp(lhs.title.c_str(), rhs.title.c_str()));    	break;
			case ColumnID_Artist:       delta = (strcmp(lhs.artist.c_str(), rhs.artist.c_str()));  	break;
			case ColumnID_Album:    	delta = (strcmp(lhs.album.c_str(), rhs.album.c_str()));   	break;
			case ColumnID_Genre:    	delta = (strcmp(lhs.genre.c_str(), rhs.genre.c_str()));   	break;
			case ColumnID_Year:        	delta = (lhs.year - rhs.year);                				break;

			default: IM_ASSERT(0); break;
		}

		if (delta != 0)
		{
			return sort_spec->SortDirection == ImGuiSortDirection_Descending ? (delta > 0) : (delta < 0);
		}
	}
	return false;
}
