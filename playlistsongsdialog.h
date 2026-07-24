#ifndef PLAYLISTSONGSDIALOG_H
#define PLAYLISTSONGSDIALOG_H

#include <QDialog>
#include "song.h"
#include <vector>
#include<QListWidgetItem>

class ListenerService;

namespace Ui
{
class PlaylistSongsDialog;
}

class PlaylistSongsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlaylistSongsDialog(int listenerId,int playlistId,
        const QString& playlistName,
        ListenerService& listenerService,
        QWidget* parent = nullptr
        );

    ~PlaylistSongsDialog();

private slots:
    void on_removeSongButton_clicked();

    void on_renamePlaylistButton_clicked();

    void on_deletePlaylistButton_clicked();

    void on_closeButton_clicked();

    void on_songsListWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    vector<Song> getBaseSongs() const;

    void setupFilterOptions();

    void refreshSongs();

    void updateButtons();

    Ui::PlaylistSongsDialog* ui;

    int m_listenerId;
    int m_playlistId;

    QString m_playlistName;
    vector<Song> m_displayedSongs;

    ListenerService& m_listenerService;
};

#endif