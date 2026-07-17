#ifndef LISTENER_REPOSITORY_H
#define LISTENER_REPOSITORY_H

#include "accountrepository.h"


class ListenerRepository : public AccountRepository
{
public:
    explicit ListenerRepository(RepositoryData& data);

    int save(const Account& account) override;
    bool remove(int id) override;
    optional<Account> search(int id) const override;

    optional<Account> searchByUserName(const std::string& username) const override;

    bool updateLiked(int listenerId, int songId, bool liked);
    bool isLiked(int listenerId, int songId) const;

    const vector<int>& getLikedSongIds(int listenerId) const;
    const vector<Account>& getAll() const;
};

#endif // LISTENER_REPOSITORY_H
