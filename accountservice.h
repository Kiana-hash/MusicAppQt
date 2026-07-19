#ifndef ACCOUNTSERVICE_H
#define ACCOUNTSERVICE_H

#include "account.h"
#include "artistrepository.h"
#include "listenerrepository.h"
#include "albumrepository.h"
#include "songrepository.h"
#include "playlistrepository.h"
#include "artistservice.h"


struct AccountActionResult
{
    bool success{false};
    string message;
    optional<Account> account;
};

class AccountService
{
public:
    AccountService(
        ArtistRepository& artistRepository,
        ListenerRepository& listenerRepository,
        AlbumRepository& albumRepository,
        SongRepository& songRepository,
        PlaylistRepository& playlistRepository,
        ArtistService& artistService
        );

    optional<Account> getAccount(int accountId) const;

    AccountActionResult updateProfile(
        int accountId,
        const string& fullName,
        const string& username,
        const string& biography,
        const string& profilePhotoPath
        );

    AccountActionResult changePassword(int accountId,const string& newPassword);

    bool deleteAccount(int accountId);

private:
    ArtistRepository& m_artistRepository;
    ListenerRepository& m_listenerRepository;
    AlbumRepository& m_albumRepository;
    SongRepository& m_songRepository;
    PlaylistRepository& m_playlistRepository;
    ArtistService& m_artistService;

    bool usernameUsedByAnotherAccount(const string& username,int currentAccountId) const;

    static string trim(const string& text);
};

#endif