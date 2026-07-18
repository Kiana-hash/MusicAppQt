#include "account.h"

Account::Account(){m_id=0;}

Account::Account(
    int id,
    const string& fullName,
    const string& username,
    const string& biography,
    const string& role,
    const string& password,
    const string& profilePhotoPath
    )
{
    m_id = id;
    m_fullName = fullName;
    m_username = username;
    m_biography = biography;
    m_role = role;
    m_password = password;
    m_profilePhotoPath = profilePhotoPath;
}

int Account::getId() const
{
    return m_id;
}

const string& Account::getFullName() const
{
    return m_fullName;
}

const string& Account::getUsername() const
{
    return m_username;
}

const string& Account::getBiography() const
{
    return m_biography;
}

const string& Account::getRole() const
{
    return m_role;
}

const string& Account::getPassword() const
{
    return m_password;
}

const string& Account::getProfilePhotoPath() const
{
    return m_profilePhotoPath;
}

void Account::setId(int id)
{
    m_id = id;
}


bool Account::hasValidRole() const
{
    return m_role == "artist" || m_role == "listener";
}

bool Account::isValid() const
{
    return m_id > 0
           && !m_fullName.empty()
           && !m_username.empty()
           && !m_password.empty()
           && hasValidRole();
}

bool Account::isArtist() const
{
    return m_role == "artist";
}

bool Account::isListener() const
{
    return m_role == "listener";
}