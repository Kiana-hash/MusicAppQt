#ifndef ARTISTDASHBOARD_H
#define ARTISTDASHBOARD_H

#include <QDialog>
#include <QListWidgetItem>
#include <vector>
#include "song.h"

class ArtistService;
class CatalogService;
class AccountService;

namespace Ui
{
class ArtistDashboard;
}

class ArtistDashboard : public QDialog
{
    Q_OBJECT

public:
    explicit ArtistDashboard(
        int artistId,
        ArtistService& artistService,
        CatalogService& catalogService,
        AccountService& accountService,
        QWidget* parent = nullptr
        );

    ~ArtistDashboard();

private slots:
    void on_logoutButton_clicked();

    void on_albumsListWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_createAlbumButton_clicked();

    void on_createSingleButton_clicked();

    void on_accountButton_clicked();
    void on_songsTableWidget_cellDoubleClicked(int row,int column);

private:
    Ui::ArtistDashboard* ui;
    void refreshDashboard();

    int m_artistId;

    ArtistService& m_artistService;
    CatalogService& m_catalogService;
    AccountService& m_accountService;

    void setupFilterOptions();

    void refreshSongs();

    vector<Song> getArtistSongs() const;

    vector<Song> m_displayedSongs;
};

#endif