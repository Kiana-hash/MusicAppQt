#ifndef LISTENERDASHBOARD_H
#define LISTENERDASHBOARD_H

#include <QDialog>
#include <QListWidget>
#include <vector>

#include "song.h"

class ListenerService;
class CatalogService;
class AccountService;

namespace Ui
{
class ListenerDashboard;
}

class ListenerDashboard : public QDialog
{
    Q_OBJECT

public:
    explicit ListenerDashboard(
        int listenerId,
        ListenerService& listenerService,
        CatalogService& catalogService,
        AccountService& accountService,
        QWidget* parent = nullptr
        );

    ~ListenerDashboard();

private slots:

    void on_logoutButton_clicked();

    void on_artistsListWidget_itemDoubleClicked(QListWidgetItem* item);

    void on_createPlaylistButton_clicked();

    void on_playlistsListWidget_itemDoubleClicked(QListWidgetItem* item);

    void on_accountButton_clicked();

    void on_likeButton_clicked();

    void on_addToPlaylistButton_clicked();

    void on_songsTableWidget_cellDoubleClicked(int row,int column);

private:

    void refreshDashboard();

    void setupFilterOptions();

    void refreshSongs();

    void updateSongButtons();

    vector<Song> getAllCatalogSongs() const;

    Ui::ListenerDashboard* ui;

    int m_listenerId;

    ListenerService& m_listenerService;
    CatalogService& m_catalogService;
    AccountService& m_accountService;

    vector<Song> m_displayedSongs;
};

#endif