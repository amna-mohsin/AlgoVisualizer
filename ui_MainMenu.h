/********************************************************************************
** Form generated from reading UI file 'MainMenu.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINMENU_H
#define UI_MAINMENU_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainMenu
{
public:
    QVBoxLayout *verticalLayout;
    QStackedWidget *stackedWidget;
    QWidget *menuPage;
    QVBoxLayout *menuLayout;
    QLabel *titleLabel;
    QLabel *subtitleLabel;
    QWidget *cardsContainer;
    QGridLayout *cardsGridLayout;
    QPushButton *sortingCard;
    QVBoxLayout *sortingCardLayout;
    QLabel *sortingIcon;
    QSpacerItem *sortingIconSpacer;
    QLabel *sortingTitle;
    QLabel *sortingDesc;
    QSpacerItem *sortingSpacer;
    QLabel *sortingLink;
    QPushButton *stackCard;
    QVBoxLayout *stackCardLayout;
    QLabel *stackIcon;
    QLabel *stackTitle;
    QLabel *stackDesc;
    QSpacerItem *stackSpacer;
    QLabel *stackLink;
    QPushButton *queueCard;
    QVBoxLayout *queueCardLayout;
    QLabel *queueIcon;
    QLabel *queueTitle;
    QLabel *queueDesc;
    QSpacerItem *queueSpacer;
    QLabel *queueLink;
    QPushButton *linkedListCard;
    QVBoxLayout *linkedListCardLayout;
    QLabel *linkedListIcon;
    QLabel *linkedListTitle;
    QLabel *linkedListDesc;
    QSpacerItem *linkedListSpacer;
    QLabel *linkedListLink;
    QPushButton *treeCard;
    QVBoxLayout *treeCardLayout;
    QLabel *treeIcon;
    QSpacerItem *treeIconSpacer;
    QLabel *treeTitle;
    QLabel *treeDesc;
    QSpacerItem *treeSpacer;
    QLabel *treeLink;
    QPushButton *graphCard;
    QVBoxLayout *graphCardLayout;
    QLabel *graphIcon;
    QLabel *graphTitle;
    QLabel *graphDesc;
    QSpacerItem *graphSpacer;
    QLabel *graphLink;
    QPushButton *stringCard;
    QVBoxLayout *stringCardLayout;
    QLabel *stringIcon;
    QLabel *stringTitle;
    QLabel *stringDesc;
    QSpacerItem *stringSpacer;
    QLabel *stringLink;
    QSpacerItem *menuBottomSpacer;
    QWidget *sortingPage;
    QVBoxLayout *sortingPageLayout;
    QPushButton *backButtonSorting;
    QLabel *sortingPageTitle;
    QWidget *sortingContentContainer;
    QVBoxLayout *sortingContentLayout;
    QWidget *stackPage;
    QVBoxLayout *stackPageLayout;
    QPushButton *backButtonStack;
    QLabel *stackPageTitle;
    QWidget *stackContentContainer;
    QVBoxLayout *stackContentLayout;
    QWidget *queuePage;
    QVBoxLayout *queuePageLayout;
    QPushButton *backButtonQueue;
    QLabel *queuePageTitle;
    QWidget *queueContentContainer;
    QVBoxLayout *queueContentLayout;
    QWidget *linkedListPage;
    QVBoxLayout *linkedListPageLayout;
    QPushButton *backButtonLinkedList;
    QLabel *linkedListPageTitle;
    QWidget *linkedListContentContainer;
    QVBoxLayout *linkedListContentLayout;
    QWidget *treePage;
    QVBoxLayout *treePageLayout;
    QPushButton *backButtonTree;
    QLabel *treePageTitle;
    QWidget *treeContentContainer;
    QVBoxLayout *treeContentLayout;
    QWidget *graphPage;
    QVBoxLayout *graphPageLayout;
    QPushButton *backButtonGraph;
    QLabel *graphPageTitle;
    QWidget *graphContentContainer;
    QVBoxLayout *graphContentLayout;
    QWidget *stringPage;
    QVBoxLayout *stringPageLayout;
    QPushButton *backButtonString;
    QLabel *stringPageTitle;
    QWidget *stringContentContainer;
    QVBoxLayout *stringContentLayout;

    void setupUi(QWidget *MainMenu)
    {
        if (MainMenu->objectName().isEmpty())
            MainMenu->setObjectName(QString::fromUtf8("MainMenu"));
        MainMenu->resize(900, 600);
        MainMenu->setStyleSheet(QString::fromUtf8("QWidget#MainMenu {\n"
"    background-color: #1a1a2e;\n"
"    }"));
        verticalLayout = new QVBoxLayout(MainMenu);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        stackedWidget = new QStackedWidget(MainMenu);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setStyleSheet(QString::fromUtf8("QStackedWidget {\n"
"    background-color: #1a1a2e;\n"
"    }"));
        menuPage = new QWidget();
        menuPage->setObjectName(QString::fromUtf8("menuPage"));
        menuPage->setStyleSheet(QString::fromUtf8("QWidget#menuPage {\n"
"    background-color: #1a1a2e;\n"
"    }"));
        menuLayout = new QVBoxLayout(menuPage);
        menuLayout->setSpacing(0);
        menuLayout->setObjectName(QString::fromUtf8("menuLayout"));
        menuLayout->setContentsMargins(30, 20, 30, 20);
        titleLabel = new QLabel(menuPage);
        titleLabel->setObjectName(QString::fromUtf8("titleLabel"));
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setStyleSheet(QString::fromUtf8("color: #64ffda;\n"
"font-size: 32px;\n"
"font-weight: bold;\n"
"font-family: 'Segoe UI', Arial, sans-serif;\n"
"padding: 5px;\n"
"margin-bottom: 5px;"));

        menuLayout->addWidget(titleLabel);

        subtitleLabel = new QLabel(menuPage);
        subtitleLabel->setObjectName(QString::fromUtf8("subtitleLabel"));
        subtitleLabel->setAlignment(Qt::AlignCenter);
        subtitleLabel->setStyleSheet(QString::fromUtf8("color: #e0e0e0;\n"
"font-size: 13px;\n"
"font-family: 'Segoe UI', Arial, sans-serif;\n"
"padding: 2px;\n"
"margin-bottom: 25px;"));

        menuLayout->addWidget(subtitleLabel);

        cardsContainer = new QWidget(menuPage);
        cardsContainer->setObjectName(QString::fromUtf8("cardsContainer"));
        cardsContainer->setStyleSheet(QString::fromUtf8("QWidget#cardsContainer {\n"
"    background-color: transparent;\n"
"    }"));
        cardsGridLayout = new QGridLayout(cardsContainer);
        cardsGridLayout->setSpacing(15);
        cardsGridLayout->setObjectName(QString::fromUtf8("cardsGridLayout"));
        cardsGridLayout->setContentsMargins(0, 0, 0, 0);
        sortingCard = new QPushButton(cardsContainer);
        sortingCard->setObjectName(QString::fromUtf8("sortingCard"));
        sortingCard->setMinimumSize(QSize(260, 140));
        sortingCard->setMaximumSize(QSize(260, 140));
        sortingCard->setCursor(QCursor(Qt::PointingHandCursor));
        sortingCard->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #16213e;\n"
"    border: 2px solid #0f3460;\n"
"    border-radius: 10px;\n"
"    text-align: left;\n"
"    padding: 0px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #1e2a4a;\n"
"    border: 2px solid #1a5fb4;\n"
"}"));
        sortingCardLayout = new QVBoxLayout(sortingCard);
        sortingCardLayout->setSpacing(6);
        sortingCardLayout->setObjectName(QString::fromUtf8("sortingCardLayout"));
        sortingCardLayout->setContentsMargins(8, 4, 8, 8);
        sortingIcon = new QLabel(sortingCard);
        sortingIcon->setObjectName(QString::fromUtf8("sortingIcon"));
        sortingIcon->setMinimumSize(QSize(44, 44));
        sortingIcon->setMaximumSize(QSize(44, 44));
        sortingIcon->setStyleSheet(QString::fromUtf8("font-size: 28px;\n"
"background-color: #2196F3;\n"
"border-radius: 6px;\n"
"padding: 8px;\n"
"margin-bottom: 5px;"));
        sortingIcon->setAlignment(Qt::AlignCenter);

        sortingCardLayout->addWidget(sortingIcon);

        sortingIconSpacer = new QSpacerItem(20, 12, QSizePolicy::Minimum, QSizePolicy::Expanding);

        sortingCardLayout->addItem(sortingIconSpacer);

        sortingTitle = new QLabel(sortingCard);
        sortingTitle->setObjectName(QString::fromUtf8("sortingTitle"));
        sortingTitle->setStyleSheet(QString::fromUtf8("color: white;\n"
"font-size: 16px;\n"
"font-weight: bold;\n"
"font-family: 'Segoe UI', Arial, sans-serif;"));

        sortingCardLayout->addWidget(sortingTitle);

        sortingDesc = new QLabel(sortingCard);
        sortingDesc->setObjectName(QString::fromUtf8("sortingDesc"));
        sortingDesc->setWordWrap(true);
        sortingDesc->setStyleSheet(QString::fromUtf8("color: #b0b0b0;\n"
"font-size: 11px;\n"
"font-family: 'Segoe UI', Arial, sans-serif;"));

        sortingCardLayout->addWidget(sortingDesc);

        sortingSpacer = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);

        sortingCardLayout->addItem(sortingSpacer);

        sortingLink = new QLabel(sortingCard);
        sortingLink->setObjectName(QString::fromUtf8("sortingLink"));
        sortingLink->setStyleSheet(QString::fromUtf8("color: #64ffda;\n"
"font-size: 12px;\n"
"font-weight: bold;\n"
"font-family: 'Segoe UI', Arial, sans-serif;"));

        sortingCardLayout->addWidget(sortingLink);


        cardsGridLayout->addWidget(sortingCard, 0, 0, 1, 1);

        stackCard = new QPushButton(cardsContainer);
        stackCard->setObjectName(QString::fromUtf8("stackCard"));
        stackCard->setMinimumSize(QSize(260, 140));
        stackCard->setMaximumSize(QSize(260, 140));
        stackCard->setCursor(QCursor(Qt::PointingHandCursor));
        stackCard->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #16213e;\n"
"    border: 2px solid #0f3460;\n"
"    border-radius: 10px;\n"
"    text-align: left;\n"
"    padding: 0px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #1e2a4a;\n"
"    border: 2px solid #1a5fb4;\n"
"}"));
        stackCardLayout = new QVBoxLayout(stackCard);
        stackCardLayout->setSpacing(8);
        stackCardLayout->setObjectName(QString::fromUtf8("stackCardLayout"));
        stackCardLayout->setContentsMargins(8, 8, 8, 8);
        stackIcon = new QLabel(stackCard);
        stackIcon->setObjectName(QString::fromUtf8("stackIcon"));
        stackIcon->setMinimumSize(QSize(44, 44));
        stackIcon->setMaximumSize(QSize(44, 44));
        stackIcon->setStyleSheet(QString::fromUtf8("font-size: 28px;\n"
"background-color: #9C27B0;\n"
"border-radius: 6px;\n"
"padding: 8px;"));
        stackIcon->setAlignment(Qt::AlignCenter);

        stackCardLayout->addWidget(stackIcon);

        stackTitle = new QLabel(stackCard);
        stackTitle->setObjectName(QString::fromUtf8("stackTitle"));
        stackTitle->setStyleSheet(QString::fromUtf8("color: white;\n"
"font-size: 16px;\n"
"font-weight: bold;"));

        stackCardLayout->addWidget(stackTitle);

        stackDesc = new QLabel(stackCard);
        stackDesc->setObjectName(QString::fromUtf8("stackDesc"));
        stackDesc->setWordWrap(true);
        stackDesc->setStyleSheet(QString::fromUtf8("color: #b0b0b0;\n"
"font-size: 11px;"));

        stackCardLayout->addWidget(stackDesc);

        stackSpacer = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);

        stackCardLayout->addItem(stackSpacer);

        stackLink = new QLabel(stackCard);
        stackLink->setObjectName(QString::fromUtf8("stackLink"));
        stackLink->setStyleSheet(QString::fromUtf8("color: #64ffda;\n"
"font-size: 12px;\n"
"font-weight: bold;"));

        stackCardLayout->addWidget(stackLink);


        cardsGridLayout->addWidget(stackCard, 0, 1, 1, 1);

        queueCard = new QPushButton(cardsContainer);
        queueCard->setObjectName(QString::fromUtf8("queueCard"));
        queueCard->setMinimumSize(QSize(260, 140));
        queueCard->setMaximumSize(QSize(260, 140));
        queueCard->setCursor(QCursor(Qt::PointingHandCursor));
        queueCard->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #16213e;\n"
"    border: 2px solid #0f3460;\n"
"    border-radius: 10px;\n"
"    text-align: left;\n"
"    padding: 0px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #1e2a4a;\n"
"    border: 2px solid #1a5fb4;\n"
"}"));
        queueCardLayout = new QVBoxLayout(queueCard);
        queueCardLayout->setSpacing(8);
        queueCardLayout->setObjectName(QString::fromUtf8("queueCardLayout"));
        queueCardLayout->setContentsMargins(8, 8, 8, 8);
        queueIcon = new QLabel(queueCard);
        queueIcon->setObjectName(QString::fromUtf8("queueIcon"));
        queueIcon->setMinimumSize(QSize(44, 44));
        queueIcon->setMaximumSize(QSize(44, 44));
        queueIcon->setStyleSheet(QString::fromUtf8("font-size: 28px;\n"
"background-color: #4CAF50;\n"
"border-radius: 6px;\n"
"padding: 8px;"));
        queueIcon->setAlignment(Qt::AlignCenter);

        queueCardLayout->addWidget(queueIcon);

        queueTitle = new QLabel(queueCard);
        queueTitle->setObjectName(QString::fromUtf8("queueTitle"));
        queueTitle->setStyleSheet(QString::fromUtf8("color: white;\n"
"font-size: 16px;\n"
"font-weight: bold;"));

        queueCardLayout->addWidget(queueTitle);

        queueDesc = new QLabel(queueCard);
        queueDesc->setObjectName(QString::fromUtf8("queueDesc"));
        queueDesc->setWordWrap(true);
        queueDesc->setStyleSheet(QString::fromUtf8("color: #b0b0b0;\n"
"font-size: 11px;"));

        queueCardLayout->addWidget(queueDesc);

        queueSpacer = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);

        queueCardLayout->addItem(queueSpacer);

        queueLink = new QLabel(queueCard);
        queueLink->setObjectName(QString::fromUtf8("queueLink"));
        queueLink->setStyleSheet(QString::fromUtf8("color: #64ffda;\n"
"font-size: 12px;\n"
"font-weight: bold;"));

        queueCardLayout->addWidget(queueLink);


        cardsGridLayout->addWidget(queueCard, 0, 2, 1, 1);

        linkedListCard = new QPushButton(cardsContainer);
        linkedListCard->setObjectName(QString::fromUtf8("linkedListCard"));
        linkedListCard->setMinimumSize(QSize(260, 140));
        linkedListCard->setMaximumSize(QSize(260, 140));
        linkedListCard->setCursor(QCursor(Qt::PointingHandCursor));
        linkedListCard->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #16213e;\n"
"    border: 2px solid #0f3460;\n"
"    border-radius: 10px;\n"
"    text-align: left;\n"
"    padding: 0px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #1e2a4a;\n"
"    border: 2px solid #1a5fb4;\n"
"}"));
        linkedListCardLayout = new QVBoxLayout(linkedListCard);
        linkedListCardLayout->setSpacing(8);
        linkedListCardLayout->setObjectName(QString::fromUtf8("linkedListCardLayout"));
        linkedListCardLayout->setContentsMargins(8, 8, 8, 8);
        linkedListIcon = new QLabel(linkedListCard);
        linkedListIcon->setObjectName(QString::fromUtf8("linkedListIcon"));
        linkedListIcon->setMinimumSize(QSize(44, 44));
        linkedListIcon->setMaximumSize(QSize(44, 44));
        linkedListIcon->setStyleSheet(QString::fromUtf8("font-size: 28px;\n"
"background-color: #FF9800;\n"
"border-radius: 6px;\n"
"padding: 8px;"));
        linkedListIcon->setAlignment(Qt::AlignCenter);

        linkedListCardLayout->addWidget(linkedListIcon);

        linkedListTitle = new QLabel(linkedListCard);
        linkedListTitle->setObjectName(QString::fromUtf8("linkedListTitle"));
        linkedListTitle->setStyleSheet(QString::fromUtf8("color: white;\n"
"font-size: 16px;\n"
"font-weight: bold;"));

        linkedListCardLayout->addWidget(linkedListTitle);

        linkedListDesc = new QLabel(linkedListCard);
        linkedListDesc->setObjectName(QString::fromUtf8("linkedListDesc"));
        linkedListDesc->setWordWrap(true);
        linkedListDesc->setStyleSheet(QString::fromUtf8("color: #b0b0b0;\n"
"font-size: 11px;"));

        linkedListCardLayout->addWidget(linkedListDesc);

        linkedListSpacer = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);

        linkedListCardLayout->addItem(linkedListSpacer);

        linkedListLink = new QLabel(linkedListCard);
        linkedListLink->setObjectName(QString::fromUtf8("linkedListLink"));
        linkedListLink->setStyleSheet(QString::fromUtf8("color: #64ffda;\n"
"font-size: 12px;\n"
"font-weight: bold;"));

        linkedListCardLayout->addWidget(linkedListLink);


        cardsGridLayout->addWidget(linkedListCard, 1, 0, 1, 1);

        treeCard = new QPushButton(cardsContainer);
        treeCard->setObjectName(QString::fromUtf8("treeCard"));
        treeCard->setMinimumSize(QSize(260, 140));
        treeCard->setMaximumSize(QSize(260, 140));
        treeCard->setCursor(QCursor(Qt::PointingHandCursor));
        treeCard->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #16213e;\n"
"    border: 2px solid #0f3460;\n"
"    border-radius: 10px;\n"
"    text-align: left;\n"
"    padding: 0px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #1e2a4a;\n"
"    border: 2px solid #1a5fb4;\n"
"}"));
        treeCardLayout = new QVBoxLayout(treeCard);
        treeCardLayout->setSpacing(6);
        treeCardLayout->setObjectName(QString::fromUtf8("treeCardLayout"));
        treeCardLayout->setContentsMargins(8, 4, 8, 8);
        treeIcon = new QLabel(treeCard);
        treeIcon->setObjectName(QString::fromUtf8("treeIcon"));
        treeIcon->setMinimumSize(QSize(44, 44));
        treeIcon->setMaximumSize(QSize(44, 44));
        treeIcon->setStyleSheet(QString::fromUtf8("font-size: 28px;\n"
"background-color: #00BCD4;\n"
"border-radius: 6px;\n"
"padding: 8px;\n"
"margin-bottom: 5px;"));
        treeIcon->setAlignment(Qt::AlignCenter);

        treeCardLayout->addWidget(treeIcon);

        treeIconSpacer = new QSpacerItem(20, 12, QSizePolicy::Minimum, QSizePolicy::Expanding);

        treeCardLayout->addItem(treeIconSpacer);

        treeTitle = new QLabel(treeCard);
        treeTitle->setObjectName(QString::fromUtf8("treeTitle"));
        treeTitle->setStyleSheet(QString::fromUtf8("color: white;\n"
"font-size: 16px;\n"
"font-weight: bold;"));

        treeCardLayout->addWidget(treeTitle);

        treeDesc = new QLabel(treeCard);
        treeDesc->setObjectName(QString::fromUtf8("treeDesc"));
        treeDesc->setWordWrap(true);
        treeDesc->setStyleSheet(QString::fromUtf8("color: #b0b0b0;\n"
"font-size: 11px;"));

        treeCardLayout->addWidget(treeDesc);

        treeSpacer = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);

        treeCardLayout->addItem(treeSpacer);

        treeLink = new QLabel(treeCard);
        treeLink->setObjectName(QString::fromUtf8("treeLink"));
        treeLink->setStyleSheet(QString::fromUtf8("color: #64ffda;\n"
"font-size: 12px;\n"
"font-weight: bold;"));

        treeCardLayout->addWidget(treeLink);


        cardsGridLayout->addWidget(treeCard, 1, 1, 1, 1);

        graphCard = new QPushButton(cardsContainer);
        graphCard->setObjectName(QString::fromUtf8("graphCard"));
        graphCard->setMinimumSize(QSize(260, 140));
        graphCard->setMaximumSize(QSize(260, 140));
        graphCard->setCursor(QCursor(Qt::PointingHandCursor));
        graphCard->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #16213e;\n"
"    border: 2px solid #0f3460;\n"
"    border-radius: 10px;\n"
"    text-align: left;\n"
"    padding: 0px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #1e2a4a;\n"
"    border: 2px solid #1a5fb4;\n"
"}"));
        graphCardLayout = new QVBoxLayout(graphCard);
        graphCardLayout->setSpacing(8);
        graphCardLayout->setObjectName(QString::fromUtf8("graphCardLayout"));
        graphCardLayout->setContentsMargins(8, 8, 8, 8);
        graphIcon = new QLabel(graphCard);
        graphIcon->setObjectName(QString::fromUtf8("graphIcon"));
        graphIcon->setMinimumSize(QSize(44, 44));
        graphIcon->setMaximumSize(QSize(44, 44));
        graphIcon->setStyleSheet(QString::fromUtf8("font-size: 28px;\n"
"background-color: #9C27B0;\n"
"border-radius: 6px;\n"
"padding: 8px;"));
        graphIcon->setAlignment(Qt::AlignCenter);

        graphCardLayout->addWidget(graphIcon);

        graphTitle = new QLabel(graphCard);
        graphTitle->setObjectName(QString::fromUtf8("graphTitle"));
        graphTitle->setStyleSheet(QString::fromUtf8("color: white;\n"
"font-size: 16px;\n"
"font-weight: bold;"));

        graphCardLayout->addWidget(graphTitle);

        graphDesc = new QLabel(graphCard);
        graphDesc->setObjectName(QString::fromUtf8("graphDesc"));
        graphDesc->setWordWrap(true);
        graphDesc->setStyleSheet(QString::fromUtf8("color: #b0b0b0;\n"
"font-size: 11px;"));

        graphCardLayout->addWidget(graphDesc);

        graphSpacer = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);

        graphCardLayout->addItem(graphSpacer);

        graphLink = new QLabel(graphCard);
        graphLink->setObjectName(QString::fromUtf8("graphLink"));
        graphLink->setStyleSheet(QString::fromUtf8("color: #64ffda;\n"
"font-size: 12px;\n"
"font-weight: bold;"));

        graphCardLayout->addWidget(graphLink);


        cardsGridLayout->addWidget(graphCard, 1, 2, 1, 1);

        stringCard = new QPushButton(cardsContainer);
        stringCard->setObjectName(QString::fromUtf8("stringCard"));
        stringCard->setMinimumSize(QSize(260, 140));
        stringCard->setMaximumSize(QSize(260, 140));
        stringCard->setCursor(QCursor(Qt::PointingHandCursor));
        stringCard->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #16213e;\n"
"    border: 2px solid #0f3460;\n"
"    border-radius: 10px;\n"
"    text-align: left;\n"
"    padding: 0px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #1e2a4a;\n"
"    border: 2px solid #1a5fb4;\n"
"}"));
        stringCardLayout = new QVBoxLayout(stringCard);
        stringCardLayout->setSpacing(8);
        stringCardLayout->setObjectName(QString::fromUtf8("stringCardLayout"));
        stringCardLayout->setContentsMargins(8, 8, 8, 8);
        stringIcon = new QLabel(stringCard);
        stringIcon->setObjectName(QString::fromUtf8("stringIcon"));
        stringIcon->setMinimumSize(QSize(44, 44));
        stringIcon->setMaximumSize(QSize(44, 44));
        stringIcon->setStyleSheet(QString::fromUtf8("font-size: 28px;\n"
"background-color: #FF5722;\n"
"border-radius: 6px;\n"
"padding: 8px;"));
        stringIcon->setAlignment(Qt::AlignCenter);

        stringCardLayout->addWidget(stringIcon);

        stringTitle = new QLabel(stringCard);
        stringTitle->setObjectName(QString::fromUtf8("stringTitle"));
        stringTitle->setStyleSheet(QString::fromUtf8("color: white;\n"
"font-size: 16px;\n"
"font-weight: bold;"));

        stringCardLayout->addWidget(stringTitle);

        stringDesc = new QLabel(stringCard);
        stringDesc->setObjectName(QString::fromUtf8("stringDesc"));
        stringDesc->setWordWrap(true);
        stringDesc->setStyleSheet(QString::fromUtf8("color: #b0b0b0;\n"
"font-size: 11px;"));

        stringCardLayout->addWidget(stringDesc);

        stringSpacer = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);

        stringCardLayout->addItem(stringSpacer);

        stringLink = new QLabel(stringCard);
        stringLink->setObjectName(QString::fromUtf8("stringLink"));
        stringLink->setStyleSheet(QString::fromUtf8("color: #64ffda;\n"
"font-size: 12px;\n"
"font-weight: bold;"));

        stringCardLayout->addWidget(stringLink);


        cardsGridLayout->addWidget(stringCard, 2, 0, 1, 1);


        menuLayout->addWidget(cardsContainer);

        menuBottomSpacer = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        menuLayout->addItem(menuBottomSpacer);

        stackedWidget->addWidget(menuPage);
        sortingPage = new QWidget();
        sortingPage->setObjectName(QString::fromUtf8("sortingPage"));
        sortingPage->setStyleSheet(QString::fromUtf8("QWidget#sortingPage {\n"
"    background-color: #1a1a2e;\n"
"    }"));
        sortingPageLayout = new QVBoxLayout(sortingPage);
        sortingPageLayout->setSpacing(20);
        sortingPageLayout->setObjectName(QString::fromUtf8("sortingPageLayout"));
        sortingPageLayout->setContentsMargins(30, 20, 30, 20);
        backButtonSorting = new QPushButton(sortingPage);
        backButtonSorting->setObjectName(QString::fromUtf8("backButtonSorting"));
        backButtonSorting->setMaximumSize(QSize(150, 40));
        backButtonSorting->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #0f3460;\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    font-size: 14px;\n"
"    padding: 8px 15px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #1a5fb4;\n"
"}"));

        sortingPageLayout->addWidget(backButtonSorting);

        sortingPageTitle = new QLabel(sortingPage);
        sortingPageTitle->setObjectName(QString::fromUtf8("sortingPageTitle"));
        sortingPageTitle->setStyleSheet(QString::fromUtf8("color: #64ffda;\n"
"font-size: 32px;\n"
"font-weight: bold;\n"
"padding: 20px 0;"));

        sortingPageLayout->addWidget(sortingPageTitle);

        sortingContentContainer = new QWidget(sortingPage);
        sortingContentContainer->setObjectName(QString::fromUtf8("sortingContentContainer"));
        sortingContentContainer->setStyleSheet(QString::fromUtf8("QWidget#sortingContentContainer {\n"
"    background-color: transparent;\n"
"}"));
        sortingContentLayout = new QVBoxLayout(sortingContentContainer);
        sortingContentLayout->setSpacing(0);
        sortingContentLayout->setObjectName(QString::fromUtf8("sortingContentLayout"));
        sortingContentLayout->setContentsMargins(0, 0, 0, 0);

        sortingPageLayout->addWidget(sortingContentContainer);

        stackedWidget->addWidget(sortingPage);
        stackPage = new QWidget();
        stackPage->setObjectName(QString::fromUtf8("stackPage"));
        stackPage->setStyleSheet(QString::fromUtf8("QWidget#stackPage {\n"
"    background-color: #1a1a2e;\n"
"    }"));
        stackPageLayout = new QVBoxLayout(stackPage);
        stackPageLayout->setSpacing(20);
        stackPageLayout->setObjectName(QString::fromUtf8("stackPageLayout"));
        stackPageLayout->setContentsMargins(30, 20, 30, 20);
        backButtonStack = new QPushButton(stackPage);
        backButtonStack->setObjectName(QString::fromUtf8("backButtonStack"));
        backButtonStack->setMaximumSize(QSize(150, 40));
        backButtonStack->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #0f3460;\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    font-size: 14px;\n"
"    padding: 8px 15px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #1a5fb4;\n"
"}"));

        stackPageLayout->addWidget(backButtonStack);

        stackPageTitle = new QLabel(stackPage);
        stackPageTitle->setObjectName(QString::fromUtf8("stackPageTitle"));
        stackPageTitle->setStyleSheet(QString::fromUtf8("color: #64ffda;\n"
"font-size: 32px;\n"
"font-weight: bold;\n"
"padding: 20px 0;"));

        stackPageLayout->addWidget(stackPageTitle);

        stackContentContainer = new QWidget(stackPage);
        stackContentContainer->setObjectName(QString::fromUtf8("stackContentContainer"));
        stackContentContainer->setStyleSheet(QString::fromUtf8("QWidget#stackContentContainer {\n"
"    background-color: transparent;\n"
"}"));
        stackContentLayout = new QVBoxLayout(stackContentContainer);
        stackContentLayout->setSpacing(0);
        stackContentLayout->setObjectName(QString::fromUtf8("stackContentLayout"));
        stackContentLayout->setContentsMargins(0, 0, 0, 0);

        stackPageLayout->addWidget(stackContentContainer);

        stackedWidget->addWidget(stackPage);
        queuePage = new QWidget();
        queuePage->setObjectName(QString::fromUtf8("queuePage"));
        queuePage->setStyleSheet(QString::fromUtf8("QWidget#queuePage {\n"
"    background-color: #1a1a2e;\n"
"    }"));
        queuePageLayout = new QVBoxLayout(queuePage);
        queuePageLayout->setSpacing(20);
        queuePageLayout->setObjectName(QString::fromUtf8("queuePageLayout"));
        queuePageLayout->setContentsMargins(30, 20, 30, 20);
        backButtonQueue = new QPushButton(queuePage);
        backButtonQueue->setObjectName(QString::fromUtf8("backButtonQueue"));
        backButtonQueue->setMaximumSize(QSize(150, 40));
        backButtonQueue->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #0f3460;\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    font-size: 14px;\n"
"    padding: 8px 15px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #1a5fb4;\n"
"}"));

        queuePageLayout->addWidget(backButtonQueue);

        queuePageTitle = new QLabel(queuePage);
        queuePageTitle->setObjectName(QString::fromUtf8("queuePageTitle"));
        queuePageTitle->setStyleSheet(QString::fromUtf8("color: #64ffda;\n"
"font-size: 32px;\n"
"font-weight: bold;\n"
"padding: 20px 0;"));

        queuePageLayout->addWidget(queuePageTitle);

        queueContentContainer = new QWidget(queuePage);
        queueContentContainer->setObjectName(QString::fromUtf8("queueContentContainer"));
        queueContentContainer->setStyleSheet(QString::fromUtf8("QWidget#queueContentContainer {\n"
"    background-color: transparent;\n"
"}"));
        queueContentLayout = new QVBoxLayout(queueContentContainer);
        queueContentLayout->setSpacing(0);
        queueContentLayout->setObjectName(QString::fromUtf8("queueContentLayout"));
        queueContentLayout->setContentsMargins(0, 0, 0, 0);

        queuePageLayout->addWidget(queueContentContainer);

        stackedWidget->addWidget(queuePage);
        linkedListPage = new QWidget();
        linkedListPage->setObjectName(QString::fromUtf8("linkedListPage"));
        linkedListPage->setStyleSheet(QString::fromUtf8("QWidget#linkedListPage {\n"
"    background-color: #1a1a2e;\n"
"    }"));
        linkedListPageLayout = new QVBoxLayout(linkedListPage);
        linkedListPageLayout->setSpacing(20);
        linkedListPageLayout->setObjectName(QString::fromUtf8("linkedListPageLayout"));
        linkedListPageLayout->setContentsMargins(30, 20, 30, 20);
        backButtonLinkedList = new QPushButton(linkedListPage);
        backButtonLinkedList->setObjectName(QString::fromUtf8("backButtonLinkedList"));
        backButtonLinkedList->setMaximumSize(QSize(150, 40));
        backButtonLinkedList->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #0f3460;\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    font-size: 14px;\n"
"    padding: 8px 15px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #1a5fb4;\n"
"}"));

        linkedListPageLayout->addWidget(backButtonLinkedList);

        linkedListPageTitle = new QLabel(linkedListPage);
        linkedListPageTitle->setObjectName(QString::fromUtf8("linkedListPageTitle"));
        linkedListPageTitle->setStyleSheet(QString::fromUtf8("color: #64ffda;\n"
"font-size: 32px;\n"
"font-weight: bold;\n"
"padding: 20px 0;"));

        linkedListPageLayout->addWidget(linkedListPageTitle);

        linkedListContentContainer = new QWidget(linkedListPage);
        linkedListContentContainer->setObjectName(QString::fromUtf8("linkedListContentContainer"));
        linkedListContentContainer->setStyleSheet(QString::fromUtf8("QWidget#linkedListContentContainer {\n"
"    background-color: transparent;\n"
"}"));
        linkedListContentLayout = new QVBoxLayout(linkedListContentContainer);
        linkedListContentLayout->setSpacing(0);
        linkedListContentLayout->setObjectName(QString::fromUtf8("linkedListContentLayout"));
        linkedListContentLayout->setContentsMargins(0, 0, 0, 0);

        linkedListPageLayout->addWidget(linkedListContentContainer);

        stackedWidget->addWidget(linkedListPage);
        treePage = new QWidget();
        treePage->setObjectName(QString::fromUtf8("treePage"));
        treePage->setStyleSheet(QString::fromUtf8("QWidget#treePage {\n"
"    background-color: #1a1a2e;\n"
"    }"));
        treePageLayout = new QVBoxLayout(treePage);
        treePageLayout->setSpacing(20);
        treePageLayout->setObjectName(QString::fromUtf8("treePageLayout"));
        treePageLayout->setContentsMargins(30, 20, 30, 20);
        backButtonTree = new QPushButton(treePage);
        backButtonTree->setObjectName(QString::fromUtf8("backButtonTree"));
        backButtonTree->setMaximumSize(QSize(150, 40));
        backButtonTree->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #0f3460;\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    font-size: 14px;\n"
"    padding: 8px 15px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #1a5fb4;\n"
"}"));

        treePageLayout->addWidget(backButtonTree);

        treePageTitle = new QLabel(treePage);
        treePageTitle->setObjectName(QString::fromUtf8("treePageTitle"));
        treePageTitle->setStyleSheet(QString::fromUtf8("color: #64ffda;\n"
"font-size: 32px;\n"
"font-weight: bold;\n"
"padding: 20px 0;"));

        treePageLayout->addWidget(treePageTitle);

        treeContentContainer = new QWidget(treePage);
        treeContentContainer->setObjectName(QString::fromUtf8("treeContentContainer"));
        treeContentContainer->setStyleSheet(QString::fromUtf8("QWidget#treeContentContainer {\n"
"    background-color: transparent;\n"
"}"));
        treeContentLayout = new QVBoxLayout(treeContentContainer);
        treeContentLayout->setSpacing(0);
        treeContentLayout->setObjectName(QString::fromUtf8("treeContentLayout"));
        treeContentLayout->setContentsMargins(0, 0, 0, 0);

        treePageLayout->addWidget(treeContentContainer);

        stackedWidget->addWidget(treePage);
        graphPage = new QWidget();
        graphPage->setObjectName(QString::fromUtf8("graphPage"));
        graphPage->setStyleSheet(QString::fromUtf8("QWidget#graphPage {\n"
"    background-color: #1a1a2e;\n"
"    }"));
        graphPageLayout = new QVBoxLayout(graphPage);
        graphPageLayout->setSpacing(20);
        graphPageLayout->setObjectName(QString::fromUtf8("graphPageLayout"));
        graphPageLayout->setContentsMargins(30, 20, 30, 20);
        backButtonGraph = new QPushButton(graphPage);
        backButtonGraph->setObjectName(QString::fromUtf8("backButtonGraph"));
        backButtonGraph->setMaximumSize(QSize(150, 40));
        backButtonGraph->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #0f3460;\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    font-size: 14px;\n"
"    padding: 8px 15px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #1a5fb4;\n"
"}"));

        graphPageLayout->addWidget(backButtonGraph);

        graphPageTitle = new QLabel(graphPage);
        graphPageTitle->setObjectName(QString::fromUtf8("graphPageTitle"));
        graphPageTitle->setStyleSheet(QString::fromUtf8("color: #64ffda;\n"
"font-size: 32px;\n"
"font-weight: bold;\n"
"padding: 20px 0;"));

        graphPageLayout->addWidget(graphPageTitle);

        graphContentContainer = new QWidget(graphPage);
        graphContentContainer->setObjectName(QString::fromUtf8("graphContentContainer"));
        graphContentContainer->setStyleSheet(QString::fromUtf8("QWidget#graphContentContainer {\n"
"    background-color: transparent;\n"
"}"));
        graphContentLayout = new QVBoxLayout(graphContentContainer);
        graphContentLayout->setSpacing(0);
        graphContentLayout->setObjectName(QString::fromUtf8("graphContentLayout"));
        graphContentLayout->setContentsMargins(0, 0, 0, 0);

        graphPageLayout->addWidget(graphContentContainer);

        stackedWidget->addWidget(graphPage);
        stringPage = new QWidget();
        stringPage->setObjectName(QString::fromUtf8("stringPage"));
        stringPage->setStyleSheet(QString::fromUtf8("QWidget#stringPage {\n"
"    background-color: #1a1a2e;\n"
"    }"));
        stringPageLayout = new QVBoxLayout(stringPage);
        stringPageLayout->setSpacing(20);
        stringPageLayout->setObjectName(QString::fromUtf8("stringPageLayout"));
        stringPageLayout->setContentsMargins(30, 20, 30, 20);
        backButtonString = new QPushButton(stringPage);
        backButtonString->setObjectName(QString::fromUtf8("backButtonString"));
        backButtonString->setMaximumSize(QSize(150, 40));
        backButtonString->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #0f3460;\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    font-size: 14px;\n"
"    padding: 8px 15px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #1a5fb4;\n"
"}"));

        stringPageLayout->addWidget(backButtonString);

        stringPageTitle = new QLabel(stringPage);
        stringPageTitle->setObjectName(QString::fromUtf8("stringPageTitle"));
        stringPageTitle->setStyleSheet(QString::fromUtf8("color: #64ffda;\n"
"font-size: 32px;\n"
"font-weight: bold;\n"
"padding: 20px 0;"));

        stringPageLayout->addWidget(stringPageTitle);

        stringContentContainer = new QWidget(stringPage);
        stringContentContainer->setObjectName(QString::fromUtf8("stringContentContainer"));
        stringContentContainer->setStyleSheet(QString::fromUtf8("QWidget#stringContentContainer {\n"
"    background-color: transparent;\n"
"}"));
        stringContentLayout = new QVBoxLayout(stringContentContainer);
        stringContentLayout->setSpacing(0);
        stringContentLayout->setObjectName(QString::fromUtf8("stringContentLayout"));
        stringContentLayout->setContentsMargins(0, 0, 0, 0);

        stringPageLayout->addWidget(stringContentContainer);

        stackedWidget->addWidget(stringPage);

        verticalLayout->addWidget(stackedWidget);


        retranslateUi(MainMenu);

        QMetaObject::connectSlotsByName(MainMenu);
    } // setupUi

    void retranslateUi(QWidget *MainMenu)
    {
        MainMenu->setWindowTitle(QCoreApplication::translate("MainMenu", "Algorithm Visualizer", nullptr));
        titleLabel->setText(QCoreApplication::translate("MainMenu", "Algorithm Visualizer", nullptr));
        subtitleLabel->setText(QCoreApplication::translate("MainMenu", "Interactive step-by-step visualization of Data Structures and Algorithms", nullptr));
        sortingIcon->setText(QCoreApplication::translate("MainMenu", "\360\237\223\212", nullptr));
        sortingTitle->setText(QCoreApplication::translate("MainMenu", "Array Operations", nullptr));
        sortingDesc->setText(QCoreApplication::translate("MainMenu", "Visualize Bubble, Selection, Insertion, Merge, and Quick Sort", nullptr));
        sortingLink->setText(QCoreApplication::translate("MainMenu", "Explore -", nullptr));
        stackIcon->setText(QCoreApplication::translate("MainMenu", "\360\237\223\232", nullptr));
        stackTitle->setText(QCoreApplication::translate("MainMenu", "Stack Operations", nullptr));
        stackDesc->setText(QCoreApplication::translate("MainMenu", "Interactive Push and Pop operations", nullptr));
        stackLink->setText(QCoreApplication::translate("MainMenu", "Explore -", nullptr));
        queueIcon->setText(QCoreApplication::translate("MainMenu", "\360\237\223\213", nullptr));
        queueTitle->setText(QCoreApplication::translate("MainMenu", "Queue Operations", nullptr));
        queueDesc->setText(QCoreApplication::translate("MainMenu", "Visualize Enqueue and Dequeue operations", nullptr));
        queueLink->setText(QCoreApplication::translate("MainMenu", "Explore -", nullptr));
        linkedListIcon->setText(QCoreApplication::translate("MainMenu", "\360\237\224\227", nullptr));
        linkedListTitle->setText(QCoreApplication::translate("MainMenu", "Linked List", nullptr));
        linkedListDesc->setText(QCoreApplication::translate("MainMenu", "Insertion and deletion at various positions", nullptr));
        linkedListLink->setText(QCoreApplication::translate("MainMenu", "Explore -", nullptr));
        treeIcon->setText(QCoreApplication::translate("MainMenu", "\360\237\214\263", nullptr));
        treeTitle->setText(QCoreApplication::translate("MainMenu", "Tree Traversals", nullptr));
        treeDesc->setText(QCoreApplication::translate("MainMenu", "Binary Tree and BST traversals (Pre, In, Post order)", nullptr));
        treeLink->setText(QCoreApplication::translate("MainMenu", "Explore -", nullptr));
        graphIcon->setText(QCoreApplication::translate("MainMenu", "\360\237\225\270\357\270\217", nullptr));
        graphTitle->setText(QCoreApplication::translate("MainMenu", "Graph Algorithms", nullptr));
        graphDesc->setText(QCoreApplication::translate("MainMenu", "BFS, DFS, and shortest path visualizations", nullptr));
        graphLink->setText(QCoreApplication::translate("MainMenu", "Explore -", nullptr));
        stringIcon->setText(QCoreApplication::translate("MainMenu", "\360\237\224\244", nullptr));
        stringTitle->setText(QCoreApplication::translate("MainMenu", "String Algorithms", nullptr));
        stringDesc->setText(QCoreApplication::translate("MainMenu", "Pattern matching with Naive and KMP algorithms", nullptr));
        stringLink->setText(QCoreApplication::translate("MainMenu", "Explore -", nullptr));
        backButtonSorting->setText(QCoreApplication::translate("MainMenu", "\342\206\220 Back to Menu", nullptr));
        sortingPageTitle->setText(QCoreApplication::translate("MainMenu", "Array Operations", nullptr));
        backButtonStack->setText(QCoreApplication::translate("MainMenu", "\342\206\220 Back to Menu", nullptr));
        stackPageTitle->setText(QCoreApplication::translate("MainMenu", "Stack Operations", nullptr));
        backButtonQueue->setText(QCoreApplication::translate("MainMenu", "\342\206\220 Back to Menu", nullptr));
        queuePageTitle->setText(QCoreApplication::translate("MainMenu", "Queue Operations", nullptr));
        backButtonLinkedList->setText(QCoreApplication::translate("MainMenu", "\342\206\220 Back to Menu", nullptr));
        linkedListPageTitle->setText(QCoreApplication::translate("MainMenu", "Linked List", nullptr));
        backButtonTree->setText(QCoreApplication::translate("MainMenu", "\342\206\220 Back to Menu", nullptr));
        treePageTitle->setText(QCoreApplication::translate("MainMenu", "Tree Traversals", nullptr));
        backButtonGraph->setText(QCoreApplication::translate("MainMenu", "\342\206\220 Back to Menu", nullptr));
        graphPageTitle->setText(QCoreApplication::translate("MainMenu", "Graph Algorithms", nullptr));
        backButtonString->setText(QCoreApplication::translate("MainMenu", "\342\206\220 Back to Menu", nullptr));
        stringPageTitle->setText(QCoreApplication::translate("MainMenu", "String Algorithms", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainMenu: public Ui_MainMenu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINMENU_H
