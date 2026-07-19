#include "musicqueryservice.h"

#include <algorithm>
#include <cctype>


string MusicQueryService::trim(const string& text)
{
    const size_t first =text.find_first_not_of(" \t\n\r");

    if (first == string::npos)
    {
        return "";
    }

    const size_t last =text.find_last_not_of(" \t\n\r");

    return text.substr(first,last - first + 1);
}


string MusicQueryService::toLower(const string& text)
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

vector<Song>  MusicQueryService::searchSongsByName(const vector<Song>& songs,const string& searchText)
{
    const string cleanedSearchText =toLower(trim(searchText));

    if (cleanedSearchText.empty())
    {
        return songs;
    }

    vector<Song> result;

    for (const Song& song : songs)
    {
        const string songName =toLower(song.getName());

        if (songName.find(cleanedSearchText) != string::npos)
        {
            result.push_back(song);
        }
    }


    return result;
}

vector<Song>  MusicQueryService::filterSongsByGenre(const vector<Song>& songs,const string& genre)
{
    const string cleanedGenre =toLower(trim(genre));

    if (cleanedGenre.empty())
    {
        return songs;
    }

    vector<Song> result;

    for (const Song& song : songs)
    {
        if (toLower(trim(song.getGenre())) == cleanedGenre)
        {
            result.push_back(song);
        }
    }

    return result;
}

vector<Song>  MusicQueryService::filterSongsByReleaseYear(const vector<Song>& songs,int releaseYear)
{

    if (releaseYear == 0)
    {
        return songs;
    }

    vector<Song> result;

    for (const Song& song : songs)
    {
        if (song.getReleaseYear()== releaseYear)
        {
            result.push_back(song);
        }
    }

    return result;
}

vector<Song>  MusicQueryService::sortSongsByName(vector<Song> songs,bool ascending)
{
    sort(songs.begin(),songs.end(),
        [ascending](const Song& first,const Song& second)
        {
            const string firstName =toLower(first.getName());

            const string secondName =toLower(second.getName());

            if (ascending)
            {
                return firstName < secondName;
            }

            return firstName> secondName;
        }
        );

    return songs;
}

vector<Song>  MusicQueryService::sortSongsByReleaseYear(vector<Song> songs,bool ascending)
{
    sort(songs.begin(),songs.end(),
        [ascending](const Song& first,const Song& second)
        {

            if (first.getReleaseYear()== second.getReleaseYear())
            {
                return toLower(first.getName())< toLower(second.getName());
            }

            if (ascending)
            {
               return first.getReleaseYear()< second.getReleaseYear();
            }

            return first.getReleaseYear() > second.getReleaseYear();
        }
        );


    return songs;
}

vector<Account>  MusicQueryService::sortArtistsByName(vector<Account> artists,bool ascending)
{
    sort(artists.begin(),artists.end(),
      [ascending](const Account& first,const Account& second)
        {
            const string firstName =toLower(first.getFullName());

            const string secondName =toLower(second.getFullName());

            if (ascending)
            {
                return firstName< secondName;
            }

            return firstName> secondName;
        }
        );

    return artists;
}

vector<Album>  MusicQueryService::sortAlbumsByName(vector<Album> albums,bool ascending)
{
    sort(albums.begin(),albums.end(),
        [ascending](const Album& first,const Album& second)
        {
            const string firstName =toLower(first.getName());

            const string secondName =toLower(second.getName());

            if (ascending)
            {
                return firstName < secondName;
            }

            return firstName > secondName;
        }
        );

    return albums;
}

vector<Playlist>  MusicQueryService::sortPlaylistsByName(vector<Playlist> playlists,bool ascending)
{
    sort(playlists.begin(),playlists.end(),
        [ascending](const Playlist& first,const Playlist& second)
        {
            const string firstName =toLower(first.getName());

            const string secondName =toLower(second.getName());

            if (ascending)
            {
                return firstName < secondName;
            }

            return firstName > secondName;
        }
        );

    return playlists;
}