#include "artistrepository.h"

#include <algorithm>

ArtistRepository::ArtistRepository(RepositoryData& data): AccountRepository(data)
{
}

int ArtistRepository::save(const Account& account)
{
    if (!account.isArtist())
    {
        return -1;
    }

    if (account.getId() > 0)
    {
        const auto existing = find_if(m_data.artists.begin(),m_data.artists.end(),
            [&](const Account& stored)
            {
                return stored.getId() == account.getId();
            }
            );

        if (existing != m_data.artists.end())
        {
            if (!account.isValid()|| !isUserNameAvailable( account.getUsername(), account.getId()))
            {
                return -1;
            }

            *existing = account;
            return account.getId();
        }
    }

    Account storedAccount = account;
    storedAccount.setId(generateNextAccountId());

    if (!storedAccount.isValid()|| !isUserNameAvailable(storedAccount.getUsername()))
    {
        return -1;
    }

    m_data.artists.push_back(storedAccount);
    return storedAccount.getId();
}

bool ArtistRepository::remove(int id)
{
    const auto position = find_if(m_data.artists.begin(), m_data.artists.end(),
        [id](const Account& account)
        {
            return account.getId() == id;
        }
        );

    if (position == m_data.artists.end())
    {
        return false;
    }

    m_data.artists.erase(position);
    return true;
}

optional<Account> ArtistRepository::search(int id) const
{
    const auto result = find_if(m_data.artists.begin(),m_data.artists.end(),
        [id](const Account& account)
        {
            return account.getId() == id;
        }
        );

    if (result == m_data.artists.end())
    {
        return nullopt;
    }

    return *result;
}

optional<Account> ArtistRepository::searchByUserName( const string& username) const
{
    const string normalized = toLower(username);

    const auto result = find_if(m_data.artists.begin(),m_data.artists.end(),
        [&](const Account& account)
        {
            return toLower(account.getUsername()) == normalized;
        }
        );

    if (result == m_data.artists.end())
    {
        return nullopt;
    }

    return *result;
}

const vector<Account>& ArtistRepository::getAll() const
{
    return m_data.artists;
}
