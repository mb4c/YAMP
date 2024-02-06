#include "Library.h"
#include <curl/curl.h>
#include <algorithm>
using namespace nlohmann;

void Library::Scan()
{
	ImGuiToast toast(ImGuiToastType_Info,"Library", "Library scan started", NOTIFY_DEFAULT_DISMISS);
	ImGui::InsertNotification(toast);
	auto start = std::chrono::high_resolution_clock::now();

    if (std::filesystem::exists("res/library.json"))
	{
		std::filesystem::remove("res/library.json");
	}

    m_Songs.clear();
	std::cout << m_LibraryPath << std::endl;

    if (std::filesystem::exists(m_LibraryPath))
    {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(std::filesystem::u8path(m_LibraryPath)))
        {
            if (entry.is_regular_file())
            {
                if (entry.path().extension() == ".mp3" || entry.path().extension() == ".flac" || entry.path().extension() == ".wav")
                {
                    TagLib::FileRef f(entry.path().string().c_str());
                    if(!f.isNull())
                    {
						bool hasMetadata = false;
                        Song song;
                        song.path = entry.path().u8string();
                        song.title = f.tag()->title().to8Bit(true);
                        if (song.title.empty())
						{
							song.title = song.path.stem().string();
							if(GetMetadataFromTitle(song.title, song))
								hasMetadata = true;
						}
						if (!hasMetadata)
						{
							song.artist = f.tag()->artist().to8Bit(true);
							if (song.artist.empty())
								song.artist = "No artist";
							song.track = f.tag()->track();
							song.album = f.tag()->album().to8Bit(true);
							if (song.album.empty())
								song.album = "No album";
							song.year = f.tag()->year();
							song.genre = f.tag()->genre().to8Bit(true);
						}

                        song.duration = f.audioProperties()->lengthInSeconds();
                        song.uuid = song.path;

                        m_Songs.push_back(song);

                    }
                    if(f.isNull())
						Log::Error("Cannot open file: file is null");
                }
            }
        }
    } else
    {
		Log::Error("Library folder does not exist");
    }

	Save();
	GenerateAlbums();

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(stop - start);
	std::stringstream ss;
	ss << "Library scanned in: " << duration.count() << "s\n";
	ss << "Total songs: " << m_Songs.size();
	ss << "\nTotal duration: " << SecondsToTimeHMS(m_TotalSongTime);
	ImGuiToast toast2(ImGuiToastType_Success, "Library", ss.str().c_str(), NOTIFY_DEFAULT_DISMISS);
	ImGui::InsertNotification(toast2);
}

void Library::Save()
{
    json j;
	for (const auto& song : m_Songs)
	{
		json json;
		json["title"] = song.title;
		json["artist"] = song.artist;
		json["album"] = song.album;
		json["year"] = song.year;
		json["track"] = song.track;
		json["genre"] = song.genre;
		json["path"] = song.path;
		json["duration"] = song.duration;
		json["uuid"] = song.uuid;
		j.push_back(json);
	}
	std::ofstream o("res/library.json");
	o << j;
	o.close();
	Log::Trace("Library saved");
}

void Library::Load()
{
    if (!std::filesystem::exists("res/library.json"))
	{
		Scan();
	}

	std::ifstream i("res/library.json");
	json j;
	i >> j;
	i.close();
	for (const auto& song : j)
	{
		Song s;
		s.title = song["title"];
		s.artist = song["artist"];
		s.album = song["album"];
		s.year = song["year"];
		s.track = song["track"];
		s.genre = song["genre"];
		std::string pathstr = song["path"];
		s.path = pathstr;
		s.duration = song["duration"];
		s.uuid = song["uuid"];
		m_Songs.push_back(s);
	}
	Log::Trace("Total songs: {}", m_Songs.size());
	GenerateAlbums();
	Log::Trace("Library loaded");

}

void Library::GenerateAlbums()
{
    for (auto & song : m_Songs)
    {
        if (std::find(m_Artists.begin(), m_Artists.end(), song.artist) == m_Artists.end())
        {
			m_Artists.push_back(song.artist);
        }
    }

    for (auto & song : m_Songs)
    {
        if (std::find(m_Albums.begin(), m_Albums.end(), song.album) == m_Albums.end())
        {
			m_Albums.push_back(song.album);
        }
    }

	CalculateTotalSongTime();
}

void Library::CalculateTotalSongTime()
{
	for (const auto& song : m_Songs)
	{
		m_TotalSongTime += song.duration;
	}
}

void Library::SavePlaylists()
{
	json j;
	for (auto & playlist : m_Playlists)
	{
		j[playlist.name]["name"] = playlist.name;
		j[playlist.name]["duration"] = playlist.duration;
		for (int k = 0; k < playlist.songs.size(); ++k)
		{
			j[playlist.name]["songs"][k] = playlist.songs.at(k).uuid;
		}
	}
	std::ofstream o("res/playlists.json");
	o << j;
	o.close();

	Log::Trace("Playlists saved");
}

void Library::LoadPlaylists()
{
	if (!std::filesystem::exists("res/playlists.json"))
	{
//		SavePlaylists();
		return;
	}

	std::ifstream i("res/playlists.json");
	json j;
	i >> j;
	i.close();
	for (const auto& playlist : j)
	{
		Playlist pl;
		pl.name = playlist["name"];
		pl.duration = playlist["duration"];

		for (const auto& song : playlist["songs"])
		{
			pl.songs.push_back(UUID2Song(song));
		}

		m_Playlists.push_back(pl);
	}
	Log::Trace("Playlists loaded");
}

Song Library::UUID2Song(const std::string& id)
{
	for (const auto& song : m_Songs)
	{
		if(song.uuid == id)
			return song;
	}
}

bool Library::GetMetadataFromTitle(const std::string& filename, Song& song)
{
	//TODO: if musicbrainz doesn't have song in database use regex
//	if (GetMetadataFromInternet(filename, song))
//		return true;
//	else
//		return GetMetadataFromRegex(filename, song);
	return GetMetadataFromRegex(filename, song);

}

size_t WriteCallback(char* contents, size_t size, size_t nmemb, std::string* output)
{
	size_t totalSize = size * nmemb;
	output->append(contents, totalSize);
	return totalSize;
}


bool Library::GetMetadataFromInternet(const std::string& filename, Song& song)
{
	CURL* curl = curl_easy_init();

	std::string escapedURL = curl_easy_escape(curl, filename.c_str(), 0);
	std::string queryUrl = "https://musicbrainz.org/ws/2/recording/?query=" + escapedURL + "&limit=1&fmt=json";
	std::string response;

	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, queryUrl.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "YAMP/1.0.0 ( https://github.com/mb4c/YAMP )");

		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK)
		{
			// Handle error
		}

		curl_easy_cleanup(curl);
	}

	nlohmann::json jsonData = nlohmann::json::parse(response);
	if (jsonData.contains("recordings"))
	{
		song.title = jsonData["recordings"][0]["title"];
		song.artist = jsonData["recordings"][0]["artist-credit"][0]["name"];
		song.track = std::stoi(jsonData["recordings"][0]["releases"][0]["media"][0]["track"][0]["number"].get<std::string>());
		song.album = jsonData["recordings"][0]["releases"][0]["title"];
		if (jsonData["recordings"][0]["releases"][0].contains("date"))
		{
			std::string date = jsonData["recordings"][0]["releases"][0]["date"];
			song.year = std::stoi(date.substr(0, 4));
		}
		else
		{
			return false;
		}

		return true;
	}
	else
	{
		return false;
	}

}

bool Library::GetMetadataFromRegex(const std::string& filename, Song& song)
{
	std::regex trackRegex("(\\d+)\\.? - ?(.*) - (.*)");
	std::smatch match;
	if (std::regex_search(filename, match, trackRegex))
		song.track = std::stoi(match[1]);
	else
	{
		std::cout << "Failed to get metadata from title:  " << filename << std::endl;
		return false;

	}

	song.artist = match[2];
	song.title = match[3];
	return true;
}

