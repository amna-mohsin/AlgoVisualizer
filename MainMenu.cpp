/**
 * @file MainMenu.cpp
 * @brief Implementation of the main menu with navigation to all visualizers
 * 
 * This class manages the main menu interface where users can select
 * which data structure or algorithm they want to visualize. It uses
 * a QStackedWidget to switch between different pages.
 */

#include "MainMenu.h"
#include "ui_MainMenu.h"

// --------------------- Qt Core Includes -------------------------
#include <QApplication>      ///< Main application object for Qt event loop
#include <QScreen>           ///< Screen information for window centering
#include <QStackedWidget>     ///< Stacked widget for page management

// --------------------- Visualizer Widget Includes ---------------
#include "widgets/sorting_searching.h"    ///< Sorting and searching algorithm visualizer
#include "widgets/StackVisualizer.h"     ///< Stack data structure visualizer
#include "widgets/QueueVisualizer.h"      ///< Queue data structure visualizer
#include "widgets/LinkedListVisualizer.h" ///< Linked list visualizer
#include "widgets/TreeVisualizer.h"      ///< Tree traversal visualizer
#include "widgets/GraphVisualizer.h"     ///< Graph algorithm visualizer
#include "widgets/StringPatternVisualizer.h" ///< String pattern matching visualizer

/**
 * @brief Constructor - Initializes the main menu and all visualizers
 * @param parent Parent widget (nullptr for top-level window)
 * 
 * Sets up the UI, creates all visualizer widgets, connects signals/slots
 * for navigation, and centers the window. Uses QStackedWidget for page
 * management - each visualizer is on a separate page.
 */
MainMenu::MainMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainMenu),
    m_sortingVisualizer(nullptr),
    m_stackVisualizer(nullptr),
    m_queueVisualizer(nullptr),
    m_linkedListVisualizer(nullptr),
    m_treeVisualizer(nullptr),
    m_graphVisualizer(nullptr),
    m_stringVisualizer(nullptr)
{
    // Initialize UI from .ui file
    ui->setupUi(this);
    this->resize(1280, 720);
    this->setMinimumSize(1120, 680);
    
    // Set initial page to menu (index 0)
    ui->stackedWidget->setCurrentIndex(0);
    
    // Configure grid layout for equal column widths
    ui->cardsGridLayout->setColumnStretch(0, 1);
    ui->cardsGridLayout->setColumnStretch(1, 1);
    ui->cardsGridLayout->setColumnStretch(2, 1);
    
    // Create and configure Sorting Visualizer
    m_sortingVisualizer = new SortingVisualizer(ui->sortingContentContainer);
    ui->sortingContentLayout->addWidget(m_sortingVisualizer);
    // Connect signal: when visualizer requests to go back, show menu
    connect(m_sortingVisualizer, &SortingVisualizer::requestBackToMenu, this, &MainMenu::showMenuPage);
    
    // Create and configure Stack Visualizer
    m_stackVisualizer = new StackVisualizer(ui->stackContentContainer);
    ui->stackContentLayout->addWidget(m_stackVisualizer);
    connect(m_stackVisualizer, &StackVisualizer::requestBackToMenu, this, &MainMenu::showMenuPage);
    
    // Create and configure Queue Visualizer
    m_queueVisualizer = new QueueVisualizer(ui->queueContentContainer);
    ui->queueContentLayout->addWidget(m_queueVisualizer);
    connect(m_queueVisualizer, &QueueVisualizer::requestBackToMenu, this, &MainMenu::showMenuPage);
    
    // Create and configure Linked List Visualizer
    m_linkedListVisualizer = new LinkedListVisualizer(ui->linkedListContentContainer);
    ui->linkedListContentLayout->addWidget(m_linkedListVisualizer);
    connect(m_linkedListVisualizer, &LinkedListVisualizer::requestBackToMenu, this, &MainMenu::showMenuPage);
    
    // Create and configure Tree Visualizer
    m_treeVisualizer = new TreeVisualizer(ui->treeContentContainer);
    ui->treeContentLayout->addWidget(m_treeVisualizer);
    connect(m_treeVisualizer, &TreeVisualizer::requestBackToMenu, this, &MainMenu::showMenuPage);
    
    // Create and configure Graph Visualizer
    m_graphVisualizer = new GraphVisualizer(ui->graphContentContainer);
    ui->graphContentLayout->addWidget(m_graphVisualizer);
    connect(m_graphVisualizer, &GraphVisualizer::requestBackToMenu, this, &MainMenu::showMenuPage);
    
    // Create and configure String Pattern Visualizer
    m_stringVisualizer = new StringPatternVisualizer(ui->stringContentContainer);
    ui->stringContentLayout->addWidget(m_stringVisualizer);
    connect(m_stringVisualizer, &StringPatternVisualizer::requestBackToMenu, this, &MainMenu::showMenuPage);
    
    // Center the window on screen
    if (parent == nullptr) {
        QScreen *screen = QApplication::primaryScreen();
        if (screen) {
            QRect screenGeometry = screen->geometry();
            int x = (screenGeometry.width() - this->width()) / 2;
            int y = (screenGeometry.height() - this->height()) / 2;
            this->move(x, y);
        }
    }
}

/**
 * @brief Destructor - Cleans up UI resources
 * 
 * Note: Child widgets (visualizers) are automatically deleted by Qt's
 * parent-child system, so we only need to delete the UI object.
 */
MainMenu::~MainMenu()
{
    delete ui;
}

/**
 * @brief Shows the main menu page
 * 
 * Switches the QStackedWidget to index 0, which displays the menu
 * with all data structure cards. Called when user clicks back button
 * from any visualizer.
 */
void MainMenu::showMenuPage()
{
    ui->stackedWidget->setCurrentIndex(0);
}

// ========== Card Click Handlers - Navigate to Respective Pages ==========

/**
 * @brief Handles click on Sorting card
 * 
 * Switches to the sorting visualizer page (index 1) where users can
 * visualize various sorting algorithms like Bubble Sort, Quick Sort, etc.
 */
void MainMenu::on_sortingCard_clicked()
{
    ui->stackedWidget->setCurrentIndex(1); // sortingPage
}

/**
 * @brief Handles click on Stack card
 * 
 * Switches to the stack visualizer page (index 2) where users can
 * visualize stack operations (push, pop, clear).
 */
void MainMenu::on_stackCard_clicked()
{
    ui->stackedWidget->setCurrentIndex(2); // stackPage
}

/**
 * @brief Handles click on Queue card
 * 
 * Switches to the queue visualizer page (index 3) where users can
 * visualize queue operations (enqueue, dequeue) for different queue types.
 */
void MainMenu::on_queueCard_clicked()
{
    ui->stackedWidget->setCurrentIndex(3); // queuePage
}

/**
 * @brief Handles click on Linked List card
 * 
 * Switches to the linked list visualizer page (index 4) where users can
 * visualize linked list operations (insert, delete) for different list types.
 */
void MainMenu::on_linkedListCard_clicked()
{
    ui->stackedWidget->setCurrentIndex(4); // linkedListPage
}

/**
 * @brief Handles click on Tree card
 * 
 * Switches to the tree visualizer page (index 5) where users can
 * visualize binary tree traversals (inorder, preorder, postorder, BFS).
 */
void MainMenu::on_treeCard_clicked()
{
    ui->stackedWidget->setCurrentIndex(5); // treePage
}

/**
 * @brief Handles click on Graph card
 * 
 * Switches to the graph visualizer page (index 6) where users can
 * visualize graph algorithms (BFS, DFS, Dijkstra's shortest path).
 */
void MainMenu::on_graphCard_clicked()
{
    ui->stackedWidget->setCurrentIndex(6); // graphPage
}

/**
 * @brief Handles click on String Pattern card
 * 
 * Switches to the string pattern visualizer page (index 7) where users can
 * visualize string pattern matching algorithms (Naive, KMP).
 */
void MainMenu::on_stringCard_clicked()
{
    ui->stackedWidget->setCurrentIndex(7); // stringPage
}

// ========== Back Button Handlers - Return to Menu ==========

/**
 * @brief Handles back button click from Sorting page
 * 
 * Returns user to the main menu by calling showMenuPage().
 */
void MainMenu::on_backButtonSorting_clicked()
{
    showMenuPage();
}

/**
 * @brief Handles back button click from Stack page
 */
void MainMenu::on_backButtonStack_clicked()
{
    showMenuPage();
}

/**
 * @brief Handles back button click from Queue page
 */
void MainMenu::on_backButtonQueue_clicked()
{
    showMenuPage();
}

/**
 * @brief Handles back button click from Linked List page
 */
void MainMenu::on_backButtonLinkedList_clicked()
{
    showMenuPage();
}

/**
 * @brief Handles back button click from Tree page
 */
void MainMenu::on_backButtonTree_clicked()
{
    showMenuPage();
}

/**
 * @brief Handles back button click from Graph page
 */
void MainMenu::on_backButtonGraph_clicked()
{
    showMenuPage();
}

/**
 * @brief Handles back button click from String Pattern page
 */
void MainMenu::on_backButtonString_clicked()
{
    showMenuPage();
}
