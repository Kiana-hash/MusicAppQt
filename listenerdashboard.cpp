#include "listenerdashboard.h"
#include "ui_listenerdashboard.h"

#include "listenerservice.h"
#include "catalogservice.h"
#include "accountservice.h"


ListenerDashboard::ListenerDashboard(
    int listenerId,
    ListenerService& listenerService,
    CatalogService& catalogService,
    AccountService& accountService,
    QWidget* parent
    )
    : QDialog(parent),
    ui(new Ui::ListenerDashboard),
    m_listenerId(listenerId),
    m_listenerService(listenerService),
    m_catalogService(catalogService),
    m_accountService(accountService)
{
    ui->setupUi(this);
}


ListenerDashboard::~ListenerDashboard()
{
    delete ui;
}