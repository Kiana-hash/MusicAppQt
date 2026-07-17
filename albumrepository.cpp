#include "albumrepository.h"

#include <algorithm>

AlbumRepository::AlbumRepository(RepositoryData& data)
: m_data(data)
{
}

int AlbumRepository::generateNextId() const
{
    int largestId = 0;

    for (const Album& album : m_data.albums)
    {
        largestId = max(largestId, album.getId());
    }

    return largestId + 1;
}

int AlbumRepository::save(const Album& album)
{
    if (album.getId() > 0)
    {
        const auto existing = find_if(m_data.albums.begin(), m_data.albums.end(),
            [&](const Album& stored)
            {
                return stored.getId() == album.getId();
            }
            );

        if (existing != m_data.albums.end())
        {
            if (!album.isValid())
            {
                return -1;
            }

            *existing = album;
            return album.getId();
        }
    }

    Album storedAlbum = album;
    storedAlbum.setId(generateNextId());

    if (!storedAlbum.isValid())
    {
        return -1;
    }

    m_data.albums.push_back(storedAlbum);
    return storedAlbum.getId();
}

bool AlbumRepository::remove(int id)
{
    const auto position = find_if(m_data.albums.begin(),m_data.albums.end(),
        [id](const Album& album)
        {
            return album.getId() == id;
        }
        );

    if (position == m_data.albums.end())
    {
        return false;
    }

    m_data.albums.erase(position);
    return true;
}

optional<Album> AlbumRepository::search(int id) const
{
    const auto result = find_if(m_data.albums.begin(),m_data.albums.end(),
        [id](const Album& album)
        {
            return album.getId() == id;
        }
        );

    if (result == m_data.albums.end())
    {
        return nullopt;
    }

    return *result;
}

vector<Album> AlbumRepository::albums(int artistId) const
{
    vector<Album> result;

    for (const Album& album : m_data.albums)
    {
        if (album.getArtistId() == artistId)
        {
            result.push_back(album);
        }
    }

    return result;
}
