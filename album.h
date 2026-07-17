#ifndef ALBUM_H
#define ALBUM_H
#pragma once
#include <string>
using namespace std;

class Album
{
public:
    Album();

    Album(
        int id,
        const string& name,
        int artistId,
        const string& coverPath = ""
        );

    int getId() const;
    const string& getName() const;
    int getArtistId() const;
    const string& getCoverPath() const;

    void setName(const string& name);
    void setCoverPath(const string& coverPath);
    void setId(int id);

    bool isValid() const;

private:
    int m_id;
    string m_name;
    int m_artistId;
    string m_coverPath;
};

#endif // ALBUM_H
