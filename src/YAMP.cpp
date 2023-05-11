#include "YAMP.h"
#include <iostream>
//#define MA_DEBUG_OUTPUT
#define MINIAUDIO_IMPLEMENTATION

YAMP::YAMP(const std::string &title, int width, int height, bool vsync) : Application(title, width, height, vsync)
{
}

void YAMP::OnInit()
{
	m_Preferences.Load();
	m_Player.m_Library.m_LibraryPath = m_Preferences.m_LibraryPath;
	m_Player.m_Library.Load();
	m_Player.m_Library.LoadPlaylists();

	m_Player.Init(std::filesystem::absolute("res/start.wav").string(), true);
	m_Themes.ScanThemes();
	m_Themes.LoadThemeFromName(m_Preferences.m_Theme);

	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = "res/imgui.ini";

	ImVector<ImWchar> ranges;
	ImFontGlyphRangesBuilder builder;
	builder.AddText("ąężźćłóśń ĄĘŻŹĆŁÓŚŃ áčďéěíňóřšťúůýž ÁČĎÉĚÍŇÓŘŠŤÚŮÝŽ”");
	builder.AddRanges(io.Fonts->GetGlyphRangesDefault());
	builder.AddRanges(io.Fonts->GetGlyphRangesCyrillic());
	builder.BuildRanges(&ranges);

	m_Font = io.Fonts->AddFontFromFileTTF("res/Roboto-Medium.ttf", m_Preferences.m_FontSize, nullptr, ranges.Data);

	float iconFontSize = m_Preferences.m_FontSize * 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly

	// Merge in icons from Font Awesome
	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
	ImFontConfig icons_config;
	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	icons_config.GlyphMinAdvanceX = iconFontSize;
	std::string fa_path;
	fa_path = "res/"  FONT_ICON_FILE_NAME_FAS;
	io.Fonts->AddFontFromFileTTF(fa_path.c_str(), iconFontSize, &icons_config, icons_ranges );
	ImGui::GetIO().Fonts->Build();

	SetWindowSize(glm::ivec2(m_Preferences.m_WindowWidth, m_Preferences.m_WindowHeight));
	m_Player.m_Volume = m_Preferences.m_Volume;
	m_Player.SetVolume(m_Preferences.m_Volume);

	m_SearchArtists = Search(m_ArtistSearchbar, m_Player.m_Library.m_Artists);
	m_SearchAlbums = Search(m_AlbumSearchbar, m_Player.m_Library.m_Albums);

	FilterAlbum();
	FilterTracks();
}

void YAMP::OnUpdate()
{
	Dockspace();
	StatusPanel();

	if (m_ShowArtistPanel)
		ArtistsPanel();

	if (m_ShowAlbumPanel)
		AlbumPanel();

	if (m_ShowTracksPanel)
		TracksPanel();

	if (m_ShowPlaylistsPanel)
		PlaylistsPanel();

	if (m_ShowPreferencesPanel)
		PreferencesPanel();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f));
	ImGui::RenderNotifications();
	ImGui::PopStyleVar(1);
	ImGui::PopStyleColor(1);
}

void YAMP::OnShutdown()
{
	m_Preferences.m_WindowWidth = GetWindowSize().x;
	m_Preferences.m_WindowHeight = GetWindowSize().y;
	m_Preferences.Save();
}

void YAMP::Dockspace()
{
    ImGuiWindowFlags flags = ImGuiWindowFlags_MenuBar;
        flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->WorkPos);
                ImGui::SetNextWindowSize(viewport->WorkSize);
                ImGui::SetNextWindowViewport(viewport->ID);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("DockSpace Demo", nullptr, flags);
        ImGui::PopStyleVar(3);
		ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
        ImGui::DockSpace(dockspace_id);

        if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Library"))
                {
                    if (ImGui::MenuItem("Scan library"))
					{
						std::thread libraryThread([&]() {
							m_Player.m_Library.Scan();
							m_Player.m_Library.Load();
							m_Player.m_Library.LoadPlaylists();
							m_SearchArtists = Search(m_ArtistSearchbar, m_Player.m_Library.m_Artists);
							FilterAlbum();
							FilterTracks();
						});
						libraryThread.detach();
					}

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Windows"))
                {
                    ImGui::MenuItem("Artist",nullptr, &m_ShowArtistPanel);
                    ImGui::MenuItem("Album",nullptr, &m_ShowAlbumPanel);
                    ImGui::MenuItem("Tracks",nullptr, &m_ShowTracksPanel);
                    ImGui::MenuItem("Playlists",nullptr, &m_ShowPlaylistsPanel);
                    ImGui::EndMenu();
                }

				ImGui::MenuItem("Preferences",nullptr, &m_ShowPreferencesPanel);

                if (m_ShowPreferencesPanel)
                {
                    ImGui::OpenPopup("Preferences");
                    m_ShowPreferencesPanel = false;
                }

                PreferencesPanel();

                ImGui::EndMenuBar();
            }

        ImGui::End();
}

void YAMP::StatusPanel()
{

    ImGui::Begin("Status");
    TextCentered(m_Player.m_CurrentSongTitle);

    if(ImGui::Button(ICON_FA_BACKWARD_STEP))
    {
        m_Player.Prev();
    }
    ImGui::SameLine();
    
    if(ImGui::Button(m_Player.m_IsPaused ? ICON_FA_PLAY : ICON_FA_PAUSE))
    {
        if(m_Player.m_IsPaused)
            m_Player.Play();
        else 
            m_Player.Pause();

    }

    ImGui::SameLine();
    if(ImGui::Button(ICON_FA_FORWARD_STEP))
    {
        m_Player.Next();
    }
    ImGui::SameLine();
    std::string timeStr = SecondsToTime(m_Player.GetCursorInSeconds());
    timeStr += " / " + SecondsToTime(m_Player.GetLengthInSeconds());
    ImGui::Text("%s", timeStr.c_str());

    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetWindowSize().x * 0.65f);

    double cursor = (double)m_Player.GetCursor() / (double)m_Player.GetLength();
    static double min = 0;
    static double max = 1;

	if (m_Player.GetEOS())
	{
		if (m_Repeat)
			m_Player.SetCursor(0);
		else
			m_Player.Next();
	}

    if(ImGui::SliderScalar("##Progress", ImGuiDataType_Double, &cursor, &min, &max, ""))
    {
		m_Holding = true;
        m_Player.Pause();
        m_Player.SetCursor(cursor * m_Player.GetLength());
    }

    if (m_Holding && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
    {
        m_Player.Play();
		m_Holding = false;
    }

    ImGui::SameLine();

    if (ImGui::Button(m_Player.m_IsMuted ? ICON_FA_VOLUME_XMARK : ICON_FA_VOLUME_HIGH))
	{
		if (m_Player.m_IsMuted)
		{
			m_Player.m_IsMuted = false;
		}
		else
		{
			m_Player.m_IsMuted = true;
		}
        m_Player.SetVolume(m_Player.m_Volume);
	}


    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetWindowSize().x * 0.15f);

	std::stringstream ss;
	ss << std::fixed << std::setprecision(0) << round(m_Player.m_Volume * 100) << "%%";

    if(ImGui::SliderFloat("##volume", &m_Player.m_Volume, 0.0f, 1.0f, ss.str().c_str()))
    {
        m_Player.SetVolume(m_Player.m_Volume);
		m_Preferences.m_Volume = m_Player.m_Volume;

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
				m_Player.m_Volume += wheel * 0.05f;
				m_Player.m_Volume = std::clamp(m_Player.m_Volume, 0.0f, 1.0f);
				m_Player.SetVolume(m_Player.m_Volume);
				m_Preferences.m_Volume = m_Player.m_Volume;
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

void YAMP::ArtistsPanel()
{
    ImGui::Begin("Artist");

	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x * 2);
	if(ImGui::InputTextWithHint("##artistsearchbar", "Search...", &m_ArtistSearchbar))
		m_SearchArtists = Search(m_ArtistSearchbar, m_Player.m_Library.m_Artists);

	if(ImGui::BeginTable("Artist", 1, m_PanelFlag))
    {
        ImGui::TableSetupColumn("Artist", ImGuiTableColumnFlags_DefaultSort);
        ImGui::TableHeadersRow();

		if (ImGuiTableSortSpecs* sorts_specs = ImGui::TableGetSortSpecs())
			if (sorts_specs->SpecsDirty)
			{
				if (m_SearchArtists.size() > 1)
					std::sort(m_SearchArtists.begin(), m_SearchArtists.end(), [&sorts_specs](const std::string& lhs, const std::string& rhs){
						if (sorts_specs->Specs == nullptr) {
							return false;
						}
						return CompareString(lhs, rhs, sorts_specs->Specs->SortDirection == ImGuiSortDirection_Descending);
					});

				sorts_specs->SpecsDirty = false;
			}

        ImGuiListClipper clipper;
        clipper.Begin(m_SearchArtists.size());
        while (clipper.Step())
            for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
            {
                std::string artist = m_SearchArtists[row_n];
                ImGui::PushID(artist.c_str());
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                bool selected = artist == m_Player.m_Library.m_SelectedArtist;
                ImGui::Selectable(artist.c_str(), selected, ImGuiSelectableFlags_SpanAllColumns);
                if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
                {
                    m_Player.m_Library.m_SelectedArtist = artist;
					FilterAlbum();
                }

                ImGui::PopID();
            }
        ImGui::EndTable();
    }

    ImGui::End();
}

void YAMP::AlbumPanel()
{
    ImGui::Begin("Album");

	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x * 2);
	if(ImGui::InputTextWithHint("##artistsearchbar", "Search...", &m_AlbumSearchbar))
		m_SearchAlbums = Search(m_AlbumSearchbar, m_FilteredAlbums);

    if(ImGui::BeginTable("Album", 1, m_PanelFlag))
    {
        ImGui::TableSetupColumn("Album", ImGuiTableColumnFlags_DefaultSort);
        ImGui::TableHeadersRow();

		if (ImGuiTableSortSpecs* sorts_specs = ImGui::TableGetSortSpecs())
			if (sorts_specs->SpecsDirty)
			{
				if (m_SearchAlbums.size() > 1)
					std::sort(m_SearchAlbums.begin() + 1, m_SearchAlbums.end(),[&sorts_specs](const std::string& lhs, const std::string& rhs){
						if (sorts_specs->Specs == nullptr) {
							return false;
						}
						return CompareString(lhs, rhs, sorts_specs->Specs->SortDirection == ImGuiSortDirection_Descending);
					});

				sorts_specs->SpecsDirty = false;
			}

        ImGuiListClipper clipper;
        clipper.Begin(m_SearchAlbums.size());
        while (clipper.Step())
            for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
            {
                std::string album = m_SearchAlbums[row_n];
                ImGui::PushID(album.c_str());
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
				bool selected = album == m_Player.m_Library.m_SelectedAlbum;

				ImGui::Selectable(album.c_str(),selected, ImGuiSelectableFlags_SpanAllColumns);
                if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
                {
                    m_Player.m_Library.m_SelectedAlbum = album;
                    m_PlaylistClicked = false;

					FilterTracks();
                }

				if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
				{
					ImGui::OpenPopup("AlbumPopup");
				}
				bool popup = false;

				if (ImGui::BeginPopup("AlbumPopup"))
				{
					if (ImGui::BeginMenu("Add to playlist"))
					{
						if (ImGui::MenuItem("Create new..."))
						{
							popup = true;
						}

						for (auto &playlist: m_Player.m_Library.m_Playlists)
						{
							if (ImGui::MenuItem(playlist.name.c_str()))
							{
								AddAlbumToPlaylist(&playlist, album);
								m_Player.m_Library.SavePlaylists();
							}
						}
						ImGui::EndMenu();
					}
					//TODO: open in file explorer

//						if (ImGui::MenuItem("Open in file explorer"))
//						{
//							OpenInFileExplorer(m_FilteredSongs[row_n].path);
//						}

					ImGui::EndPopup();
				}
				OpenNewPlaylistModal(row_n, popup, true);

                ImGui::PopID();
            }
        ImGui::EndTable();
    }
    ImGui::End();
}

void YAMP::TracksPanel()
{
    ImGui::Begin("Tracks");

    if(ImGui::BeginTable("Tracks", 6, m_PanelFlag))
    {
        ImGui::TableSetupColumn("Track", ImGuiTableColumnFlags_DefaultSort, 0.0f, ColumnID_Track);
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_None, 0.0f, ColumnID_Name);
        ImGui::TableSetupColumn("Artist", ImGuiTableColumnFlags_None, 0.0f, ColumnID_Artist);
        ImGui::TableSetupColumn("Album", ImGuiTableColumnFlags_None, 0.0f, ColumnID_Album);
        ImGui::TableSetupColumn("Genre");
        ImGui::TableSetupColumn("Year");
        ImGui::TableHeadersRow();

		if (ImGuiTableSortSpecs* sorts_specs = ImGui::TableGetSortSpecs())
			if (sorts_specs->SpecsDirty)
			{
				if (m_FilteredSongs.size() > 1)
					std::sort(m_FilteredSongs.begin(), m_FilteredSongs.end(),[sorts_specs](const Song& lhs, const Song& rhs){
						return CompareSong(lhs, rhs, sorts_specs);
					});

				if (m_Playlist.songs.size() > 1)
					std::sort(m_Playlist.songs.begin(), m_Playlist.songs.end(),[sorts_specs](const Song& lhs, const Song& rhs){
						return CompareSong(lhs, rhs, sorts_specs);
					});

				sorts_specs->SpecsDirty = false;
			}

        ImGuiListClipper clipper;
        clipper.Begin(m_FilteredSongs.size());
        while (clipper.Step())
            for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
            {
                Song* song = &m_FilteredSongs[row_n];
                ImGui::PushID(song->path.c_str());
                ImGui::TableNextRow();
                ImGui::TableNextColumn();

				bool selected = song == &m_FilteredSongs[m_Player.m_CurrentSongIndex];

				ImGui::Selectable(std::to_string(song->track).c_str(),selected, ImGuiSelectableFlags_SpanAllColumns);
                if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
                {
                    m_Player.SetPlaylist(m_Playlist, row_n);
//					FilterTracks();
                }

				if(m_PlaylistClicked)
				{
					Reorder(m_FilteredSongs, row_n);
					Reorder(m_Playlist.songs, row_n);
				}

                if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
                {
                    ImGui::OpenPopup("TrackPopup");
                }
                bool popup = false;

                if (ImGui::BeginPopup("TrackPopup"))
                {
					if (!m_PlaylistClicked)
					{
						if (ImGui::BeginMenu("Add to playlist"))
						{
							if (ImGui::MenuItem("Create new..."))
							{
								popup = true;
							}

							for (auto &playlist: m_Player.m_Library.m_Playlists)
							{
								if (ImGui::MenuItem(playlist.name.c_str()))
								{
									AddSongToPlaylist(&playlist, row_n);

									m_Player.m_Library.SavePlaylists();
								}
							}
							ImGui::EndMenu();
						}
						if (ImGui::MenuItem("Open in file explorer"))
						{
							OpenInFileExplorer(m_FilteredSongs[row_n].path);
						}
						if (ImGui::MenuItem("Select album"))
						{
							m_Player.m_Library.m_SelectedAlbum = m_FilteredSongs[row_n].album;
							FilterTracks();
						}
					}
					else
					{
						if(ImGui::Selectable("Remove from playlist"))
						{
							m_SelectedPlaylist->duration -= m_SelectedPlaylist->songs.at(row_n).duration;
							m_SelectedPlaylist->songs.erase(m_SelectedPlaylist->songs.begin() + row_n);
							m_Player.m_Library.SavePlaylists();
							FilterTracks();
						}
					}

                    ImGui::EndPopup();
                }


				OpenNewPlaylistModal(row_n, popup, false);

                ImGui::TableNextColumn();
                ImGui::Text("%s", song->title.c_str());
                ImGui::TableNextColumn();
                ImGui::Text("%s", song->artist.c_str());
                ImGui::TableNextColumn();
                ImGui::Text("%s", song->album.c_str());
                ImGui::TableNextColumn();
                ImGui::Text("%s", song->genre.c_str());
                ImGui::TableNextColumn();
                ImGui::Text("%d", song->year);
                ImGui::PopID();
            }
        ImGui::EndTable();
    }
    ImGui::End();
}

void YAMP::PreferencesPanel()
{
    if (ImGui::BeginPopupModal("Preferences",  nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
		ImGui::Text("FontSize");
		ImGui::SameLine();
        ImGui::InputFloat("##font", &m_Preferences.m_FontSize);
		ImGui::Text("Library path");
		ImGui::SameLine();
        ImGui::InputText("##librarypath", &m_Preferences.m_LibraryPath);
		ImGui::SameLine();
		if(ImGui::Button("Select..."))
		{
			auto selection = pfd::select_folder("Select a folder").result();
			if (!selection.empty())
			{
				m_Preferences.m_LibraryPath = selection;
				m_Player.m_Library.m_LibraryPath = selection;
				std::cout << "User selected folder " << selection << "\n";
			}
		}

		std::string current_item = m_Preferences.m_Theme;
		ImGui::Text("Theme");
		ImGui::SameLine();

		if (ImGui::BeginCombo("##combo", current_item.c_str()))
		{
			for (int n = 0; n < m_Themes.m_Themes.size(); n++)
			{
				bool is_selected = (current_item == m_Themes.m_Themes[n].name);
				if (ImGui::Selectable(m_Themes.m_Themes[n].name.c_str(), is_selected))
				{
					current_item = m_Themes.m_Themes[n].name;
					m_Preferences.m_Theme = current_item;
					m_Themes.LoadThemeFromName(m_Themes.m_Themes[n].name);
				}
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

        if (ImGui::Button("Save"))
        {
			m_Preferences.m_WindowWidth = GetWindowSize().x;
			m_Preferences.m_WindowHeight = GetWindowSize().y;
            m_Preferences.Save();
        }
        ImGui::SameLine();
        if (ImGui::Button("Load"))
        {
            m_Preferences.Load();
            m_Font->FontSize = m_Preferences.m_FontSize;
			SetWindowSize(glm::ivec2(m_Preferences.m_WindowWidth, m_Preferences.m_WindowHeight));

		}
        ImGui::SameLine();
        if (ImGui::Button("Close"))
        {
            ImGui::CloseCurrentPopup();
            m_ShowPreferencesPanel = false;
        }
        ImGui::EndPopup();
    }
}

void YAMP::PlaylistsPanel()
{
    ImGui::Begin("Playlists");

    if(ImGui::BeginTable("Playlist", 1, m_PanelFlag))
    {
        ImGui::TableSetupColumn("Playlist", ImGuiTableColumnFlags_DefaultSort);
        ImGui::TableHeadersRow();

		if (ImGuiTableSortSpecs* sorts_specs = ImGui::TableGetSortSpecs())
			if (sorts_specs->SpecsDirty)
			{
				if (m_Player.m_Library.m_Playlists.size() > 1)
					std::sort(m_Player.m_Library.m_Playlists.begin(),m_Player.m_Library.m_Playlists.end(), ComparePlaylist);
				sorts_specs->SpecsDirty = false;
			}

		bool playlistRenamePopup = false;
		bool playlistInfoPopup = false;

        ImGuiListClipper clipper;
        clipper.Begin(m_Player.m_Library.m_Playlists.size());
        while (clipper.Step())
            for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
            {
                Playlist playlist = m_Player.m_Library.m_Playlists[row_n];
                ImGui::PushID(playlist.name.c_str());
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                bool selected = false;
                ImGui::Selectable(playlist.name.c_str(),selected, ImGuiSelectableFlags_SpanAllColumns);
                if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
                {
                    m_SelectedPlaylist = &m_Player.m_Library.m_Playlists[row_n];
                    m_PlaylistClicked = true;
					FilterTracks();
                }

				Reorder(m_Player.m_Library.m_Playlists, row_n);

				if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
				{
					ImGui::OpenPopup("playlistpopup");
				}

				if (ImGui::BeginPopup("playlistpopup"))
				{
					if(ImGui::Selectable("Remove playlist"))
					{
						if (m_SelectedPlaylist == &m_Player.m_Library.m_Playlists.at(row_n))
						{
							m_SelectedPlaylist = nullptr;
							m_PlaylistClicked = false;
						}

						m_Player.m_Library.m_Playlists.erase(m_Player.m_Library.m_Playlists.begin() + row_n);
						m_Player.m_Library.SavePlaylists();

					}

					if(ImGui::Selectable("Rename"))
					{
						playlistRenamePopup = true;
						strcpy(m_NewPlayListName, m_Player.m_Library.m_Playlists.at(row_n).name.c_str());
					}

					if(ImGui::Selectable("Info"))
					{
						playlistInfoPopup = true;
					}

					ImGui::EndPopup();
				}

				if (playlistRenamePopup)
				{
					ImGui::OpenPopup("Rename playlist");
					playlistRenamePopup = false;
				}
				if (playlistInfoPopup)
				{
					ImGui::OpenPopup("Info");
					playlistInfoPopup = false;
				}

				if (ImGui::BeginPopupModal("Rename playlist",  nullptr, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::InputText("Name", m_NewPlayListName, IM_ARRAYSIZE(m_NewPlayListName));

					if (ImGui::Button("Rename"))
					{
						m_Player.m_Library.m_Playlists.at(row_n).name = m_NewPlayListName;
						m_Player.m_Library.SavePlaylists();
						ImGui::CloseCurrentPopup();
					}

					ImGui::SameLine();
					if (ImGui::Button("Cancel"))
					{
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}

				if (ImGui::BeginPopupModal("Info",  nullptr, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Playlist name: %s", playlist.name.c_str());
					ImGui::Text("Songs: %zu", playlist.songs.size());
					ImGui::Text("Total duration: %s", SecondsToTimeHMS(playlist.duration).c_str());

					ImGui::SameLine();
					if (ImGui::Button("Close"))
					{
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
                ImGui::PopID();
            }
        ImGui::EndTable();
    }
    ImGui::End();
}

void YAMP::FilterAlbum()
{
	std::string selectedArtist = m_Player.m_Library.m_SelectedArtist;
	m_FilteredAlbums.clear();
	m_FilteredAlbums.emplace_back("All");

	for (auto & song : m_Player.m_Library.m_Songs)
	{
		if (song.artist == m_Player.m_Library.m_SelectedArtist || selectedArtist == "All")
		{
			if (std::find(m_FilteredAlbums.begin(), m_FilteredAlbums.end(), song.album) == m_FilteredAlbums.end())
			{
				m_FilteredAlbums.push_back(song.album);
			}
		}
	}
	m_SearchAlbums = Search(m_AlbumSearchbar, m_FilteredAlbums);

}

void YAMP::FilterTracks()
{
	m_Playlist.name = "Queue";
	m_Playlist.songs.clear();
	m_FilteredSongs.clear();

	if (!m_PlaylistClicked)
	{
		for (auto & song : m_Player.m_Library.m_Songs)
		{
			if (song.album == m_Player.m_Library.m_SelectedAlbum || m_Player.m_Library.m_SelectedAlbum == "All")
			{
				m_FilteredSongs.push_back(song);
				m_Playlist.songs.push_back(song);
			}
		}
	}
	else
	{
		m_FilteredSongs = m_SelectedPlaylist->songs;
		m_Playlist = *m_SelectedPlaylist;
	}
}

int YAMP::ComparePlaylist(const Playlist& lhs, const Playlist& rhs)
{
	return strcmp(lhs.name.c_str(), rhs.name.c_str());
}

bool YAMP::CompareString(const std::string &lhs, const std::string &rhs, bool descending)
{
	int result = lhs.compare(rhs);
//	int result = strcmp(lhs.c_str(), rhs.c_str());
	return descending ? (result > 0) : (result < 0);
}

bool YAMP::CompareSong(const Song &lhs, const Song &rhs, ImGuiTableSortSpecs *specs)
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
			case ColumnID_Name:         delta = (strcmp(lhs.title.c_str(), rhs.title.c_str()));     break;
			case ColumnID_Artist:       delta = (strcmp(lhs.artist.c_str(), rhs.artist.c_str()));  	break;
			case ColumnID_Album:    	delta = (strcmp(lhs.album.c_str(), rhs.album.c_str()));   	break;
			case ColumnID_Genre:    	delta = (strcmp(lhs.genre.c_str(), rhs.genre.c_str()));   	break;
			case ColumnID_Year:        	delta = (lhs.track - rhs.track);                			break;

			default: IM_ASSERT(0); break;
		}

		return sort_spec->SortDirection == ImGuiSortDirection_Descending ? (delta > 0) : (delta < 0);
	}
	return (lhs.track - rhs.track);
}

void YAMP::TextCentered(const std::string &text)
{
	float win_width = ImGui::GetWindowSize().x;
	float text_width = ImGui::CalcTextSize(text.c_str()).x;

	// calculate the indentation that centers the text on one line, relative
	// to window left, regardless of the `ImGuiStyleVar_WindowPadding` value
	float text_indentation = (win_width - text_width) * 0.5f;

	// if text is too long to be drawn on one line, `text_indentation` can
	// become too small or even negative, so we check a minimum indentation
	float min_indentation = 20.0f;
	if (text_indentation <= min_indentation) {
		text_indentation = min_indentation;
	}

	ImGui::SameLine(text_indentation);
	ImGui::PushTextWrapPos(win_width - text_indentation);
	ImGui::TextWrapped("%s", text.c_str());
	ImGui::PopTextWrapPos();
}

std::vector<std::string> YAMP::Search(std::string searchText, const std::vector<std::string>& strings)
{
	std::vector<std::string> filteredStrings;
	std::transform(searchText.begin(), searchText.end(), searchText.begin(), ::tolower);

	for (const auto& str : strings) {
		std::string str_copy = str;
		std::transform(str_copy.begin(), str_copy.end(), str_copy.begin(), ::tolower);
		if (str_copy.find(searchText) != std::string::npos) {
			filteredStrings.push_back(str);
		}
	}
	return filteredStrings;
}

void YAMP::OpenNewPlaylistModal(int index, bool open, bool isAlbum)
{
	if (open)
	{
		ImGui::OpenPopup("Create playlist");
	}

	if (ImGui::BeginPopupModal("Create playlist",  nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::InputTextWithHint("Name", "Name", m_NewPlayListName, IM_ARRAYSIZE(m_NewPlayListName));
		if (ImGui::Button("Create"))
		{
			Playlist playlist;
			playlist.name = m_NewPlayListName;
			memset(&m_NewPlayListName[0], 0, sizeof(m_NewPlayListName));

//			playlist.songs.push_back(m_FilteredSongs.at(index));
//			playlist.duration += m_FilteredSongs.at(index).duration;
			if (isAlbum)
			{

			} else
			{
				AddSongToPlaylist(&playlist, index);
			}
			m_Player.m_Library.m_Playlists.push_back(playlist);
			m_Player.m_Library.SavePlaylists();
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void YAMP::AddSongToPlaylist(Playlist *playlist, int index)
{
	playlist->songs.push_back(m_FilteredSongs.at(index));
	playlist->duration += m_FilteredSongs.at(index).duration;
}

std::vector<Song> YAMP::GetSongsFromAlbumName(const std::string& name)
{

	std::vector<Song> songs;

	for (auto & song : m_Player.m_Library.m_Songs)
	{
		if (song.album == name)
		{
			songs.push_back(song);
		}
	}

	return songs;
}

void YAMP::AddAlbumToPlaylist(Playlist *playlist, const std::string& albumName)
{
	std::vector<Song> songs = GetSongsFromAlbumName(albumName);

	for (const auto& song : songs)
	{
		playlist->songs.push_back(song);
		playlist->duration += song.duration;
	}
}

template<typename T>
void YAMP::Reorder(std::vector<T> &vec, int index)
{
	T type = vec[index];
	if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
	{
		int n_next = index + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
		if (n_next >= 0 && n_next < vec.size())
		{
			vec[index] = vec[n_next];
			vec[n_next] = type;
			ImGui::ResetMouseDragDelta();
		}
	}
}
