#include "YAMP.hpp"
#include "Log.hpp"
#include "Compare.hpp"
#include <iostream>
#include <Panels/Modals.hpp>


YAMP::YAMP(const std::string &title, int width, int height, bool vsync, int argc, char** argv) : Application(title, width, height, vsync, argc, argv)
{
}

void YAMP::OnInit()
{
	m_Preferences.Load();
	m_Player.m_Library.m_LibraryPath = m_Preferences.m_LibraryPath;
	m_Player.m_Library.Load();
	m_Player.m_Library.LoadPlaylists();

	if(GetArguments().size() == 2)
	{
		m_Player.Init(std::filesystem::absolute(GetArguments()[1]).string(), true);
	}
	else
	{
		m_Player.Init(std::filesystem::absolute("res/start.wav").string(), true);
	}

	m_Themes.ScanThemes();
	m_Themes.LoadThemeFromName(m_Preferences.m_Theme);

	m_Style = ImGui::GetStyle();
	ImGui::GetStyle().ScaleAllSizes(m_Preferences.m_Scale);

	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = "res/imgui.ini";

	BuildFont();

	SetWindowSize(glm::ivec2(m_Preferences.m_WindowWidth, m_Preferences.m_WindowHeight));
	m_Player.m_Volume = m_Preferences.m_Volume;
	m_Player.SetVolume(m_Preferences.m_Volume);

	m_SearchProps.searchArtists = Search(m_SearchProps.artistSearchbar, m_Player.m_Library.m_Artists);
	Filter::FilterAlbum(m_Player, m_FilteredAlbums, m_SearchProps.searchAlbums, m_SearchProps.albumSearchbar);

	m_ShouldFilterTracks = true;
}

void YAMP::OnUpdate()
{
	Dockspace();
	DrawStatusPanel();
	if (m_ShowArtistPanel)
		DrawArtistPanel();
	if (m_ShowAlbumPanel)
		DrawAlbumPanel();
	if (m_ShowTracksPanel)
		DrawTracksPanel();

	if (m_ShowPlaylistsPanel)
		DrawPlaylistPanel();

	if (m_ShowPreferencesPanel)
		PreferencesPanel();

	if (m_ShouldFilterTracks)
	{
		m_ShouldFilterTracks = false;
		Filter::FilterTracks(m_Player, m_Playlist, m_SelectedPlaylist, m_FilteredSongs, m_PlaylistClicked);
		if (m_TracksSortSpecs)
			m_TracksSortSpecs->SpecsDirty = true;
	}
	if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Space)))
	{
		if(m_Player.m_IsPaused)
			m_Player.Play();
		else
			m_Player.Pause();
	}

	m_LibraryInfoModal.Render();
	m_AboutModal.Render();

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
							m_SearchProps.searchArtists = Search(m_SearchProps.artistSearchbar, m_Player.m_Library.m_Artists);
							Filter::FilterAlbum(m_Player, m_FilteredAlbums, m_SearchProps.searchArtists, m_SearchProps.artistSearchbar);
							Filter::FilterTracks(m_Player, m_Playlist, m_SelectedPlaylist, m_FilteredSongs, m_PlaylistClicked);
						});
						libraryThread.detach();
					}

					if (ImGui::MenuItem("Info"))
					{
						m_LibraryInfoModal.Open(std::make_shared<Library>(m_Player.m_Library));
					}

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("View"))
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

				if (ImGui::MenuItem("About"))
				{
					m_AboutModal.Open();
				}



                ImGui::EndMenuBar();
            }

        ImGui::End();
}

void YAMP::PreferencesPanel()
{
    if (ImGui::BeginPopupModal("Preferences",  nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
		ImGui::Text("UI scale");
		ImGui::SameLine();
		if (ImGui::DragFloat("##scale", &m_Preferences.m_Scale, 0.01, 0.1, 4.0))
		{
			if (m_Preferences.m_Scale <= 0)
				m_Preferences.m_Scale = 0.1;

			ImGui::GetStyle() = m_Style;
			ImGui::GetStyle().ScaleAllSizes(m_Preferences.m_Scale);
		}

		ImGui::Text("Font size");
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
				Log::Trace("User selected folder: {}", selection);
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

void YAMP::BuildFont()
{
	ImGuiIO& io = ImGui::GetIO();
	static ImWchar fontRange[] = { 0x1, static_cast<ImWchar>(0x1FFFF), 0 };

	ImVector<ImWchar> ranges;
	ImFontGlyphRangesBuilder builder;
	builder.AddRanges(fontRange);
	builder.BuildRanges(&ranges);

	m_Font = io.Fonts->AddFontFromFileTTF("res/Roboto-Medium.ttf", m_Preferences.m_FontSize, nullptr, ranges.Data);

	float iconFontSize = m_Preferences.m_FontSize * 1.5f * 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly

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
}

YAMP::~YAMP()
{
	m_Player.Stop();
}

void YAMP::DrawStatusPanel()
{
	ImGui::Begin("Status");
	TextCentered(m_Player.m_CurrentSongTitle);
	float originalPadding = ImGui::GetStyle().FramePadding.x;
	float buttonPadding = originalPadding + 4.0f;
	float sliderPadding = buttonPadding/4;
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,{buttonPadding, buttonPadding});
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
	ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.75f);

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

	ImGui::PopStyleVar(1);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,{sliderPadding,sliderPadding});
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetStyle().FramePadding.y + ImGui::GetStyle().ItemSpacing.y); //BUG: only works with ui scale set to 1.0
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

	ImGui::PopStyleVar(1);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,{buttonPadding, buttonPadding});
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

	ImGui::PopStyleVar(1);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,{sliderPadding,sliderPadding});

	ImGui::SameLine();
	ImGui::PushItemWidth(-(ImGui::GetFontSize() * 2.0f + ImGui::GetStyle().ItemSpacing.x));

	std::stringstream ss;
	ss << std::fixed << std::setprecision(0) << round(m_Player.m_Volume * 100) << "%%";

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetStyle().FramePadding.y + ImGui::GetStyle().ItemSpacing.y);
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
	ImGui::PopStyleVar(1);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,{buttonPadding, buttonPadding});
	if (ImGui::Button(m_Repeat ? ICON_FA_ARROW_RIGHT : ICON_FA_REPEAT))
	{
		m_Repeat = !m_Repeat;
	}
	ImGui::PopStyleVar(1);

	ImGui::End();
}

void YAMP::DrawTracksPanel()
{
	ImGui::Begin("Track");

	if(ImGui::BeginTable("Track", 6, m_PanelFlags))
	{
		ImGui::TableSetupColumn("Track", ImGuiTableColumnFlags_DefaultSort, 0.0f, Compare::ColumnID_Track);
		ImGui::TableSetupColumn("Title", ImGuiTableColumnFlags_None, 0.0f, Compare::ColumnID_Title);
		ImGui::TableSetupColumn("Artist", ImGuiTableColumnFlags_None, 0.0f, Compare::ColumnID_Artist);
		ImGui::TableSetupColumn("Album", ImGuiTableColumnFlags_None, 0.0f, Compare::ColumnID_Album);
		ImGui::TableSetupColumn("Genre", ImGuiTableColumnFlags_None, 0.0f, Compare::ColumnID_Genre);
		ImGui::TableSetupColumn("Year", ImGuiTableColumnFlags_None, 0.0f, Compare::ColumnID_Year);
		ImGui::TableHeadersRow();

		m_TracksSortSpecs = ImGui::TableGetSortSpecs();
		if (m_TracksSortSpecs)
			if (m_TracksSortSpecs->SpecsDirty)
			{
				if (m_FilteredSongs.size() > 1)
					std::sort(m_FilteredSongs.begin(), m_FilteredSongs.end(),[this](const Song& lhs, const Song& rhs){
						return Compare::CompareSong(lhs, rhs, m_TracksSortSpecs);
					});

				if (m_Playlist.songs.size() > 1)
					std::sort(m_Playlist.songs.begin(), m_Playlist.songs.end(),[this](const Song& lhs, const Song& rhs){
						return Compare::CompareSong(lhs, rhs, m_TracksSortSpecs);
					});
				m_TracksSortSpecs->SpecsDirty = false;
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

				bool selected = song == &m_FilteredSongs[m_Player.m_CurrentSongIndex] && song->album == m_Player.m_CurrentAlbum;

				ImGui::Selectable(std::to_string(song->track).c_str(),selected, ImGuiSelectableFlags_SpanAllColumns);
				if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
				{
					m_Player.SetPlaylist(m_Playlist, row_n);
//					m_ShouldFilterTracks = true;
//					m_TracksSortSpecs->SpecsDirty = true;
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

							for (auto &playlist2: m_Player.m_Library.m_Playlists)
							{
								if (ImGui::MenuItem(playlist2.name.c_str()))
								{
									Playlist::AddSongToPlaylist(&playlist2, row_n, m_FilteredSongs);

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
							m_ShouldFilterTracks = true;
						}
						if (ImGui::MenuItem("Info"))
						{
							m_TrackInfoModal.Open(m_FilteredSongs[row_n]);
						}
					}
					else
					{
						if(ImGui::Selectable("Remove from playlist"))
						{
							m_SelectedPlaylist.duration -= m_SelectedPlaylist.songs.at(row_n).duration;
							m_SelectedPlaylist.songs.erase(m_SelectedPlaylist.songs.begin() + row_n);
							m_Player.m_Library.SavePlaylists();
							m_ShouldFilterTracks = true;

						}
					}

					ImGui::EndPopup();
				}

				m_TrackInfoModal.Render();
				Modals::OpenNewPlaylistModal(row_n, popup, false, m_Player, m_NewPlayListName, m_FilteredSongs);

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

void YAMP::DrawAlbumPanel()
{
	ImGui::Begin("Album");

	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x * 2);
	if (ImGui::InputTextWithHint("##artistsearchbar", "Search...", &m_SearchProps.albumSearchbar))
		m_SearchProps.searchAlbums = Search(m_SearchProps.albumSearchbar, m_FilteredAlbums);

	if (ImGui::BeginTable("Album", 1, m_PanelFlags))
	{
		ImGui::TableSetupColumn("Album", ImGuiTableColumnFlags_DefaultSort);
		ImGui::TableHeadersRow();

		if (ImGuiTableSortSpecs* sorts_specs = ImGui::TableGetSortSpecs())
			if (sorts_specs->SpecsDirty)
			{
				if (m_SearchProps.searchAlbums.size() > 1)
					std::sort(m_SearchProps.searchAlbums.begin(), m_SearchProps.searchAlbums.end(), [&sorts_specs](const std::string& lhs, const std::string& rhs)
					{
						if (sorts_specs->Specs == nullptr)
						{
							return false;
						}
						return Compare::CompareString(lhs, rhs, sorts_specs->Specs->SortDirection == ImGuiSortDirection_Descending);
					});

				sorts_specs->SpecsDirty = false;
			}


		ImGui::PushID("All");
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Selectable("All", "", ImGuiSelectableFlags_SpanAllColumns);
		if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		{
			m_Player.m_Library.m_SelectedAlbum = "All";
			m_PlaylistClicked = false;

			m_ShouldFilterTracks = true;
		}
		ImGui::PopID();

		ImGuiListClipper clipper;
		clipper.Begin(m_SearchProps.searchAlbums.size());
		while (clipper.Step())
			for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
			{
				std::string album = m_SearchProps.searchAlbums[row_n];
				ImGui::PushID(album.c_str());
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				bool selected = album == m_Player.m_Library.m_SelectedAlbum;

				ImGui::Selectable(album.c_str(), selected, ImGuiSelectableFlags_SpanAllColumns);
				if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
				{
					m_Player.m_Library.m_SelectedAlbum = album;
					m_PlaylistClicked = false;

					m_ShouldFilterTracks = true;
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

						for (auto& playlist: m_Player.m_Library.m_Playlists)
						{
							if (ImGui::MenuItem(playlist.name.c_str()))
							{
								AddAlbumToPlaylist(m_Player, &playlist, album);
								m_Player.m_Library.SavePlaylists();
							}
						}
						ImGui::EndMenu();
					}
					if (ImGui::MenuItem("Open in file explorer"))
					{
						OpenInFileExplorer(GetSongsFromAlbumName(m_SearchProps.searchAlbums[row_n], m_Player)[0].path);
					}
					if (ImGui::MenuItem("Info"))
					{
						m_AlbumInfo.m_Playlist = Playlist::GetPlaylistFromAlbumName(m_SearchProps.searchAlbums[row_n], m_Player.m_Library.m_Songs);
						m_AlbumInfo.Open(Playlist::GetPlaylistFromAlbumName(m_SearchProps.searchAlbums[row_n], m_Player.m_Library.m_Songs));
					}

					ImGui::EndPopup();
				}

				m_AlbumInfo.Render();
				Modals::OpenNewPlaylistModal(row_n, popup, true, m_Player, m_NewPlayListName, m_FilteredSongs);

				ImGui::PopID();
			}
		ImGui::EndTable();
	}
	ImGui::End();
}

void YAMP::AddAlbumToPlaylist(Player& player, Playlist* playlist, const std::string& albumName)
{
	std::vector<Song> songs = GetSongsFromAlbumName(albumName, player);

	for (const auto& song: songs)
	{
		playlist->songs.push_back(song);
		playlist->duration += song.duration;
	}
}

std::vector<Song> YAMP::GetSongsFromAlbumName(const std::string& name, Player& player)
{
	std::vector<Song> songs;

	for (auto& song: player.m_Library.m_Songs)
	{
		if (song.album == name)
		{
			songs.push_back(song);
		}
	}

	return songs;
}

void YAMP::DrawArtistPanel()
{
	ImGui::Begin("Artist");

	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x * 2);
	if (ImGui::InputTextWithHint("##artistsearchbar", "Search...", &m_SearchProps.artistSearchbar))
		m_SearchProps.searchArtists = Search(m_SearchProps.artistSearchbar, m_Player.m_Library.m_Artists);

	if (ImGui::BeginTable("Artist", 1, m_PanelFlags))
	{
		ImGui::TableSetupColumn("Artist", ImGuiTableColumnFlags_DefaultSort);
		ImGui::TableHeadersRow();

		if (ImGuiTableSortSpecs* sorts_specs = ImGui::TableGetSortSpecs())
			if (sorts_specs->SpecsDirty)
			{
				if (m_SearchProps.searchArtists.size() > 1)
					std::sort(m_SearchProps.searchArtists.begin(), m_SearchProps.searchArtists.end(), [&sorts_specs](const std::string& lhs, const std::string& rhs)
					{
						if (sorts_specs->Specs == nullptr)
						{
							return false;
						}
						return Compare::CompareString(lhs, rhs, sorts_specs->Specs->SortDirection == ImGuiSortDirection_Descending);
					});

				sorts_specs->SpecsDirty = false;
			}

		ImGui::PushID("All");
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Selectable("All", "All" == m_Player.m_Library.m_SelectedArtist, ImGuiSelectableFlags_SpanAllColumns);
		if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		{
			m_Player.m_Library.m_SelectedArtist = "All";
			Filter::FilterAlbum(m_Player, m_FilteredAlbums, m_SearchProps.searchAlbums, m_SearchProps.albumSearchbar);
		}
		ImGui::PopID();

		ImGuiListClipper clipper;
		clipper.Begin(m_SearchProps.searchArtists.size());
		while (clipper.Step())
		{
			for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
			{
				std::string artist = m_SearchProps.searchArtists[row_n];
				ImGui::PushID(artist.c_str());
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				bool selected = artist == m_Player.m_Library.m_SelectedArtist;
				ImGui::Selectable(artist.c_str(), selected, ImGuiSelectableFlags_SpanAllColumns);
				if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
				{
					m_Player.m_Library.m_SelectedArtist = artist;
					Filter::FilterAlbum(m_Player, m_FilteredAlbums, m_SearchProps.searchAlbums, m_SearchProps.albumSearchbar);
				}
				ImGui::PopID();
			}
		}
		ImGui::EndTable();
	}

	ImGui::End();
}

void YAMP::DrawPlaylistPanel()
{
	ImGui::Begin("Playlist");

	if (ImGui::BeginTable("Playlist", 1, m_PanelFlags))
	{
		ImGui::TableSetupColumn("Playlist", ImGuiTableColumnFlags_DefaultSort);
		ImGui::TableHeadersRow();

		if (ImGuiTableSortSpecs* sorts_specs = ImGui::TableGetSortSpecs())
			if (sorts_specs->SpecsDirty)
			{
				if (m_Player.m_Library.m_Playlists.size() > 1)
					std::sort(m_Player.m_Library.m_Playlists.begin(), m_Player.m_Library.m_Playlists.end(), Compare::ComparePlaylist);
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
				ImGui::Selectable(playlist.name.c_str(), selected, ImGuiSelectableFlags_SpanAllColumns);
				if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
				{
					m_SelectedPlaylist = m_Player.m_Library.m_Playlists[row_n];
					m_PlaylistClicked = true;
					m_ShouldFilterTracks = true;
				}

				Reorder(m_Player.m_Library.m_Playlists, row_n);

				if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
				{
					ImGui::OpenPopup("playlistpopup");
				}

				if (ImGui::BeginPopup("playlistpopup"))
				{
					if (ImGui::Selectable("Remove playlist"))
					{
						if (m_SelectedPlaylist.name == m_Player.m_Library.m_Playlists.at(row_n).name)
						{
							m_PlaylistClicked = false;
						}

						m_Player.m_Library.m_Playlists.erase(m_Player.m_Library.m_Playlists.begin() + row_n);
						m_Player.m_Library.SavePlaylists();
					}

					if (ImGui::Selectable("Rename"))
					{
						playlistRenamePopup = true;
						m_NewPlayListName = m_Player.m_Library.m_Playlists.at(row_n).name;
					}

					if (ImGui::Selectable("Info"))
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

				if (ImGui::BeginPopupModal("Rename playlist", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::InputText("Name", &m_NewPlayListName);

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

				if (ImGui::BeginPopupModal("Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
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


