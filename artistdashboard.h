#ifndef ARTISTDASHBOARD_H
#define ARTISTDASHBOARD_H

#include <QDialog>

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

private:
    Ui::ArtistDashboard* ui;

    int m_artistId;

    ArtistService& m_artistService;
    CatalogService& m_catalogService;
    AccountService& m_accountService;
};

#endif