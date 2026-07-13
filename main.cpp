#include "mainwindow.h"
#include "Account.h"

#include <QApplication>
#include <QDebug>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Account testAccount(
        1,
        "Alex Garcia",
        "alex",
        "My first test account",
        "artist",
        "temporary_password_hash"
        );

    qDebug() << "Account ID:"
             << testAccount.getId();

    qDebug() << "Username:"
             << QString::fromStdString(
                    testAccount.getUsername()
                    );

    qDebug() << "Role:"
             << QString::fromStdString(
                    testAccount.getRole()
                    );

    qDebug() << "Is valid:"
             << testAccount.isValid();

    MainWindow window;
    window.show();

    return app.exec();
}