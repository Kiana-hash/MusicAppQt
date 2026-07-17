#include "accountrepository.h"

#include <algorithm>
#include <cctype>

AccountRepository::AccountRepository(RepositoryData& data)
: m_data(data)
{
}

string AccountRepository::toLower(const string& text)
{
    string result = text;

    transform(result.begin(),result.end(),result.begin(),
        [](unsigned char character)
        {
            return static_cast<char>(tolower(character));
        }
        );

    return result;
}

int AccountRepository::generateNextAccountId() const
{
    int largestId = 0;

    for (const Account& account : m_data.artists)
    {
        largestId = max(largestId, account.getId());
    }

    for (const Account& account : m_data.listeners)
    {
        largestId = max(largestId, account.getId());
    }

    return largestId + 1;
}

bool AccountRepository::isUserNameAvailable(const string& username,int ignoredAccountId) const
{
    const string normalized = toLower(username);

    auto artist = find_if(m_data.artists.begin(),m_data.artists.end(),
        [&](const Account& account)
        {
            return account.getId() != ignoredAccountId && toLower(account.getUsername()) == normalized;
        });

    if (artist != m_data.artists.end())
    {
        return false;
    }

    auto listener = find_if(m_data.listeners.begin(),m_data.listeners.end(),
        [&](const Account& account)
        {
            return account.getId() != ignoredAccountId && toLower(account.getUsername()) == normalized;
        });

    if (listener != m_data.listeners.end())
    {
        return false;
    }

    return true;
}
