#ifndef CATALOGSERVICE_H
#define CATALOGSERVICE_H

#include "albumrepository.h"
#include "artistrepository.h"
#include "songrepository.h"


class CatalogService
{
public:
    CatalogService(ArtistRepository& artistRepository,AlbumRepository& albumRepository,SongRepository& songRepository );

    vector<Account> getAllArtists() const;

    optional<Account> getArtist(int artistId) const;

    vector<Album> getArtistAlbums(int artistId) const;

    vector<Song> getArtistSingles(int artistId) const;

    vector<Song> getAlbumSongs(int albumId) const;

    optional<Song> getSong(int songId) const;
    optional<Album> getAlbum(int albumId) const;

private:
    ArtistRepository& m_artistRepository;
    AlbumRepository& m_albumRepository;
    SongRepository& m_songRepository;
};

#endif