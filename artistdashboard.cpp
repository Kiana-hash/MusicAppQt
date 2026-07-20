#include "artistdashboard.h"
#include "ui_artistdashboard.h"

#include "artistservice.h"
#include "catalogservice.h"
#include "accountservice.h"


ArtistDashboard::ArtistDashboard(
    int artistId,
    ArtistService& artistService,
    CatalogService& catalogService,
    AccountService& accountService,
    QWidget* parent
    )
    : QDialog(parent),
    ui(new Ui::ArtistDashboard),
    m_artistId(artistId),
    m_artistService(artistService),
    m_catalogService(catalogService),
    m_accountService(accountService)
{
    ui->setupUi(this);
}


ArtistDashboard::~ArtistDashboard()
{
    delete ui;
}