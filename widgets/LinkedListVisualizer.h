#ifndef ALGOVISUALIZER_WIDGETS_LINKEDLISTVISUALIZER_H
#define ALGOVISUALIZER_WIDGETS_LINKEDLISTVISUALIZER_H

/**
 * @file LinkedListVisualizer.h
 * @brief Declares the LinkedListVisualizer class used for visualizing linked list operations.
 *
 * @details
 * This widget supports:
 *  - Singly Linked List
 *  - Doubly Linked List
 *  - Circular Linked List
 *
 * Features:
 *  - Insert (begin, end, position)
 *  - Delete (begin, end, position)
 *  - Clear list
 *  - Generate random values
 *  - Switch linked list type while saving state
 *  - Custom UI and custom painter-based visualization
 */

//
// ------------------ Qt and Standard Includes ------------------
//

#include <QWidget>        ///< Base class for all UI components.
#include <QMap>           ///< General-purpose Qt map container (minimal use).
#include <QTimer>         ///< Timer for scheduled animations/delays.
#include <queue>          ///< std::queue used internally if needed.
#include <functional>     ///< std::function support for flexible callbacks.
#include <vector>         ///< Standard C++ vector used for storing input values.
#include <QScrollArea>    ///< Enables scrollable visualization area.
#include <QPainter>       ///< Used for drawing linked list nodes/arrows.

//
// ------------------ Forward Declarations ------------------
// These reduce compile time since full header includes are unnecessary.
//

class QPushButton;       ///< UI button for operations.
class QLineEdit;         ///< Input field for custom values.
class QSpinBox;          ///< Number input for position, size, etc.
class QLabel;            ///< Text display for messages and titles.
class QButtonGroup;      ///< Groups linked list type selection buttons.
class QHBoxLayout;       ///< Horizontal layout for button arrangement.

/**
 * @class LinkedListVisualizer
 * @brief Widget responsible for visualizing and animating Linked List operations.
 */
class LinkedListVisualizer : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Creates a LinkedListVisualizer widget.
     * @param parent Optional parent QWidget.
     */
    explicit LinkedListVisualizer(QWidget *parent = nullptr);

signals:
    /**
     * @brief Emitted when the user presses the "Back" button to return to menu.
     */
    void requestBackToMenu();

private slots:
    // Insertion operations
    void handleInsertBeginning();     ///< Insert node at beginning.
    void handleInsertEnd();           ///< Insert node at end.
    void handleInsertPosition();      ///< Insert node at specific index.

    // Deletion operations
    void handleDeleteBeginning();     ///< Delete first node.
    void handleDeleteEnd();           ///< Delete last node.
    void handleDeletePosition();      ///< Delete node at given index.

    // Other actions
    void handleClear();               ///< Clear entire list.
    void handleGenerateRandom();      ///< Generate random values.
    void handleApplyInput();          ///< Insert custom list from input.
    void handleBackButton();          ///< Return to menu.
    void handleLinkedListTypeButton(int id); ///< Called when switching list type.

private:

    /**
     * @enum LinkedListType
     * @brief Supported linked list types.
     */
    enum class LinkedListType {
        Singly = 0,   ///< Singly linked list.
        Doubly,       ///< Doubly linked list.
        Circular      ///< Circular linked list.
    };

    /**
     * @struct LinkedListTypeInfo
     * @brief Metadata for each list type (title, subtitle, description).
     */
    struct LinkedListTypeInfo {
        LinkedListType type;      ///< The linked list type.
        QString title;            ///< Heading used in UI.
        QString subtitle;         ///< Short explanation.
        QString description;      ///< Detailed description.
    };

    /**
     * @struct Node
     * @brief Represents a node in the linked list.
     */
    struct Node {
        int value;     ///< The integer stored in the node.
        Node* next;    ///< Pointer to next node.
        Node* prev;    ///< Pointer to previous node (only used in doubly list).

        /**
         * @brief Creates a node with given value.
         * @param v Value to store.
         */
        Node(int v) : value(v), next(nullptr), prev(nullptr) {}
    };

    //
    // ------------------ UI Setup and Visualization ------------------
    //

    void setupUi();                                                   ///< Builds UI layout.
    void setupLinkedListTypeButtons(QHBoxLayout *layout);             ///< Creates Singly/Doubly/Circular buttons.
    void updateLinkedListType(LinkedListType type);                   ///< Switches list mode and reloads state.
    void updateInfoPanel();                                           ///< Updates algorithm description section.
    void updateVisualization(const QString &message = QString());     ///< Repaints the list.
    void setStatusMessage(const QString &message);                    ///< Updates bottom status text.
    void resetList();                                                 ///< Resets pointers for the active list type.
    void showEvent(QShowEvent *event) override;                       ///< Handles first-time setup.

    //
    // ------------------ Input Helpers ------------------
    //

    std::vector<int> parseInputValues(bool *ok) const;                ///< Parses values from text field.
    std::vector<int> generateRandomValues(int size) const;            ///< Creates random list values.

    //
    // ------------------ Linked List Operations ------------------
    //

    void insertAtBeginning(int value);           ///< Insert node at beginning of active list.
    void insertAtEnd(int value);                 ///< Insert node at end of active list.
    void insertAtPosition(int value, int position); ///< Insert node at given index.

    void deleteAtBeginning();                    ///< Remove head node.
    void deleteAtEnd();                          ///< Remove tail node.
    void deleteAtPosition(int position);         ///< Remove node at index.

    void clearList();                            ///< Clear entire active list.

    //
    // ------------------ UI Elements ------------------
    //

    QWidget *m_visualizerWidget;                 ///< Paint area for linked list.
    QButtonGroup *m_linkedListTypeButtons;       ///< Group for list-type selection.
    QLineEdit *m_inputField;                     ///< User value input.
    QSpinBox *m_positionSpinBox;                 ///< Position input for insertion/deletion.
    QSpinBox *m_sizeSpinBox;                     ///< Size input for random generation.
    QLabel *m_statusLabel;                       ///< Status update text.
    QLabel *m_linkedListTypeTitleLabel;          ///< List type title.
    QLabel *m_linkedListTypeSubtitleLabel;       ///< Short description.
    QLabel *m_linkedListTypeDescriptionLabel;    ///< Detailed explanation.
    QLabel *m_listStatsLabel;                    ///< List size, head, tail info.

    // Action buttons
    QPushButton *m_insertBeginButton;            ///< Insert at beginning.
    QPushButton *m_insertEndButton;              ///< Insert at end.
    QPushButton *m_insertPosButton;              ///< Insert at position.
    QPushButton *m_deleteBeginButton;            ///< Delete at beginning.
    QPushButton *m_deleteEndButton;              ///< Delete at end.
    QPushButton *m_deletePosButton;              ///< Delete at position.
    QPushButton *m_clearButton;                  ///< Clear list button.

    //
    // ------------------ Linked List State ------------------
    //

    LinkedListType m_currentLinkedListType;      ///< Currently selected list type.

    Node* m_head;                                ///< Active list head pointer.
    Node* m_tail;                                ///< Active list tail pointer (for doubly/circular).
    size_t m_size;                               ///< Current list size.
    QString m_lastStatusMessage;                 ///< Most recent status info.

    //
    // ------------------ Custom Visualization Widget ------------------
    //

    /**
     * @class LinkedListVizWidget
     * @brief Responsible for drawing nodes, arrows, and animations.
     */
    class LinkedListVizWidget;
    LinkedListVizWidget *m_vizWidget;            ///< Instance of custom painter widget.

    //
    // ------------------ Storage for Switching Between List Types ------------------
    //

    Node* m_singlyHead;                          ///< Saved singly list head.
    Node* m_doublyHead;                          ///< Saved doubly list head.
    Node* m_doublyTail;                          ///< Saved doubly list tail.
    Node* m_circularHead;                        ///< Saved circular list head.

    void saveCurrentListState();                 ///< Saves active list before switching type.
    void loadListState(LinkedListType type);     ///< Loads saved list when switching.
    void clearNodeList(Node*& head);             ///< Deletes nodes recursively.

    /**
     * @brief Metadata for each linked list type.
     */
    static const std::vector<LinkedListTypeInfo> kLinkedListTypeInfos;
};

#endif  // ALGOVISUALIZER_WIDGETS_LINKEDLISTVISUALIZER_H
