 #ifndef REPOSITORY_DATA_H
#define REPOSITORY_DATA_H

#include "account.h"
#include "album.h"
#include "playlist.h"
#include "song.h"

#include <vector>

struct RepositoryData
{
    vector<Account> artists;
    vector<Account> listeners;
    vector<Album> albums;
    vector<Song> songs;
    vector<Playlist> playlists;

};

#endif // REPOSITORY_DATA_H
