#include "YAMP.h"
#include "Log.h"
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

	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = "res/imgui.ini";

	BuildFont();

	SetWindowSize(glm::ivec2(m_Preferences.m_WindowWidth, m_Preferences.m_WindowHeight));
	m_Player.m_Volume = m_Preferences.m_Volume;
	m_Player.SetVolume(m_Preferences.m_Volume);

	m_ArtistPanel.Init(m_Player, m_FilteredAlbums, m_SearchProps);
	m_AlbumPanel.Init(m_ShouldFilterTracks, m_FilteredAlbums, m_SearchProps, m_Player.m_Library.m_Songs);
	m_ShouldFilterTracks = true;
}

void YAMP::OnUpdate()
{
	Dockspace();
	m_StatusPanel.RenderPanel(m_Player);

	if (m_ShowArtistPanel)
		m_ArtistPanel.RenderPanel(m_Player, m_FilteredAlbums, m_SearchProps);

	if (m_ShowAlbumPanel)
		m_AlbumPanel.RenderPanel(m_Player, m_FilteredAlbums, m_FilteredSongs, m_PlaylistClicked, m_ShouldFilterTracks, m_SearchProps);

	if (m_ShowTracksPanel)
		m_TracksPanel.RenderPanel(m_Player, m_FilteredSongs, m_Playlist, m_SelectedPlaylist, m_PlaylistClicked, m_ShouldFilterTracks);

	if (m_ShowPlaylistsPanel)
		m_PlaylistsPanel.RenderPanel(m_Player, m_SelectedPlaylist, m_PlaylistClicked, m_ShouldFilterTracks);

	if (m_ShowPreferencesPanel)
		PreferencesPanel();

	if (m_ShouldFilterTracks)
	{
		m_ShouldFilterTracks = false;
		Filter::FilterTracks(m_Player, m_Playlist, m_SelectedPlaylist, m_FilteredSongs, m_PlaylistClicked);
		if (m_TracksPanel.m_SortsSpecs)
			m_TracksPanel.m_SortsSpecs->SpecsDirty = true;
	}
	if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Space)))
	{
		if(m_Player.m_IsPaused)
			m_Player.Play();
		else
			m_Player.Pause();
	}

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

                ImGui::EndMenuBar();
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
