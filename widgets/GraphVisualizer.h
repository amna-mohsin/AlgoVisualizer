#ifndef ALGOVISUALIZER_WIDGETS_GRAPHVISUALIZER_H
#define ALGOVISUALIZER_WIDGETS_GRAPHVISUALIZER_H

/**
 * @file GraphVisualizer.h
 * @brief Header file for the GraphVisualizer widget.
 *
 * @details
 * This file defines the GraphVisualizer class, responsible for building,
 * displaying, and animating graph algorithms such as BFS, DFS, and
 * Shortest Path. It contains UI controls, graph-management logic,
 * and a custom rendering widget.
 */

//
// ------------------ Qt Includes ------------------
//

#include <QWidget>       ///< Base class for all UI widgets.
#include <QMap>          ///< Provides Qt's map container (not heavily used here).
#include <vector>        ///< Standard C++ vector container for graph adjacency lists.
#include <list>          ///< std::list used to store adjacency lists with weights.
#include <set>           ///< std::set used to track visited vertices.
#include <queue>         ///< std::queue used in BFS algorithm implementation.
#include <QScrollArea>   ///< Scrollable view used for larger graph displays.
#include <QPainter>      ///< Drawing class used for custom graph visualization.

//
// ------------------ Forward Declarations ------------------
// These avoid heavy include files and speed up compilation.
//

class QPushButton;    ///< Button widgets for UI actions.
class QLineEdit;      ///< Input field (not used much here but declared).
class QSpinBox;       ///< Spin box inputs for numeric values (vertices, weights, etc.).
class QLabel;         ///< Text label widgets in the UI.
class QButtonGroup;   ///< Groups related algorithm-selection buttons.
class QHBoxLayout;    ///< Horizontal layout for arranging UI components.

/**
 * @class GraphVisualizer
 * @brief A QWidget used to visualize graph creation and graph algorithms.
 *
 * @details
 * Features:
 *  - Build custom or random graphs
 *  - Add weighted/unweighted edges
 *  - Run BFS, DFS, or Shortest Path
 *  - Display algorithm steps, visited nodes, and animations
 *  - Show algorithm info such as title, explanation, and status
 */
class GraphVisualizer : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a new GraphVisualizer widget.
     * @param parent Optional parent QWidget.
     */
    explicit GraphVisualizer(QWidget *parent = nullptr);

signals:
    /**
     * @brief Emitted when user clicks "Back to Menu" button.
     */
    void requestBackToMenu();

    //
    // ------------------ Slots: User Interactions ------------------
    //

private slots:

    void handleBuildGraph();       ///< Builds the graph with given number of vertices.
    void handleAddEdge();          ///< Adds an edge between two nodes.
    void handleRunAlgorithm();     ///< Executes the selected algorithm.
    void handleClear();            ///< Clears graph, UI state, and visualization.
    void handleRandomGraph();      ///< Generates a random graph.
    void handleBackButton();       ///< Returns to main menu.
    void handleAlgorithmButton(int id); ///< Handles algorithm selection.

    //
    // ------------------ Private Helper Types ------------------
    //

private:

    /**
     * @enum Algorithm
     * @brief Supported graph algorithms.
     */
    enum class Algorithm {
        BFS = 0,        ///< Breadth-First Search.
        DFS,            ///< Depth-First Search.
        ShortestPath    ///< Weighted shortest path (Dijkstra-like approach).
    };

    /**
     * @struct AlgorithmInfo
     * @brief Stores UI-friendly metadata for each algorithm.
     */
    struct AlgorithmInfo {
        Algorithm type;      ///< Algorithm identifier.
        QString title;       ///< Algorithm title shown on UI.
        QString subtitle;    ///< Short explanation subtitle.
        QString description; ///< Detailed algorithm description.
    };

    //
    // ------------------ UI Setup Functions ------------------
    //

    void setupUi();                                              ///< Builds UI layout and widgets.
    void setupAlgorithmButtons(QHBoxLayout *layout);              ///< Creates BFS/DFS/Shortest Path buttons.
    void updateAlgorithm(Algorithm algorithm);                     ///< Updates current algorithm selection.
    void updateInfoPanel();                                       ///< Updates algorithm details in UI.
    void updateVisualization();                                    ///< Repaints the graph display.
    void setStatusMessage(const QString &message);                 ///< Updates status/feedback text.

    //
    // ------------------ Graph Algorithms ------------------
    //
    void performBFS();               ///< Executes Breadth-First Search.
    void performDFS();               ///< Executes Depth-First Search.
    void performShortestPath();      ///< Computes weighted shortest path.

    //
    // ------------------ UI Components ------------------
    //
    QWidget *m_visualizerWidget;           ///< Parent widget for graph drawing.
    QButtonGroup *m_algorithmButtons;      ///< Group for BFS / DFS / Shortest Path buttons.

    // Input controls
    QSpinBox *m_verticesSpinBox;           ///< Number of vertices input.
    QSpinBox *m_fromSpinBox;               ///< Edge: starting vertex.
    QSpinBox *m_toSpinBox;                 ///< Edge: ending vertex.
    QSpinBox *m_weightSpinBox;             ///< Edge weight input.
    QSpinBox *m_startSpinBox;              ///< Algorithm start vertex.
    QSpinBox *m_endSpinBox;                ///< Algorithm end vertex (for shortest path).

    // Labels
    QLabel *m_statusLabel;                 ///< Shows algorithm progress message.
    QLabel *m_algorithmTitleLabel;         ///< Algorithm name.
    QLabel *m_algorithmSubtitleLabel;      ///< Brief description subtitle.
    QLabel *m_algorithmDescriptionLabel;   ///< Long description/explanation.
    QLabel *m_graphStatsLabel;             ///< Shows vertex/edge summary text.

    // Buttons
    QPushButton *m_buildButton;            ///< Build graph.
    QPushButton *m_addEdgeButton;          ///< Add edge.
    QPushButton *m_runButton;              ///< Run selected algorithm.
    QPushButton *m_clearButton;            ///< Clear graph.
    QPushButton *m_randomButton;           ///< Generate a random graph.

    //
    // ------------------ Graph Data ------------------
    //

    Algorithm m_currentAlgorithm;                 ///< Currently selected algorithm.
    int m_vertices;                               ///< Total number of vertices.

    std::vector<std::list<std::pair<int, int>>> m_adjList; ///< Adjacency list (vertex → (neighbor, weight)).
    std::set<int> m_visitedVertices;              ///< Tracks visited nodes during algorithms.
    std::vector<int> m_path;                      ///< Stores computed shortest path.
    int m_startVertex;                            ///< Starting vertex for algorithm.
    int m_endVertex;                              ///< Ending vertex for shortest path.
    QString m_lastStatusMessage;                  ///< Last status message.

    //
    // ------------------ Custom Visualization Widget ------------------
    //

    /**
     * @class GraphVizWidget
     * @brief Internal widget responsible for drawing the graph nodes and edges.
     *
     * @details
     * Uses QPainter to draw circles, edges, colors, visited nodes, and paths.
     */
    class GraphVizWidget;
    GraphVizWidget *m_vizWidget;                  ///< Instance of the visualization widget.

    /**
     * @brief Static metadata for each algorithm (name, description, etc.).
     */
    static const std::vector<AlgorithmInfo> kAlgorithmInfos;
};

#endif  // ALGOVISUALIZER_WIDGETS_GRAPHVISUALIZER_H
