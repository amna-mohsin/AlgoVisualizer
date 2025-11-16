/**
 * @file main.cpp
 * @brief Application entry point for AlgoVisualizer
 * 
 * This is the main function that initializes the Qt application and displays
 * the start screen. It follows the standard Qt application structure.
 */

#include <QApplication>
#include "StartScreen.h"

/**
 * @brief Main function - Entry point of the application
 * @param argc Number of command-line arguments
 * @param argv Array of command-line argument strings
 * @return Exit code (0 on success)
 * 
 * Creates a QApplication instance, initializes the StartScreen widget,
 * displays it, and starts the event loop. The application runs until
 * the user closes all windows.
 */
int main(int argc, char *argv[])
{
    // Create Qt application instance - required for any Qt GUI application
    QApplication a(argc, argv);
    
    // Create and show the start screen (welcome window)
    StartScreen w;
    w.show();
    
    // Start the event loop - processes events until application exits
    return a.exec();
}
