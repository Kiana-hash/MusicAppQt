#ifndef ALBUM_REPOSITORY_H
#define ALBUM_REPOSITORY_H

#include "AbstractRepository.h"
#include "RepositoryData.h"
#include "album.h"

class AlbumRepository : public AbstractRepository<Album>
{
public:
    explicit AlbumRepository(RepositoryData& data);

    int save(const Album& album) override;
    bool remove(int id) override;
    optional<Album> search(int id) const override;

    vector<Album> albums(int artistId) const;

private:
    int generateNextId() const;

    RepositoryData& m_data;
};

#endif // ALBUM_REPOSITORY_H
