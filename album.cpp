#include "album.h"

Album::Album()
{
    m_id = 0;
    m_artistId = 0;
}

Album::Album(
    int id,
    const string& name,
    int artistId,
    const string& coverPath
    )
{
    m_id = id;
    m_name = name;
    m_artistId = artistId;
    m_coverPath = coverPath;
}

int Album::getId() const
{
    return m_id;
}

const string& Album::getName() const
{
    return m_name;
}

int Album::getArtistId() const
{
    return m_artistId;
}

const string& Album::getCoverPath() const
{
    return m_coverPath;
}

void Album::setName(const string& name)
{
    m_name = name;
}

void Album::setCoverPath(const string& coverPath)
{
    m_coverPath = coverPath;
}

bool Album::isValid() const
{
    return m_id > 0 && !m_name.empty() && m_artistId > 0;
}