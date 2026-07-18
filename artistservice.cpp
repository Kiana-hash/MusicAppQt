#include "artistservice.h"

ArtistService::ArtistService(
    ArtistRepository& artistRepository,
    AlbumRepository& albumRepository,
    SongRepository& songRepository,
    PlaylistRepository& playlistRepository
    )
    : m_artistRepository(artistRepository),
    m_albumRepository(albumRepository),
    m_songRepository(songRepository),
    m_playlistRepository(playlistRepository)
{
}

bool ArtistService::artistExists(int artistId) const
{
    return m_artistRepository.search(artistId).has_value();
}

ArtistActionResult ArtistService::createAlbum(int artistId,const string& name,const string& coverPath)
{
    if (!artistExists(artistId))
    {
        return {
            false,
            "Artist does not exist.",
            -1
        };
    }

    if (name.empty())
    {
        return {
            false,
            "Album name cannot be empty.",
            -1
        };
    }

    Album album(0,name,artistId,coverPath);

    const int albumId =m_albumRepository.save(album);

    if (albumId == -1)
    {
        return {
            false,
            "Could not create album.",
            -1
        };
    }

    return {
        true,
        "Album created successfully.",
        albumId
    };
}

ArtistActionResult ArtistService::createSong(
    int artistId,
    int albumId,
    const string& name,
    int releaseYear,
    const string& genre,
    const string& audioFilePath,
    const string& coverPath
    )
{
    if (!artistExists(artistId))
    {
        return {
            false,
            "Artist does not exist.",
            -1
        };
    }

    const optional<Album> album =m_albumRepository.search(albumId);

    if (!album.has_value())
    {
        return {
            false,
            "Album does not exist.",
            -1
        };
    }

    if (album->getArtistId() != artistId)
    {
        return {
            false,
            "This album does not belong to the artist.",
            -1
        };
    }

    Song song(
        0,
        name,
        releaseYear,
        genre,
        artistId,
        albumId,
        audioFilePath,
        coverPath
        );

    const int songId =m_songRepository.save(song);

    if (songId == -1)
    {
        return {
            false,
            "Could not create song.",
            -1
        };
    }

    return {
        true,
        "Song created successfully.",
        songId
    };
}

ArtistActionResult ArtistService::createSingle(
    int artistId,
    const string& name,
    int releaseYear,
    const string& genre,
    const string& audioFilePath,
    const string& coverPath
    )
{
    if (!artistExists(artistId))
    {
        return {
            false,
            "Artist does not exist.",
            -1
        };
    }

    Song song(
        0,
        name,
        releaseYear,
        genre,
        artistId,
        0,
        audioFilePath,
        coverPath
        );

    const int songId =m_songRepository.save(song);

    if (songId == -1)
    {
        return {
            false,
            "Could not create single.",
            -1
        };
    }

    return {
        true,
        "Single created successfully.",
        songId
    };
}