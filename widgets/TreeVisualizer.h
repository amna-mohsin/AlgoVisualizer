/**
 * @file TreeVisualizer.h
 * @brief Defines the TreeVisualizer class, a QWidget for visualizing binary trees
 * and their traversal algorithms (e.g., Preorder, Inorder, Postorder, Level Order).
 *
 * This class handles UI interaction, tree construction, state management for
 * step-by-step traversal, and rendering the tree structure.
 */
#ifndef ALGOVISUALIZER_WIDGETS_TREEVISUALIZER_H
#define ALGOVISUALIZER_WIDGETS_TREEVISUALIZER_H

// --- Qt Includes ---
#include <QWidget>      // @brief Base class for all user interface objects.
#include <QMap>         // @brief Provides a dictionary (hash map) for key-value storage.
#include <vector>       // @brief Standard C++ container for dynamic arrays.
#include <queue>        // @brief Standard C++ container for queue operations (useful for level order traversal).
#include <QScrollArea>  // @brief Provides a scrolling viewport.
#include <QPainter>     // @brief Used for performing custom drawing on the visualization widget.

// --- Forward Declarations for UI Components ---
class QPushButton;  // @brief A clickable command button.
class QLineEdit;    // @brief A widget for single-line text input (for tree construction values).
class QSpinBox;     // @brief A widget for integer input (e.g., for setting random tree size).
class QLabel;       // @brief A widget that displays non-editable text or images.
class QButtonGroup; // @brief Used to group traversal selection buttons.
class QHBoxLayout;  // @brief A layout manager that arranges widgets horizontally.

/**
 * @struct TreeNode
 * @brief Represents a single node in the binary tree.
 */
struct TreeNode {
    int data;           ///< @brief The value stored in the node.
    TreeNode* left;     ///< @brief Pointer to the left child node.
    TreeNode* right;    ///< @brief Pointer to the right child node.
    /**
     * @brief Constructor that initializes a node with a given value.
     */
    TreeNode(int value) : data(value), left(nullptr), right(nullptr) {}
};

/**
 * @class TreeVisualizer
 * @brief Widget responsible for visualizing binary tree structure and traversals.
 *
 * It manages tree data, controls traversal execution, and updates the graphical
 * display to show the algorithm steps.
 */
class TreeVisualizer : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for TreeVisualizer.
     * @param parent The parent widget (defaulted to nullptr).
     */
    explicit TreeVisualizer(QWidget *parent = nullptr);

signals:
    /**
     * @brief Signal emitted when the user requests to return to the main menu.
     */
    void requestBackToMenu();

private slots:
    /**
     * @brief Handles the event to build the tree from the input values.
     */
    void handleBuildTree();

    /**
     * @brief Handles the event to start the step-by-step traversal visualization.
     */
    void handleRun();

    /**
     * @brief Handles the event to stop a running traversal visualization.
     */
    void handleStop();

    /**
     * @brief Handles clearing the tree structure and resetting the visualization state.
     */
    void handleClear();

    /**
     * @brief Handles generating a random tree based on the size spin box value.
     */
    void handleRandomTree();

    /**
     * @brief Applies the values from the input line edit to the internal state.
     */
    void handleApplyInput();

    /**
     * @brief Handles the click event for the back button.
     */
    void handleBackButton();

    /**
     * @brief Handles the selection of a new traversal algorithm via a button click.
     * @param id The ID corresponding to the selected traversal type.
     */
    void handleTraversalTypeButton(int id);

private:
    /**
     * @enum TraversalType
     * @brief Enumerates the different tree traversal algorithms.
     */
    enum class TraversalType {
        Preorder = 0,   ///< Root, Left, Right
        Inorder,        ///< Left, Root, Right
        Postorder,      ///< Left, Right, Root
        LevelOrder      ///< Breadth-First Search (BFS)
    };

    /**
     * @struct TraversalTypeInfo
     * @brief Static data structure to hold descriptive metadata about each traversal type.
     */
    struct TraversalTypeInfo {
        TraversalType type;         ///< The enumeration value of the traversal type.
        QString title;              ///< The name (e.g., "Inorder Traversal").
        QString subtitle;           ///< A short descriptor.
        QString description;        ///< A brief explanation of the traversal order.
    };

    /**
     * @brief Sets up and lays out all user interface elements.
     */
    void setupUi();

    /**
     * @brief Creates and connects the traversal type selection buttons.
     * @param layout The layout manager to add the buttons to.
     */
    void setupTraversalTypeButtons(QHBoxLayout *layout);

    /**
     * @brief Updates the current traversal type and resets visualization state.
     * @param type The newly selected traversal type.
     */
    void updateTraversalType(TraversalType type);

    /**
     * @brief Updates the information panel with the current traversal type's details.
     */
    void updateInfoPanel();

    /**
     * @brief Triggers the repainting of the custom visualization widget.
     */
    void updateVisualization();

    /**
     * @brief Updates the status bar/label with a message.
     * @param message The string message to display.
     */
    void setStatusMessage(const QString &message);

    /**
     * @brief Recursively deletes all nodes starting from the given node.
     * @param node The root of the subtree to clear.
     */
    void clearTree(TreeNode* node);

    /**
     * @brief Builds a complete binary tree from a vector of integer values (using level order).
     * @param values The vector of node data, where 0 or a special value can represent null/empty.
     * @return The root node of the newly built tree.
     */
    TreeNode* buildTreeFromArray(const std::vector<int>& values);

    /**
     * @brief Handles the custom painting logic to draw the tree structure.
     * @param painter The QPainter object used for drawing.
     * @param node The current node to draw.
     * @param x The current node's x-coordinate.
     * @param y The current node's y-coordinate.
     * @param level The current depth of the node in the tree.
     * @param width The width allocated for the current subtree.
     */
    void drawTree(QPainter& painter, TreeNode* node, int x, int y, int level, int width);

    /**
     * @brief Parses the comma-separated string input into a vector of integers.
     * @param ok Pointer to a boolean flag that is set to true if parsing succeeds.
     * @return A vector of integer values representing node data.
     */
    std::vector<int> parseInputValues(bool *ok) const;

    /**
     * @brief Generates a vector of random integer values for tree construction.
     * @param size The number of random values to generate.
     * @return A vector of random integers.
     */
    std::vector<int> generateRandomValues(int size) const;

    // --- UI Member Variables ---
    QWidget *m_visualizerWidget;                ///< @brief Main container for the drawing area.
    QButtonGroup *m_traversalTypeButtons;       ///< @brief Grouping for traversal selection buttons.
    QLineEdit *m_treeInput;                     ///< @brief Input field for tree node values (e.g., "1,2,3,4,5").
    QSpinBox *m_sizeSpinBox;                    ///< @brief Spin box for selecting the size of a random tree.
    QLabel *m_statusLabel;                      ///< @brief Label for displaying operation status and messages.
    QLabel *m_traversalTypeTitleLabel;          ///< @brief Displays the current traversal title.
    QLabel *m_traversalTypeSubtitleLabel;       ///< @brief Displays the traversal subtitle.
    QLabel *m_traversalTypeDescriptionLabel;    ///< @brief Displays the traversal description/logic.
    QLabel *m_treeStatsLabel;                   ///< @brief Displays statistics about the current tree (e.g., height, nodes).
    QPushButton *m_buildButton;                 ///< @brief Button to trigger tree construction.
    QPushButton *m_runButton;                   ///< @brief Button to start traversal visualization.
    QPushButton *m_stopButton;                  ///< @brief Button to stop or pause traversal.
    QPushButton *m_clearButton;                 ///< @brief Button to clear the tree.
    QPushButton *m_randomButton;                ///< @brief Button to generate a random tree.

    // --- State Member Variables ---
    TraversalType m_currentTraversalType;       ///< @brief The traversal algorithm currently selected.
    TreeNode* m_root;                           ///< @brief Pointer to the root node of the binary tree.
    std::vector<int> m_traversalResult;         ///< @brief Stores the sequence of node data resulting from the traversal.
    std::vector<int> m_highlightedNodes;        ///< @brief Nodes currently highlighted in the visualization during execution.
    QString m_currentTraversal;                 ///< @brief Text representation of the current traversal sequence shown to user.
    QString m_lastStatusMessage;                ///< @brief Stores the last status message.
    bool m_isRunning;                           ///< @brief Flag indicating if the visualization animation is currently running.

    // --- Custom Visualization Widget ---
    class TreeVizWidget;                        ///< @brief Forward declaration of the custom painting class.
    TreeVizWidget *m_vizWidget;                 ///< @brief Pointer to the custom visualization canvas.

    /**
     * @brief Static constant vector holding the descriptive information for all traversal types.
     */
    static const std::vector<TraversalTypeInfo> kTraversalTypeInfos;
};

#endif  // ALGOVISUALIZER_WIDGETS_TREEVISUALIZER_H
