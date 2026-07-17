#ifndef ARTIST_REPOSITORY_H
#define ARTIST_REPOSITORY_H

#include "accountrepository.h"


class ArtistRepository : public AccountRepository
{
public:
    explicit ArtistRepository(RepositoryData& data);

    int save(const Account& account) override;
    bool remove(int id) override;
    optional<Account> search(int id) const override;

    optional<Account> searchByUserName(const string& username) const override;

    const vector<Account>& getAll() const;
};

#endif // ARTIST_REPOSITORY_H
