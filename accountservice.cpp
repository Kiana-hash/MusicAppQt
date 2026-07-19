#include "accountservice.h"


AccountService::AccountService(
    ArtistRepository& artistRepository,
    ListenerRepository& listenerRepository,
    AlbumRepository& albumRepository,
    SongRepository& songRepository,
    PlaylistRepository& playlistRepository,
    ArtistService& artistService
    )
    : m_artistRepository(artistRepository),
    m_listenerRepository(listenerRepository),
    m_albumRepository(albumRepository),
    m_songRepository(songRepository),
    m_playlistRepository(playlistRepository),
    m_artistService(artistService)
{
}


string AccountService::trim(const string& text)
{
    const size_t first = text.find_first_not_of(" \t\n\r");

    if (first == string::npos)
    {
        return "";
    }

    const size_t last =text.find_last_not_of(" \t\n\r");

    return text.substr(first,last - first + 1);
}

optional<Account>  AccountService::getAccount(int accountId) const
{
    optional<Account> artist =m_artistRepository.search(accountId);

    if (artist.has_value())
    {
        return artist;
    }

    return m_listenerRepository.search(accountId);
}

bool AccountService::usernameUsedByAnotherAccount(const string& username,int currentAccountId) const
{
    optional<Account> artist =m_artistRepository.searchByUserName(username);

    if ( artist.has_value()&& artist->getId() != currentAccountId)
    {
        return true;
    }


    optional<Account> listener =m_listenerRepository.searchByUserName(username);

    if (listener.has_value()&& listener->getId() != currentAccountId)
    {
        return true;
    }

    return false;
}

AccountActionResult  AccountService::updateProfile(
    int accountId,
    const string& fullName,
    const string& username,
    const string& biography,
    const string& profilePhotoPath
    )
{
    const optional<Account> existingAccount =getAccount(accountId);

    if (!existingAccount.has_value())
    {
        return {
            false,
            "Account does not exist.",
            nullopt
        };
    }


    const string cleanedFullName =trim(fullName);

    const string cleanedUsername =trim(username);

    const string cleanedBiography =trim(biography);

    const string cleanedPhotoPath =trim(profilePhotoPath);


    if (cleanedFullName.empty())
    {
        return {
            false,
            "Full name cannot be empty.",
            nullopt
        };
    }


    if (cleanedUsername.empty())
    {
        return {
            false,
            "Username cannot be empty.",
            nullopt
        };
    }


    if (usernameUsedByAnotherAccount(cleanedUsername,accountId))
    {
        return {
            false,
            "This username is already in use.",
            nullopt
        };
    }


    Account updatedAccount(
        accountId,
        cleanedFullName,
        cleanedUsername,
        cleanedBiography,

        existingAccount->getRole(),

        existingAccount->getPassword(),

        cleanedPhotoPath
        );


    int savedId = -1;


    if (existingAccount->isArtist())
    {
        savedId =m_artistRepository.save(updatedAccount);
    }
    else
    {
        savedId =m_listenerRepository.save(updatedAccount);
    }


    if (savedId == -1)
    {
        return {
            false,
            "Could not update account.",
            nullopt
        };
    }


    return {
        true,
        "Account updated successfully.",
        getAccount(accountId)
    };
}

AccountActionResult  AccountService::changePassword(int accountId,const string& newPassword)
{
    const optional<Account> existingAccount =getAccount(accountId);

    if (!existingAccount.has_value())
    {
        return {
            false,
            "Account does not exist.",
            nullopt
        };
    }


    if (newPassword.empty())
    {
        return {
            false,
            "Password cannot be empty.",
            nullopt
        };
    }


    Account updatedAccount(
        existingAccount->getId(),
        existingAccount->getFullName(),
        existingAccount->getUsername(),
        existingAccount->getBiography(),
        existingAccount->getRole(),
        newPassword,
        existingAccount->getProfilePhotoPath()
        );


    int savedId = -1;


    if (existingAccount->isArtist())
    {
        savedId = m_artistRepository.save(updatedAccount);
    }
    else
    {
        savedId =m_listenerRepository.save(updatedAccount);
    }


    if (savedId == -1)
    {
        return {
            false,
            "Could not change password.",
            nullopt
        };
    }


    return {
        true,
        "Password changed successfully.",
        getAccount(accountId)
    };
}

bool AccountService::deleteAccount(int accountId)
{
    const optional<Account> account =getAccount(accountId);

    if (!account.has_value())
    {
        return false;
    }


    if (account->isListener())
    {
        const vector<Playlist> playlists =m_playlistRepository.playlists(accountId);

        for (const Playlist& playlist : playlists)
        {
            m_playlistRepository.remove(playlist.getId());
        }

        return m_listenerRepository.remove(accountId);
    }


    const vector<Album> albums =m_albumRepository.albums(accountId);


    for (const Album& album : albums)
    {
        if (!m_artistService.deleteAlbum(accountId,album.getId()))
        {
            return false;
        }
    }


    const vector<Song> remainingSongs = m_songRepository.getByArtist(accountId);


    for (const Song& song : remainingSongs)
    {
        if (!m_artistService.deleteSong(accountId,song.getId()))
        {
            return false;
        }
    }

    return m_artistRepository.remove(accountId);
}
