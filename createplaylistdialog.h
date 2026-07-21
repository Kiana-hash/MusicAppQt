#ifndef CREATEPLAYLISTDIALOG_H
#define CREATEPLAYLISTDIALOG_H

#include <QDialog>

namespace Ui {
class CreatePlaylistDialog;
}

class CreatePlaylistDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreatePlaylistDialog(QWidget *parent = nullptr);
    ~CreatePlaylistDialog();

private:
    Ui::CreatePlaylistDialog *ui;
};

#endif // CREATEPLAYLISTDIALOG_H
