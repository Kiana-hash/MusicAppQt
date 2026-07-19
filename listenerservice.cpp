#include "listenerservice.h"

#include <algorithm>
#include <cctype>


ListenerService::ListenerService(
    ListenerRepository& listenerRepository,
    PlaylistRepository& playlistRepository,
    SongRepository& songRepository
    )
    : m_listenerRepository(listenerRepository),
    m_playlistRepository(playlistRepository),
    m_songRepository(songRepository)
{
}

string ListenerService::trim(const string& text)
{
    const size_t first = text.find_first_not_of(" \t\n\r");

    if (first == string::npos)
    {
        return "";
    }

    const size_t last = text.find_last_not_of(" \t\n\r");

    return text.substr(first,last - first + 1);
}

string ListenerService::toLower( const string& text)
{
    string result = text;

    transform(result.begin(),result.end(),result.begin(),
        [](unsigned char character)
        {
            return static_cast<char>(tolower(character));
        }
        );

    return result;
}

bool ListenerService::listenerExists(int listenerId) const
{
    return m_listenerRepository.search(listenerId).has_value();
}

bool ListenerService::listenerOwnsPlaylist(int listenerId,int playlistId) const
{
    const optional<Playlist> playlist =m_playlistRepository.search(playlistId);

    if (!playlist.has_value())
    {
        return false;
    }

    return playlist->getListenerId()== listenerId;
}

bool ListenerService::isFavoriteSongs(const Playlist& playlist) const
{
    return toLower(trim(playlist.getName())) == "favorite songs";
}

ListenerActionResult  ListenerService::createPlaylist(int listenerId,const string& name)
{
    if (!listenerExists(listenerId))
    {
        return {
            false,
            "Listener does not exist.",
            -1
        };
    }

    const string cleanedName =trim(name);

    if (cleanedName.empty())
    {
        return {
            false,
            "Playlist name cannot be empty.",
            -1
        };
    }


    if (toLower(cleanedName)== "favorite songs")
    {
        return {
            false,
            "Favorite Songs is a reserved playlist.",
            -1
        };
    }

    Playlist playlist(0,cleanedName,listenerId);

    const int playlistId =m_playlistRepository.save(playlist);

    if (playlistId == -1)
    {
        return {
            false,
            "Could not create playlist.",
            -1
        };
    }

    return {
        true,
        "Playlist created successfully.",
        playlistId
    };
}

ListenerActionResult  ListenerService::renamePlaylist(int listenerId,int playlistId,const string& newName)
{
    if (!listenerExists(listenerId))
    {
        return {
            false,
            "Listener does not exist.",
            -1
        };
    }

    const optional<Playlist> playlist =m_playlistRepository.search(playlistId);

    if (!playlist.has_value())
    {
        return {
            false,
            "Playlist does not exist.",
            -1
        };
    }

    if (playlist->getListenerId()!= listenerId)
    {
        return {
            false,
            "This playlist does not belong to the listener.",
            -1
        };
    }

    if (isFavoriteSongs(*playlist))
    {
        return {
            false,
            "Favorite Songs cannot be renamed.",
            -1
        };
    }

    const string cleanedName =trim(newName);

    if (cleanedName.empty())
    {
        return {
            false,
            "Playlist name cannot be empty.",
            -1
        };
    }

    if (toLower(cleanedName)== "favorite songs")
    {
        return {
            false,
            "Favorite Songs is a reserved playlist name.",
            -1
        };
    }

    Playlist updatedPlaylist =*playlist;

    updatedPlaylist.setName(cleanedName);

    const int savedId =m_playlistRepository.save(updatedPlaylist);

    if (savedId == -1)
    {
        return {
            false,
            "Could not rename playlist.",
            -1
        };
    }

    return {
        true,
        "Playlist renamed successfully.",
        savedId
    };
}

ListenerActionResult  ListenerService::deletePlaylist(int listenerId,int playlistId)
{
    if (!listenerExists(listenerId))
    {
        return {
            false,
            "Listener does not exist.",
            -1
        };
    }

    const optional<Playlist> playlist =m_playlistRepository.search(playlistId);

    if (!playlist.has_value())
    {
        return {
            false,
            "Playlist does not exist.",
            -1
        };
    }

    if (playlist->getListenerId()!= listenerId)
    {
        return {
            false,
            "This playlist does not belong to the listener.",
            -1
        };
    }

    if (isFavoriteSongs(*playlist))
    {
        return {
            false,
            "Favorite Songs cannot be deleted.",
            -1
        };
    }

    if (!m_playlistRepository.remove(playlistId))
    {
        return {
            false,
            "Could not delete playlist.",
            -1
        };
    }

    return {
        true,
        "Playlist deleted successfully.",
        playlistId
    };
}

ListenerActionResult  ListenerService::addSongToPlaylist(int listenerId,int playlistId,int songId)
{
    if (!listenerExists(listenerId))
    {
        return {
            false,
            "Listener does not exist.",
            -1
        };
    }

    const optional<Playlist> playlist = m_playlistRepository.search(playlistId);

    if (!playlist.has_value())
    {
        return {
            false,
            "Playlist does not exist.",
            -1
        };
    }

    if (playlist->getListenerId()!= listenerId)
    {
        return {
            false,
            "This playlist does not belong to the listener.",
            -1
        };
    }

    if (!m_songRepository.search(songId).has_value())
    {
        return {
            false,
            "Song does not exist.",
            -1
        };
    }

    if (playlist->containsSong(songId))
    {
        return {
            false,
            "Song is already in this playlist.",
            playlistId
        };
    }

    if (!m_playlistRepository.insertSong(playlistId,songId))
    {
        return {
            false,
            "Could not add song to playlist.",
            -1
        };
    }

    return {
        true,
        "Song added to playlist successfully.",
        playlistId
    };
}

ListenerActionResult  ListenerService::removeSongFromPlaylist(int listenerId,int playlistId,int songId)
{
    if (!listenerExists(listenerId))
    {
        return {
            false,
            "Listener does not exist.",
            -1
        };
    }

    const optional<Playlist> playlist =m_playlistRepository.search(playlistId);

    if (!playlist.has_value())
    {
        return {
            false,
            "Playlist does not exist.",
            -1
        };
    }

    if (playlist->getListenerId()!= listenerId)
    {
        return {
            false,
            "This playlist does not belong to the listener.",
            -1
        };
    }

    if (!playlist->containsSong(songId))
    {
        return {
            false,
            "Song is not in this playlist.",
            -1
        };
    }

    if (!m_playlistRepository.removeSong(playlistId,songId))
    {
        return {
            false,
            "Could not remove song from playlist.",
            -1
        };
    }

    return {
        true,
        "Song removed from playlist successfully.",
        playlistId
    };
}

ListenerActionResult  ListenerService::likeSong(int listenerId,int songId)
{
    if (!listenerExists(listenerId))
    {
        return {
            false,
            "Listener does not exist.",
            -1
        };
    }

    if (!m_songRepository.search(songId).has_value())
    {
        return {
            false,
            "Song does not exist.",
            -1
        };
    }

    if (m_listenerRepository.isLiked(listenerId,songId))
    {
        return {
            false,
            "Song is already liked.",
            songId
        };
    }

    if (!m_listenerRepository.updateLiked(listenerId,songId,true))
    {
        return {
            false,
            "Could not like song.",
            -1
        };
    }

    return {
        true,
        "Song liked successfully.",
        songId
    };
}

ListenerActionResult  ListenerService::unlikeSong(int listenerId,int songId)
{
    if (!listenerExists(listenerId))
    {
        return {
            false,
            "Listener does not exist.",
            -1
        };
    }

    if (!m_listenerRepository.isLiked(listenerId,songId))
    {
        return {
            false,
            "Song is not liked.",
            songId
        };
    }

    if (!m_listenerRepository.updateLiked(listenerId,songId,false))
    {
        return {
            false,
            "Could not unlike song.",
            -1
        };
    }

    return {
        true,
        "Song unliked successfully.",
        songId
    };
}

bool ListenerService::isLiked(int listenerId,int songId) const
{
    if (!listenerExists(listenerId))
    {
        return false;
    }

    return m_listenerRepository.isLiked(listenerId,songId);
}

vector<Playlist>  ListenerService::getPlaylists(int listenerId) const
{
    if (!listenerExists(listenerId))
    {
        return {};
    }

    return m_playlistRepository.playlists(listenerId);
}

vector<Song>  ListenerService::getPlaylistSongs(int listenerId,int playlistId) const
{
    if (!listenerOwnsPlaylist(listenerId,playlistId))
    {
        return {};
    }

    return m_songRepository.getByPlaylist(playlistId);
}

vector<Song>  ListenerService::getLikedSongs(int listenerId) const
{
    if (!listenerExists(listenerId))
    {
        return {};
    }

    return m_songRepository.getByLikedSongs(listenerId);
}