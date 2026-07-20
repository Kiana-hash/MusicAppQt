#ifndef LISTENERDASHBOARD_H
#define LISTENERDASHBOARD_H

#include <QDialog>

class ListenerService;
class CatalogService;
class AccountService;

namespace Ui
{
class ListenerDashboard;
}

class ListenerDashboard : public QDialog
{
    Q_OBJECT

public:
    explicit ListenerDashboard(
        int listenerId,
        ListenerService& listenerService,
        CatalogService& catalogService,
        AccountService& accountService,
        QWidget* parent = nullptr
        );

    ~ListenerDashboard();

private:
    Ui::ListenerDashboard* ui;

    int m_listenerId;

    ListenerService& m_listenerService;
    CatalogService& m_catalogService;
    AccountService& m_accountService;
};

#endif