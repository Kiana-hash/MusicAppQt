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

bool ArtistService::updateAlbum(int artistId,int albumId,const string& name,const string& coverPath)
{
    const optional<Album> existingAlbum = m_albumRepository.search(albumId);

    if (!existingAlbum.has_value())
    {
        return false;
    }

    if (existingAlbum->getArtistId() != artistId)
    {
        return false;
    }

    if (name.empty())
    {
        return false;
    }

    Album updatedAlbum(albumId,name,artistId,coverPath );

    return m_albumRepository.save(updatedAlbum) != -1;
}

bool ArtistService::updateSong(
    int artistId,
    int songId,
    const string& name,
    int releaseYear,
    const string& genre,
    int albumId,
    const string& audioFilePath,
    const string& coverPath
    )
{
    const optional<Song> existingSong =m_songRepository.search(songId);

    if (!existingSong.has_value())
    {
        return false;
    }

    if (existingSong->getArtistId() != artistId)
    {
        return false;
    }

    if (name.empty()|| genre.empty()|| audioFilePath.empty()|| releaseYear <= 0)
    {
        return false;
    }

    if (albumId != 0)
    {
        const optional<Album> album = m_albumRepository.search(albumId);

        if (!album.has_value())
        {
            return false;
        }

        if (album->getArtistId() != artistId)
        {
            return false;
        }
    }

    Song updatedSong(
        songId,
        name,
        releaseYear,
        genre,
        artistId,
        albumId,
        audioFilePath,
        coverPath
        );

    return m_songRepository.save(updatedSong) != -1;
}

void ArtistService::removeSongFromAllPlaylists(int songId)
{

    const vector<Playlist> playlists =m_playlistRepository.getAll();

    for (const Playlist& playlist : playlists)
    {
        if (playlist.containsSong(songId))
        {
            m_playlistRepository.removeSong(playlist.getId(),songId);
        }
    }
}

bool ArtistService::deleteSong(int artistId,int songId)
{
    const optional<Song> song =m_songRepository.search(songId);

    if (!song.has_value())
    {
        return false;
    }

    if (song->getArtistId() != artistId)
    {
        return false;
    }

    removeSongFromAllPlaylists(songId);

    return m_songRepository.remove(songId);
}

bool ArtistService::deleteAlbum(int artistId,int albumId)
{
    const optional<Album> album =m_albumRepository.search(albumId);

    if (!album.has_value())
    {
        return false;
    }

    if (album->getArtistId() != artistId)
    {
        return false;
    }

    const vector<Song> albumSongs =m_songRepository.getByAlbum(albumId);

    for (const Song& song : albumSongs)
    {
        if (!deleteSong( artistId,song.getId()))
        {
            return false;
        }
    }

    return m_albumRepository.remove(albumId);
}

vector<Album>  ArtistService::getAlbums(int artistId) const
{
    if (!artistExists(artistId))
    {
        return {};
    }

    return m_albumRepository.albums(artistId);
}

vector<Song>  ArtistService::getSingles(int artistId) const
{
    if (!artistExists(artistId))
    {
        return {};
    }

    return m_songRepository.singleSongs(artistId);
}