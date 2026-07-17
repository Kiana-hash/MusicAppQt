#ifndef SONG_H
#define SONG_H

#include <string>
using namespace std;

class Song
{
public:
    Song();

    Song(
        int id,
        const string& name,
        int releaseYear,
        const string& genre,
        int artistId,
        int albumId,
        const string& audioFilePath,
        const string& coverPath = ""
        );

    int getId() const;
    const string& getName() const;
    int getReleaseYear() const;
    const string& getGenre() const;
    int getArtistId() const;
    int getAlbumId() const;
    const string& getAudioFilePath() const;
    const string& getCoverPath() const;

    void setName(const string& name);
    void setReleaseYear(int releaseYear);
    void setGenre(const string& genre);
    void setAlbumId(int albumId);
    void setAudioFilePath(const string& audioFilePath);
    void setCoverPath(const string& coverPath);
    void setId(int id);

    bool isSingle() const;
    bool isValid() const;

private:
    int m_id;
    string m_name;
    int m_releaseYear;
    string m_genre;
    int m_artistId;
    int m_albumId;
    string m_audioFilePath;
    string m_coverPath;

};

#endif // SONG_H
