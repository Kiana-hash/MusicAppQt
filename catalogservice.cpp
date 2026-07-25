#include "catalogservice.h"
#include "musicqueryservice.h"


CatalogService::CatalogService(
    ArtistRepository& artistRepository,
    AlbumRepository& albumRepository,
    SongRepository& songRepository
    )
    : m_artistRepository(artistRepository),
    m_albumRepository(albumRepository),
    m_songRepository(songRepository)
{
}


vector<Account>  CatalogService::getAllArtists() const
{
    vector<Account> artists =m_artistRepository.getAll();

    return MusicQueryService::sortArtistsByName(artists);
}


optional<Account>  CatalogService::getArtist(int artistId) const
{
    return m_artistRepository.search(artistId);
}


vector<Album> CatalogService::getArtistAlbums(int artistId) const
{
    if (!m_artistRepository.search(artistId).has_value())
    {
        return {};
    }

    vector<Album> albums =m_albumRepository.albums(artistId);

    return MusicQueryService::sortAlbumsByName(albums);
}


vector<Song>  CatalogService::getArtistSingles(int artistId) const
{
    if (!m_artistRepository.search(artistId).has_value())
    {
        return {};
    }

    return m_songRepository.singleSongs(artistId);
}


vector<Song>  CatalogService::getAlbumSongs(int albumId) const
{
    if (!m_albumRepository.search(albumId).has_value())
    {
        return {};
    }

    return m_songRepository.getByAlbum(albumId);
}


optional<Song>  CatalogService::getSong(int songId) const
{
    return m_songRepository.search(songId);
}

optional<Album> CatalogService::getAlbum(int albumId) const
{
    return m_albumRepository.search(albumId);
}

vector<Song> CatalogService::getAllSongs() const
{
    vector<Song> songs = m_songRepository.getAll();

    return MusicQueryService::sortSongsByName(songs);
}