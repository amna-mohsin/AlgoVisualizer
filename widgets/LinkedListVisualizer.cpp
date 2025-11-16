/**
 * @file LinkedListVisualizer.cpp
 * @brief Implementation of the LinkedListVisualizer widget for linked list visualization
 * @author AlgoVisualizer Team
 * @date 2025
 * 
 * @details
 * This file implements the LinkedListVisualizer class which provides interactive
 * visualization for linked list data structures:
 * - Singly Linked List (one-way links)
 * - Doubly Linked List (bidirectional links)
 * - Circular Linked List (last node points to first)
 * 
 * Features:
 * - Insert operations (beginning, end, position)
 * - Delete operations (beginning, end, position)
 * - Visual representation with nodes and arrows
 * - State preservation when switching list types
 * - Random value generation and custom input
 */

#include "LinkedListVisualizer.h"

// --------------------- Qt Widget Includes -----------------------
#include <QButtonGroup>      ///< Groups linked list type selection buttons
#include <QHBoxLayout>        ///< Horizontal layout for arranging widgets
#include <QVBoxLayout>        ///< Vertical layout for stacking widgets
#include <QPushButton>        ///< Clickable button widgets
#include <QLineEdit>          ///< Single-line text input for values
#include <QSpinBox>           ///< Number input for position and size
#include <QAbstractSpinBox>   ///< Base class for spin box widgets
#include <QLabel>             ///< Text label for displaying information
#include <QFrame>             ///< Container widget with border styling
#include <QScrollArea>        ///< Scrollable container for long lists
#include <QMessageBox>        ///< Dialog boxes for user notifications

// --------------------- Qt Graphics Includes ---------------------
#include <QPainter>           ///< 2D painting system for drawing nodes and arrows
#include <QPainterPath>       ///< Complex path drawing for curved arrows
#include <QLinearGradient>    ///< Linear gradient fills for nodes
#include <QBrush>             ///< Brush for filling shapes
#include <QPen>               ///< Pen for drawing outlines
#include <QPolygon>           ///< Polygon shapes for arrowheads

// --------------------- Qt Core Includes -------------------------
#include <QRandomGenerator>   ///< Generates random numbers
#include <QtMath>             ///< Mathematical functions
#include <QShowEvent>         ///< Event fired when widget becomes visible

// --------------------- Standard Library Includes ----------------
#include <algorithm>          ///< STL algorithms

namespace {

constexpr int kMinListSize = 3;
constexpr int kMaxListSize = 16;

QString formatArray(const std::vector<int> &values)
{
    QStringList parts;
    parts.reserve(static_cast<int>(values.size()));
    for (int value : values) {
        parts << QString::number(value);
    }
    return parts.join(' ');
}

}  // namespace

const std::vector<LinkedListVisualizer::LinkedListTypeInfo> LinkedListVisualizer::kLinkedListTypeInfos = {
    {LinkedListType::Singly,
     QObject::tr("Singly Linked List"),
     QObject::tr("Nodes with single next pointer"),
     QObject::tr("A Singly Linked List is a linear data structure where each node contains data and a pointer to the next node. "
                 "Traversal is unidirectional - you can only move forward through the list.")},
    {LinkedListType::Doubly,
     QObject::tr("Doubly Linked List"),
     QObject::tr("Nodes with next and previous pointers"),
     QObject::tr("A Doubly Linked List allows traversal in both directions. Each node has pointers to both the next and previous nodes, "
                 "providing more flexibility but requiring more memory.")},
    {LinkedListType::Circular,
     QObject::tr("Circular Linked List"),
     QObject::tr("Last node points back to first"),
     QObject::tr("A Circular Linked List is a variation where the last node points back to the first node, forming a circle. "
                 "This allows continuous traversal without reaching an end.")}
};

// Custom visualization widget for Linked List
class LinkedListVisualizer::LinkedListVizWidget : public QWidget {
public:
    LinkedListVisualizer* parentVisualizer;
    LinkedListVizWidget(QWidget* parent) : QWidget(parent), parentVisualizer(nullptr) {
        setMinimumHeight(300);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
        setStyleSheet("background-color: #ffffff; border: 1px solid #d8e3f0; border-radius: 12px;");
        setAttribute(Qt::WA_OpaquePaintEvent, true);
    }
    void paintEvent(QPaintEvent* event) override {
        Q_UNUSED(event);
        if (!parentVisualizer) return;
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setClipRect(rect());
        
        int startX = 120;  // Increased to make room for HEAD pointer
        int startY = height() / 2 - 35;
        int baseBoxWidth = 80;
        int baseBoxHeight = 70;
        int baseSpacing = 100;
        
        // Calculate available width (accounting for margins and HEAD pointer)
        int availableWidth = width() - startX - 40; // 40px right margin
        int minBoxWidth = 50; // Minimum box width to maintain readability
        int minSpacing = 60;  // Minimum spacing between nodes
        
        // Count total nodes
        int totalNodes = 0;
        Node* countNode = parentVisualizer->m_head;
        Node* firstNode = parentVisualizer->m_head;
        bool isCircular = (parentVisualizer->m_currentLinkedListType == LinkedListType::Circular);
        
        if (countNode != nullptr) {
            do {
                totalNodes++;
                countNode = countNode->next;
                if (isCircular && countNode == firstNode) break;
                if (!isCircular && countNode == nullptr) break;
            } while (countNode != nullptr && countNode != firstNode);
        }
        
        // Calculate dynamic sizing
        int boxWidth = baseBoxWidth;
        int boxHeight = baseBoxHeight;
        int spacing = baseSpacing;
        int maxNodesPerRow = 8;
        
        if (totalNodes > 0) {
            // Calculate how many nodes can fit with base sizing
            int nodesThatFit = (availableWidth + baseSpacing) / (baseBoxWidth + baseSpacing);
            
            if (nodesThatFit < totalNodes) {
                // Need to scale down or wrap to multiple rows
                // Calculate required spacing per node
                int requiredSpacingPerNode = availableWidth / totalNodes;
                
                // Ensure minimum sizes
                if (requiredSpacingPerNode < minBoxWidth + minSpacing) {
                    requiredSpacingPerNode = minBoxWidth + minSpacing;
                    // Recalculate maxNodesPerRow based on minimum sizes
                    maxNodesPerRow = (availableWidth + minSpacing) / (minBoxWidth + minSpacing);
                    if (maxNodesPerRow < 1) maxNodesPerRow = 1;
                    
                    // Scale boxWidth and spacing proportionally
                    spacing = minSpacing;
                    boxWidth = (availableWidth - (maxNodesPerRow - 1) * spacing) / maxNodesPerRow;
                    if (boxWidth < minBoxWidth) {
                        boxWidth = minBoxWidth;
                        spacing = (availableWidth - maxNodesPerRow * boxWidth) / (maxNodesPerRow - 1);
                        if (spacing < 20) spacing = 20;
                    }
                } else {
                    // Try to fit all in one row first
                    int nodesInOneRow = (availableWidth + baseSpacing) / (baseBoxWidth + baseSpacing);
                    if (nodesInOneRow >= totalNodes) {
                        // Can fit all in one row with base sizing
                        maxNodesPerRow = totalNodes;
                        boxWidth = baseBoxWidth;
                        spacing = baseSpacing;
                    } else {
                        // Scale proportionally (40% box, 60% spacing)
                        boxWidth = (requiredSpacingPerNode * 4) / 10; // 40% of spacing for box
                        spacing = (requiredSpacingPerNode * 6) / 10;   // 60% for spacing
                        if (boxWidth < minBoxWidth) {
                            boxWidth = minBoxWidth;
                            spacing = (availableWidth - totalNodes * boxWidth) / (totalNodes - 1);
                            if (spacing < minSpacing) {
                                spacing = minSpacing;
                                // Need to wrap to multiple rows
                                maxNodesPerRow = (availableWidth + minSpacing) / (minBoxWidth + minSpacing);
                                if (maxNodesPerRow < 1) maxNodesPerRow = 1;
                            } else {
                                maxNodesPerRow = totalNodes; // Try to fit all in one row
                            }
                        } else {
                            maxNodesPerRow = totalNodes; // Try to fit all in one row
                        }
                    }
                }
            } else {
                // Can fit with base sizing, but still limit maxNodesPerRow
                maxNodesPerRow = qMin(nodesThatFit, 10); // Max 10 nodes per row for readability
            }
        }
        
        // Calculate total rows needed
        int totalRows = (totalNodes + maxNodesPerRow - 1) / maxNodesPerRow;
        if (totalRows < 1) totalRows = 1;
        
        // Calculate required height
        int requiredHeight = startY + totalRows * (boxHeight + 40) + 50; // 40px row spacing, 50px bottom margin
        int minRequiredHeight = qMax(300, requiredHeight);
        
        // Update widget minimum height to show all nodes
        if (parentVisualizer && parentVisualizer->m_vizWidget) {
            if (parentVisualizer->m_vizWidget->minimumHeight() != minRequiredHeight) {
                parentVisualizer->m_vizWidget->setMinimumHeight(minRequiredHeight);
                parentVisualizer->m_vizWidget->updateGeometry();
            }
        }
        
        if (parentVisualizer->m_head == nullptr) {
            int emptyBoxX = startX;
            int emptyBoxY = startY;
            int emptyBoxWidth = boxWidth * 2;
            
            // Draw empty list box
            painter.setPen(QPen(QColor("#95a5a6"), 2, Qt::DashLine));
            painter.setBrush(QBrush(QColor("#ecf0f1")));
            painter.drawRoundedRect(emptyBoxX, emptyBoxY, emptyBoxWidth, boxHeight, 10, 10);
            
            // Draw NULL text
            painter.setPen(QColor("#7f8c8d"));
            painter.setFont(QFont("Segoe UI", 12, QFont::Bold));
            painter.drawText(QRect(emptyBoxX, emptyBoxY, emptyBoxWidth, boxHeight), Qt::AlignCenter, "NULL");
            
            // Draw HEAD pointer
            int headPointerX = emptyBoxX - 70;
            int headPointerY = emptyBoxY + boxHeight / 2;
            painter.setPen(QPen(QColor("#e74c3c"), 3));
            painter.setFont(QFont("Segoe UI", 10, QFont::Bold));
            painter.drawText(headPointerX - 30, headPointerY - 5, "HEAD");
            painter.drawLine(headPointerX - 15, headPointerY, emptyBoxX, headPointerY);
            QPolygon headArrow;
            headArrow << QPoint(emptyBoxX, headPointerY)
                      << QPoint(emptyBoxX - 8, headPointerY - 5)
                      << QPoint(emptyBoxX - 8, headPointerY + 5);
            painter.setBrush(QBrush(QColor("#e74c3c")));
            painter.drawPolygon(headArrow);
            return;
        }
        
        // Draw HEAD pointer
        painter.setPen(QPen(QColor("#e74c3c"), 3));
        painter.setFont(QFont("Segoe UI", 10, QFont::Bold));
        painter.drawText(startX - 70, startY + boxHeight / 2, "HEAD");
        painter.drawLine(startX - 50, startY + boxHeight / 2, startX, startY + boxHeight / 2);
        QPolygon headArrow;
        headArrow << QPoint(startX, startY + boxHeight / 2)
                  << QPoint(startX - 10, startY + boxHeight / 2 - 5)
                  << QPoint(startX - 10, startY + boxHeight / 2 + 5);
        painter.setBrush(QBrush(QColor("#e74c3c")));
        painter.drawPolygon(headArrow);
        
        // Draw nodes
        Node* current = parentVisualizer->m_head;
        int index = 0;
        Node* first = parentVisualizer->m_head;
        
        while (current != nullptr && index < totalNodes) {
            int row = index / maxNodesPerRow;
            int col = index % maxNodesPerRow;
            int x = startX + col * (boxWidth + spacing);
            int y = startY + row * (boxHeight + 40);
            
            // Draw node box with gradient
            painter.setPen(QPen(QColor("#3498db"), 3));
            QLinearGradient gradient(x, y, x, y + boxHeight);
            gradient.setColorAt(0, QColor("#d6eaf8"));
            gradient.setColorAt(1, QColor("#aed6f1"));
            painter.setBrush(QBrush(gradient));
            painter.drawRoundedRect(x, y, boxWidth, boxHeight, 10, 10);
            
            // Draw value
            painter.setPen(QColor("#2c3e50"));
            // Scale font size based on box width for readability
            int fontSize = qMax(10, qMin(14, boxWidth / 6));
            painter.setFont(QFont("Segoe UI", fontSize, QFont::Bold));
            painter.drawText(QRect(x, y, boxWidth, boxHeight), Qt::AlignCenter, QString::number(current->value));
            
            // Draw arrow to next
            Node* nextNode = current->next;
            bool isLastInRow = (col == maxNodesPerRow - 1);
            bool hasMoreRows = (row < static_cast<int>((totalNodes + maxNodesPerRow - 1) / maxNodesPerRow - 1));
            bool isCircularList = (parentVisualizer->m_currentLinkedListType == LinkedListType::Circular);
            bool isLastNode = (nextNode == nullptr || (isCircularList && nextNode == first));
            
            if (!isLastNode) {
                if (!isLastInRow) {
                    // Draw horizontal arrow to next node in same row
                    painter.setPen(QPen(QColor("#34495e"), 3));
                    int arrowEndX = x + boxWidth + spacing - 20;
                    painter.drawLine(x + boxWidth, y + boxHeight / 2, arrowEndX, y + boxHeight / 2);
                    QPolygon arrowHead;
                    arrowHead << QPoint(arrowEndX, y + boxHeight / 2)
                              << QPoint(arrowEndX - 10, y + boxHeight / 2 - 7)
                              << QPoint(arrowEndX - 10, y + boxHeight / 2 + 7);
                    painter.setBrush(QBrush(QColor("#34495e")));
                    painter.drawPolygon(arrowHead);
                } else if (hasMoreRows) {
                    // Draw vertical arrow to next row
                    painter.setPen(QPen(QColor("#34495e"), 3));
                    int nextY = y + boxHeight + 40;
                    painter.drawLine(x + boxWidth / 2, y + boxHeight, x + boxWidth / 2, nextY);
                    QPolygon arrowHead;
                    arrowHead << QPoint(x + boxWidth / 2, nextY)
                              << QPoint(x + boxWidth / 2 - 7, nextY - 10)
                              << QPoint(x + boxWidth / 2 + 7, nextY - 10);
                    painter.setBrush(QBrush(QColor("#34495e")));
                    painter.drawPolygon(arrowHead);
                }
            } else {
                // Check if circular - if so, draw arrow around the edge from tail to head
                // Only draw this once for the last node
                if (isCircularList && parentVisualizer->m_head != nullptr && index == totalNodes - 1) {
                    // First node is always at index 0
                    int firstRow = 0;
                    int firstCol = 0;
                    int firstX = startX + firstCol * (boxWidth + spacing);
                    int firstY = startY + firstRow * (boxHeight + 40);
                    
                    // Draw simple dotted line from last node (tail) around the bottom edge to first node (head)
                    painter.setPen(QPen(QColor("#9b59b6"), 2, Qt::DotLine)); // Use DotLine for dotted effect
                    painter.setBrush(Qt::NoBrush); // No fill
                    
                    // Calculate start point (tail - last node, exit from right side)
                    int startX_arrow = x + boxWidth;
                    int startY_arrow = y + boxHeight / 2;
                    
                    // Calculate end point (head - first node)
                    int endX_arrow = firstX;
                    int endY_arrow = firstY + boxHeight / 2;
                    
                    // Get widget dimensions for edge routing
                    int widgetWidth = this->width();
                    int widgetHeight = this->height();
                    int margin = 20; // Margin from edges to avoid overlap
                    
                    // Find the bottom-most Y position of all nodes to route below them
                    // Calculate based on the last row
                    int lastRow = (totalNodes - 1) / maxNodesPerRow;
                    int bottomY = startY + (lastRow + 1) * (boxHeight + 40) + margin; // Below the last row
                    
                    // Create simple path that goes around the bottom edges
                    QPainterPath path;
                    path.moveTo(startX_arrow, startY_arrow);
                    
                    // Step 1: Go right from tail to the right edge
                    int rightEdgeX = widgetWidth - margin;
                    path.lineTo(rightEdgeX, startY_arrow);
                    
                    // Step 2: Go down along the right edge to bottom
                    path.lineTo(rightEdgeX, bottomY);
                    
                    // Step 3: Go left along the bottom edge to below the head
                    path.lineTo(endX_arrow, bottomY);
                    
                    // Step 4: Go up from bottom to the head node
                    path.lineTo(endX_arrow, endY_arrow);
                    
                    painter.drawPath(path);
                    
                    // Draw simple arrowhead outline (no fill) at the end pointing up to first node
                    painter.setPen(QPen(QColor("#9b59b6"), 2));
                    painter.setBrush(Qt::NoBrush);
                    QPolygon arrowHead;
                    arrowHead << QPoint(endX_arrow, endY_arrow)
                              << QPoint(endX_arrow - 6, endY_arrow + 5)
                              << QPoint(endX_arrow + 6, endY_arrow + 5);
                    painter.drawPolygon(arrowHead);
                } else if (!isCircularList) {
                    // Draw NULL with better visibility
                    int nullX = x + boxWidth + 15;
                    int nullY = y + boxHeight / 2;
                    
                    // Draw background box for NULL
                    painter.setPen(QPen(QColor("#95a5a6"), 1));
                    painter.setBrush(QBrush(QColor("#ecf0f1")));
                    QRect nullRect(nullX - 5, nullY - 12, 40, 24);
                    painter.drawRoundedRect(nullRect, 4, 4);
                    
                    // Draw NULL text
                    painter.setPen(QColor("#7f8c8d"));
                    painter.setFont(QFont("Segoe UI", 12, QFont::Bold));
                    painter.drawText(nullRect, Qt::AlignCenter, "NULL");
                }
            }
            
            // For doubly linked list, draw prev pointer
            if (parentVisualizer->m_currentLinkedListType == LinkedListType::Doubly && current->prev != nullptr) {
                painter.setPen(QPen(QColor("#e67e22"), 2, Qt::DashLine));
                int prevX = x - (boxWidth + spacing);
                int prevY = y;
                if (col > 0) {
                    painter.drawLine(x, y + boxHeight / 2, prevX + boxWidth, prevY + boxHeight / 2);
                    QPolygon prevArrow;
                    prevArrow << QPoint(prevX + boxWidth, prevY + boxHeight / 2)
                              << QPoint(prevX + boxWidth + 8, prevY + boxHeight / 2 - 5)
                              << QPoint(prevX + boxWidth + 8, prevY + boxHeight / 2 + 5);
                    painter.setBrush(QBrush(QColor("#e67e22")));
                    painter.drawPolygon(prevArrow);
                }
            }
            
            // Move to next node
            current = current->next;
            if (isCircularList && current == first) {
                break; // Completed the circle
            }
            index++;
        }
    }
};

LinkedListVisualizer::LinkedListVisualizer(QWidget *parent)
    : QWidget(parent),
      m_visualizerWidget(nullptr),
      m_linkedListTypeButtons(new QButtonGroup(this)),
      m_inputField(nullptr),
      m_positionSpinBox(nullptr),
      m_sizeSpinBox(nullptr),
      m_statusLabel(nullptr),
      m_linkedListTypeTitleLabel(nullptr),
      m_linkedListTypeSubtitleLabel(nullptr),
      m_linkedListTypeDescriptionLabel(nullptr),
      m_listStatsLabel(nullptr),
      m_insertBeginButton(nullptr),
      m_insertEndButton(nullptr),
      m_insertPosButton(nullptr),
      m_deleteBeginButton(nullptr),
      m_deleteEndButton(nullptr),
      m_deletePosButton(nullptr),
      m_clearButton(nullptr),
      m_currentLinkedListType(LinkedListType::Singly),
      m_head(nullptr),
      m_tail(nullptr),
      m_size(0),
      m_vizWidget(nullptr),
      m_singlyHead(nullptr),
      m_doublyHead(nullptr),
      m_doublyTail(nullptr),
      m_circularHead(nullptr)
{
    setupUi();
    updateInfoPanel();
    updateLinkedListType(LinkedListType::Singly);
    resetList();
    updateVisualization(tr("Linked list is empty. Use Insert or Randomize to add elements."));
}

void LinkedListVisualizer::setupUi()
{
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet(QStringLiteral("background-color: #f0f4f8; color: #2c3e50; font-family: 'Segoe UI', Arial, sans-serif;"));

    auto outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->setSpacing(0);

    auto scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded); // Show vertical scrollbar when needed
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet(QStringLiteral("QScrollArea { border: none; background-color: #f0f4f8; }"));
    outerLayout->addWidget(scrollArea);

    auto centralContainer = new QWidget();
    centralContainer->setObjectName("centralContainer");
    centralContainer->setStyleSheet(QStringLiteral("QWidget#centralContainer { background-color: #f0f4f8; }"));
    centralContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    scrollArea->setWidget(centralContainer);

    auto mainLayout = new QVBoxLayout(centralContainer);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(20);

    // Header Bar
    auto headerBar = new QFrame(centralContainer);
    headerBar->setStyleSheet(QStringLiteral("QFrame { background-color: #ffffff; border-radius: 12px; border: 1px solid #d8e3f0; }"));
    auto headerLayout = new QHBoxLayout(headerBar);
    headerLayout->setContentsMargins(16, 12, 16, 12);
    headerLayout->setSpacing(16);

    auto headerLabel = new QLabel(tr("Linked List Visualizer"), headerBar);
    headerLabel->setStyleSheet(QStringLiteral("color: #0f3460; font-size: 22px; font-weight: 700;"));
    headerLayout->addWidget(headerLabel);
    headerLayout->addStretch();

    mainLayout->addWidget(headerBar);

    // Linked List Type Navigation Bar
    auto navBar = new QFrame(centralContainer);
    navBar->setStyleSheet(QStringLiteral("QFrame { background-color: #ffffff; border-radius: 12px; border: 1px solid #d8e3f0; }"));
    auto algorithmLayout = new QHBoxLayout(navBar);
    algorithmLayout->setSpacing(10);
    algorithmLayout->setContentsMargins(12, 10, 12, 10);
    setupLinkedListTypeButtons(algorithmLayout);
    mainLayout->addWidget(navBar);

    // Main Content Area
    auto contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(20);
    mainLayout->addLayout(contentLayout, 1);

    // Left Column
    auto leftColumn = new QVBoxLayout();
    leftColumn->setSpacing(16);
    contentLayout->addLayout(leftColumn, 3);

    // Chart Card
    auto chartCard = new QFrame(centralContainer);
    chartCard->setObjectName("chartCard");
    chartCard->setStyleSheet(QStringLiteral(
        "QFrame#chartCard { background-color: #ffffff; border-radius: 16px; border: 1px solid #d8e3f0; }"));
    chartCard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    auto chartLayout = new QVBoxLayout(chartCard);
    chartLayout->setContentsMargins(20, 16, 20, 20);
    chartLayout->setSpacing(8);

    auto chartTitle = new QLabel(tr("Visualization"), chartCard);
    chartTitle->setStyleSheet(QStringLiteral("color: #0f3460; font-size: 15px; font-weight: 600;"));
    chartLayout->addWidget(chartTitle);

    m_vizWidget = new LinkedListVizWidget(this);
    m_vizWidget->parentVisualizer = this;
    m_vizWidget->setMinimumHeight(300);
    m_vizWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    chartLayout->addWidget(m_vizWidget);
    m_visualizerWidget = m_vizWidget;

    leftColumn->addWidget(chartCard, 3);

    // Controls Card
    const QString controlStyle =
        "QPushButton { background-color: #0f3460; color: #ffffff; font-size: 14px; padding: 10px 18px;"
        " border-radius: 8px; border: none; font-weight: 600;"
        " min-height: 40px; max-height: 40px;"
        " min-width: 100px; max-width: 120px; }"
        "QPushButton:hover { background-color: #1a5fb4; }"
        "QPushButton:disabled { background-color: #d0d7e2; color: #7a8ca6; }";

    m_insertBeginButton = new QPushButton(tr("Insert Begin"), this);
    m_insertEndButton = new QPushButton(tr("Insert End"), this);
    m_insertPosButton = new QPushButton(tr("Insert Pos"), this);
    m_deleteBeginButton = new QPushButton(tr("Delete Begin"), this);
    m_deleteEndButton = new QPushButton(tr("Delete End"), this);
    m_deletePosButton = new QPushButton(tr("Delete Pos"), this);
    m_clearButton = new QPushButton(tr("Clear"), this);

    m_insertBeginButton->setStyleSheet(controlStyle);
    m_insertEndButton->setStyleSheet(controlStyle);
    m_insertPosButton->setStyleSheet(controlStyle);
    m_deleteBeginButton->setStyleSheet(controlStyle);
    m_deleteEndButton->setStyleSheet(controlStyle);
    m_deletePosButton->setStyleSheet(controlStyle);
    m_clearButton->setStyleSheet(controlStyle);

    auto controlsCard = new QFrame(centralContainer);
    controlsCard->setObjectName("controlsCard");
    controlsCard->setStyleSheet(QStringLiteral(
        "QFrame#controlsCard { background-color: #ffffff; border-radius: 16px; border: 1px solid #d8e3f0; }"));
    auto controlsLayout = new QVBoxLayout(controlsCard);
    controlsLayout->setContentsMargins(20, 16, 20, 18);
    controlsLayout->setSpacing(14);

    auto insertRow = new QHBoxLayout();
    insertRow->setSpacing(12);
    insertRow->addWidget(m_insertBeginButton);
    insertRow->addWidget(m_insertEndButton);
    insertRow->addWidget(m_insertPosButton);
    insertRow->addStretch();
    controlsLayout->addLayout(insertRow);

    auto deleteRow = new QHBoxLayout();
    deleteRow->setSpacing(12);
    deleteRow->addWidget(m_deleteBeginButton);
    deleteRow->addWidget(m_deleteEndButton);
    deleteRow->addWidget(m_deletePosButton);
    deleteRow->addWidget(m_clearButton);
    deleteRow->addStretch();
    controlsLayout->addLayout(deleteRow);

    leftColumn->addWidget(controlsCard, 1);

    // Input Card
    auto inputCard = new QFrame(centralContainer);
    inputCard->setObjectName("inputCard");
    inputCard->setStyleSheet(QStringLiteral(
        "QFrame#inputCard { background-color: #ffffff; border-radius: 16px; border: 1px solid #d8e3f0; }"));
    auto inputLayout = new QVBoxLayout(inputCard);
    inputLayout->setContentsMargins(20, 18, 20, 18);
    inputLayout->setSpacing(14);

    auto valueLayout = new QHBoxLayout();
    valueLayout->setSpacing(10);
    auto valueLabel = new QLabel(tr("Value"), this);
    valueLabel->setStyleSheet(QStringLiteral("color: #0f3460; font-size: 13px; font-weight: 600;"));
    m_inputField = new QLineEdit(this);
    m_inputField->setPlaceholderText(tr("Enter value"));
    m_inputField->setStyleSheet(QStringLiteral(
        "QLineEdit { background-color: #f9fbff; border: 1px solid #c2d4eb; "
        "border-radius: 8px; color: #2c3e50; padding: 10px 14px; font-size: 13px; min-height: 40px; max-height: 40px; }"
        "QLineEdit:focus { border-color: #0f3460; }"));
    valueLayout->addWidget(valueLabel);
    valueLayout->addWidget(m_inputField, 1);
    inputLayout->addLayout(valueLayout);

    auto positionLayout = new QHBoxLayout();
    positionLayout->setSpacing(10);
    auto positionLabel = new QLabel(tr("Position"), this);
    positionLabel->setStyleSheet(QStringLiteral("color: #0f3460; font-size: 13px; font-weight: 600;"));
    m_positionSpinBox = new QSpinBox(this);
    m_positionSpinBox->setRange(0, 100);
    m_positionSpinBox->setValue(0);
    QString spinBoxInputStyle = QStringLiteral(
        "QSpinBox { background-color: #f9fbff; border: 1px solid #c2d4eb; "
        "border-radius: 8px; color: #2c3e50; padding: 8px; font-size: 13px; min-width: 80px; min-height: 40px; max-height: 40px; "
        "border-top-right-radius: 0; border-bottom-right-radius: 0; }"
        "QSpinBox::up-button, QSpinBox::down-button { width: 0; border: none; background: transparent; }");
    m_positionSpinBox->setStyleSheet(spinBoxInputStyle);
    m_positionSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);

    auto positionButtonContainer = new QWidget(this);
    positionButtonContainer->setStyleSheet(QStringLiteral(
        "QWidget { border: 1px solid #c2d4eb; border-left: none; background-color: #d0d7e2; "
        "border-top-right-radius: 8px; border-bottom-right-radius: 8px; }"));
    positionButtonContainer->setMaximumWidth(20);

    auto positionButtonLayout = new QVBoxLayout(positionButtonContainer);
    positionButtonLayout->setContentsMargins(0, 0, 0, 0);
    positionButtonLayout->setSpacing(0);

    auto positionUpButton = new QPushButton(tr("+"), positionButtonContainer);
    positionUpButton->setStyleSheet(QStringLiteral(
        "QPushButton { background: transparent; color: #0f3460; font-size: 16px; font-weight: 700; border: none; padding: 0; margin: 0; height: 20px; }"
        "QPushButton:hover { background: #b5c1d3; }"));
    auto positionDownButton = new QPushButton(tr("\u2212"), positionButtonContainer);
    positionDownButton->setStyleSheet(QStringLiteral(
        "QPushButton { background: transparent; color: #0f3460; font-size: 16px; font-weight: 700; border: none; padding: 0; margin: 0; height: 20px; }"
        "QPushButton:hover { background: #b5c1d3; }"));

    positionButtonLayout->addWidget(positionUpButton);
    positionButtonLayout->addWidget(positionDownButton);

    auto positionCustomSpinBoxLayout = new QHBoxLayout();
    positionCustomSpinBoxLayout->setSpacing(0);
    positionCustomSpinBoxLayout->addWidget(m_positionSpinBox, 1);
    positionCustomSpinBoxLayout->addWidget(positionButtonContainer);

    connect(positionUpButton, &QPushButton::clicked, m_positionSpinBox, &QSpinBox::stepUp);
    connect(positionDownButton, &QPushButton::clicked, m_positionSpinBox, &QSpinBox::stepDown);

    positionLayout->addWidget(positionLabel);
    positionLayout->addLayout(positionCustomSpinBoxLayout);
    positionLayout->addStretch();
    inputLayout->addLayout(positionLayout);

    auto manualInputLayout = new QHBoxLayout();
    manualInputLayout->setSpacing(10);
    auto manualInputLabel = new QLabel(tr("Values"), this);
    manualInputLabel->setStyleSheet(QStringLiteral("color: #0f3460; font-size: 13px; font-weight: 600;"));
    auto manualInputField = new QLineEdit(this);
    manualInputField->setPlaceholderText(tr("Enter numbers separated by spaces"));
    manualInputField->setStyleSheet(QStringLiteral(
        "QLineEdit { background-color: #f9fbff; border: 1px solid #c2d4eb; "
        "border-radius: 8px; color: #2c3e50; padding: 10px 14px; font-size: 13px; min-height: 40px; max-height: 40px; }"
        "QLineEdit:focus { border-color: #0f3460; }"));
    auto applyButton = new QPushButton(tr("Apply"), this);
    applyButton->setStyleSheet(controlStyle);
    applyButton->setMinimumWidth(120);
    connect(applyButton, &QPushButton::clicked,
            this, &LinkedListVisualizer::handleApplyInput);
    manualInputLayout->addWidget(manualInputLabel);
    manualInputLayout->addWidget(manualInputField, 1);
    manualInputLayout->addWidget(applyButton);
    inputLayout->addLayout(manualInputLayout);
    m_inputField = manualInputField; // Use this for manual input

    auto randomLayout = new QHBoxLayout();
    randomLayout->setSpacing(10);
    auto sizeLabel = new QLabel(tr("Size"), this);
    sizeLabel->setStyleSheet(QStringLiteral("color: #0f3460; font-size: 13px; font-weight: 600;"));
    m_sizeSpinBox = new QSpinBox(this);
    m_sizeSpinBox->setRange(kMinListSize, kMaxListSize);
    m_sizeSpinBox->setValue(5);
    QString sizeSpinBoxInputStyle = QStringLiteral(
        "QSpinBox { background-color: #f9fbff; border: 1px solid #c2d4eb; "
        "border-radius: 8px; color: #2c3e50; padding: 8px; font-size: 13px; min-width: 80px; min-height: 40px; max-height: 40px; "
        "border-top-right-radius: 0; border-bottom-right-radius: 0; }"
        "QSpinBox::up-button, QSpinBox::down-button { width: 0; border: none; background: transparent; }");
    m_sizeSpinBox->setStyleSheet(sizeSpinBoxInputStyle);
    m_sizeSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);

    auto sizeButtonContainer = new QWidget(this);
    sizeButtonContainer->setStyleSheet(QStringLiteral(
        "QWidget { border: 1px solid #c2d4eb; border-left: none; background-color: #d0d7e2; "
        "border-top-right-radius: 8px; border-bottom-right-radius: 8px; }"));
    sizeButtonContainer->setMaximumWidth(20);

    auto sizeButtonLayout = new QVBoxLayout(sizeButtonContainer);
    sizeButtonLayout->setContentsMargins(0, 0, 0, 0);
    sizeButtonLayout->setSpacing(0);

    auto sizeUpButton = new QPushButton(tr("+"), sizeButtonContainer);
    sizeUpButton->setStyleSheet(QStringLiteral(
        "QPushButton { background: transparent; color: #0f3460; font-size: 16px; font-weight: 700; border: none; padding: 0; margin: 0; height: 20px; }"
        "QPushButton:hover { background: #b5c1d3; }"));
    auto sizeDownButton = new QPushButton(tr("\u2212"), sizeButtonContainer);
    sizeDownButton->setStyleSheet(QStringLiteral(
        "QPushButton { background: transparent; color: #0f3460; font-size: 16px; font-weight: 700; border: none; padding: 0; margin: 0; height: 20px; }"
        "QPushButton:hover { background: #b5c1d3; }"));

    sizeButtonLayout->addWidget(sizeUpButton);
    sizeButtonLayout->addWidget(sizeDownButton);

    auto sizeCustomSpinBoxLayout = new QHBoxLayout();
    sizeCustomSpinBoxLayout->setSpacing(0);
    sizeCustomSpinBoxLayout->addWidget(m_sizeSpinBox, 1);
    sizeCustomSpinBoxLayout->addWidget(sizeButtonContainer);

    connect(sizeUpButton, &QPushButton::clicked, m_sizeSpinBox, &QSpinBox::stepUp);
    connect(sizeDownButton, &QPushButton::clicked, m_sizeSpinBox, &QSpinBox::stepDown);

    auto generateButton = new QPushButton(tr("Randomize"), this);
    generateButton->setStyleSheet(controlStyle);
    generateButton->setMinimumWidth(130);
    connect(generateButton, &QPushButton::clicked,
            this, &LinkedListVisualizer::handleGenerateRandom);
    randomLayout->addWidget(sizeLabel);
    randomLayout->addLayout(sizeCustomSpinBoxLayout);
    randomLayout->addSpacing(6);
    randomLayout->addWidget(generateButton);
    randomLayout->addStretch();
    inputLayout->addLayout(randomLayout);

    leftColumn->addWidget(inputCard, 1);
    leftColumn->addStretch(1);
    contentLayout->setStretch(0, 3);

    // Right Column: Info
    auto rightColumn = new QVBoxLayout();
    rightColumn->setSpacing(20);
    contentLayout->addLayout(rightColumn, 2);

    auto infoFrame = new QFrame(centralContainer);
    infoFrame->setObjectName("infoCard");
    infoFrame->setStyleSheet(QStringLiteral(
        "QFrame#infoCard { background-color: #ffffff; border-radius: 16px; border: 1px solid #d8e3f0; }"));
    auto infoLayout = new QVBoxLayout(infoFrame);
    infoLayout->setContentsMargins(24, 28, 24, 24);
    infoLayout->setSpacing(16);

    m_linkedListTypeTitleLabel = new QLabel(this);
    m_linkedListTypeTitleLabel->setStyleSheet(QStringLiteral(
        "color: #0f3460; font-size: 24px; font-weight: 700;"));
    infoLayout->addWidget(m_linkedListTypeTitleLabel);

    m_linkedListTypeSubtitleLabel = new QLabel(this);
    m_linkedListTypeSubtitleLabel->setStyleSheet(QStringLiteral(
        "color: #7a8ca6; font-size: 13px; font-weight: 600;"));
    infoLayout->addWidget(m_linkedListTypeSubtitleLabel);

    auto infoDivider = new QFrame(infoFrame);
    infoDivider->setFrameShape(QFrame::HLine);
    infoDivider->setFrameShadow(QFrame::Plain);
    infoDivider->setStyleSheet(QStringLiteral("QFrame { color: #d8e3f0; }"));
    infoLayout->addWidget(infoDivider);

    m_linkedListTypeDescriptionLabel = new QLabel(this);
    m_linkedListTypeDescriptionLabel->setWordWrap(true);
    m_linkedListTypeDescriptionLabel->setStyleSheet(QStringLiteral(
        "color: #2c3e50; font-size: 13px; line-height: 1.6;"));
    infoLayout->addWidget(m_linkedListTypeDescriptionLabel);

    auto statsContainer = new QFrame(infoFrame);
    statsContainer->setObjectName("statsContainer");
    statsContainer->setStyleSheet(QStringLiteral(
        "QFrame#statsContainer { background-color: #f9fbff; border-radius: 12px; border: 1px solid #d8e3f0; }"));
    auto statsLayout = new QVBoxLayout(statsContainer);
    statsLayout->setContentsMargins(18, 14, 18, 14);
    statsLayout->setSpacing(6);

    m_listStatsLabel = new QLabel(this);
    m_listStatsLabel->setStyleSheet(QStringLiteral(
        "color: #0f3460; font-size: 13px; font-weight: 600;"));
    statsLayout->addWidget(m_listStatsLabel);

    infoLayout->addWidget(statsContainer);

    auto statusPanel = new QFrame(infoFrame);
    statusPanel->setObjectName("statusPanel");
    statusPanel->setStyleSheet(QStringLiteral(
        "QFrame#statusPanel { background-color: #f9fbff; border-radius: 12px; border: 1px solid #d8e3f0; }"));
    auto statusLayout = new QVBoxLayout(statusPanel);
    statusLayout->setContentsMargins(18, 14, 18, 14);
    statusLayout->setSpacing(6);

    auto statusLabelTitle = new QLabel(tr("Live Status"), statusPanel);
    statusLabelTitle->setStyleSheet(QStringLiteral("color: #0f3460; font-size: 13px; font-weight: 600;"));
    statusLayout->addWidget(statusLabelTitle);

    auto statusMessageLabel = new QLabel(this);
    statusMessageLabel->setWordWrap(true);
    statusMessageLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    statusMessageLabel->setStyleSheet(QStringLiteral(
        "color: #2c3e50; font-size: 13px; line-height: 1.55;"));
    statusMessageLabel->setMinimumHeight(64);
    statusLayout->addWidget(statusMessageLabel);
    infoLayout->addWidget(statusPanel);

    m_statusLabel = statusMessageLabel;

    infoLayout->addStretch(1);

    rightColumn->addWidget(infoFrame, 1);
    contentLayout->setStretch(1, 2);

    // Connect signals
    connect(m_linkedListTypeButtons, &QButtonGroup::idClicked,
            this, &LinkedListVisualizer::handleLinkedListTypeButton);
    connect(m_insertBeginButton, &QPushButton::clicked,
            this, &LinkedListVisualizer::handleInsertBeginning);
    connect(m_insertEndButton, &QPushButton::clicked,
            this, &LinkedListVisualizer::handleInsertEnd);
    connect(m_insertPosButton, &QPushButton::clicked,
            this, &LinkedListVisualizer::handleInsertPosition);
    connect(m_deleteBeginButton, &QPushButton::clicked,
            this, &LinkedListVisualizer::handleDeleteBeginning);
    connect(m_deleteEndButton, &QPushButton::clicked,
            this, &LinkedListVisualizer::handleDeleteEnd);
    connect(m_deletePosButton, &QPushButton::clicked,
            this, &LinkedListVisualizer::handleDeletePosition);
    connect(m_clearButton, &QPushButton::clicked,
            this, &LinkedListVisualizer::handleClear);
    connect(m_inputField, &QLineEdit::returnPressed,
            this, &LinkedListVisualizer::handleInsertEnd);
}

void LinkedListVisualizer::setupLinkedListTypeButtons(QHBoxLayout *layout)
{
    m_linkedListTypeButtons->setExclusive(true);
    int index = 0;
    for (const auto &info : kLinkedListTypeInfos) {
        auto button = new QPushButton(info.title, this);
        button->setCheckable(true);
        button->setStyleSheet(
            "QPushButton { background-color: #ffffff; color: #0f3460; padding: 10px 18px; "
            "border-radius: 10px; border: 1px solid #d8e3f0; font-size: 13px; font-weight: 600; }"
            "QPushButton:hover { background-color: #e8f1ff; }"
            "QPushButton:checked { background-color: #0f3460; color: #ffffff; border: 1px solid #0f3460; }");
        m_linkedListTypeButtons->addButton(button, index);
        layout->addWidget(button);

        if (info.type == LinkedListType::Singly) {
            button->setChecked(true);
        }
        ++index;
    }
    layout->addStretch();
}

void LinkedListVisualizer::handleLinkedListTypeButton(int id)
{
    const int clampedId = qBound(0, id, static_cast<int>(kLinkedListTypeInfos.size()) - 1);
    LinkedListType newType = kLinkedListTypeInfos.at(clampedId).type;
    
    if (m_currentLinkedListType != newType) {
        saveCurrentListState();
        loadListState(newType);
        updateLinkedListType(newType);
    }
}

void LinkedListVisualizer::updateLinkedListType(LinkedListType type)
{
    if (m_currentLinkedListType == type)
        return;
    m_currentLinkedListType = type;
    updateInfoPanel();
    updateVisualization();
}

void LinkedListVisualizer::handleBackButton()
{
    emit requestBackToMenu();
}

void LinkedListVisualizer::handleInsertBeginning()
{
    bool ok = false;
    const auto values = parseInputValues(&ok);
    if (!ok || values.empty()) {
        setStatusMessage(tr("Invalid input. Please enter a single integer."));
        return;
    }
    if (values.size() > 1) {
        setStatusMessage(tr("Please enter only one value to insert."));
        return;
    }
    insertAtBeginning(values[0]);
    m_inputField->clear();
}

void LinkedListVisualizer::handleInsertEnd()
{
    bool ok = false;
    const auto values = parseInputValues(&ok);
    if (!ok || values.empty()) {
        setStatusMessage(tr("Invalid input. Please enter a single integer."));
        return;
    }
    if (values.size() > 1) {
        setStatusMessage(tr("Please enter only one value to insert."));
        return;
    }
    insertAtEnd(values[0]);
    m_inputField->clear();
}

void LinkedListVisualizer::handleInsertPosition()
{
    bool ok = false;
    const auto values = parseInputValues(&ok);
    if (!ok || values.empty()) {
        setStatusMessage(tr("Invalid input. Please enter a single integer."));
        return;
    }
    if (values.size() > 1) {
        setStatusMessage(tr("Please enter only one value to insert."));
        return;
    }
    int position = m_positionSpinBox->value();
    if (position < 0 || position > static_cast<int>(m_size)) {
        setStatusMessage(tr("Invalid position. Must be between 0 and %1.").arg(m_size));
        return;
    }
    insertAtPosition(values[0], position);
    m_inputField->clear();
}

void LinkedListVisualizer::handleDeleteBeginning()
{
    if (m_head == nullptr) {
        setStatusMessage(tr("List is empty! Cannot delete."));
        return;
    }
    deleteAtBeginning();
}

void LinkedListVisualizer::handleDeleteEnd()
{
    if (m_head == nullptr) {
        setStatusMessage(tr("List is empty! Cannot delete."));
        return;
    }
    deleteAtEnd();
}

void LinkedListVisualizer::handleDeletePosition()
{
    if (m_head == nullptr) {
        setStatusMessage(tr("List is empty! Cannot delete."));
        return;
    }
    int position = m_positionSpinBox->value();
    if (position < 0 || position >= static_cast<int>(m_size)) {
        setStatusMessage(tr("Invalid position. Must be between 0 and %1.").arg(m_size - 1));
        return;
    }
    deleteAtPosition(position);
}

void LinkedListVisualizer::handleClear()
{
    clearList();
    updateVisualization(tr("List cleared."));
}

void LinkedListVisualizer::handleGenerateRandom()
{
    const auto values = generateRandomValues(m_sizeSpinBox->value());
    clearList();
    for (int value : values) {
        insertAtEnd(value);
    }
    saveCurrentListState();
    setStatusMessage(tr("Generated random list of size %1.").arg(values.size()));
}

void LinkedListVisualizer::handleApplyInput()
{
    bool ok = false;
    const auto values = parseInputValues(&ok);
    if (!ok || values.empty()) {
        setStatusMessage(tr("Invalid input. Please enter integers separated by spaces."));
        return;
    }
    clearList();
    for (int value : values) {
        insertAtEnd(value);
    }
    saveCurrentListState();
    m_inputField->setText(formatArray(values));
    setStatusMessage(tr("Loaded custom list of size %1.").arg(values.size()));
}

void LinkedListVisualizer::updateInfoPanel()
{
    const auto it = std::find_if(kLinkedListTypeInfos.begin(), kLinkedListTypeInfos.end(),
                                 [this](const LinkedListTypeInfo &info) {
                                     return info.type == m_currentLinkedListType;
                                 });
    if (it == kLinkedListTypeInfos.end())
        return;

    m_linkedListTypeTitleLabel->setText(it->title);
    m_linkedListTypeSubtitleLabel->setText(it->subtitle);
    m_linkedListTypeDescriptionLabel->setText(it->description);
}

void LinkedListVisualizer::updateVisualization(const QString &message)
{
    if (m_vizWidget) {
        m_vizWidget->update();
    }
    m_listStatsLabel->setText(tr("List Size: %1").arg(m_size));
    if (!message.isEmpty()) {
        setStatusMessage(message);
    }
}

void LinkedListVisualizer::setStatusMessage(const QString &message)
{
    m_lastStatusMessage = message;
    m_statusLabel->setText(message);
}

void LinkedListVisualizer::resetList()
{
    clearList();
    m_singlyHead = nullptr;
    m_doublyHead = nullptr;
    m_doublyTail = nullptr;
    m_circularHead = nullptr;
}

void LinkedListVisualizer::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    resetList();
    updateVisualization(tr("Linked list is empty. Use Insert or Randomize to add elements."));
}

std::vector<int> LinkedListVisualizer::parseInputValues(bool *ok) const
{
    if (ok) {
        *ok = false;
    }
    const QString text = m_inputField->text();
    const QStringList parts = text.split(' ', Qt::SkipEmptyParts);

    std::vector<int> values;
    values.reserve(parts.size());
    for (const QString &part : parts) {
        bool localOk = false;
        const int value = part.toInt(&localOk);
        if (!localOk) {
            return {};
        }
        values.push_back(value);
    }

    if (values.empty()) {
        return {};
    }

    if (ok) {
        *ok = true;
    }
    return values;
}

std::vector<int> LinkedListVisualizer::generateRandomValues(int size) const
{
    std::vector<int> values;
    values.reserve(size);
    for (int i = 0; i < size; ++i) {
        values.push_back(static_cast<int>(QRandomGenerator::global()->bounded(10, 120)));
    }
    return values;
}

void LinkedListVisualizer::insertAtBeginning(int value)
{
    Node* newNode = new Node(value);
    
    switch (m_currentLinkedListType) {
    case LinkedListType::Singly:
        newNode->next = m_head;
        m_head = newNode;
        break;
    case LinkedListType::Doubly:
        newNode->next = m_head;
        newNode->prev = nullptr;
        if (m_head != nullptr) {
            m_head->prev = newNode;
        } else {
            m_tail = newNode;
        }
        m_head = newNode;
        break;
    case LinkedListType::Circular:
        if (m_head == nullptr) {
            newNode->next = newNode; // Point to itself
            m_head = newNode;
            m_tail = newNode;
        } else {
            newNode->next = m_head;
            m_tail->next = newNode;
            m_head = newNode;
        }
        break;
    }
    m_size++;
    saveCurrentListState();
    updateVisualization(tr("Inserted %1 at beginning.").arg(value));
}

void LinkedListVisualizer::insertAtEnd(int value)
{
    Node* newNode = new Node(value);
    
    switch (m_currentLinkedListType) {
    case LinkedListType::Singly:
        if (m_head == nullptr) {
            m_head = newNode;
        } else {
            Node* current = m_head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
        break;
    case LinkedListType::Doubly:
        newNode->prev = m_tail;
        newNode->next = nullptr;
        if (m_tail != nullptr) {
            m_tail->next = newNode;
        } else {
            m_head = newNode;
        }
        m_tail = newNode;
        break;
    case LinkedListType::Circular:
        if (m_head == nullptr) {
            newNode->next = newNode;
            m_head = newNode;
            m_tail = newNode;
        } else {
            newNode->next = m_head;
            m_tail->next = newNode;
            m_tail = newNode;
        }
        break;
    }
    m_size++;
    saveCurrentListState();
    updateVisualization(tr("Inserted %1 at end.").arg(value));
}

void LinkedListVisualizer::insertAtPosition(int value, int position)
{
    if (position == 0) {
        insertAtBeginning(value);
        return;
    }
    if (position == static_cast<int>(m_size)) {
        insertAtEnd(value);
        return;
    }
    
    Node* newNode = new Node(value);
    Node* current = m_head;
    
    for (int i = 0; i < position - 1; i++) {
        current = current->next;
    }
    
    switch (m_currentLinkedListType) {
    case LinkedListType::Singly:
        newNode->next = current->next;
        current->next = newNode;
        break;
    case LinkedListType::Doubly:
        newNode->next = current->next;
        newNode->prev = current;
        if (current->next != nullptr) {
            current->next->prev = newNode;
        }
        current->next = newNode;
        if (newNode->next == nullptr) {
            m_tail = newNode;
        }
        break;
    case LinkedListType::Circular:
        newNode->next = current->next;
        current->next = newNode;
        if (newNode->next == m_head) {
            m_tail = newNode;
        }
        break;
    }
    m_size++;
    saveCurrentListState();
    updateVisualization(tr("Inserted %1 at position %2.").arg(value).arg(position));
}

void LinkedListVisualizer::deleteAtBeginning()
{
    if (m_head == nullptr) return;
    
    Node* temp = m_head;
    int value = temp->value;
    
    switch (m_currentLinkedListType) {
    case LinkedListType::Singly:
        m_head = m_head->next;
        break;
    case LinkedListType::Doubly:
        m_head = m_head->next;
        if (m_head != nullptr) {
            m_head->prev = nullptr;
        } else {
            m_tail = nullptr;
        }
        break;
    case LinkedListType::Circular:
        if (m_head == m_tail) {
            m_head = nullptr;
            m_tail = nullptr;
        } else {
            m_head = m_head->next;
            m_tail->next = m_head;
        }
        break;
    }
    
    delete temp;
    m_size--;
    saveCurrentListState();
    updateVisualization(tr("Deleted %1 from beginning.").arg(value));
}

void LinkedListVisualizer::deleteAtEnd()
{
    if (m_head == nullptr) return;
    
    int value;
    
    switch (m_currentLinkedListType) {
    case LinkedListType::Singly:
        if (m_head->next == nullptr) {
            value = m_head->value;
            delete m_head;
            m_head = nullptr;
        } else {
            Node* current = m_head;
            while (current->next->next != nullptr) {
                current = current->next;
            }
            value = current->next->value;
            delete current->next;
            current->next = nullptr;
        }
        break;
    case LinkedListType::Doubly: {
        value = m_tail->value;
        Node* temp = m_tail;
        m_tail = m_tail->prev;
        if (m_tail != nullptr) {
            m_tail->next = nullptr;
        } else {
            m_head = nullptr;
        }
        delete temp;
        break;
    }
    case LinkedListType::Circular:
        if (m_head == m_tail) {
            value = m_head->value;
            delete m_head;
            m_head = nullptr;
            m_tail = nullptr;
        } else {
            Node* current = m_head;
            while (current->next != m_tail) {
                current = current->next;
            }
            value = m_tail->value;
            current->next = m_head;
            delete m_tail;
            m_tail = current;
        }
        break;
    }
    
    m_size--;
    saveCurrentListState();
    updateVisualization(tr("Deleted %1 from end.").arg(value));
}

void LinkedListVisualizer::deleteAtPosition(int position)
{
    if (position == 0) {
        deleteAtBeginning();
        return;
    }
    if (position == static_cast<int>(m_size) - 1) {
        deleteAtEnd();
        return;
    }
    
    Node* current = m_head;
    for (int i = 0; i < position - 1; i++) {
        current = current->next;
    }
    
    Node* temp = current->next;
    int value = temp->value;
    
    switch (m_currentLinkedListType) {
    case LinkedListType::Singly:
        current->next = temp->next;
        break;
    case LinkedListType::Doubly:
        current->next = temp->next;
        if (temp->next != nullptr) {
            temp->next->prev = current;
        } else {
            m_tail = current;
        }
        break;
    case LinkedListType::Circular:
        current->next = temp->next;
        if (temp == m_tail) {
            m_tail = current;
        }
        break;
    }
    
    delete temp;
    m_size--;
    saveCurrentListState();
    updateVisualization(tr("Deleted %1 from position %2.").arg(value).arg(position));
}

void LinkedListVisualizer::clearList()
{
    if (m_head == nullptr) {
        m_tail = nullptr;
        m_size = 0;
        updateVisualization();
        return;
    }
    
    // Handle circular list differently
    if (m_currentLinkedListType == LinkedListType::Circular) {
        // Check for single node circular list
        if (m_head->next == m_head) {
            delete m_head;
        } else {
            // Multiple nodes - break the cycle first
            Node* current = m_head;
            Node* first = m_head;
            // Find the last node (the one whose next points to first)
            Node* prev = nullptr;
            do {
                prev = current;
                current = current->next;
            } while (current != first && current != nullptr);
            
            // Break the cycle
            if (prev != nullptr) {
                prev->next = nullptr;
            }
            
            // Now delete all nodes linearly
            current = m_head;
            while (current != nullptr) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }
    } else {
        // Linear list (Singly or Doubly)
        while (m_head != nullptr) {
            Node* temp = m_head;
            m_head = m_head->next;
            delete temp;
        }
    }
    
    m_head = nullptr;
    m_tail = nullptr;
    m_size = 0;
    updateVisualization();
}

void LinkedListVisualizer::clearNodeList(Node*& head)
{
    if (head == nullptr) return;
    
    // Check if it's a single node circular list
    if (head->next == head) {
        delete head;
        head = nullptr;
        return;
    }
    
    // Check if it's circular by seeing if any node's next points back to head
    Node* first = head;
    Node* check = head;
    bool isCircular = false;
    int count = 0;
    const int maxNodes = 1000; // Safety limit to prevent infinite loops
    
    // Traverse and check if we come back to first
    while (check != nullptr && check->next != nullptr && count < maxNodes) {
        if (check->next == first) {
            isCircular = true;
            break;
        }
        check = check->next;
        count++;
        // If we've gone too far without finding a cycle, it's probably linear
        if (count > 100) break;
    }
    
    if (isCircular) {
        // Circular list - break the cycle and delete
        Node* current = head;
        Node* first = head;
        int deleteCount = 0;
        do {
            if (deleteCount++ > maxNodes) break; // Safety check
            Node* temp = current;
            current = current->next;
            temp->next = nullptr; // Break the cycle before deleting
            delete temp;
            if (current == first || current == nullptr) break;
        } while (current != first);
    } else {
        // Linear list
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
    head = nullptr;
}

void LinkedListVisualizer::saveCurrentListState()
{
    // Deep copy the list
    switch (m_currentLinkedListType) {
    case LinkedListType::Singly:
        clearNodeList(m_singlyHead);
        if (m_head != nullptr) {
            Node* current = m_head;
            m_singlyHead = new Node(current->value);
            Node* copyCurrent = m_singlyHead;
            while (current->next != nullptr) {
                current = current->next;
                copyCurrent->next = new Node(current->value);
                copyCurrent = copyCurrent->next;
            }
        }
        break;
    case LinkedListType::Doubly:
        clearNodeList(m_doublyHead);
        if (m_head != nullptr) {
            Node* current = m_head;
            m_doublyHead = new Node(current->value);
            Node* copyCurrent = m_doublyHead;
            Node* prevCopy = nullptr;
            while (current->next != nullptr) {
                current = current->next;
                copyCurrent->next = new Node(current->value);
                copyCurrent->prev = prevCopy;
                prevCopy = copyCurrent;
                copyCurrent = copyCurrent->next;
            }
            copyCurrent->prev = prevCopy;
            m_doublyTail = copyCurrent;
        } else {
            m_doublyTail = nullptr;
        }
        break;
    case LinkedListType::Circular:
        clearNodeList(m_circularHead);
        if (m_head != nullptr) {
            Node* current = m_head;
            m_circularHead = new Node(current->value);
            Node* copyCurrent = m_circularHead;
            Node* first = m_circularHead;
            if (current->next != m_head) { // More than one node
                current = current->next;
                while (current != m_head) {
                    copyCurrent->next = new Node(current->value);
                    copyCurrent = copyCurrent->next;
                    current = current->next;
                }
            }
            copyCurrent->next = first; // Make it circular
        }
        break;
    }
}

void LinkedListVisualizer::loadListState(LinkedListType type)
{
    clearList();
    
    switch (type) {
    case LinkedListType::Singly:
        if (m_singlyHead != nullptr) {
            Node* current = m_singlyHead;
            m_head = new Node(current->value);
            Node* copyCurrent = m_head;
            while (current->next != nullptr) {
                current = current->next;
                copyCurrent->next = new Node(current->value);
                copyCurrent = copyCurrent->next;
            }
            m_size = 0;
            Node* count = m_head;
            while (count != nullptr) {
                m_size++;
                count = count->next;
            }
        }
        break;
    case LinkedListType::Doubly:
        if (m_doublyHead != nullptr) {
            Node* current = m_doublyHead;
            m_head = new Node(current->value);
            Node* copyCurrent = m_head;
            Node* prevCopy = nullptr;
            while (current->next != nullptr) {
                current = current->next;
                copyCurrent->next = new Node(current->value);
                copyCurrent->prev = prevCopy;
                prevCopy = copyCurrent;
                copyCurrent = copyCurrent->next;
            }
            copyCurrent->prev = prevCopy;
            m_tail = copyCurrent;
            m_size = 0;
            Node* count = m_head;
            while (count != nullptr) {
                m_size++;
                count = count->next;
            }
        }
        break;
    case LinkedListType::Circular:
        if (m_circularHead != nullptr) {
            Node* current = m_circularHead;
            m_head = new Node(current->value);
            Node* copyCurrent = m_head;
            Node* first = m_head;
            while (current->next != m_circularHead) {
                current = current->next;
                copyCurrent->next = new Node(current->value);
                copyCurrent = copyCurrent->next;
            }
            copyCurrent->next = first;
            m_tail = copyCurrent;
            m_size = 0;
            Node* count = m_head;
            do {
                m_size++;
                count = count->next;
            } while (count != m_head);
        }
        break;
    }
    updateVisualization();
}

