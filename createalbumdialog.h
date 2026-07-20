#ifndef CREATEALBUMDIALOG_H
#define CREATEALBUMDIALOG_H

#include <QDialog>

class ArtistService;

namespace Ui
{
class CreateAlbumDialog;
}

class CreateAlbumDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateAlbumDialog(int artistId,ArtistService& artistService,QWidget* parent = nullptr);

    ~CreateAlbumDialog();

private slots:
    void on_chooseCoverButton_clicked();

    void on_createButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::CreateAlbumDialog* ui;

    int m_artistId;
    ArtistService& m_artistService;
};

#endif