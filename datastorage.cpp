#include "datastorage.h"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <string>



DataStorage::DataStorage(const string& filePath)
: m_filePath(filePath)
{
}


bool DataStorage::saveAll(const RepositoryData& data) const
{

    filesystem::path path(m_filePath);

    if (path.has_parent_path())
    {
        try
        {
            filesystem::create_directories(path.parent_path());
        }
        catch (const filesystem::filesystem_error&)
        {
            return false;
        }
    }

    ofstream file(m_filePath,ios::trunc);

    if (!file.is_open())
    {
        return false;
    }

    file << "APP_DATA_V1\n";

    file << "ARTISTS " << data.artists.size()<< '\n';

    for (const Account& artist : data.artists)
    {
        file << artist.getId() << ' '
            << quoted(artist.getFullName()) << ' '
            << quoted(artist.getUsername()) << ' '
            << quoted(artist.getBiography()) << ' '
            << quoted(artist.getRole()) << ' '
            << quoted(artist.getPassword()) << ' '
            << quoted(artist.getProfilePhotoPath())
            << '\n';
    }

    file << "LISTENERS " << data.listeners.size()<< '\n';

    for (const Account& listener : data.listeners)
    {
        file<< listener.getId() << ' '
            << quoted(listener.getFullName()) << ' '
            << quoted(listener.getUsername()) << ' '
            << quoted(listener.getBiography()) << ' '
            << quoted(listener.getRole()) << ' '
            << quoted(listener.getPassword()) << ' '
             << quoted(listener.getProfilePhotoPath()) << '\n';
    }

    file << "ALBUMS "<< data.albums.size() << '\n';

    for (const Album& album : data.albums)
    {
        file<< album.getId() << ' '
            << quoted(album.getName()) << ' '
            << album.getArtistId() << ' '
            << quoted(album.getCoverPath())<< '\n';
    }

    file << "SONGS " << data.songs.size() << '\n';

    for (const Song& song : data.songs)
    {
        file<< song.getId() << ' '
            << quoted(song.getName())<< ' '
            << song.getReleaseYear() << ' '
            << quoted(song.getGenre()) << ' '
            << song.getArtistId() << ' '
            << song.getAlbumId() << ' '
            << quoted(song.getAudioFilePath())<< ' '
            << quoted(song.getCoverPath())<< '\n';
    }

    file << "PLAYLISTS " << data.playlists.size() << '\n';


    for (const Playlist& playlist : data.playlists)
    {
        file<< playlist.getId() << ' '
            << quoted(playlist.getName()) << ' '
            << playlist.getListenerId() << ' '
            << playlist.getSongIds().size();

        for (int songId : playlist.getSongIds())
        {
            file << ' ' << songId;
        }

        file << '\n';
    }

    if (!file.good())
    {
        return false;
    }

    return true;
}

bool DataStorage::loadAll(RepositoryData& data) const
{
    ifstream file(m_filePath);

    if (!file.is_open())
    {
        return true;
    }

    RepositoryData loadedData;

    string version;

    file >> version;

    if (version != "APP_DATA_V1")
    {
        return false;
    }

    string sectionName;
    size_t count;

    file >> sectionName >> count;

    if (sectionName != "ARTISTS")
    {
        return false;
    }

    for (size_t i = 0; i < count; ++i)
    {
        int id;
        string fullName;
        string username;
        string biography;
        string role;
        string password;
        string profilePhotoPath;

        file>> id
            >> quoted(fullName)
            >> quoted(username)
            >> quoted(biography)
            >> quoted(role)
            >> quoted(password)
            >> quoted(profilePhotoPath);

        if (file.fail())
        {
            return false;
        }

        Account artist(
            id,
            fullName,
            username,
            biography,
            role,
            password,
            profilePhotoPath
            );


        if (!artist.isValid() || !artist.isArtist())
        {
            return false;
        }

        loadedData.artists.push_back(artist);
    }


    file >> sectionName >> count;

    if (sectionName != "LISTENERS")
    {
        return false;
    }

    for (size_t i = 0; i < count; ++i)
    {
        int id;
        string fullName;
        string username;
        string biography;
        string role;
        string password;
        string profilePhotoPath;

        file>>id
            >>quoted(fullName)
            >>quoted(username)
            >>quoted(biography)
            >>quoted(role)
            >>quoted(password)
            >>quoted(profilePhotoPath);

        if (file.fail())
        {
            return false;
        }

        Account listener(
            id,
            fullName,
            username,
            biography,
            role,
            password,
            profilePhotoPath
            );

        if (!listener.isValid() || !listener.isListener())
        {
            return false;
        }

        loadedData.listeners.push_back(listener);
    }


    file >> sectionName >> count;

    if (sectionName != "ALBUMS")
    {
        return false;
    }

    for (size_t i = 0; i < count; ++i)
    {
        int id;
        int artistId;
        string name;
        string coverPath;

        file >> id
            >> quoted(name)
            >> artistId
            >> quoted(coverPath);

        if (file.fail())
        {
            return false;
        }

        Album album(
            id,
            name,
            artistId,
            coverPath
            );


        if (!album.isValid())
        {
            return false;
        }

        loadedData.albums.push_back(album);
    }

    file >> sectionName >> count;

    if (sectionName != "SONGS")
    {
        return false;
    }

    for (size_t i = 0; i < count; ++i)
    {
        int id;
        int releaseYear;
        int artistId;
        int albumId;

        string name;
        string genre;
        string audioFilePath;
        string coverPath;

        file >> id
            >> quoted(name)
            >> releaseYear
            >> quoted(genre)
            >> artistId
            >> albumId
            >> quoted(audioFilePath)
            >> quoted(coverPath);

        if (file.fail())
        {
            return false;
        }

        Song song(
            id,
            name,
            releaseYear,
            genre,
            artistId,
            albumId,
            audioFilePath,
            coverPath
            );


        if (!song.isValid())
        {
            return false;
        }

        loadedData.songs.push_back(song);
    }

    file >> sectionName >> count;

    if (sectionName != "PLAYLISTS")
    {
        return false;
    }

    for (size_t i = 0; i < count; ++i)
    {
        int id;
        int listenerId;
        string name;
        size_t songCount;

        file>> id
            >> quoted(name)
            >> listenerId
            >> songCount;

        if (file.fail())
        {
            return false;
        }

        Playlist playlist(id,name,listenerId);

        for (size_t j = 0; j < songCount; ++j)
        {
            int songId;

            file >> songId;

            if (file.fail())
            {
                return false;
            }

            if (!playlist.addSong(songId))
            {
                return false;
            }
        }


        if (!playlist.isValid())
        {
            return false;
        }


        loadedData.playlists.push_back(playlist);
    }

    data = loadedData;

    return true;
}