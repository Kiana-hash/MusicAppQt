#ifndef PLAYLIST_REPOSITORY_H
#define PLAYLIST_REPOSITORY_H

#include "AbstractRepository.h"
#include "RepositoryData.h"
#include "playlist.h"

#include <optional>
#include <vector>

class PlaylistRepository : public AbstractRepository<Playlist>
{
public:
    explicit PlaylistRepository(RepositoryData& data);

    int save(const Playlist& playlist) override;
    bool remove(int id) override;
    optional<Playlist> search(int id) const override;

    bool insertSong(int playlistId, int songId);
    bool removeSong(int playlistId, int songId);
    vector<Playlist> playlists(int listenerId) const;

private:
    int generateNextId() const;

    RepositoryData& m_data;
};

#endif // PLAYLIST_REPOSITORY_H
