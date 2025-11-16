## @file AlgoVisualizer.pro
## @brief Qt project file for the AlgoVisualizer application.
##        Contains configuration, source files, headers, and UI forms.
## @details
## This project implements multiple data-structure and algorithm visualizers
## including arrays, sorting, stacks, queues, linked lists, trees,
## graphs, and string-pattern matching modules.
## The project uses Qt Widgets (GUI) framework.

# -----------------------------
# Qt Modules Required
# -----------------------------

QT += core gui widgets       ## Core Qt functionality + GUI support
QT += widgets                ## Additional widgets module

# -----------------------------
# Target / Output Application
# -----------------------------

TARGET = AlgoVisualizer       ## Name of the final executable
TEMPLATE = app                ## This is a GUI application

# -----------------------------
# Source Files
# -----------------------------

SOURCES += \
    main.cpp \                                   ## Application entry point
    StartScreen.cpp \                            ## Welcome screen logic
    MainMenu.cpp \                               ## Menu for selecting visualizers
    widgets/ArrayVisualizer.cpp \                ## Array visualizer implementation
    widgets/sorting_searching.cpp \              ## Sorting and searching algorithm visualizer
    widgets/StackVisualizer.cpp \                ## Stack operations visualizer
    widgets/QueueVisualizer.cpp \                ## Queue operations visualizer
    widgets/LinkedListVisualizer.cpp \           ## Linked list visualizer
    widgets/TreeVisualizer.cpp \                 ## Tree traversal / BST visualizer
    widgets/GraphVisualizer.cpp \                ## Graph algorithms visualizer
    widgets/StringPatternVisualizer.cpp          ## Pattern matching visualizer

# -----------------------------
# Header Files
# -----------------------------

HEADERS += \
    StartScreen.h \                               ## Start screen class
    MainMenu.h \                                  ## Main menu class
    widgets/ArrayVisualizer.h \
    widgets/sorting_searching.h \
    widgets/StackVisualizer.h \
    widgets/QueueVisualizer.h \
    widgets/LinkedListVisualizer.h \
    widgets/TreeVisualizer.h \
    widgets/GraphVisualizer.h \
    widgets/StringPatternVisualizer.h

# -----------------------------
# UI Files (Qt Designer)
# -----------------------------

FORMS += \
    StartScreen.ui \                              ## UI layout for welcome screen
    MainMenu.ui                                   ## UI layout for main menu
