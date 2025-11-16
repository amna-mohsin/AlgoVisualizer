/**
 * @file StackVisualizer.h
 * @brief Defines the StackVisualizer class, a QWidget for visualizing Stack operations (LIFO).
 *
 * This class handles the UI setup, user interaction (push, pop, clear),
 * and manages the state of the underlying stack data structure for visualization.
 */
#ifndef ALGOVISUALIZER_WIDGETS_STACKVISUALIZER_H
#define ALGOVISUALIZER_WIDGETS_STACKVISUALIZER_H

// --- Qt Includes ---
#include <QWidget>      // @brief Base class for all user interface objects.
#include <QMap>         // @brief Provides a dictionary (hash map) for key-value storage.
#include <QTimer>       // @brief Provides a high-level means of starting and stopping timers.
#include <queue>        // @brief Standard C++ container for FIFO data structure. (Included for general utility, although a stack is LIFO).
#include <functional>   // @brief For objects that act as functions (e.g., std::function).
#include <vector>       // @brief Standard C++ container for dynamic arrays, used for the stack data.
#include <QScrollArea>  // @brief Provides a scrolling viewport for other widgets.
#include <QPainter>     // @brief Used for performing custom, low-level painting on the visualization widget.

// --- Forward Declarations for UI Components ---
class QPushButton;  // @brief A clickable command button.
class QLineEdit;    // @brief A widget for single-line text input (used for bulk push).
class QSpinBox;     // @brief A widget for integer input (used for random size selection).
class QSlider;      // @brief A widget for controlling a value within a range (e.g., speed control).
class QLabel;       // @brief A widget that displays non-editable text or images.
class QButtonGroup; // @brief Used to organize groups of buttons.
class QHBoxLayout;  // @brief A layout manager that arranges widgets horizontally.

/**
 * @class StackVisualizer
 * @brief Widget responsible for visualizing the Stack data structure and its operations.
 *
 * Inherits QWidget to provide a custom UI panel for Push, Pop, and Clear operations,
 * and manages the internal state of the stack for dynamic drawing.
 */
class StackVisualizer : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for StackVisualizer.
     * @param parent The parent widget (defaulted to nullptr).
     */
    explicit StackVisualizer(QWidget *parent = nullptr);

signals:
    /**
     * @brief Signal emitted when the user requests to return to the main menu.
     */
    void requestBackToMenu();

private slots:
    /**
     * @brief Handles the operation to push a new element onto the stack.
     */
    void handlePush();

    /**
     * @brief Handles the operation to pop the top element from the stack.
     */
    void handlePop();

    /**
     * @brief Handles clearing all elements from the stack.
     */
    void handleClear();

    /**
     * @brief Handles generating and setting a random set of values into the stack.
     */
    void handleGenerateRandom();

    /**
     * @brief Handles applying user-typed input values to initialize the stack.
     */
    void handleApplyInput();

    /**
     * @brief Handles the click event for the back button to return to the menu.
     */
    void handleBackButton();

private:
    /**
     * @struct StructureInfo
     * @brief Static data structure to hold descriptive metadata about the Stack.
     */
    struct StructureInfo {
        QString title;       ///< The main title of the data structure (e.g., "Stack").
        QString subtitle;    ///< A short subtitle or definition (e.g., "Last-In, First-Out (LIFO)").
        QString description; ///< A brief explanation of its use cases.
    };

    /**
     * @brief Sets up and lays out all user interface elements (buttons, inputs, labels).
     */
    void setupUi();

    /**
     * @brief Updates the information panel with current statistics and descriptive text.
     */
    void updateInfoPanel();

    /**
     * @brief Triggers the repainting of the visualization widget and optionally updates the status.
     * @param message An optional message to display in the status bar.
     */
    void updateVisualization(const QString &message = QString());

    /**
     * @brief Updates the status bar/label with a non-transient message.
     * @param message The string message to display.
     */
    void setStatusMessage(const QString &message);

    /**
     * @brief Directly sets the internal stack data and updates the input field.
     * @param values The new vector of integers to set as the stack data.
     * @param updateInputField If true, updates the QLineEdit to reflect the new values.
     */
    void setStack(const std::vector<int> &values, bool updateInputField = true);

    /**
     * @brief Resets the internal stack and related UI state variables to their initial values.
     */
    void resetState();

    /**
     * @brief Overrides QWidget::showEvent to perform necessary setup when the widget becomes visible.
     * @param event The show event object.
     */
    void showEvent(QShowEvent *event) override;

    /**
     * @brief Parses the user input string from the QLineEdit into a vector of integers.
     * @param ok Pointer to a boolean that will be set to true on successful parsing, false otherwise.
     * @return A vector of parsed integer values.
     */
    std::vector<int> parseInputValues(bool *ok) const;

    /**
     * @brief Generates a vector of random integer values.
     * @param size The number of random values to generate.
     * @return A vector of random integers.
     */
    std::vector<int> generateRandomValues(int size) const;

    // --- UI Member Variables ---
    QWidget *m_visualizerWidget;            ///< @brief The main container for the visualization area.
    QLineEdit *m_inputField;                ///< @brief Input field for manual or bulk data entry.
    QSpinBox *m_sizeSpinBox;                ///< @brief Spin box to set the size for random generation.
    QLabel *m_statusLabel;                  ///< @brief Label used to display operation status messages.
    QLabel *m_structureTitleLabel;          ///< @brief Displays the structure's main title.
    QLabel *m_structureSubtitleLabel;       ///< @brief Displays the LIFO subtitle.
    QLabel *m_structureDescriptionLabel;    ///< @brief Displays the structure's description.
    QLabel *m_stackStatsLabel;              ///< @brief Displays runtime stats (e.g., current size, top element).
    QPushButton *m_pushButton;              ///< @brief Button to execute the Push operation.
    QPushButton *m_popButton;               ///< @brief Button to execute the Pop operation.
    QPushButton *m_clearButton;             ///< @brief Button to execute the Clear operation.

    // --- State Member Variables ---
    std::vector<int> m_stack;               ///< @brief The internal vector representing the stack data (LIFO).
    QString m_lastStatusMessage;            ///< @brief Stores the status message displayed before an operation.

    // --- Custom Visualization Widget ---
    class StackVizWidget;                   ///< @brief Forward declaration of the custom painting class.
    StackVizWidget *m_vizWidget;            ///< @brief Pointer to the custom visualization canvas.

    /**
     * @brief Static constant structure holding descriptive information about the Stack.
     */
    static const StructureInfo kStructureInfo;
};

#endif  // ALGOVISUALIZER_WIDGETS_STACKVISUALIZER_H
