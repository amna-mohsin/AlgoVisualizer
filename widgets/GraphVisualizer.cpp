/**
 * @file GraphVisualizer.cpp
 * @brief Implementation of the GraphVisualizer widget for graph algorithm visualization
 * @author AlgoVisualizer Team
 * @date 2025
 * 
 * @details
 * This file implements the GraphVisualizer class which provides interactive
 * visualization for graph algorithms including:
 * - Breadth-First Search (BFS)
 * - Depth-First Search (DFS)
 * - Shortest Path (Dijkstra's algorithm)
 * 
 * Features:
 * - Build custom or random graphs
 * - Add weighted/unweighted edges
 * - Visualize algorithm execution step-by-step
 * - Display visited nodes and paths
 */

#include "GraphVisualizer.h"

// --------------------- Qt Widget Includes -----------------------
#include <QButtonGroup>      ///< Groups algorithm selection buttons together
#include <QHBoxLayout>        ///< Horizontal layout for arranging widgets
#include <QVBoxLayout>        ///< Vertical layout for stacking widgets
#include <QPushButton>        ///< Clickable button widgets
#include <QSpinBox>           ///< Number input with increment/decrement
#include <QAbstractSpinBox>   ///< Base class for spin box widgets
#include <QLabel>             ///< Text label for displaying information
#include <QFrame>             ///< Container widget with border styling
#include <QScrollArea>        ///< Scrollable container for large graphs
#include <QMessageBox>        ///< Dialog boxes for user notifications
#include <QApplication>       ///< Main application object

// --------------------- Qt Graphics Includes ---------------------
#include <QPainter>           ///< 2D painting system for drawing graphs
#include <QRadialGradient>    ///< Radial gradient fills for nodes
#include <QBrush>             ///< Brush for filling shapes
#include <QPen>               ///< Pen for drawing outlines

// --------------------- Qt Core Includes -------------------------
#include <QRandomGenerator>   ///< Generates random numbers for graph generation
#include <QShowEvent>         ///< Event fired when widget becomes visible
#include <QThread>            ///< Thread management (if needed for animations)
#include <QtMath>             ///< Mathematical functions

// --------------------- Standard Library Includes ----------------
#include <algorithm>          ///< STL algorithms (std::find, std::sort, etc.)
#include <functional>         ///< Function objects and lambdas
#include <queue>              ///< FIFO queue for BFS algorithm
#include <climits>            ///< Integer limits (INT_MAX, etc.)

namespace {
constexpr int kMinVertices = 3;
constexpr int kMaxVertices = 10;
}  // namespace

const std::vector<GraphVisualizer::AlgorithmInfo> GraphVisualizer::kAlgorithmInfos = {
    {Algorithm::BFS,
     QObject::tr("BFS"),
     QObject::tr("Breadth-First Search"),
     QObject::tr("BFS explores all vertices at the current depth level before moving to the next level. "
                 "It uses a queue and guarantees finding the shortest path in unweighted graphs.")},
    {Algorithm::DFS,
     QObject::tr("DFS"),
     QObject::tr("Depth-First Search"),
     QObject::tr("DFS explores as far as possible along each branch before backtracking. "
                 "It uses recursion or a stack and is useful for topological sorting and cycle detection.")},
    {Algorithm::ShortestPath,
     QObject::tr("Shortest Path"),
     QObject::tr("Dijkstra's algorithm"),
     QObject::tr("Dijkstra's algorithm finds the shortest path from a source vertex to all other vertices. "
                 "It works with weighted graphs and uses a greedy approach with priority queue.")}
};

// Custom visualization widget
class GraphVisualizer::GraphVizWidget : public QWidget {
public:
    GraphVisualizer* parentVisualizer;
    GraphVizWidget(QWidget* parent) : QWidget(parent), parentVisualizer(nullptr) {
        setMinimumHeight(400);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setStyleSheet("background-color: #ffffff; border: 1px solid #d8e3f0; border-radius: 12px;");
        setAttribute(Qt::WA_OpaquePaintEvent, true);
    }
    void paintEvent(QPaintEvent* event) override {
        Q_UNUSED(event);
        if (!parentVisualizer) return;
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setClipRect(rect());
        
        if (parentVisualizer->m_vertices == 0) {
            painter.setPen(QPen(QColor("#95a5a6"), 2, Qt::DashLine));
            painter.setBrush(QBrush(QColor("#ecf0f1")));
            int centerX = width() / 2 - 100;
            int centerY = height() / 2;
            painter.drawRect(centerX, centerY, 200, 50);
            painter.setPen(QColor("#7f8c8d"));
            painter.setFont(QFont("Segoe UI", 12));
            painter.drawText(QRect(centerX, centerY, 200, 50), Qt::AlignCenter, "EMPTY GRAPH");
            return;
        }
        
        int centerX = width() / 2;
        int centerY = height() / 2;
        int radius = std::min(width(), height()) / 3;
        
        std::vector<QPoint> positions(parentVisualizer->m_vertices);
        for (int i = 0; i < parentVisualizer->m_vertices; i++) {
            double angle = 2.0 * M_PI * i / parentVisualizer->m_vertices - M_PI / 2.0;
            positions[i] = QPoint(
                centerX + radius * cos(angle),
                centerY + radius * sin(angle)
            );
        }
        
        painter.setPen(QPen(QColor("#bdc3c7"), 2));
        for (int u = 0; u < parentVisualizer->m_vertices; u++) {
            for (const auto& edge : parentVisualizer->m_adjList[u]) {
                int v = edge.first;
                if (u < v) {
                    painter.drawLine(positions[u], positions[v]);
                    QPoint mid = (positions[u] + positions[v]) / 2;
                    QPoint dir = positions[v] - positions[u];
                    QPoint perp(-dir.y(), dir.x());
                    double perpLen = sqrt(perp.x() * perp.x() + perp.y() * perp.y());
                    if (perpLen > 0) {
                        perp = QPoint(perp.x() * 15 / perpLen, perp.y() * 15 / perpLen);
                    }
                    QPoint weightPos = mid + perp;
                    painter.setPen(QColor("#ffffff"));
                    painter.setBrush(QBrush(QColor("#ffffff")));
                    painter.drawRect(weightPos.x() - 18, weightPos.y() - 8, 36, 16);
                    painter.setPen(QColor("#7f8c8d"));
                    painter.setFont(QFont("Segoe UI", 9, QFont::Bold));
                    painter.drawText(QRect(weightPos.x() - 18, weightPos.y() - 8, 36, 16), Qt::AlignCenter, QString::number(edge.second));
                    painter.setPen(QPen(QColor("#bdc3c7"), 2));
                }
            }
        }
        
        for (int i = 0; i < parentVisualizer->m_vertices; i++) {
            int nodeRadius = 20;
            bool isVisited = parentVisualizer->m_visitedVertices.find(i) != parentVisualizer->m_visitedVertices.end();
            bool inPath = std::find(parentVisualizer->m_path.begin(), parentVisualizer->m_path.end(), i) != parentVisualizer->m_path.end();
            
            if (inPath) {
                painter.setPen(QPen(QColor("#e74c3c"), 3));
                QRadialGradient gradient(positions[i].x(), positions[i].y(), nodeRadius);
                gradient.setColorAt(0, QColor("#ff6b6b"));
                gradient.setColorAt(1, QColor("#ee5a6f"));
                painter.setBrush(QBrush(gradient));
            } else if (isVisited) {
                painter.setPen(QPen(QColor("#3498db"), 2));
                QRadialGradient gradient(positions[i].x(), positions[i].y(), nodeRadius);
                gradient.setColorAt(0, QColor("#85c1e2"));
                gradient.setColorAt(1, QColor("#5dade2"));
                painter.setBrush(QBrush(gradient));
            } else {
                painter.setPen(QPen(QColor("#95a5a6"), 2));
                painter.setBrush(QBrush(QColor("#ecf0f1")));
            }
            
            painter.drawEllipse(positions[i].x() - nodeRadius, positions[i].y() - nodeRadius, nodeRadius * 2, nodeRadius * 2);
            
            painter.setPen(QColor("#2c3e50"));
            painter.setFont(QFont("Segoe UI", 11, QFont::Bold));
            painter.drawText(QRect(positions[i].x() - nodeRadius, positions[i].y() - nodeRadius, nodeRadius * 2, nodeRadius * 2), Qt::AlignCenter, QString::number(i));
        }
    }
};

GraphVisualizer::GraphVisualizer(QWidget *parent)
    : QWidget(parent),
      m_visualizerWidget(nullptr),
      m_algorithmButtons(new QButtonGroup(this)),
      m_verticesSpinBox(nullptr),
      m_fromSpinBox(nullptr),
      m_toSpinBox(nullptr),
      m_weightSpinBox(nullptr),
      m_startSpinBox(nullptr),
      m_endSpinBox(nullptr),
      m_statusLabel(nullptr),
      m_algorithmTitleLabel(nullptr),
      m_algorithmSubtitleLabel(nullptr),
      m_algorithmDescriptionLabel(nullptr),
      m_graphStatsLabel(nullptr),
      m_buildButton(nullptr),
      m_addEdgeButton(nullptr),
      m_runButton(nullptr),
      m_clearButton(nullptr),
      m_randomButton(nullptr),
      m_currentAlgorithm(Algorithm::BFS),
      m_vertices(0),
      m_startVertex(0),
      m_endVertex(0),
      m_vizWidget(nullptr)
{
    setupUi();
    updateInfoPanel();
    updateAlgorithm(Algorithm::BFS);
    updateVisualization();
}

void GraphVisualizer::setupUi()
{
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet(QStringLiteral("background-color: #f0f4f8; color: #2c3e50; font-family: 'Segoe UI', Arial, sans-serif;"));

    auto outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->setSpacing(0);

    auto scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet(QStringLiteral("QScrollArea { border: none; background-color: #f0f4f8; }"));
    outerLayout->addWidget(scrollArea);

    auto centralContainer = new QWidget();
    centralContainer->setObjectName("centralContainer");
    centralContainer->setStyleSheet(QStringLiteral("QWidget#centralContainer { background-color: #f0f4f8; }"));
    scrollArea->setWidget(centralContainer);

    auto mainLayout = new QVBoxLayout(centralContainer);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(20);

    auto headerBar = new QFrame(centralContainer);
    headerBar->setStyleSheet(QStringLiteral("QFrame { background-color: #ffffff; border-radius: 12px; border: 1px solid #d8e3f0; }"));
    auto headerLayout = new QHBoxLayout(headerBar);
    headerLayout->setContentsMargins(16, 12, 16, 12);
    auto headerLabel = new QLabel(tr("Graph Algorithm Visualizer"), headerBar);
    headerLabel->setStyleSheet(QStringLiteral("color: #0f3460; font-size: 22px; font-weight: 700;"));
    headerLayout->addWidget(headerLabel);
    headerLayout->addStretch();
    mainLayout->addWidget(headerBar);

    auto navBar = new QFrame(centralContainer);
    navBar->setStyleSheet(QStringLiteral("QFrame { background-color: #ffffff; border-radius: 12px; border: 1px solid #d8e3f0; }"));
    auto algorithmLayout = new QHBoxLayout(navBar);
    algorithmLayout->setSpacing(10);
    algorithmLayout->setContentsMargins(12, 10, 12, 10);
    setupAlgorithmButtons(algorithmLayout);
    mainLayout->addWidget(navBar);

    auto contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(20);
    mainLayout->addLayout(contentLayout, 1);

    auto leftColumn = new QVBoxLayout();
    leftColumn->setSpacing(16);
    contentLayout->addLayout(leftColumn, 3);

    auto chartCard = new QFrame(centralContainer);
    chartCard->setObjectName("chartCard");
    chartCard->setStyleSheet(QStringLiteral("QFrame#chartCard { background-color: #ffffff; border-radius: 16px; border: 1px solid #d8e3f0; }"));
    auto chartLayout = new QVBoxLayout(chartCard);
    chartLayout->setContentsMargins(20, 16, 20, 20);
    auto chartTitle = new QLabel(tr("Visualization"), chartCard);
    chartTitle->setStyleSheet(QStringLiteral("color: #0f3460; font-size: 15px; font-weight: 600;"));
    chartLayout->addWidget(chartTitle);

    m_vizWidget = new GraphVizWidget(this);
    m_vizWidget->parentVisualizer = this;
    m_vizWidget->setMinimumHeight(400);
    chartLayout->addWidget(m_vizWidget);
    m_visualizerWidget = m_vizWidget;
    leftColumn->addWidget(chartCard, 3);

    const QString controlStyle = "QPushButton { background-color: #0f3460; color: #ffffff; font-size: 14px; padding: 10px 18px; border-radius: 8px; border: none; font-weight: 600; min-height: 40px; max-height: 40px; min-width: 100px; max-width: 120px; } QPushButton:hover { background-color: #1a5fb4; }";

    m_buildButton = new QPushButton(tr("Build Graph"), this);
    m_addEdgeButton = new QPushButton(tr("Add Edge"), this);
    m_runButton = new QPushButton(tr("Run Algorithm"), this);
    m_clearButton = new QPushButton(tr("Clear"), this);
    m_randomButton = new QPushButton(tr("Random Graph"), this);
    m_buildButton->setStyleSheet(controlStyle);
    m_addEdgeButton->setStyleSheet(controlStyle);
    m_runButton->setStyleSheet(controlStyle);
    m_clearButton->setStyleSheet(controlStyle);
    m_randomButton->setStyleSheet(controlStyle);

    auto controlsCard = new QFrame(centralContainer);
    controlsCard->setObjectName("controlsCard");
    controlsCard->setStyleSheet(QStringLiteral("QFrame#controlsCard { background-color: #ffffff; border-radius: 16px; border: 1px solid #d8e3f0; }"));
    auto controlsLayout = new QVBoxLayout(controlsCard);
    controlsLayout->setContentsMargins(20, 16, 20, 18);
    controlsLayout->setSpacing(14);
    auto buttonRow1 = new QHBoxLayout();
    buttonRow1->setSpacing(12);
    buttonRow1->addWidget(m_buildButton);
    buttonRow1->addWidget(m_addEdgeButton);
    buttonRow1->addWidget(m_runButton);
    buttonRow1->addStretch();
    controlsLayout->addLayout(buttonRow1);
    auto buttonRow2 = new QHBoxLayout();
    buttonRow2->setSpacing(12);
    buttonRow2->addWidget(m_clearButton);
    buttonRow2->addWidget(m_randomButton);
    buttonRow2->addStretch();
    controlsLayout->addLayout(buttonRow2);
    leftColumn->addWidget(controlsCard, 1);

    auto inputCard = new QFrame(centralContainer);
    inputCard->setObjectName("inputCard");
    inputCard->setStyleSheet(QStringLiteral("QFrame#inputCard { background-color: #ffffff; border-radius: 16px; border: 1px solid #d8e3f0; }"));
    auto inputLayout = new QVBoxLayout(inputCard);
    inputLayout->setContentsMargins(20, 18, 20, 18);
    inputLayout->setSpacing(14);

    auto verticesLayout = new QHBoxLayout();
    auto verticesLabel = new QLabel(tr("Vertices"), this);
    verticesLabel->setStyleSheet(QStringLiteral("color: #0f3460; font-size: 13px; font-weight: 600;"));
    m_verticesSpinBox = new QSpinBox(this);
    m_verticesSpinBox->setRange(kMinVertices, kMaxVertices);
    m_verticesSpinBox->setValue(5);
    QString spinBoxInputStyle = QStringLiteral(
        "QSpinBox { background-color: #f9fbff; border: 1px solid #c2d4eb; "
        "border-radius: 8px; color: #2c3e50; padding: 8px; font-size: 13px; min-width: 80px; min-height: 40px; max-height: 40px; "
        "border-top-right-radius: 0; border-bottom-right-radius: 0; }"
        "QSpinBox::up-button, QSpinBox::down-button { width: 0; border: none; background: transparent; }");
    m_verticesSpinBox->setStyleSheet(spinBoxInputStyle);
    m_verticesSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);

    auto buttonContainer = new QWidget(this);
    buttonContainer->setStyleSheet(QStringLiteral(
        "QWidget { border: 1px solid #c2d4eb; border-left: none; background-color: #d0d7e2; "
        "border-top-right-radius: 8px; border-bottom-right-radius: 8px; }"));
    buttonContainer->setMaximumWidth(20);

    auto buttonLayout = new QVBoxLayout(buttonContainer);
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->setSpacing(0);

    auto upButton = new QPushButton(tr("+"), buttonContainer);
    upButton->setStyleSheet(QStringLiteral(
        "QPushButton { background: transparent; color: #0f3460; font-size: 16px; font-weight: 700; border: none; padding: 0; margin: 0; height: 20px; }"
        "QPushButton:hover { background: #b5c1d3; }"));
    auto downButton = new QPushButton(tr("\u2212"), buttonContainer);
    downButton->setStyleSheet(QStringLiteral(
        "QPushButton { background: transparent; color: #0f3460; font-size: 16px; font-weight: 700; border: none; padding: 0; margin: 0; height: 20px; }"
        "QPushButton:hover { background: #b5c1d3; }"));

    buttonLayout->addWidget(upButton);
    buttonLayout->addWidget(downButton);

    auto customSpinBoxLayout = new QHBoxLayout();
    customSpinBoxLayout->setSpacing(0);
    customSpinBoxLayout->addWidget(m_verticesSpinBox, 1);
    customSpinBoxLayout->addWidget(buttonContainer);

    connect(upButton, &QPushButton::clicked, m_verticesSpinBox, &QSpinBox::stepUp);
    connect(downButton, &QPushButton::clicked, m_verticesSpinBox, &QSpinBox::stepDown);

    verticesLayout->addWidget(verticesLabel);
    verticesLayout->addLayout(customSpinBoxLayout);
    verticesLayout->addStretch();
    inputLayout->addLayout(verticesLayout);

    auto edgeLayout = new QHBoxLayout();
    auto fromLabel = new QLabel(tr("From"), this);
    fromLabel->setStyleSheet(QStringLiteral("color: #0f3460; font-size: 13px; font-weight: 600;"));
    m_fromSpinBox = new QSpinBox(this);
    m_fromSpinBox->setRange(0, kMaxVertices - 1);
    QString spinBoxStyle = QStringLiteral(
        "QSpinBox { background-color: #f9fbff; border: 1px solid #c2d4eb; "
        "border-radius: 8px; color: #2c3e50; padding: 8px; font-size: 13px; min-width: 60px; min-height: 40px; max-height: 40px; "
        "border-top-right-radius: 0; border-bottom-right-radius: 0; }"
        "QSpinBox::up-button, QSpinBox::down-button { width: 0; border: none; background: transparent; }");
    m_fromSpinBox->setStyleSheet(spinBoxStyle);
    m_fromSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    auto fromButtonContainer = new QWidget(this);
    fromButtonContainer->setStyleSheet(QStringLiteral("QWidget { border: 1px solid #c2d4eb; border-left: none; background-color: #d0d7e2; border-top-right-radius: 8px; border-bottom-right-radius: 8px; }"));
    fromButtonContainer->setMaximumWidth(20);
    auto fromButtonLayout = new QVBoxLayout(fromButtonContainer);
    fromButtonLayout->setContentsMargins(0, 0, 0, 0);
    fromButtonLayout->setSpacing(0);
    auto fromUpButton = new QPushButton(tr("+"), fromButtonContainer);
    fromUpButton->setStyleSheet(QStringLiteral("QPushButton { background: transparent; color: #0f3460; font-size: 16px; font-weight: 700; border: none; padding: 0; margin: 0; height: 20px; } QPushButton:hover { background: #b5c1d3; }"));
    auto fromDownButton = new QPushButton(tr("\u2212"), fromButtonContainer);
    fromDownButton->setStyleSheet(QStringLiteral("QPushButton { background: transparent; color: #0f3460; font-size: 16px; font-weight: 700; border: none; padding: 0; margin: 0; height: 20px; } QPushButton:hover { background: #b5c1d3; }"));
    fromButtonLayout->addWidget(fromUpButton);
    fromButtonLayout->addWidget(fromDownButton);
    auto fromCustomLayout = new QHBoxLayout();
    fromCustomLayout->setSpacing(0);
    fromCustomLayout->addWidget(m_fromSpinBox, 1);
    fromCustomLayout->addWidget(fromButtonContainer);
    connect(fromUpButton, &QPushButton::clicked, m_fromSpinBox, &QSpinBox::stepUp);
    connect(fromDownButton, &QPushButton::clicked, m_fromSpinBox, &QSpinBox::stepDown);

    auto toLabel = new QLabel(tr("To"), this);
    toLabel->setStyleSheet(QStringLiteral("color: #0f3460; font-size: 13px; font-weight: 600;"));
    m_toSpinBox = new QSpinBox(this);
    m_toSpinBox->setRange(0, kMaxVertices - 1);
    m_toSpinBox->setStyleSheet(spinBoxStyle);
    m_toSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    auto toButtonContainer = new QWidget(this);
    toButtonContainer->setStyleSheet(QStringLiteral("QWidget { border: 1px solid #c2d4eb; border-left: none; background-color: #d0d7e2; border-top-right-radius: 8px; border-bottom-right-radius: 8px; }"));
    toButtonContainer->setMaximumWidth(20);
    auto toButtonLayout = new QVBoxLayout(toButtonContainer);
    toButtonLayout->setContentsMargins(0, 0, 0, 0);
    toButtonLayout->setSpacing(0);
    auto toUpButton = new QPushButton(tr("+"), toButtonContainer);
    toUpButton->setStyleSheet(QStringLiteral("QPushButton { background: transparent; color: #0f3460; font-size: 16px; font-weight: 700; border: none; padding: 0; margin: 0; height: 20px; } QPushButton:hover { background: #b5c1d3; }"));
    auto toDownButton = new QPushButton(tr("\u2212"), toButtonContainer);
    toDownButton->setStyleSheet(QStringLiteral("QPushButton { background: transparent; color: #0f3460; font-size: 16px; font-weight: 700; border: none; padding: 0; margin: 0; height: 20px; } QPushButton:hover { background: #b5c1d3; }"));
    toButtonLayout->addWidget(toUpButton);
    toButtonLayout->addWidget(toDownButton);
    auto toCustomLayout = new QHBoxLayout();
    toCustomLayout->setSpacing(0);
    toCustomLayout->addWidget(m_toSpinBox, 1);
    toCustomLayout->addWidget(toButtonContainer);
    connect(toUpButton, &QPushButton::clicked, m_toSpinBox, &QSpinBox::stepUp);
    connect(toDownButton, &QPushButton::clicked, m_toSpinBox, &QSpinBox::stepDown);

    auto weightLabel = new QLabel(tr("Weight"), this);
    weightLabel->setStyleSheet(QStringLiteral("color: #0f3460; font-size: 13px; font-weight: 600;"));
    m_weightSpinBox = new QSpinBox(this);
    m_weightSpinBox->setRange(1, 10);
    m_weightSpinBox->setValue(1);
    m_weightSpinBox->setStyleSheet(spinBoxStyle);
    m_weightSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    auto weightButtonContainer = new QWidget(this);
    weightButtonContainer->setStyleSheet(QStringLiteral("QWidget { border: 1px solid #c2d4eb; border-left: none; background-color: #d0d7e2; border-top-right-radius: 8px; border-bottom-right-radius: 8px; }"));
    weightButtonContainer->setMaximumWidth(20);
    auto weightButtonLayout = new QVBoxLayout(weightButtonContainer);
    weightButtonLayout->setContentsMargins(0, 0, 0, 0);
    weightButtonLayout->setSpacing(0);
    auto weightUpButton = new QPushButton(tr("+"), weightButtonContainer);
    weightUpButton->setStyleSheet(QStringLiteral("QPushButton { background: transparent; color: #0f3460; font-size: 16px; font-weight: 700; border: none; padding: 0; margin: 0; height: 20px; } QPushButton:hover { background: #b5c1d3; }"));
    auto weightDownButton = new QPushButton(tr("\u2212"), weightButtonContainer);
    weightDownButton->setStyleSheet(QStringLiteral("QPushButton { background: transparent; color: #0f3460; font-size: 16px; font-weight: 700; border: none; padding: 0; margin: 0; height: 20px; } QPushButton:hover { background: #b5c1d3; }"));
    weightButtonLayout->addWidget(weightUpButton);
    weightButtonLayout->addWidget(weightDownButton);
    auto weightCustomLayout = new QHBoxLayout();
    weightCustomLayout->setSpacing(0);
    weightCustomLayout->addWidget(m_weightSpinBox, 1);
    weightCustomLayout->addWidget(weightButtonContainer);
    connect(weightUpButton, &QPushButton::clicked, m_weightSpinBox, &QSpinBox::stepUp);
    connect(weightDownButton, &QPushButton::clicked, m_weightSpinBox, &QSpinBox::stepDown);

    edgeLayout->addWidget(fromLabel);
    edgeLayout->addLayout(fromCustomLayout);
    edgeLayout->addWidget(toLabel);
    edgeLayout->addLayout(toCustomLayout);
    edgeLayout->addWidget(weightLabel);
    edgeLayout->addLayout(weightCustomLayout);
    edgeLayout->addStretch();
    inputLayout->addLayout(edgeLayout);

    auto pathLayout = new QHBoxLayout();
    auto startLabel = new QLabel(tr("Start"), this);
    startLabel->setStyleSheet(QStringLiteral("color: #0f3460; font-size: 13px; font-weight: 600;"));
    m_startSpinBox = new QSpinBox(this);
    m_startSpinBox->setRange(0, kMaxVertices - 1);
    m_startSpinBox->setStyleSheet(spinBoxStyle);
    m_startSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    auto startButtonContainer = new QWidget(this);
    startButtonContainer->setStyleSheet(QStringLiteral("QWidget { border: 1px solid #c2d4eb; border-left: none; background-color: #d0d7e2; border-top-right-radius: 8px; border-bottom-right-radius: 8px; }"));
    startButtonContainer->setMaximumWidth(20);
    auto startButtonLayout = new QVBoxLayout(startButtonContainer);
    startButtonLayout->setContentsMargins(0, 0, 0, 0);
    startButtonLayout->setSpacing(0);
    auto startUpButton = new QPushButton(tr("+"), startButtonContainer);
    startUpButton->setStyleSheet(QStringLiteral("QPushButton { background: transparent; color: #0f3460; font-size: 16px; font-weight: 700; border: none; padding: 0; margin: 0; height: 20px; } QPushButton:hover { background: #b5c1d3; }"));
    auto startDownButton = new QPushButton(tr("\u2212"), startButtonContainer);
    startDownButton->setStyleSheet(QStringLiteral("QPushButton { background: transparent; color: #0f3460; font-size: 16px; font-weight: 700; border: none; padding: 0; margin: 0; height: 20px; } QPushButton:hover { background: #b5c1d3; }"));
    startButtonLayout->addWidget(startUpButton);
    startButtonLayout->addWidget(startDownButton);
    auto startCustomLayout = new QHBoxLayout();
    startCustomLayout->setSpacing(0);
    startCustomLayout->addWidget(m_startSpinBox, 1);
    startCustomLayout->addWidget(startButtonContainer);
    connect(startUpButton, &QPushButton::clicked, m_startSpinBox, &QSpinBox::stepUp);
    connect(startDownButton, &QPushButton::clicked, m_startSpinBox, &QSpinBox::stepDown);

    auto endLabel = new QLabel(tr("End"), this);
    endLabel->setStyleSheet(QStringLiteral("color: #0f3460; font-size: 13px; font-weight: 600;"));
    m_endSpinBox = new QSpinBox(this);
    m_endSpinBox->setRange(0, kMaxVertices - 1);
    m_endSpinBox->setStyleSheet(spinBoxStyle);
    m_endSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    auto endButtonContainer = new QWidget(this);
    endButtonContainer->setStyleSheet(QStringLiteral("QWidget { border: 1px solid #c2d4eb; border-left: none; background-color: #d0d7e2; border-top-right-radius: 8px; border-bottom-right-radius: 8px; }"));
    endButtonContainer->setMaximumWidth(20);
    auto endButtonLayout = new QVBoxLayout(endButtonContainer);
    endButtonLayout->setContentsMargins(0, 0, 0, 0);
    endButtonLayout->setSpacing(0);
    auto endUpButton = new QPushButton(tr("+"), endButtonContainer);
    endUpButton->setStyleSheet(QStringLiteral("QPushButton { background: transparent; color: #0f3460; font-size: 16px; font-weight: 700; border: none; padding: 0; margin: 0; height: 20px; } QPushButton:hover { background: #b5c1d3; }"));
    auto endDownButton = new QPushButton(tr("\u2212"), endButtonContainer);
    endDownButton->setStyleSheet(QStringLiteral("QPushButton { background: transparent; color: #0f3460; font-size: 16px; font-weight: 700; border: none; padding: 0; margin: 0; height: 20px; } QPushButton:hover { background: #b5c1d3; }"));
    endButtonLayout->addWidget(endUpButton);
    endButtonLayout->addWidget(endDownButton);
    auto endCustomLayout = new QHBoxLayout();
    endCustomLayout->setSpacing(0);
    endCustomLayout->addWidget(m_endSpinBox, 1);
    endCustomLayout->addWidget(endButtonContainer);
    connect(endUpButton, &QPushButton::clicked, m_endSpinBox, &QSpinBox::stepUp);
    connect(endDownButton, &QPushButton::clicked, m_endSpinBox, &QSpinBox::stepDown);

    pathLayout->addWidget(startLabel);
    pathLayout->addLayout(startCustomLayout);
    pathLayout->addWidget(endLabel);
    pathLayout->addLayout(endCustomLayout);
    pathLayout->addStretch();
    inputLayout->addLayout(pathLayout);
    leftColumn->addWidget(inputCard, 1);
    leftColumn->addStretch(1);
    contentLayout->setStretch(0, 3);

    auto rightColumn = new QVBoxLayout();
    rightColumn->setSpacing(20);
    contentLayout->addLayout(rightColumn, 2);

    auto infoFrame = new QFrame(centralContainer);
    infoFrame->setObjectName("infoCard");
    infoFrame->setStyleSheet(QStringLiteral("QFrame#infoCard { background-color: #ffffff; border-radius: 16px; border: 1px solid #d8e3f0; }"));
    auto infoLayout = new QVBoxLayout(infoFrame);
    infoLayout->setContentsMargins(24, 28, 24, 24);
    infoLayout->setSpacing(16);

    m_algorithmTitleLabel = new QLabel(this);
    m_algorithmTitleLabel->setStyleSheet(QStringLiteral("color: #0f3460; font-size: 24px; font-weight: 700;"));
    infoLayout->addWidget(m_algorithmTitleLabel);

    m_algorithmSubtitleLabel = new QLabel(this);
    m_algorithmSubtitleLabel->setStyleSheet(QStringLiteral("color: #7a8ca6; font-size: 13px; font-weight: 600;"));
    infoLayout->addWidget(m_algorithmSubtitleLabel);

    auto infoDivider = new QFrame(infoFrame);
    infoDivider->setFrameShape(QFrame::HLine);
    infoDivider->setStyleSheet(QStringLiteral("QFrame { color: #d8e3f0; }"));
    infoLayout->addWidget(infoDivider);

    m_algorithmDescriptionLabel = new QLabel(this);
    m_algorithmDescriptionLabel->setWordWrap(true);
    m_algorithmDescriptionLabel->setStyleSheet(QStringLiteral("color: #2c3e50; font-size: 13px; line-height: 1.6;"));
    infoLayout->addWidget(m_algorithmDescriptionLabel);

    auto statsContainer = new QFrame(infoFrame);
    statsContainer->setObjectName("statsContainer");
    statsContainer->setStyleSheet(QStringLiteral("QFrame#statsContainer { background-color: #f9fbff; border-radius: 12px; border: 1px solid #d8e3f0; }"));
    auto statsLayout = new QVBoxLayout(statsContainer);
    statsLayout->setContentsMargins(18, 14, 18, 14);
    m_graphStatsLabel = new QLabel(this);
    m_graphStatsLabel->setStyleSheet(QStringLiteral("color: #0f3460; font-size: 13px; font-weight: 600;"));
    statsLayout->addWidget(m_graphStatsLabel);
    infoLayout->addWidget(statsContainer);

    auto statusPanel = new QFrame(infoFrame);
    statusPanel->setObjectName("statusPanel");
    statusPanel->setStyleSheet(QStringLiteral("QFrame#statusPanel { background-color: #f9fbff; border-radius: 12px; border: 1px solid #d8e3f0; }"));
    auto statusLayout = new QVBoxLayout(statusPanel);
    statusLayout->setContentsMargins(18, 14, 18, 14);
    auto statusLabelTitle = new QLabel(tr("Live Status"), statusPanel);
    statusLabelTitle->setStyleSheet(QStringLiteral("color: #0f3460; font-size: 13px; font-weight: 600;"));
    statusLayout->addWidget(statusLabelTitle);
    m_statusLabel = new QLabel(this);
    m_statusLabel->setWordWrap(true);
    m_statusLabel->setStyleSheet(QStringLiteral("color: #2c3e50; font-size: 13px; line-height: 1.55;"));
    m_statusLabel->setMinimumHeight(64);
    statusLayout->addWidget(m_statusLabel);
    infoLayout->addWidget(statusPanel);
    infoLayout->addStretch(1);
    rightColumn->addWidget(infoFrame, 1);
    contentLayout->setStretch(1, 2);

    connect(m_algorithmButtons, &QButtonGroup::idClicked, this, &GraphVisualizer::handleAlgorithmButton);
    connect(m_buildButton, &QPushButton::clicked, this, &GraphVisualizer::handleBuildGraph);
    connect(m_addEdgeButton, &QPushButton::clicked, this, &GraphVisualizer::handleAddEdge);
    connect(m_runButton, &QPushButton::clicked, this, &GraphVisualizer::handleRunAlgorithm);
    connect(m_clearButton, &QPushButton::clicked, this, &GraphVisualizer::handleClear);
    connect(m_randomButton, &QPushButton::clicked, this, &GraphVisualizer::handleRandomGraph);
}

void GraphVisualizer::setupAlgorithmButtons(QHBoxLayout *layout)
{
    m_algorithmButtons->setExclusive(true);
    int index = 0;
    for (const auto &info : kAlgorithmInfos) {
        auto button = new QPushButton(info.title, this);
        button->setCheckable(true);
        button->setStyleSheet("QPushButton { background-color: #ffffff; color: #0f3460; padding: 10px 18px; border-radius: 10px; border: 1px solid #d8e3f0; font-size: 13px; font-weight: 600; } QPushButton:hover { background-color: #e8f1ff; } QPushButton:checked { background-color: #0f3460; color: #ffffff; border: 1px solid #0f3460; }");
        m_algorithmButtons->addButton(button, index);
        layout->addWidget(button);
        if (info.type == Algorithm::BFS) button->setChecked(true);
        ++index;
    }
    layout->addStretch();
}

void GraphVisualizer::handleAlgorithmButton(int id)
{
    const int clampedId = qBound(0, id, static_cast<int>(kAlgorithmInfos.size()) - 1);
    updateAlgorithm(kAlgorithmInfos.at(clampedId).type);
}

void GraphVisualizer::updateAlgorithm(Algorithm algorithm)
{
    if (m_currentAlgorithm == algorithm) return;
    m_currentAlgorithm = algorithm;
    updateInfoPanel();
}

void GraphVisualizer::updateInfoPanel()
{
    const auto it = std::find_if(kAlgorithmInfos.begin(), kAlgorithmInfos.end(),
                                 [this](const AlgorithmInfo &info) { return info.type == m_currentAlgorithm; });
    if (it == kAlgorithmInfos.end()) return;
    m_algorithmTitleLabel->setText(it->title);
    m_algorithmSubtitleLabel->setText(it->subtitle);
    m_algorithmDescriptionLabel->setText(it->description);
}

void GraphVisualizer::updateVisualization()
{
    if (m_vizWidget) m_vizWidget->update();
    if (m_vertices == 0) {
        m_graphStatsLabel->setText(tr("Graph: Empty"));
        setStatusMessage(tr("Graph is empty. Build a graph first."));
    } else {
        int edgeCount = 0;
        for (const auto& list : m_adjList) {
            edgeCount += list.size();
        }
        edgeCount /= 2;
        m_graphStatsLabel->setText(tr("Graph: %1 vertices, %2 edges").arg(m_vertices).arg(edgeCount));
    }
}

void GraphVisualizer::setStatusMessage(const QString &message)
{
    m_lastStatusMessage = message;
    if (m_statusLabel) m_statusLabel->setText(message);
}

void GraphVisualizer::handleBuildGraph()
{
    m_vertices = m_verticesSpinBox->value();
    m_adjList.clear();
    m_adjList.resize(m_vertices);
    m_visitedVertices.clear();
    m_path.clear();

    m_fromSpinBox->setMaximum(m_vertices - 1);
    m_toSpinBox->setMaximum(m_vertices - 1);
    m_startSpinBox->setMaximum(m_vertices - 1);
    m_endSpinBox->setMaximum(m_vertices - 1);

    updateVisualization();
    setStatusMessage(tr("Graph created with %1 vertices. Add edges to connect vertices.").arg(m_vertices));
}

void GraphVisualizer::handleAddEdge()
{
    if (m_vertices == 0) {
        QMessageBox::warning(this, tr("No Graph"), tr("Please build a graph first!"));
        return;
    }

    int from = m_fromSpinBox->value();
    int to = m_toSpinBox->value();
    int weight = m_weightSpinBox->value();

    if (from >= m_vertices || to >= m_vertices) {
        QMessageBox::warning(this, tr("Invalid Vertex"), tr("Vertex must be between 0 and %1").arg(m_vertices - 1));
        return;
    }

    if (from == to) {
        QMessageBox::warning(this, tr("Invalid Edge"), tr("Cannot add self-loop!"));
        return;
    }

    for (const auto& edge : m_adjList[from]) {
        if (edge.first == to) {
            QMessageBox::information(this, tr("Edge Exists"), tr("Edge already exists!"));
            return;
        }
    }

    m_adjList[from].push_back({to, weight});
    m_adjList[to].push_back({from, weight});

    updateVisualization();
    int edgeCount = 0;
    for (const auto& list : m_adjList) {
        edgeCount += list.size();
    }
    edgeCount /= 2;
    setStatusMessage(tr("Added edge: %1 → %2 (weight: %3). Graph: %4 vertices, %5 edges.").arg(from).arg(to).arg(weight).arg(m_vertices).arg(edgeCount));
}

void GraphVisualizer::handleRandomGraph()
{
    m_vertices = m_verticesSpinBox->value();
    m_adjList.clear();
    m_adjList.resize(m_vertices);
    m_visitedVertices.clear();
    m_path.clear();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> vertexDis(0, m_vertices - 1);
    std::uniform_int_distribution<> weightDis(1, 10);

    int targetEdgeCount = m_vertices + (gen() % (m_vertices * 2));
    for (int i = 0; i < targetEdgeCount; i++) {
        int from = vertexDis(gen);
        int to = vertexDis(gen);
        if (from != to) {
            int weight = weightDis(gen);
            bool exists = false;
            for (const auto& edge : m_adjList[from]) {
                if (edge.first == to) {
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                m_adjList[from].push_back({to, weight});
                m_adjList[to].push_back({from, weight});
            }
        }
    }

    m_fromSpinBox->setMaximum(m_vertices - 1);
    m_toSpinBox->setMaximum(m_vertices - 1);
    m_startSpinBox->setMaximum(m_vertices - 1);
    m_endSpinBox->setMaximum(m_vertices - 1);

    int edgeCount = 0;
    for (const auto& list : m_adjList) {
        edgeCount += list.size();
    }
    edgeCount /= 2;

    updateVisualization();
    setStatusMessage(tr("Random graph generated with %1 vertices and %2 edges.").arg(m_vertices).arg(edgeCount));
}

void GraphVisualizer::handleRunAlgorithm()
{
    if (m_vertices == 0) {
        QMessageBox::warning(this, tr("No Graph"), tr("Please build a graph first!"));
        return;
    }

    m_startVertex = m_startSpinBox->value();
    m_endVertex = m_endSpinBox->value();

    if (m_currentAlgorithm == Algorithm::BFS) {
        performBFS();
    } else if (m_currentAlgorithm == Algorithm::DFS) {
        performDFS();
    } else {
        performShortestPath();
    }
}

void GraphVisualizer::performBFS()
{
    m_visitedVertices.clear();
    m_path.clear();

    std::queue<int> q;
    std::vector<bool> visited(m_vertices, false);

    q.push(m_startVertex);
    visited[m_startVertex] = true;

    setStatusMessage(tr("BFS starting from vertex %1...").arg(m_startVertex));

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        m_path.push_back(current);
        m_visitedVertices.insert(current);
        updateVisualization();
        QApplication::processEvents();
        QThread::msleep(500);

        for (const auto& edge : m_adjList[current]) {
            int neighbor = edge.first;
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }

    QStringList resultList;
    for (int v : m_path) {
        resultList << QString::number(v);
    }
    setStatusMessage(tr("BFS Order: %1").arg(resultList.join(" → ")));
    m_visitedVertices.clear();
    updateVisualization();
}

void GraphVisualizer::performDFS()
{
    m_visitedVertices.clear();
    m_path.clear();

    std::vector<bool> visited(m_vertices, false);

    setStatusMessage(tr("DFS starting from vertex %1...").arg(m_startVertex));

    std::function<void(int)> dfs = [&](int vertex) {
        visited[vertex] = true;
        m_path.push_back(vertex);
        m_visitedVertices.insert(vertex);
        updateVisualization();
        QApplication::processEvents();
        QThread::msleep(500);

        for (const auto& edge : m_adjList[vertex]) {
            int neighbor = edge.first;
            if (!visited[neighbor]) {
                dfs(neighbor);
            }
        }
    };

    dfs(m_startVertex);

    QStringList resultList;
    for (int v : m_path) {
        resultList << QString::number(v);
    }
    setStatusMessage(tr("DFS Order: %1").arg(resultList.join(" → ")));
    m_visitedVertices.clear();
    updateVisualization();
}

/**
 * @brief Implements Dijkstra's algorithm to find the shortest path in the graph.
 * * This function calculates the shortest distance and path from the starting vertex
 * (m_startVertex) to all other vertices, and specifically highlights the path
 * to the end vertex (m_endVertex). It uses the standard O(V^2) implementation
 * suitable for visualization, as it iterates through all vertices to find the
 * minimum distance unvisited node in each step.
 */
void GraphVisualizer::performShortestPath()
{
    // Clear previous visualization states for a new run.
    m_visitedVertices.clear();
    m_path.clear();

    // Initialize distance vector (dist) to infinity (INT_MAX), parent vector
    // for path reconstruction, and visited vector to track finalized vertices.
    std::vector<int> dist(m_vertices, INT_MAX);
    std::vector<int> parent(m_vertices, -1);
    std::vector<bool> visited(m_vertices, false);

    // Distance from the starting vertex to itself is 0.
    dist[m_startVertex] = 0;

    // Update the status message in the UI.
    setStatusMessage(tr("Finding shortest path from vertex %1 to %2...").arg(m_startVertex).arg(m_endVertex));

    // The main loop of Dijkstra's algorithm. It runs V times (once for each vertex).
    for (int count = 0; count < m_vertices; count++) {
        int u = -1;
        int minDist = INT_MAX;

        // Find the unvisited vertex 'u' with the minimum distance 'minDist'. (Greedy step)
        for (int v = 0; v < m_vertices; v++) {
            if (!visited[v] && dist[v] < minDist) {
                minDist = dist[v];
                u = v;
            }
        }

        // If no unvisited vertex is reachable, we break the loop.
        if (u == -1) break;

        // Mark the selected vertex 'u' as visited (finalized).
        visited[u] = true;
        // Update visualization state to highlight the currently processed vertex.
        m_visitedVertices.insert(u);
        updateVisualization();
        // Allow the GUI to update and pause for visual effect.
        QApplication::processEvents();
        QThread::msleep(300);

        // Relaxation step: Iterate through all neighbors of 'u'.
        for (const auto& edge : m_adjList[u]) {
            int v = edge.first;     // Neighbor vertex
            int weight = edge.second; // Edge weight

            // Check for relaxation:
            // 1. Neighbor 'v' is not finalized.
            // 2. 'u' is reachable (dist[u] is not INT_MAX).
            // 3. Path through 'u' (dist[u] + weight) is shorter than current known distance to 'v' (dist[v]).
            if (!visited[v] && dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u; // Record 'u' as the predecessor of 'v'.
            }
        }
    }

    // Path Reconstruction and Final Status Update.
    if (dist[m_endVertex] != INT_MAX) {
        // If the end vertex is reachable, reconstruct the path by backtracking from the end.
        int current = m_endVertex;
        while (current != -1) {
            m_path.push_back(current);
            current = parent[current];
        }
        // The path is built in reverse, so reverse it to get start -> end order.
        std::reverse(m_path.begin(), m_path.end());

        // Format and display the final shortest path and total distance.
        QStringList resultList;
        for (int v : m_path) {
            resultList << QString::number(v);
        }
        setStatusMessage(tr("Shortest Path: %1 (Distance: %2)").arg(resultList.join(" → ")).arg(dist[m_endVertex]));
    } else {
        // Handle the case where the end vertex is unreachable.
        setStatusMessage(tr("No path found from vertex %1 to %2").arg(m_startVertex).arg(m_endVertex));
    }

    // Clear visualization highlights and trigger a final redraw to show the final path.
    m_visitedVertices.clear();
    updateVisualization();
}
void GraphVisualizer::handleClear()
{
    m_adjList.clear();
    m_vertices = 0;
    m_visitedVertices.clear();
    m_path.clear();
    updateVisualization();
    setStatusMessage(tr("Graph cleared."));
}

void GraphVisualizer::handleBackButton()
{
    emit requestBackToMenu();
}

