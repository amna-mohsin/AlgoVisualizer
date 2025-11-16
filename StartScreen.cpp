/**
 * @file StartScreen.cpp
 * @brief Implementation of the welcome/start screen
 * 
 * This is the first window users see when launching the application.
 * It provides a welcome screen with a button to proceed to the main menu.
 */

#include "StartScreen.h"
#include "ui_StartScreen.h"
#include "MainMenu.h"

// --------------------- Qt Core Includes -------------------------
#include <QApplication>      ///< Main application object for Qt event loop
#include <QScreen>           ///< Screen information for window centering

/**
 * @brief Constructor - Initializes the start screen
 * @param parent Parent widget (nullptr for top-level window)
 * 
 * Sets up the UI, configures window size, and centers the window
 * on the screen. This is the welcome screen shown at application startup.
 */
StartScreen::StartScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartScreen)
{
    // Initialize UI components from the .ui file
    ui->setupUi(this);
    
    // Set window size and minimum size for proper display
    this->resize(1280, 720);
    this->setMinimumSize(1120, 680);
    
    // Center the window on screen for better user experience
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
 * Deletes the UI object to free allocated memory.
 */
StartScreen::~StartScreen()
{
    delete ui;
}

/**
 * @brief Slot function - Handles start button click event
 * 
 * When the user clicks the "Start" button, this function:
 * 1. Creates a new MainMenu window
 * 2. Displays the MainMenu
 * 3. Closes the current StartScreen
 * 
 * This implements navigation from welcome screen to main menu.
 */
void StartScreen::on_startButton_clicked()
{
    // Create and show the main menu window
    MainMenu *menu = new MainMenu();
    menu->show();
    
    // Close the start screen
    this->close();
}
