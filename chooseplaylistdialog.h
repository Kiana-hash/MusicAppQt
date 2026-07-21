#ifndef CHOOSEPLAYLISTDIALOG_H
#define CHOOSEPLAYLISTDIALOG_H

#include <QDialog>

class ListenerService;

namespace Ui
{
class ChoosePlaylistDialog;
}

class ChoosePlaylistDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChoosePlaylistDialog(int listenerId,int songId,
        ListenerService& listenerService,
        QWidget* parent = nullptr
        );

    ~ChoosePlaylistDialog();

private slots:
    void on_addButton_clicked();

    void on_cancelButton_clicked();

private:
    void loadPlaylists();
    void addToSelectedPlaylist();

    Ui::ChoosePlaylistDialog* ui;

    int m_listenerId;
    int m_songId;

    ListenerService& m_listenerService;
};

#endif