#ifndef LISTENERSERVICE_H
#define LISTENERSERVICE_H

#include "listenerrepository.h"
#include "playlistrepository.h"
#include "songrepository.h"


struct ListenerActionResult
{
    bool success{false};
    string message;
    int id{-1};
};

class ListenerService
{
public:
    ListenerService(
        ListenerRepository& listenerRepository,
        PlaylistRepository& playlistRepository,
        SongRepository& songRepository
        );

    ListenerActionResult createPlaylist(int listenerId,const string& name);

    ListenerActionResult renamePlaylist(int listenerId,int playlistId,const string& newName);

    ListenerActionResult deletePlaylist(int listenerId,int playlistId);

    ListenerActionResult addSongToPlaylist(int listenerId,int playlistId,int songId);

    ListenerActionResult removeSongFromPlaylist(int listenerId,int playlistId,int songId);

    ListenerActionResult likeSong(int listenerId,int songId);

    ListenerActionResult unlikeSong(int listenerId,int songId);

    bool isLiked(int listenerId,int songId) const;

    vector<Playlist> getPlaylists(int listenerId) const;

    vector<Song> getPlaylistSongs(int listenerId,int playlistId) const;

    vector<Song> getLikedSongs(int listenerId) const;

private:
    ListenerRepository& m_listenerRepository;
    PlaylistRepository& m_playlistRepository;
    SongRepository& m_songRepository;

    bool listenerExists(int listenerId) const;

    bool listenerOwnsPlaylist(int listenerId,int playlistId) const;

    bool isFavoriteSongs(const Playlist& playlist) const;

    static string trim(const string& text);

    static string toLower(const string& text);
};

#endif