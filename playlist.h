#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <string>
#include <vector>
using namespace std;

class Playlist
{
public:
    Playlist();

    Playlist(int id,const string& name,int listenerId);

    int getId() const;
    const string& getName() const;
    int getListenerId() const;
    const vector<int>& getSongIds() const;

    void setName(const string& name);
    void setId(int id);

    bool addSong(int songId);
    bool removeSong(int songId);
    bool containsSong(int songId) const;

    bool isValid() const;

private:
    int m_id;
    string m_name;
    int m_listenerId;
    vector<int> m_songIds;
};

#endif // PLAYLIST_H
