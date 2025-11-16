/********************************************************************************
** Form generated from reading UI file 'StartScreen.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STARTSCREEN_H
#define UI_STARTSCREEN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StartScreen
{
public:
    QVBoxLayout *mainLayout;
    QSpacerItem *topSpacer;
    QLabel *titleLabel;
    QLabel *subtitleLabel;
    QLabel *featuresLabel;
    QSpacerItem *middleSpacer;
    QHBoxLayout *buttonLayout;
    QSpacerItem *leftButtonSpacer;
    QPushButton *startButton;
    QSpacerItem *rightButtonSpacer;
    QLabel *footerLabel;
    QSpacerItem *bottomSpacer;

    void setupUi(QWidget *StartScreen)
    {
        if (StartScreen->objectName().isEmpty())
            StartScreen->setObjectName(QString::fromUtf8("StartScreen"));
        StartScreen->resize(900, 600);
        StartScreen->setStyleSheet(QString::fromUtf8("QWidget#StartScreen {\n"
"    background-color: #1a1a2e;\n"
"    }\n"
"   "));
        mainLayout = new QVBoxLayout(StartScreen);
        mainLayout->setSpacing(0);
        mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
        mainLayout->setContentsMargins(0, 0, 0, 0);
        topSpacer = new QSpacerItem(20, 120, QSizePolicy::Minimum, QSizePolicy::Expanding);

        mainLayout->addItem(topSpacer);

        titleLabel = new QLabel(StartScreen);
        titleLabel->setObjectName(QString::fromUtf8("titleLabel"));
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setStyleSheet(QString::fromUtf8("color: #64ffda;\n"
"font-size: 48px;\n"
"font-weight: bold;\n"
"font-family: 'Segoe UI', Arial, sans-serif;\n"
"margin: 15px;\n"
"padding: 10px;"));

        mainLayout->addWidget(titleLabel);

        subtitleLabel = new QLabel(StartScreen);
        subtitleLabel->setObjectName(QString::fromUtf8("subtitleLabel"));
        subtitleLabel->setAlignment(Qt::AlignCenter);
        subtitleLabel->setStyleSheet(QString::fromUtf8("color: #e0e0e0;\n"
"font-size: 16px;\n"
"font-weight: normal;\n"
"font-family: 'Segoe UI', Arial, sans-serif;\n"
"margin: 10px 30px 30px 30px;\n"
"padding: 5px;"));

        mainLayout->addWidget(subtitleLabel);

        featuresLabel = new QLabel(StartScreen);
        featuresLabel->setObjectName(QString::fromUtf8("featuresLabel"));
        featuresLabel->setAlignment(Qt::AlignCenter);
        featuresLabel->setStyleSheet(QString::fromUtf8("color: #b0b0b0;\n"
"font-size: 14px;\n"
"font-family: 'Segoe UI', Arial, sans-serif;\n"
"margin: 20px 50px;\n"
"padding: 18px;\n"
"background-color: #16213e;\n"
"border-radius: 12px;\n"
"border: 2px solid #0f3460;"));

        mainLayout->addWidget(featuresLabel);

        middleSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        mainLayout->addItem(middleSpacer);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setSpacing(0);
        buttonLayout->setObjectName(QString::fromUtf8("buttonLayout"));
        leftButtonSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        buttonLayout->addItem(leftButtonSpacer);

        startButton = new QPushButton(StartScreen);
        startButton->setObjectName(QString::fromUtf8("startButton"));
        startButton->setMinimumSize(QSize(220, 55));
        startButton->setMaximumSize(QSize(220, 55));
        startButton->setCursor(QCursor(Qt::PointingHandCursor));
        startButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #64ffda;\n"
"    color: #1a1a2e;\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    font-family: 'Segoe UI', Arial, sans-serif;\n"
"    border: 2px solid #64ffda;\n"
"    border-radius: 8px;\n"
"    padding: 12px 35px;\n"
"    text-align: center;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #7dffe8;\n"
"    border: 2px solid #7dffe8;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #52e6c4;\n"
"    border: 2px solid #52e6c4;\n"
"}"));

        buttonLayout->addWidget(startButton);

        rightButtonSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        buttonLayout->addItem(rightButtonSpacer);


        mainLayout->addLayout(buttonLayout);

        footerLabel = new QLabel(StartScreen);
        footerLabel->setObjectName(QString::fromUtf8("footerLabel"));
        footerLabel->setAlignment(Qt::AlignCenter);
        footerLabel->setStyleSheet(QString::fromUtf8("color: #808080;\n"
"font-size: 13px;\n"
"font-family: 'Segoe UI', Arial, sans-serif;\n"
"margin-top: 25px;\n"
"margin-bottom: 30px;"));

        mainLayout->addWidget(footerLabel);

        bottomSpacer = new QSpacerItem(20, 60, QSizePolicy::Minimum, QSizePolicy::Expanding);

        mainLayout->addItem(bottomSpacer);


        retranslateUi(StartScreen);

        QMetaObject::connectSlotsByName(StartScreen);
    } // setupUi

    void retranslateUi(QWidget *StartScreen)
    {
        StartScreen->setWindowTitle(QCoreApplication::translate("StartScreen", "Algorithm Visualizer", nullptr));
        titleLabel->setText(QCoreApplication::translate("StartScreen", "Algorithm Visualizer", nullptr));
        subtitleLabel->setText(QCoreApplication::translate("StartScreen", "Learn Data Structures & Algorithms Visually", nullptr));
        featuresLabel->setText(QCoreApplication::translate("StartScreen", "\342\234\223 Array Operations  \342\200\242  \342\234\223 Stack & Queue  \342\200\242  \342\234\223 Linked Lists  \342\200\242  \342\234\223 Trees  \342\200\242  \342\234\223 Graphs  \342\200\242  \342\234\223 String Matching", nullptr));
        startButton->setText(QCoreApplication::translate("StartScreen", "Get Started", nullptr));
        footerLabel->setText(QCoreApplication::translate("StartScreen", "Interactive learning made easy", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StartScreen: public Ui_StartScreen {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STARTSCREEN_H
