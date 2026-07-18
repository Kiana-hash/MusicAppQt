#include "authservice.h"

#include <algorithm>
#include <cctype>


AuthService::AuthService(ArtistRepository& artistRepository, ListenerRepository& listenerRepository,PlaylistRepository& playlistRepository )
: m_artistRepository(artistRepository),
m_listenerRepository(listenerRepository),
m_playlistRepository(playlistRepository)
{

}

string AuthService::trim(const string& text)
{
    const size_t first = text.find_first_not_of(" \t\n\r");

    if (first == string::npos)
    {
        return "";
    }

    const size_t last =text.find_last_not_of(" \t\n\r");

    return text.substr(first,last - first + 1);
}

string AuthService::toLower(const string& text)
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


optional<Account>  AuthService::findAccountByUserName(const std::string& username) const
{
    optional<Account> artist =m_artistRepository.searchByUserName(username);

    if (artist.has_value())
    {
        return artist;
    }

    return m_listenerRepository.searchByUserName(username);
}


AuthResult AuthService::registerAccount(
    const string& fullName,
    const string& username,
    const string& biography,
    const string& role,
    const string& password,
    const string& profilePhotoPath
    )
{
    const string cleanedFullName =trim(fullName);

    const string cleanedUsername =trim(username);

    const string cleanedBiography =trim(biography);

    const string cleanedRole =toLower(trim(role));

    const string cleanedProfilePhotoPath =trim(profilePhotoPath);

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

    if (password.empty())
    {
        return {
            false,
            "Password cannot be empty.",
            nullopt
        };
    }


    if (cleanedRole != "artist"&& cleanedRole != "listener")
    {
        return {
            false,
            "Role must be artist or listener.",
            nullopt
        };
    }

    if (findAccountByUserName(cleanedUsername).has_value())
    {
        return {
            false,
            "This username is already in use.",
            nullopt
        };
    }


    Account newAccount(
        0,
        cleanedFullName,
        cleanedUsername,
        cleanedBiography,
        cleanedRole,
        password,
        cleanedProfilePhotoPath
        );


    int accountId = -1;

    if (cleanedRole == "artist")
    {
        accountId =m_artistRepository.save(newAccount);

        if (accountId == -1)
        {
            return {
                false,
                "Could not create artist account.",
                std::nullopt
            };
        }

        optional<Account> savedAccount =m_artistRepository.search(accountId);

        if (!savedAccount.has_value())
        {
            m_artistRepository.remove(accountId);

            return {
                false,
                "Account could not be loaded after registration.",
                nullopt
            };
        }

        return {
            true,
            "Artist account created successfully.",
            savedAccount
        };
    }

    accountId =m_listenerRepository.save(newAccount);

    if (accountId == -1)
    {
        return {
            false,
            "Could not create listener account.",
            nullopt
        };
    }

    Playlist favoriteSongs(0,"Favorite Songs",accountId);

    const int favoritePlaylistId =m_playlistRepository.save(favoriteSongs);

    if (favoritePlaylistId == -1)
    {
        m_listenerRepository.remove(accountId);

        return {
            false,
            "Could not create Favorite Songs playlist.",
            nullopt
        };
    }


    optional<Account> savedAccount =m_listenerRepository.search(accountId);

    if (!savedAccount.has_value())
    {
        m_playlistRepository.remove(favoritePlaylistId);

        m_listenerRepository.remove(accountId);

        return {
            false,
            "Account could not be loaded after registration.",
            nullopt
        };
    }


    return {
        true,
        "Listener account created successfully.",
        savedAccount
    };
}


AuthResult AuthService::login(const string& username,const string& password) const
{
    const string cleanedUsername =trim(username);

    if (cleanedUsername.empty())
    {
        return {
            false,
            "Username cannot be empty.",
            nullopt
        };
    }


    if (password.empty())
    {
        return {
            false,
            "Password cannot be empty.",
            nullopt
        };
    }


    optional<Account> account =findAccountByUserName(cleanedUsername);


    if (!account.has_value())
    {
        return {
            false,
            "Invalid username or password.",
            nullopt
        };
    }


    if (password != account->getPassword())
    {
        return {
            false,
            "Invalid username or password.",
            nullopt
        };
    }


    return {
        true,
        "Login successful.",
        account
    };
}