#ifndef ARTISTSERVICE_H
#define ARTISTSERVICE_H

#include "albumrepository.h"
#include "artistrepository.h"
#include "playlistrepository.h"
#include "songrepository.h"


struct ArtistActionResult
{
    bool success{false};
    string message;
    int id{-1};
};

class ArtistService
{
public:
    ArtistService(
        ArtistRepository& artistRepository,
        AlbumRepository& albumRepository,
        SongRepository& songRepository,
        PlaylistRepository& playlistRepository
        );

    ArtistActionResult createAlbum(int artistId,const string& name,const string& coverPath = "");

    ArtistActionResult createSong(
        int artistId,
        int albumId,
        const string& name,
        int releaseYear,
        const string& genre,
        const string& audioFilePath,
        const string& coverPath = ""
        );

    ArtistActionResult createSingle(
        int artistId,
        const string& name,
        int releaseYear,
        const string& genre,
        const string& audioFilePath,
        const string& coverPath = ""
        );

    bool updateAlbum(
        int artistId,
        int albumId,
        const string& name,
        const string& coverPath
        );

    bool updateSong(
        int artistId,
        int songId,
        const string& name,
        int releaseYear,
        const string& genre,
        int albumId,
        const string& audioFilePath,
        const string& coverPath
        );

    bool deleteAlbum(int artistId,int albumId);

    bool deleteSong(int artistId,int songId);

    vector<Album> getAlbums(int artistId) const;

    vector<Song> getSingles(int artistId) const;

private:
    ArtistRepository& m_artistRepository;
    AlbumRepository& m_albumRepository;
    SongRepository& m_songRepository;
    PlaylistRepository& m_playlistRepository;

    bool artistExists(int artistId) const;

    void removeSongFromAllPlaylists(int songId);
};

#endif