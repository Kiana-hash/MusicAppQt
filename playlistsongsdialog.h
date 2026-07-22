#ifndef PLAYLISTSONGSDIALOG_H
#define PLAYLISTSONGSDIALOG_H

#include <QDialog>

#include "song.h"

#include <vector>

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

private:
    vector<Song> getBaseSongs() const;

    void setupFilterOptions();

    void refreshSongs();

    void updateButtons();

    Ui::PlaylistSongsDialog* ui;

    int m_listenerId;
    int m_playlistId;

    QString m_playlistName;

    ListenerService& m_listenerService;
};

#endif