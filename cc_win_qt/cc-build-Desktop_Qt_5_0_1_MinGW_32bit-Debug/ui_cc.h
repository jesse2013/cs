/********************************************************************************
** Form generated from reading UI file 'cc.ui'
**
** Created by: Qt User Interface Compiler version 5.0.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CC_H
#define UI_CC_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_cc
{
public:

    void setupUi(QWidget *cc)
    {
        if (cc->objectName().isEmpty())
            cc->setObjectName(QStringLiteral("cc"));
        cc->resize(240, 600);

        retranslateUi(cc);

        QMetaObject::connectSlotsByName(cc);
    } // setupUi

    void retranslateUi(QWidget *cc)
    {
        cc->setWindowTitle(QApplication::translate("cc", "cc", 0));
    } // retranslateUi

};

namespace Ui {
    class cc: public Ui_cc {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CC_H
