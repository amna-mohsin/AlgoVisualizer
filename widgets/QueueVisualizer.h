#ifndef ALGOVISUALIZER_WIDGETS_QUEUEVISUALIZER_H
#define ALGOVISUALIZER_WIDGETS_QUEUEVISUALIZER_H

/**
 * @file QueueVisualizer.h
 * @brief Declares the QueueVisualizer class used to visualize Queue, Circular Queue, and Deque operations.
 *
 * Includes various Qt widgets for UI, STL containers for queue storage,
 * QPainter for drawing, and QScrollArea for visualization handling.
 */

// ----------- Qt Includes (What They Do) -------------------

#include <QWidget>      ///< Base class for all UI widgets.
#include <QMap>         ///< Used for mapping queue type information.
#include <QTimer>       ///< Used for animations or delays in queue visualization.
#include <QScrollArea>  ///< Provides scrolling container for visualization.
#include <QPainter>     ///< Used for custom drawing of queue boxes & arrows.

// ----------- STL Includes (What They Do) ------------------

#include <queue>        ///< Used for standard queue operations (enqueue/dequeue).
#include <vector>       ///< Stores queue elements in different modes.
#include <functional>   ///< Used for callbacks, lambdas when required.

// ----------- Forward Declarations (Qt Classes) ------------

class QPushButton;
class QLineEdit;
class QSpinBox;
class QLabel;
class QButtonGroup;
class QHBoxLayout;

/**
 * @class QueueVisualizer
 * @brief A QWidget-based class that provides interactive queue visualizations.
 *
 * Supports:
 * - Standard Queue
 * - Circular Queue
 * - Double-Ended Queue (Deque)
 *
 * Allows operations like enqueue, dequeue, random generation,
 * applying custom input, switching queue types, and clearing data.
 */
class QueueVisualizer : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for QueueVisualizer.
     * @param parent Parent widget.
     */
    explicit QueueVisualizer(QWidget *parent = nullptr);

signals:
    /**
     * @brief Emitted when user presses the Back button.
     */
    void requestBackToMenu();

private slots:
    /// Handles enqueue operation for current queue type.
    void handleEnqueue();

    /// Handles dequeue operation for current queue type.
    void handleDequeue();

    /// Clears all stored queue data.
    void handleClear();

    /// Generates a random queue based on sizeSpinBox.
    void handleGenerateRandom();

    /// Applies user-entered values from input field.
    void handleApplyInput();

    /// Navigates back to main menu.
    void handleBackButton();

    /**
     * @brief Handles queue type switching (Queue / CircularQueue / Deque).
     * @param id Button ID.
     */
    void handleQueueTypeButton(int id);

private:

    // =====================================================
    //                 ENUMS & INFO STRUCTS
    // =====================================================

    /**
     * @enum QueueType
     * @brief Represents the different queue modes supported.
     */
    enum class QueueType {
        Queue = 0,        ///< Standard FIFO Queue.
        CircularQueue,    ///< Circular buffer based queue.
        Deque             ///< Double-ended queue.
    };

    /**
     * @struct QueueTypeInfo
     * @brief Stores display information for each queue type.
     */
    struct QueueTypeInfo {
        QueueType type;       ///< Enum value.
        QString title;        ///< Display title.
        QString subtitle;     ///< Short description line.
        QString description;  ///< Full description.
    };

    // =====================================================
    //                       UI SETUP
    // =====================================================

    /// Sets up UI components and layouts.
    void setupUi();

    /// Creates queue type buttons (Queue / Circular / Deque).
    void setupQueueTypeButtons(QHBoxLayout *layout);

    /// Updates current queue type and reloads saved state.
    void updateQueueType(QueueType type);

    /// Updates right-side information panel.
    void updateInfoPanel();

    /// Updates visual representation of the queue.
    void updateVisualization(const QString &message = QString());

    /// Sets status message visible at bottom.
    void setStatusMessage(const QString &message);

    /// Sets queue contents manually.
    void setQueue(const std::vector<int> &values, bool updateInputField = true);

    /// Resets all queue data.
    void resetState();

    /// Clears queue data depending on type.
    void resetQueueData();

    /// Saves current queue state when switching types.
    void saveCurrentQueueState();

    /// Loads previously saved queue state.
    void loadQueueState(QueueType type);

    /// When the widget appears, refresh visualization.
    void showEvent(QShowEvent *event) override;

    // =====================================================
    //                  INPUT & GENERATION
    // =====================================================

    /// Parses comma-separated values from input field.
    std::vector<int> parseInputValues(bool *ok) const;

    /// Generates a vector of random integers.
    std::vector<int> generateRandomValues(int size) const;

    // =====================================================
    //                   QUEUE OPERATIONS
    // =====================================================

    void enqueueQueue(int value);        ///< Standard queue enqueue.
    int  dequeueQueue();                 ///< Standard queue dequeue.

    void enqueueCircularQueue(int value);///< Circular queue enqueue.
    int  dequeueCircularQueue();         ///< Circular queue dequeue.

    void enqueueDequeFront(int value);   ///< Enqueue at front of deque.
    void enqueueDequeRear(int value);    ///< Enqueue at rear of deque.

    int  dequeueDequeFront();            ///< Remove from front of deque.
    int  dequeueDequeRear();             ///< Remove from rear of deque.

    // =====================================================
    //                         UI FIELDS
    // =====================================================

    QWidget *m_visualizerWidget;            ///< Scrollable area widget.
    QButtonGroup *m_queueTypeButtons;       ///< Button group for queue types.
    QLineEdit *m_inputField;                ///< User input for queue values.
    QSpinBox *m_sizeSpinBox;                ///< For random generation size.
    QLabel *m_statusLabel;                  ///< Status / operation messages.
    QLabel *m_queueTypeTitleLabel;          ///< Displays queue type name.
    QLabel *m_queueTypeSubtitleLabel;       ///< Displays short label.
    QLabel *m_queueTypeDescriptionLabel;    ///< Displays descriptive info.
    QLabel *m_queueStatsLabel;              ///< Shows queue size & status.

    QPushButton *m_enqueueButton;           ///< Enqueue button.
    QPushButton *m_dequeueButton;           ///< Dequeue button.
    QPushButton *m_enqueueFrontButton;      ///< For deque front enqueue.
    QPushButton *m_dequeueFrontButton;      ///< For deque front dequeue.
    QPushButton *m_clearButton;             ///< Clear queue.

    // =====================================================
    //                       STATE DATA
    // =====================================================

    QueueType m_currentQueueType;           ///< Active queue mode.

    std::vector<int> m_queue;               ///< Queue data.
    int m_front;                            ///< Front index (circular/normal).
    int m_rear;                             ///< Rear index.
    int m_capacity;                         ///< Queue capacity.
    QString m_lastStatusMessage;            ///< Last shown message.

    // Saved states for switching
    std::vector<int> m_queueData;
    std::vector<int> m_circularQueueData;
    std::vector<int> m_dequeData;

    int m_queueFront, m_queueRear;
    int m_circularFront, m_circularRear;
    int m_dequeFront, m_dequeRear;

    /**
     * @class QueueVizWidget
     * @brief Custom drawing widget for visualizing queue elements.
     */
    class QueueVizWidget;
    QueueVizWidget *m_vizWidget;

    /// Static info list for descriptions of queue types.
    static const std::vector<QueueTypeInfo> kQueueTypeInfos;
};

#endif  // ALGOVISUALIZER_WIDGETS_QUEUEVISUALIZER_H
