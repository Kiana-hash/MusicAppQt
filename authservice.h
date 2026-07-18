#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include "account.h"
#include "artistrepository.h"
#include "listenerrepository.h"
#include "playlistrepository.h"


struct AuthResult
{
    bool success{false};
    string message;
    optional<Account> account;
};

class AuthService
{
public:
    AuthService(
        ArtistRepository& artistRepository,
        ListenerRepository& listenerRepository,
        PlaylistRepository& playlistRepository
        );

    AuthResult registerAccount(
        const string& fullName,
        const string& username,
        const string& biography,
        const string& role,
        const string& password,
        const string& profilePhotoPath = ""
        );

    AuthResult login(const string& username,const string& password) const;

private:
    ArtistRepository& m_artistRepository;
    ListenerRepository& m_listenerRepository;
    PlaylistRepository& m_playlistRepository;

    optional<Account> findAccountByUserName(const string& username) const;

    static string trim(const string& text);

    static string toLower(const string& text);
};

#endif // AUTHSERVICE_H