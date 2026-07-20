#ifndef ARTISTDASHBOARD_H
#define ARTISTDASHBOARD_H

#include <QDialog>
#include <QListWidgetItem>

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

private:
    Ui::ArtistDashboard* ui;
    void refreshDashboard();

    int m_artistId;

    ArtistService& m_artistService;
    CatalogService& m_catalogService;
    AccountService& m_accountService;
};

#endif