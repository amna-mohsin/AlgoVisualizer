#ifndef ALGOVISUALIZER_WIDGETS_SORTING_SEARCHING_H
#define ALGOVISUALIZER_WIDGETS_SORTING_SEARCHING_H

/**
 * @file sorting_searching.h
 * @brief Declares the SortingVisualizer class used for visualizing sorting and searching algorithms.
 *
 * This file manages UI setup, animations, sorting step queues, state management,
 * and communicates with ArrayVisualizer for drawing.
 */

// --------------------- Qt Includes (What They Do) -----------------------

#include <QWidget>        ///< Base class for all Qt UI widgets.
#include <QMap>           ///< Used to map algorithm information internally.
#include <QTimer>         ///< Used for autoplay sorting animation.
#include <QScrollArea>    ///< Provides scrolling container for visualizer.
#include <QLineEdit>      ///< Allows user to input custom array values.
#include <QSpinBox>       ///< Used for size selection in random generation.
#include <QSlider>        ///< Used for animation speed control.
#include <QLabel>         ///< Displays algorithm info, statistics, and status.
#include <QButtonGroup>   ///< Groups algorithm buttons for easy selection.
#include <QHBoxLayout>    ///< Used to layout sorting algorithm buttons.

#include <queue>          ///< Used to queue sorting steps (operations).
#include <functional>     ///< Stores lambdas representing step actions.
#include <vector>         ///< Stores the array and states used in visualization.

#include "ArrayVisualizer.h" ///< Custom bar-graph widget for drawing the array.

// ----------------------- Forward Declarations ---------------------------

class QPushButton;

/**
 * @class SortingVisualizer
 * @brief Interactive visualizer for sorting and searching algorithms
 * 
 * @details
 * This widget provides a comprehensive visualization system for:
 * - Search algorithms: Linear Search and Binary Search
 * - Sorting algorithms: Bubble Sort, Selection Sort, Insertion Sort, Merge Sort, and Quick Sort
 * 
 * Features:
 * - Step-by-step execution with visual highlighting
 * - Auto-play mode with adjustable speed control
 * - Random array generation and custom input support
 * - Real-time algorithm information and statistics
 * - Color-coded visualization (red=unsorted, yellow=comparing, green=sorted, purple=pivot)
 * 
 * @note
 * Uses a command queue pattern where algorithm steps are pre-computed as lambda functions.
 * This allows for replay, step-by-step execution, and smooth animations.
 */
class SortingVisualizer : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor that initializes UI and sorting state.
     * @param parent The parent widget.
     */
    explicit SortingVisualizer(QWidget *parent = nullptr);

signals:
    /**
     * @brief Emitted when user presses the Back button in the UI.
     */
    void requestBackToMenu();

private slots:
    /// Starts autoplay sorting.
    void handlePlay();

    /// Executes a single sorting step.
    void handleStep();

    /// Resets the sorting visualizer.
    void handleReset();

    /// Continues auto-advancing sorting steps using timer.
    void handleAutoAdvance();

    /// Generates random array of selected size.
    void handleGenerateRandom();

    /// Applies user-entered custom array input.
    void handleApplyInput();

    /// Updates animation speed when slider changes.
    void handleSpeedChanged(int value);

    /// Handles algorithm type button press.
    void handleAlgorithmButton(int id);

    /// Navigates back to main menu.
    void handleBackButton();

private:

    // ---------------------- ENUMS & INFO STRUCTS -------------------------

    /**
     * @enum Algorithm
     * @brief Supported sorting and searching algorithms.
     */
    enum class Algorithm {
        LinearSearch = 0, ///< Linear Search
        BinarySearch,     ///< Binary Search
        Bubble,           ///< Bubble Sort
        Selection,        ///< Selection Sort
        Insertion,        ///< Insertion Sort
        Merge,            ///< Merge Sort
        Quick             ///< Quick Sort
    };

    /**
     * @struct AlgorithmInfo
     * @brief Human-readable description for each algorithm.
     */
    struct AlgorithmInfo {
        Algorithm type;       ///< Algorithm enum value.
        QString title;        ///< Display title.
        QString subtitle;     ///< One-line summary.
        QString description;  ///< Full description text.
    };

    // --------------------- UI INITIALIZATION -----------------------------

    /// Creates all UI components and layout.
    void setupUi();

    /// Creates algorithm selector buttons.
    void setupAlgorithmButtons(QHBoxLayout *layout);

    /// Switches currently selected algorithm.
    void updateAlgorithm(Algorithm algorithm);

    /// Updates info panel showing algorithm details.
    void updateInfoPanel();

    /// Enables/disables play button depending on state.
    void updatePlayButtonState();

    /// Redraws array visualization with message.
    void updateVisualization(const QString &message = QString());

    /// Displays status text under controls.
    void setStatusMessage(const QString &message);

    /// Sets new array (random or user input).
    void setArray(const std::vector<int> &values, bool updateInputField = true);

    /// Resets sorting state; hardReset clears original array too.
    void resetState(bool hardReset = false);

    /// Prepares step queue for selected algorithm.
    bool prepareAlgorithm();

    /// Executes next sorting operation from queue.
    bool executeNextStep();

    /// Called when sorting is fully completed.
    void finalizeRun();

    // -------------------- VISUAL STATE HELPERS ---------------------------

    /// Clears highlight states like comparing/swapping.
    void clearTransientStates();

    /// Marks specific bars as sorted.
    void markSortedIndices(const std::vector<int> &indices);

    /// Marks entire array sorted.
    void markAllSorted();

    /// Applies specific BarState to multiple indices.
    void setStatesForIndices(const std::vector<int> &indices,
                             ArrayVisualizer::BarState state);

    // ------------------ SEARCH & SORTING ENQUEUE FUNCTIONS ------------------------

    /// Enqueue Linear Search steps.
    void enqueueLinearSearch(std::vector<int> arr, int target);

    /// Enqueue Binary Search steps.
    void enqueueBinarySearch(std::vector<int> arr, int target);

    /// Enqueue Bubble Sort steps.
    void enqueueBubbleSort(std::vector<int> arr);

    /// Enqueue Selection Sort steps.
    void enqueueSelectionSort(std::vector<int> arr);

    /// Enqueue Insertion Sort steps.
    void enqueueInsertionSort(std::vector<int> arr);

    /// Enqueue Merge Sort steps.
    void enqueueMergeSort(std::vector<int> arr);

    /// Recursive helper for Merge Sort.
    void enqueueMergeSortRecursive(std::vector<int> &arr, int left, int right);

    /// Enqueue Quick Sort steps.
    void enqueueQuickSort(std::vector<int> arr);

    /// Recursive helper for Quick Sort.
    void enqueueQuickSortRecursive(std::vector<int> &arr, int low, int high);

    // -------------------- INPUT & RANDOM HELPERS -------------------------

    /// Parses user array input like: 3, 4, 1, 9
    std::vector<int> parseInputValues(bool *ok) const;

    /// Generates N random values from sizeSpinBox.
    std::vector<int> generateRandomValues(int size) const;

    /// Returns animation interval based on speed slider.
    int currentInterval() const;

    // ----------------------------- UI MEMBERS ----------------------------

    ArrayVisualizer *m_visualizer;               ///< Draws the bar array.
    QButtonGroup *m_algorithmButtons;            ///< Algorithm selector.
    QLineEdit *m_inputField;                     ///< User custom input.
    QSpinBox *m_sizeSpinBox;                     ///< Random array size.
    QSpinBox *m_targetSpinBox;                   ///< Target value for search algorithms.
    QHBoxLayout *m_targetLayout;                 ///< Layout containing target input (for show/hide).
    QWidget *m_targetWidget;                      ///< Widget containing target input (for show/hide).
    QSlider *m_speedSlider;                      ///< Speed control slider.
    QLabel *m_speedValueLabel;                   ///< Shows numerical speed.
    QLabel *m_stepLabel;                         ///< Shows current step count.
    QLabel *m_statusLabel;                       ///< Shows status text.
    QLabel *m_algorithmTitleLabel;               ///< Algorithm name.
    QLabel *m_algorithmSubtitleLabel;            ///< Short description.
    QLabel *m_algorithmDescriptionLabel;         ///< Long description.
    QLabel *m_arrayStatsLabel;                   ///< Shows array stats.
    QPushButton *m_playButton;                   ///< Play/start button.
    QPushButton *m_stepButton;                   ///< Step-by-step button.
    QPushButton *m_resetButton;                  ///< Reset button.

    // --------------------------- STATE MEMBERS ---------------------------

    Algorithm m_currentAlgorithm;                ///< Active algorithm.
    std::vector<int> m_originalArray;            ///< Original array.
    std::vector<int> m_currentArray;             ///< Working array.
    std::vector<ArrayVisualizer::BarState> m_barStates; ///< Visual states.
    std::vector<bool> m_sortedFlags;             ///< Sorted index flags.
    std::queue<std::function<void()>> m_stepQueue; ///< Steps of algorithm.
    int m_currentStep;                           ///< Step counter.
    bool m_isPrepared;                           ///< Whether queue built.
    bool m_isFinished;                           ///< Whether algorithm ended.
    bool m_isAutoPlaying;                        ///< Whether auto mode active.
    QTimer *m_autoTimer;                         ///< Timer for autoplay.
    QString m_lastStatusMessage;                 ///< Stored status text.
    int m_searchTarget;                          ///< Target value for search algorithms.

    /// Static list of algorithm info for UI.
    static const std::vector<AlgorithmInfo> kAlgorithmInfos;
};

#endif  // ALGOVISUALIZER_WIDGETS_SORTING_SEARCHING_H
