#include "playlist.h"
#include <algorithm>

Playlist::Playlist()
{
    m_id = 0;
    m_listenerId = 0;
}

Playlist::Playlist(int id,const string& name,int listenerId)
{
    m_id = id;
    m_name = name;
    m_listenerId = listenerId;
}

int Playlist::getId() const
{
    return m_id;
}

const string& Playlist::getName() const
{
    return m_name;
}

int Playlist::getListenerId() const
{
    return m_listenerId;
}

const vector<int>& Playlist::getSongIds() const
{
    return m_songIds;
}

void Playlist::setName(const string& name)
{
    m_name = name;
}

bool Playlist::containsSong(int songId) const
{
    for (int currentSongId : m_songIds)
    {
        if (currentSongId == songId)
        {
            return true;
        }
    }

    return false;
}

bool Playlist::addSong(int songId)
{
    if (songId <= 0)
    {
        return false;
    }

    if (containsSong(songId))
    {
        return false;
    }

    m_songIds.push_back(songId);
    return true;
}

bool Playlist::removeSong(int songId)
{
    auto position = find(m_songIds.begin(),m_songIds.end(),songId);

    if (position == m_songIds.end())
    {
        return false;
    }

    m_songIds.erase(position);
    return true;
}

bool Playlist::isValid() const
{
    return m_id > 0 && !m_name.empty() && m_listenerId > 0;
}