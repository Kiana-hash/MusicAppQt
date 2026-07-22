#ifndef CREATEPLAYLISTDIALOG_H
#define CREATEPLAYLISTDIALOG_H

#include <QDialog>

class ListenerService;

namespace Ui
{
class CreatePlaylistDialog;
}

class CreatePlaylistDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreatePlaylistDialog(int listenerId,ListenerService& listenerService,
        QWidget* parent = nullptr
        );

    ~CreatePlaylistDialog();

private slots:
    void on_createButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::CreatePlaylistDialog* ui;

    int m_listenerId;

    ListenerService& m_listenerService;
};

#endif