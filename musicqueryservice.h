#ifndef MUSICQUERYSERVICE_H
#define MUSICQUERYSERVICE_H

#include "account.h"
#include "album.h"
#include "playlist.h"
#include "song.h"


class MusicQueryService
{
public:

    static vector<Song> searchSongsByName(const vector<Song>& songs,const string& searchText);

    static vector<Song> filterSongsByGenre(const vector<Song>& songs,const string& genre);

    static vector<Song> filterSongsByReleaseYear(const std::vector<Song>& songs,int releaseYear);


    static vector<Song> sortSongsByName(vector<Song> songs,bool ascending = true);

    static vector<Song> sortSongsByReleaseYear(vector<Song> songs,bool ascending = true);

    static vector<Account> sortArtistsByName(vector<Account> artists,bool ascending = true);

    static vector<Album> sortAlbumsByName(vector<Album> albums,bool ascending = true);

    static vector<Playlist> sortPlaylistsByName(vector<Playlist> playlists,bool ascending = true);


private:

    static string toLower(const string& text);

    static string trim(const string& text);
};

#endif