#include "songrepository.h"

#include <algorithm>

SongRepository::SongRepository(RepositoryData& data)
: m_data(data)
{
}

int SongRepository::generateNextId() const
{
    int largestId = 0;

    for (const Song& song : m_data.songs)
    {
        largestId = max(largestId, song.getId());
    }

    return largestId + 1;
}

int SongRepository::save(const Song& song)
{
    if (song.getId() > 0)
    {
        const auto existing = find_if( m_data.songs.begin(), m_data.songs.end(),
            [&](const Song& stored)
            {
                return stored.getId() == song.getId();
            }
            );

        if (existing != m_data.songs.end())
        {
            if (!song.isValid())
            {
                return -1;
            }

            *existing = song;
            return song.getId();
        }
    }

    Song storedSong = song;
    storedSong.setId(generateNextId());

    if (!storedSong.isValid())
    {
        return -1;
    }

    m_data.songs.push_back(storedSong);
    return storedSong.getId();
}

bool SongRepository::remove(int id)
{
    const auto position = find_if(m_data.songs.begin(),m_data.songs.end(),
        [id](const Song& song)
        {
            return song.getId() == id;
        }
        );

    if (position == m_data.songs.end())
    {
        return false;
    }

    m_data.songs.erase(position);
    return true;
}

optional<Song> SongRepository::search(int id) const
{
    const auto result = std::find_if( m_data.songs.begin(), m_data.songs.end(),
        [id](const Song& song)
        {
            return song.getId() == id;
        }
        );

    if (result == m_data.songs.end())
    {
        return nullopt;
    }

    return *result;
}

vector<Song> SongRepository::singleSongs(int artistId) const
{
    vector<Song> result;

    for (const Song& song : m_data.songs)
    {
        if (song.getArtistId() == artistId && song.isSingle())
        {
            result.push_back(song);
        }
    }

    return result;
}

vector<Song> SongRepository::getByAlbum(int albumId) const
{
    vector<Song> result;

    for (const Song& song : m_data.songs)
    {
        if (song.getAlbumId() == albumId)
        {
            result.push_back(song);
        }
    }

    return result;
}

vector<Song> SongRepository::getByArtist(int artistId) const
{
    vector<Song> result;

    for (const Song& song : m_data.songs)
    {
        if (song.getArtistId() == artistId)
        {
            result.push_back(song);
        }
    }

    return result;
}

vector<Song> SongRepository::getByPlaylist(int playlistId) const
{
    vector<Song> result;

    const auto playlist = find_if( m_data.playlists.begin(),m_data.playlists.end(),
        [playlistId](const Playlist& item)
        {
            return item.getId() == playlistId;
        }
        );

    if (playlist == m_data.playlists.end())
    {
        return result;
    }

    for (int songId : playlist->getSongIds())
    {
        const auto song = search(songId);

        if (song.has_value())
        {
            result.push_back(*song);
        }
    }

    return result;
}

vector<Song>  SongRepository::getByLikedSongs(int listenerId) const
{
    vector<Song> result;

    const auto favoritePlaylist = find_if(m_data.playlists.begin(),m_data.playlists.end(),
        [listenerId](const Playlist& playlist)
        {
            return playlist.getListenerId() == listenerId && playlist.getName() == "Favorite Songs";
        }
        );

    if (favoritePlaylist == m_data.playlists.end())
    {
        return result;
    }

    for (int songId : favoritePlaylist->getSongIds())
    {
        const auto song = search(songId);

        if (song.has_value())
        {
            result.push_back(*song);
        }
    }

    return result;
}

const vector<Song>& SongRepository::getAll() const
{
    return m_data.songs;
}
