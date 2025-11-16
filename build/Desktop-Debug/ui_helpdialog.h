/********************************************************************************
** Form generated from reading UI file 'helpdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HELPDIALOG_H
#define UI_HELPDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_HelpDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *labelTitle;
    QLabel *labelInfo;
    QPushButton *btnContinue;

    void setupUi(QDialog *HelpDialog)
    {
        if (HelpDialog->objectName().isEmpty())
            HelpDialog->setObjectName(QString::fromUtf8("HelpDialog"));
        HelpDialog->setMinimumSize(QSize(400, 300));
        verticalLayout = new QVBoxLayout(HelpDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelTitle = new QLabel(HelpDialog);
        labelTitle->setObjectName(QString::fromUtf8("labelTitle"));
        labelTitle->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(labelTitle);

        labelInfo = new QLabel(HelpDialog);
        labelInfo->setObjectName(QString::fromUtf8("labelInfo"));
        labelInfo->setAlignment(Qt::AlignLeft);
        labelInfo->setWordWrap(true);

        verticalLayout->addWidget(labelInfo);

        btnContinue = new QPushButton(HelpDialog);
        btnContinue->setObjectName(QString::fromUtf8("btnContinue"));
        btnContinue->setMinimumWidth(120);
        btnContinue->setMaximumWidth(160);

        verticalLayout->addWidget(btnContinue);


        retranslateUi(HelpDialog);

        QMetaObject::connectSlotsByName(HelpDialog);
    } // setupUi

    void retranslateUi(QDialog *HelpDialog)
    {
        HelpDialog->setWindowTitle(QCoreApplication::translate("HelpDialog", "About Algorithm Visualizer", nullptr));
        labelTitle->setText(QCoreApplication::translate("HelpDialog", "Welcome to Algorithm Visualizer", nullptr));
        labelInfo->setText(QCoreApplication::translate("HelpDialog", "\n"
"This tool helps you learn Data Structures and Algorithms visually.\n"
"\n"
"How to Use:\n"
"1. Click \"Get Started\".\n"
"2. Choose any topic such as Sorting, Linked List, or Graphs.\n"
"3. Click \"Explore\" to see step-by-step visualization.\n"
"\n"
"Enjoy learning! \360\237\231\202\n"
"      ", nullptr));
        btnContinue->setText(QCoreApplication::translate("HelpDialog", "Continue", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HelpDialog: public Ui_HelpDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HELPDIALOG_H
