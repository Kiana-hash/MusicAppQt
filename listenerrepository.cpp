#include "listenerrepository.h"

#include <algorithm>

ListenerRepository::ListenerRepository(RepositoryData& data): AccountRepository(data)
{
}

int ListenerRepository::save(const Account& account)
{
    if (!account.isListener())
    {
        return -1;
    }

    if (account.getId() > 0)
    {
        const auto existing = find_if(m_data.listeners.begin(),m_data.listeners.end(),
            [&](const Account& stored)
            {
                return stored.getId() == account.getId();
            }
            );

        if (existing != m_data.listeners.end())
        {
            if (!account.isValid()|| !isUserNameAvailable(account.getUsername(),account.getId()))
            {
                return -1;
            }

            *existing = account;
            return account.getId();
        }
    }

    Account storedAccount = account;
    storedAccount.setId(generateNextAccountId());

    if (!storedAccount.isValid()|| !isUserNameAvailable(storedAccount.getUsername()))
    {
        return -1;
    }

    m_data.listeners.push_back(storedAccount);
    return storedAccount.getId();
}

bool ListenerRepository::remove(int id)
{
    const auto position = find_if(m_data.listeners.begin(),m_data.listeners.end(),
        [id](const Account& account)
        {
            return account.getId() == id;
        }
        );

    if (position == m_data.listeners.end())
    {
        return false;
    }

    m_data.listeners.erase(position);
    return true;
}

optional<Account> ListenerRepository::search(int id) const
{
    const auto result = find_if(m_data.listeners.begin(),m_data.listeners.end(),
        [id](const Account& account)
        {
            return account.getId() == id;
        }
        );

    if (result == m_data.listeners.end())
    {
        return nullopt;
    }

    return *result;
}

optional<Account> ListenerRepository::searchByUserName(const string& username) const
{
    const string normalized = toLower(username);

    const auto result = find_if(m_data.listeners.begin(),m_data.listeners.end(),
        [&](const Account& account)
        {
            return toLower(account.getUsername()) == normalized;
        }
        );

    if (result == m_data.listeners.end())
    {
        return nullopt;
    }

    return *result;
}

bool ListenerRepository::updateLiked(int listenerId,int songId,bool liked)
{
    if (!search(listenerId).has_value())
    {
        return false;
    }

    const auto song = find_if(m_data.songs.begin(),m_data.songs.end(),
        [songId](const Song& currentSong)
        {
            return currentSong.getId() == songId;
        }
        );

    if (song == m_data.songs.end())
    {
        return false;
    }

    const auto favoritePlaylist = find_if(m_data.playlists.begin(),m_data.playlists.end(),
        [listenerId](const Playlist& playlist)
        {
            return playlist.getListenerId() == listenerId && playlist.getName() == "Favorite Songs";
        }
        );

    if (favoritePlaylist == m_data.playlists.end())
    {
        return false;
    }

    if (liked)
    {
        if (favoritePlaylist->containsSong(songId))
        {
            return true;
        }

        return favoritePlaylist->addSong(songId);
    }

    if (!favoritePlaylist->containsSong(songId))
    {
        return true;
    }

    return favoritePlaylist->removeSong(songId);
}

bool ListenerRepository::isLiked(int listenerId,int songId) const
{
    const auto favoritePlaylist = find_if(m_data.playlists.begin(), m_data.playlists.end(),
        [listenerId](const Playlist& playlist)
        {
            return playlist.getListenerId() == listenerId && playlist.getName() == "Favorite Songs";
        }
        );

    if (favoritePlaylist == m_data.playlists.end())
    {
        return false;
    }

    return favoritePlaylist->containsSong(songId);
}

const vector<int>& ListenerRepository::getLikedSongIds(int listenerId) const
{
    const auto favoritePlaylist = find_if(
        m_data.playlists.begin(),
        m_data.playlists.end(),
        [listenerId](const Playlist& playlist)
        {
            return playlist.getListenerId() == listenerId && playlist.getName() == "Favorite Songs";
        }
        );

    if (favoritePlaylist == m_data.playlists.end())
    {
        static const vector<int> emptyLikedSongs;
        return emptyLikedSongs;
    }

    return favoritePlaylist->getSongIds();
}

const vector<Account>& ListenerRepository::getAll() const
{
    return m_data.listeners;
}
