#ifndef ACCOUNT_REPOSITORY_H
#define ACCOUNT_REPOSITORY_H

#include "AbstractRepository.h"
#include "RepositoryData.h"
#include "account.h"

#include <string>

class AccountRepository : public AbstractRepository<Account>
{
public:
    ~AccountRepository() override = default;

    virtual optional<Account> searchByUserName(const string& username) const = 0;

protected:
    explicit AccountRepository(RepositoryData& data);

    int generateNextAccountId() const;

    bool isUserNameAvailable(const string& username,int ignoredAccountId = 0) const;

    static string toLower(const string& text);

    RepositoryData& m_data;
};

#endif // ACCOUNT_REPOSITORY_H
