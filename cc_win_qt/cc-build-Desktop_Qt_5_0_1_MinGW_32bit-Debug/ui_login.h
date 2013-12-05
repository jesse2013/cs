/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 5.0.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_login
{
public:
    QPushButton *btn_login;
    QPushButton *btn_register;
    QLineEdit *le_username;
    QLineEdit *le_passwd;
    QCheckBox *checkBox;
    QLabel *label;

    void setupUi(QDialog *login)
    {
        if (login->objectName().isEmpty())
            login->setObjectName(QStringLiteral("login"));
        login->resize(240, 600);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(login->sizePolicy().hasHeightForWidth());
        login->setSizePolicy(sizePolicy);
        login->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        login->setModal(false);
        btn_login = new QPushButton(login);
        btn_login->setObjectName(QStringLiteral("btn_login"));
        btn_login->setGeometry(QRect(50, 310, 141, 23));
        btn_register = new QPushButton(login);
        btn_register->setObjectName(QStringLiteral("btn_register"));
        btn_register->setGeometry(QRect(50, 350, 141, 23));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(btn_register->sizePolicy().hasHeightForWidth());
        btn_register->setSizePolicy(sizePolicy1);
        le_username = new QLineEdit(login);
        le_username->setObjectName(QStringLiteral("le_username"));
        le_username->setGeometry(QRect(40, 200, 161, 20));
        le_username->setStyleSheet(QStringLiteral(""));
        le_passwd = new QLineEdit(login);
        le_passwd->setObjectName(QStringLiteral("le_passwd"));
        le_passwd->setGeometry(QRect(40, 235, 161, 20));
        le_passwd->setEchoMode(QLineEdit::Password);
        checkBox = new QCheckBox(login);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setGeometry(QRect(40, 270, 71, 16));
        label = new QLabel(login);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(160, 570, 54, 12));

        retranslateUi(login);
        QObject::connect(btn_register, SIGNAL(clicked()), login, SLOT(close()));

        QMetaObject::connectSlotsByName(login);
    } // setupUi

    void retranslateUi(QDialog *login)
    {
        login->setWindowTitle(QApplication::translate("login", "login", 0));
        btn_login->setText(QApplication::translate("login", "\347\231\273\345\275\225", 0));
        btn_register->setText(QApplication::translate("login", "\346\263\250\345\206\214", 0));
#ifndef QT_NO_STATUSTIP
        le_username->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
        le_username->setPlaceholderText(QApplication::translate("login", "\347\224\250\346\210\267\345\220\215", 0));
        le_passwd->setPlaceholderText(QApplication::translate("login", "\345\257\206\347\240\201", 0));
        checkBox->setText(QApplication::translate("login", "\350\256\260\344\275\217\345\257\206\347\240\201", 0));
        label->setText(QApplication::translate("login", "\345\277\230\350\256\260\345\257\206\347\240\201?", 0));
    } // retranslateUi

};

namespace Ui {
    class login: public Ui_login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
