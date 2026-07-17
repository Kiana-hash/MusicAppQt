#ifndef SONG_REPOSITORY_H
#define SONG_REPOSITORY_H

#include "AbstractRepository.h"
#include "RepositoryData.h"
#include "song.h"

#include <optional>
#include <vector>

class SongRepository : public AbstractRepository<Song>
{
public:
    explicit SongRepository(RepositoryData& data);

    int save(const Song& song) override;
    bool remove(int id) override;
    optional<Song> search(int id) const override;

   vector<Song> singleSongs(int artistId) const;
   vector<Song> getByAlbum(int albumId) const;
   vector<Song> getByArtist(int artistId) const;
   vector<Song> getByPlaylist(int playlistId) const;
   vector<Song> getByLikedSongs(int listenerId) const;

private:
    int generateNextId() const;

    RepositoryData& m_data;
};

#endif // SONG_REPOSITORY_H
