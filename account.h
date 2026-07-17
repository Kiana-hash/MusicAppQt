#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
using namespace std;

class Account
{
public:
    Account();
    Account(
        int id,
        const string& fullName,
        const string& username,
        const string& biography,
        const string& role,
        const string& passwordHash,
        const string& profilePhotoPath = ""
        );


    int getId() const;
    const string& getFullName() const;
    const string& getUsername() const;
    const string& getBiography() const;
    const string& getRole() const;
    const string& getPasswordHash() const;
    const string& getProfilePhotoPath() const;

    void setId(int id);

    bool hasValidRole() const;
    bool isValid() const;
    bool isArtist() const;
    bool isListener() const;

private:
    int m_id;
    string m_fullName;
    string m_username;
    string m_biography;
    string m_role;
    string m_passwordHash;
    string m_profilePhotoPath;
};

#endif // ACCOUNT_H
