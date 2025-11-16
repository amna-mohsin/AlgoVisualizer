/**
 * @file StringPatternVisualizer.h
 * @brief Defines the StringPatternVisualizer class, a QWidget for visualizing
 * string matching algorithms (e.g., Naive, KMP).
 *
 * This class provides the UI for selecting an algorithm, inputting text and pattern,
 * and managing the visualization state for step-by-step execution.
 */
#ifndef ALGOVISUALIZER_WIDGETS_STRINGPATTERNVISUALIZER_H
#define ALGOVISUALIZER_WIDGETS_STRINGPATTERNVISUALIZER_H

// --- Qt Includes ---
#include <QWidget>      // @brief Base class for all user interface objects.
#include <QMap>         // @brief Provides a dictionary (hash map) for key-value storage.
#include <QTimer>       // @brief Provides a high-level means of starting and stopping timers for animation control.
#include <vector>       // @brief Standard C++ container for dynamic arrays.
#include <string>       // @brief Standard C++ string class for text and pattern data.
#include <QScrollArea>  // @brief Provides a scrolling viewport.
#include <QPainter>     // @brief Used for performing custom drawing on the visualization widget.

// --- Forward Declarations for UI Components ---
class QPushButton;  // @brief A clickable command button.
class QLineEdit;    // @brief A widget for single-line text input (for text and pattern).
class QSpinBox;     // @brief A widget for integer input.
class QLabel;       // @brief A widget that displays non-editable text or images.
class QButtonGroup; // @brief Used to group algorithm selection buttons.
class QHBoxLayout;  // @brief A layout manager that arranges widgets horizontally.

/**
 * @class StringPatternVisualizer
 * @brief Widget responsible for visualizing string pattern matching algorithms.
 *
 * It manages the user interface, algorithm state, and provides methods to perform
 * and visualize Naive and KMP search algorithms.
 */
class StringPatternVisualizer : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for StringPatternVisualizer.
     * @param parent The parent widget (defaulted to nullptr).
     */
    explicit StringPatternVisualizer(QWidget *parent = nullptr);

signals:
    /**
     * @brief Signal emitted when the user requests to return to the main menu.
     */
    void requestBackToMenu();

private slots:
    /**
     * @brief Handles the operation to start the string search visualization.
     */
    void handleSearch();

    /**
     * @brief Handles clearing the search results and resetting the visualization state.
     */
    void handleClear();

    /**
     * @brief Handles loading and setting a random text/pattern example.
     */
    void handleRandomExample();

    /**
     * @brief Handles the click event for the back button.
     */
    void handleBackButton();

    /**
     * @brief Handles the selection of a new algorithm via a button click.
     * @param id The ID corresponding to the selected algorithm (e.g., Naive or KMP).
     */
    void handleAlgorithmButton(int id);

private:
    /**
     * @enum Algorithm
     * @brief Enumerates the different string matching algorithms available.
     */
    enum class Algorithm {
        Naive = 0, ///< The simple, brute-force search algorithm.
        KMP        ///< Knuth-Morris-Pratt algorithm.
    };

    /**
     * @struct AlgorithmInfo
     * @brief Static data structure to hold descriptive metadata about each algorithm.
     */
    struct AlgorithmInfo {
        Algorithm type;         ///< The enumeration value of the algorithm.
        QString title;          ///< The name of the algorithm (e.g., "KMP").
        QString subtitle;       ///< A short descriptor (e.g., "Linear Time Search").
        QString description;    ///< A brief explanation of how the algorithm works.
    };

    /**
     * @brief Sets up and lays out all user interface elements.
     */
    void setupUi();

    /**
     * @brief Creates and connects the algorithm selection buttons.
     * @param layout The layout manager to add the buttons to.
     */
    void setupAlgorithmButtons(QHBoxLayout *layout);

    /**
     * @brief Updates the current algorithm, saving the old state and loading the new one.
     * @param algorithm The newly selected algorithm.
     */
    void updateAlgorithm(Algorithm algorithm);

    /**
     * @brief Updates the information panel with the current algorithm's details.
     */
    void updateInfoPanel();

    /**
     * @brief Triggers the repainting of the custom visualization widget.
     */
    void updateVisualization();

    /**
     * @brief Updates the status bar/label with a non-transient message.
     * @param message The string message to display.
     */
    void setStatusMessage(const QString &message);

    /**
     * @brief Saves the current text and pattern input to the specific storage variables
     * associated with the currently active algorithm.
     */
    void saveCurrentState();

    /**
     * @brief Loads the saved text and pattern for the specified algorithm into the input fields.
     * @param algorithm The algorithm whose state should be loaded.
     */
    void loadAlgorithmState(Algorithm algorithm);

    /**
     * @brief Overrides QWidget::showEvent to perform necessary setup when the widget becomes visible.
     * @param event The show event object.
     */
    void showEvent(QShowEvent *event) override;

    /**
     * @brief Implements and visualizes the Naive string search algorithm step-by-step.
     */
    void performNaiveSearch();

    /**
     * @brief Implements and visualizes the KMP string search algorithm step-by-step.
     */
    void performKMPSearch();

    /**
     * @brief Helper function for KMP: Computes the Longest Proper Prefix suffix (LPS) array.
     * @param pattern The string pattern to analyze.
     * @return A vector of integers representing the LPS array.
     */
    std::vector<int> computeLPS(const std::string& pattern);

    // --- UI Member Variables ---
    QWidget *m_visualizerWidget;            ///< @brief Main container for the drawing area.
    QButtonGroup *m_algorithmButtons;       ///< @brief Grouping for Naive/KMP selection buttons.
    QLineEdit *m_textInput;                 ///< @brief Input field for the main text string (T).
    QLineEdit *m_patternInput;              ///< @brief Input field for the search pattern string (P).
    QLabel *m_statusLabel;                  ///< @brief Label used to display operation status messages.
    QLabel *m_algorithmTitleLabel;          ///< @brief Displays the current algorithm's title.
    QLabel *m_algorithmSubtitleLabel;       ///< @brief Displays the algorithm's subtitle.
    QLabel *m_algorithmDescriptionLabel;    ///< @brief Displays the algorithm's detailed description.
    QPushButton *m_searchButton;            ///< @brief Button to start the search process.
    QPushButton *m_clearButton;             ///< @brief Button to clear results and reset inputs.
    QPushButton *m_randomButton;            ///< @brief Button to load a random example.

    // --- State Member Variables ---
    Algorithm m_currentAlgorithm;           ///< @brief The algorithm currently selected and being executed.
    std::string m_text;                     ///< @brief The current text string (T) being processed.
    std::string m_pattern;                  ///< @brief The current pattern string (P) being processed.
    std::vector<int> m_matches;             ///< @brief Stores the starting indices of all found matches.
    std::vector<int> m_highlightIndices;    ///< @brief Indices in T and P currently being compared/inspected.
    int m_currentTextPos;                   ///< @brief Current index in the text (T) being examined.
    int m_currentPatternPos;                ///< @brief Current index in the pattern (P) being examined.
    QString m_lastStatusMessage;            ///< @brief Stores the status message displayed before an operation.

    // --- Separate Storage for Algorithm Inputs ---
    // @brief Stores the last used text/pattern for each algorithm, allowing users to switch back without losing input.
    QString m_naiveText;
    QString m_naivePattern;
    QString m_kmpText;
    QString m_kmpPattern;

    // --- Custom Visualization Widget ---
    class StringVizWidget;                  ///< @brief Forward declaration of the custom painting class.
    StringVizWidget *m_vizWidget;           ///< @brief Pointer to the custom visualization canvas.

    /**
     * @brief Static constant vector holding the descriptive information for all algorithms.
     */
    static const std::vector<AlgorithmInfo> kAlgorithmInfos;
};

#endif  // ALGOVISUALIZER_WIDGETS_STRINGPATTERNVISUALIZER_H
