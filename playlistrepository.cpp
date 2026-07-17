#include "playlistrepository.h"

#include <algorithm>

PlaylistRepository::PlaylistRepository(RepositoryData& data)
: m_data(data)
{
}

int PlaylistRepository::generateNextId() const
{
    int largestId = 0;

    for (const Playlist& playlist : m_data.playlists)
    {
        largestId = max(largestId, playlist.getId());
    }

    return largestId + 1;
}

int PlaylistRepository::save(const Playlist& playlist)
{
    if (playlist.getId() > 0)
    {
        const auto existing = find_if(m_data.playlists.begin(), m_data.playlists.end(),
            [&](const Playlist& stored)
            {
                return stored.getId() == playlist.getId();
            }
            );

        if (existing != m_data.playlists.end())
        {
            if (!playlist.isValid())
            {
                return -1;
            }

            *existing = playlist;
            return playlist.getId();
        }
    }

    Playlist storedPlaylist = playlist;
    storedPlaylist.setId(generateNextId());

    if (!storedPlaylist.isValid())
    {
        return -1;
    }

    m_data.playlists.push_back(storedPlaylist);
    return storedPlaylist.getId();
}

bool PlaylistRepository::remove(int id)
{
    const auto position = find_if(m_data.playlists.begin(), m_data.playlists.end(),
        [id](const Playlist& playlist)
        {
            return playlist.getId() == id;
        }
        );

    if (position == m_data.playlists.end())
    {
        return false;
    }

    m_data.playlists.erase(position);
    return true;
}

optional<Playlist> PlaylistRepository::search(int id) const
{
    const auto result = std::find_if(m_data.playlists.begin(),m_data.playlists.end(),
        [id](const Playlist& playlist)
        {
            return playlist.getId() == id;
        }
        );

    if (result == m_data.playlists.end())
    {
        return nullopt;
    }

    return *result;
}

bool PlaylistRepository::insertSong(int playlistId, int songId)
{
    const auto songExists = find_if(m_data.songs.begin(),m_data.songs.end(),
        [songId](const Song& song)
        {
            return song.getId() == songId;
        }
        );

    if (songExists == m_data.songs.end())
    {
        return false;
    }

    const auto playlist = find_if(m_data.playlists.begin(),m_data.playlists.end(),
        [playlistId](const Playlist& item)
        {
            return item.getId() == playlistId;
        }
        );

    if (playlist == m_data.playlists.end())
    {
        return false;
    }

    return playlist->addSong(songId);
}

bool PlaylistRepository::removeSong(int playlistId, int songId)
{
    const auto playlist = find_if(m_data.playlists.begin(), m_data.playlists.end(),
        [playlistId](const Playlist& item)
        {
            return item.getId() == playlistId;
        }
        );

    if (playlist == m_data.playlists.end())
    {
        return false;
    }

    return playlist->removeSong(songId);
}

vector<Playlist> PlaylistRepository::playlists(int listenerId) const
{
    vector<Playlist> result;

    for (const Playlist& playlist : m_data.playlists)
    {
        if (playlist.getListenerId() == listenerId)
        {
            result.push_back(playlist);
        }
    }

    return result;
}
