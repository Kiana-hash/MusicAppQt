#include "song.h"

Song::Song()
{
    m_id = 0;
    m_releaseYear = 0;
    m_artistId = 0;
    m_albumId = 0;
}

Song::Song(
    int id,
    const string& name,
    int releaseYear,
    const string& genre,
    int artistId,
    int albumId,
    const string& audioFilePath,
    const string& coverPath
    )
{
    m_id = id;
    m_name = name;
    m_releaseYear = releaseYear;
    m_genre = genre;
    m_artistId = artistId;
    m_albumId = albumId;
    m_audioFilePath = audioFilePath;
    m_coverPath = coverPath;
}

int Song::getId() const
{
    return m_id;
}

const string& Song::getName() const
{
    return m_name;
}

int Song::getReleaseYear() const
{
    return m_releaseYear;
}

const string& Song::getGenre() const
{
    return m_genre;
}

int Song::getArtistId() const
{
    return m_artistId;
}

int Song::getAlbumId() const
{
    return m_albumId;
}

const string& Song::getAudioFilePath() const
{
    return m_audioFilePath;
}

const string& Song::getCoverPath() const
{
    return m_coverPath;
}

void Song::setName(const string& name)
{
    m_name = name;
}

void Song::setReleaseYear(int releaseYear)
{
    m_releaseYear = releaseYear;
}

void Song::setGenre(const string& genre)
{
    m_genre = genre;
}

void Song::setAlbumId(int albumId)
{
    m_albumId = albumId;
}

void Song::setAudioFilePath(const string& audioFilePath)
{
    m_audioFilePath = audioFilePath;
}

void Song::setCoverPath(const string& coverPath)
{
    m_coverPath = coverPath;
}

void Song::setId(int id)
{
    m_id = id;
}

bool Song::isSingle() const
{
    return m_albumId == 0;
}

bool Song::isValid() const
{
    return m_id > 0
           && !m_name.empty()
           && m_releaseYear > 0
           && !m_genre.empty()
           && m_artistId > 0
           && m_albumId >= 0
           && !m_audioFilePath.empty();
}