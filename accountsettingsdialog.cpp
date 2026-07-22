#include "accountsettingsdialog.h"
#include "ui_accountsettingsdialog.h"

#include "accountservice.h"
#include <QFileDialog>
#include <QMessageBox>


AccountSettingsDialog::AccountSettingsDialog(int accountId,AccountService& accountService,QWidget* parent)
    : QDialog(parent),
    ui(new Ui::AccountSettingsDialog),
    m_accountId(accountId),
    m_accountService(accountService)
{
    ui->setupUi(this);

    loadAccount();
}


AccountSettingsDialog::~AccountSettingsDialog()
{
    delete ui;
}

void AccountSettingsDialog::loadAccount()
{
    const optional<Account> account =m_accountService.getAccount(m_accountId);

    if (!account.has_value())
    {
        return;
    }

    ui->fullNameLineEdit->setText(QString::fromStdString(account->getFullName()));

    ui->usernameLineEdit->setText(QString::fromStdString(account->getUsername()));

    ui->biographyTextEdit->setPlainText(QString::fromStdString(account->getBiography()));

    ui->profilePhotoLineEdit->setText(QString::fromStdString(account->getProfilePhotoPath()));
}

bool AccountSettingsDialog::wasAccountDeleted() const
{
    return m_accountDeleted;
}

void AccountSettingsDialog::on_choosePhotoButton_clicked()
{
    const QString filePath =QFileDialog::getOpenFileName(this,"Choose Profile Photo","","Images (*.png *.jpg *.jpeg)");

    if (filePath.isEmpty())
    {
        return;
    }

    ui->profilePhotoLineEdit->setText(filePath);
}


void AccountSettingsDialog::on_saveProfileButton_clicked()
{
    const string fullName =ui->fullNameLineEdit->text().toStdString();

    const string username =ui->usernameLineEdit->text().toStdString();

    const string biography =ui->biographyTextEdit->toPlainText().toStdString();

    const string profilePhotoPath =ui->profilePhotoLineEdit->text().toStdString();

    const AccountActionResult result =m_accountService.updateProfile(m_accountId,fullName,username,biography,profilePhotoPath);


    if (!result.success)
    {
        QMessageBox::warning(this,"Update Failed",QString::fromStdString(result.message));

        return;
    }

    QMessageBox::information(this,"Profile Updated","Your profile was updated successfully.");

    loadAccount();
}


void AccountSettingsDialog::on_changePasswordButton_clicked()
{
    const string newPassword =ui->newPasswordLineEdit->text().toStdString();

    const AccountActionResult result =m_accountService.changePassword(m_accountId,newPassword);

    if (!result.success)
    {
        QMessageBox::warning(this,"Password Change Failed",QString::fromStdString(result.message));

        return;
    }

    QMessageBox::information(this,"Password Changed","Your password was changed successfully.");

    ui->newPasswordLineEdit->clear();
}


void AccountSettingsDialog::on_deleteAccountButton_clicked()
{
    const QMessageBox::StandardButton answer =QMessageBox::question(this,"Delete Account",
            "Are you sure you want to permanently delete your account?",
            QMessageBox::Yes |QMessageBox::No);

    if (answer != QMessageBox::Yes)
    {
        return;
    }

    const bool success =m_accountService.deleteAccount(m_accountId);

    if (!success)
    {
        QMessageBox::warning(this,"Delete Failed","The account could not be deleted.");

        return;
    }

    m_accountDeleted = true;
    accept();
}


void AccountSettingsDialog::on_closeButton_clicked()
{
    accept();
}

