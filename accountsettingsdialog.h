#ifndef ACCOUNTSETTINGSDIALOG_H
#define ACCOUNTSETTINGSDIALOG_H

#include <QDialog>

class AccountService;

namespace Ui
{
class AccountSettingsDialog;
}

class AccountSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AccountSettingsDialog(int accountId,AccountService& accountService,QWidget* parent = nullptr);

    ~AccountSettingsDialog();
    bool wasAccountDeleted() const;

private slots:
    void on_choosePhotoButton_clicked();

    void on_saveProfileButton_clicked();

    void on_changePasswordButton_clicked();

    void on_deleteAccountButton_clicked();

    void on_closeButton_clicked();

private:
    void loadAccount();
    bool m_accountDeleted = false;

    Ui::AccountSettingsDialog* ui;

    int m_accountId;

    AccountService& m_accountService;
};

#endif