/**
 * @file QueueVisualizer.cpp
 * @brief Implementation of the QueueVisualizer widget for queue operations visualization
 * @author AlgoVisualizer Team
 * @date 2025
 * 
 * @details
 * This file implements the QueueVisualizer class which provides interactive
 * visualization for queue data structures:
 * - Standard Queue (FIFO - First In, First Out)
 * - Circular Queue (bounded circular buffer)
 * - Deque (Double-Ended Queue - insert/remove from both ends)
 * 
 * Features:
 * - Visual representation of queue operations
 * - Support for multiple queue types
 * - State preservation when switching types
 * - Random value generation and custom input
 */

#include "QueueVisualizer.h"

// --------------------- Qt Widget Includes -----------------------
#include <QButtonGroup>      ///< Groups queue type selection buttons
#include <QHBoxLayout>        ///< Horizontal layout for arranging widgets
#include <QVBoxLayout>        ///< Vertical layout for stacking widgets
#include <QPushButton>        ///< Clickable button widgets
#include <QLineEdit>          ///< Single-line text input for values
#include <QSpinBox>           ///< Number input for random size
#include <QLabel>             ///< Text label for displaying information
#include <QFrame>             ///< Container widget with border styling
#include <QScrollArea>        ///< Scrollable container

// --------------------- Qt Graphics Includes ---------------------
#include <QPainter>           ///< 2D painting system for drawing queue
#include <QLinearGradient>    ///< Linear gradient fills
#include <QBrush>             ///< Brush for filling shapes
#include <QPen>               ///< Pen for drawing outlines
#include <QPolygon>           ///< Polygon shapes for arrows

// --------------------- Qt Core Includes -------------------------
#include <QRandomGenerator>   ///< Generates random numbers
#include <QtMath>             ///< Mathematical functions
#include <QShowEvent>         ///< Event fired when widget becomes visible

// --------------------- Standard Library Includes ----------------
#include <algorithm>          ///< STL algorithms

namespace {

constexpr int kMinQueueSize = 3;
constexpr int kMaxQueueSize = 16;

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

const std::vector<QueueVisualizer::QueueTypeInfo> QueueVisualizer::kQueueTypeInfos = {
    {QueueType::Queue,
     QObject::tr("Queue"),
     QObject::tr("First In First Out (FIFO)"),
     QObject::tr("A Queue is a linear data structure that follows the First In First Out (FIFO) principle. "
                 "Elements are added (enqueued) at the rear and removed (dequeued) from the front. "
                 "The first element added is always the first to be removed.")},
    {QueueType::CircularQueue,
     QObject::tr("Circular Queue"),
     QObject::tr("FIFO with circular buffer"),
     QObject::tr("A Circular Queue is a variation of a queue where the last position is connected back to the first position. "
                 "This allows efficient use of memory by reusing space when elements are dequeued.")},
    {QueueType::Deque,
     QObject::tr("Deque"),
     QObject::tr("Double-ended queue"),
     QObject::tr("A Deque (Double-ended Queue) allows insertion and deletion from both ends. "
                 "It combines the features of both stack and queue, providing flexibility in data manipulation.")}
};

// Custom visualization widget for Queue
class QueueVisualizer::QueueVizWidget : public QWidget {
public:
    QueueVisualizer* parentVisualizer;
    QueueVizWidget(QWidget* parent) : QWidget(parent), parentVisualizer(nullptr) {
        setMinimumHeight(200);
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
        
        int startX = 50;
        int startY = height() / 2 - 35;
        int baseBoxWidth = 90;
        int baseBoxHeight = 70;
        int baseSpacing = 10;
        
        // Calculate available width
        int availableWidth = width() - startX - 40; // 40px right margin
        int minBoxWidth = 50; // Minimum box width
        int minSpacing = 8;   // Minimum spacing between boxes
        
        // Get active queue elements
        std::vector<int> displayQueue;
        if (parentVisualizer->m_currentQueueType == QueueType::CircularQueue) {
            if (parentVisualizer->m_rear >= parentVisualizer->m_front) {
                for (int i = parentVisualizer->m_front; i <= parentVisualizer->m_rear; ++i) {
                    if (parentVisualizer->m_queue[i] != 0) {
                        displayQueue.push_back(parentVisualizer->m_queue[i]);
                    }
                }
            } else if (parentVisualizer->m_rear != -1) {
                for (int i = parentVisualizer->m_front; i < parentVisualizer->m_capacity; ++i) {
                    if (parentVisualizer->m_queue[i] != 0) {
                        displayQueue.push_back(parentVisualizer->m_queue[i]);
                    }
                }
                for (int i = 0; i <= parentVisualizer->m_rear; ++i) {
                    if (parentVisualizer->m_queue[i] != 0) {
                        displayQueue.push_back(parentVisualizer->m_queue[i]);
                    }
                }
            }
        } else {
            for (int val : parentVisualizer->m_queue) {
                if (val != 0) {
                    displayQueue.push_back(val);
                }
            }
        }
        
        // Calculate dynamic sizing
        int boxWidth = baseBoxWidth;
        int boxHeight = baseBoxHeight;
        int spacing = baseSpacing;
        int totalElements = static_cast<int>(displayQueue.size());
        
        if (totalElements > 0) {
            // Calculate total width needed with base sizing
            int totalWidthNeeded = startX + totalElements * (baseBoxWidth + baseSpacing) - baseSpacing + 40;
            
            // If elements don't fit, scale down
            if (totalWidthNeeded > availableWidth + startX) {
                // Calculate how much space we have per element
                int spacePerElement = availableWidth / totalElements;
                
                // Ensure minimum sizes are respected
                if (spacePerElement < minBoxWidth + minSpacing) {
                    // Use minimum sizes - elements will be tight but visible
                    spacing = minSpacing;
                    boxWidth = (availableWidth - (totalElements - 1) * spacing) / totalElements;
                    if (boxWidth < minBoxWidth) {
                        // Even with minimum spacing, we need smaller boxes
                        boxWidth = qMax(30, (availableWidth - (totalElements - 1) * minSpacing) / totalElements);
                        spacing = minSpacing;
                    }
                } else {
                    // Scale proportionally (85% box, 15% spacing)
                    boxWidth = (spacePerElement * 85) / 100;
                    spacing = (spacePerElement * 15) / 100;
                    
                    // Ensure minimums
                    if (boxWidth < minBoxWidth) {
                        boxWidth = minBoxWidth;
                        spacing = (availableWidth - totalElements * boxWidth) / (totalElements - 1);
                        if (spacing < minSpacing) spacing = minSpacing;
                    }
                    if (spacing < minSpacing) {
                        spacing = minSpacing;
                        boxWidth = (availableWidth - (totalElements - 1) * spacing) / totalElements;
                        if (boxWidth < 30) boxWidth = 30; // Absolute minimum
                    }
                }
                
                // Final check: ensure everything fits
                int finalWidthNeeded = startX + totalElements * (boxWidth + spacing) - spacing + 40;
                if (finalWidthNeeded > availableWidth + startX) {
                    // Still doesn't fit, scale down more aggressively
                    int maxAllowedWidth = availableWidth + startX - 40;
                    boxWidth = (maxAllowedWidth - startX - (totalElements - 1) * minSpacing) / totalElements;
                    if (boxWidth < 30) boxWidth = 30;
                    spacing = minSpacing;
                }
            }
        }
        
        // Calculate required height
        int requiredHeight = startY + boxHeight + 60; // 60px for labels and margins
        int minRequiredHeight = qMax(200, requiredHeight);
        
        // Update widget minimum height
        if (parentVisualizer && parentVisualizer->m_vizWidget) {
            if (parentVisualizer->m_vizWidget->minimumHeight() != minRequiredHeight) {
                parentVisualizer->m_vizWidget->setMinimumHeight(minRequiredHeight);
                parentVisualizer->m_vizWidget->updateGeometry();
            }
        }
        
        if (displayQueue.empty()) {
            int emptyBoxX = startX;
            int emptyBoxY = startY;
            int emptyBoxWidth = boxWidth * 2;
            
            // Draw empty queue box
            painter.setPen(QPen(QColor("#95a5a6"), 2, Qt::DashLine));
            painter.setBrush(QBrush(QColor("#ecf0f1")));
            painter.drawRoundedRect(emptyBoxX, emptyBoxY, emptyBoxWidth, boxHeight, 10, 10);
            
            // Draw NULL text
            painter.setPen(QColor("#7f8c8d"));
            painter.setFont(QFont("Segoe UI", 12, QFont::Bold));
            painter.drawText(QRect(emptyBoxX, emptyBoxY, emptyBoxWidth, boxHeight), Qt::AlignCenter, "NULL");
            
            // Draw FRONT pointer
            int frontPointerX = emptyBoxX + emptyBoxWidth / 2;
            int frontPointerY = emptyBoxY - 30;
            painter.setPen(QPen(QColor("#27ae60"), 3));
            painter.setFont(QFont("Segoe UI", 10, QFont::Bold));
            painter.drawText(frontPointerX - 25, frontPointerY - 5, "FRONT");
            painter.drawLine(frontPointerX, frontPointerY, frontPointerX, emptyBoxY);
            QPolygon frontArrow;
            frontArrow << QPoint(frontPointerX, emptyBoxY)
                       << QPoint(frontPointerX - 5, emptyBoxY - 8)
                       << QPoint(frontPointerX + 5, emptyBoxY - 8);
            painter.setBrush(QBrush(QColor("#27ae60")));
            painter.drawPolygon(frontArrow);
            
            // Draw REAR pointer
            int rearPointerX = emptyBoxX + emptyBoxWidth / 2;
            int rearPointerY = emptyBoxY + boxHeight + 30;
            painter.setPen(QPen(QColor("#f39c12"), 3));
            painter.setFont(QFont("Segoe UI", 10, QFont::Bold));
            painter.drawText(rearPointerX - 20, rearPointerY + 15, "REAR");
            painter.drawLine(rearPointerX, rearPointerY, rearPointerX, emptyBoxY + boxHeight);
            QPolygon rearArrow;
            rearArrow << QPoint(rearPointerX, emptyBoxY + boxHeight)
                      << QPoint(rearPointerX - 5, emptyBoxY + boxHeight + 8)
                      << QPoint(rearPointerX + 5, emptyBoxY + boxHeight + 8);
            painter.setBrush(QBrush(QColor("#f39c12")));
            painter.drawPolygon(rearArrow);
            return;
        }
        
        // Draw queue from front to rear (linear layout for all types)
        for (size_t i = 0; i < displayQueue.size(); i++) {
            int x = startX + static_cast<int>(i) * (boxWidth + spacing);
            
            // Highlight front and rear with gradients
            if (i == 0) {
                painter.setPen(QPen(QColor("#27ae60"), 3));
                QLinearGradient gradient(x, startY, x, startY + boxHeight);
                gradient.setColorAt(0, QColor("#a8e6cf"));
                gradient.setColorAt(1, QColor("#7dcea0"));
                painter.setBrush(QBrush(gradient));
            } else if (i == displayQueue.size() - 1) {
                painter.setPen(QPen(QColor("#f39c12"), 3));
                QLinearGradient gradient(x, startY, x, startY + boxHeight);
                gradient.setColorAt(0, QColor("#fdeaa7"));
                gradient.setColorAt(1, QColor("#f7dc6f"));
                painter.setBrush(QBrush(gradient));
            } else {
                painter.setPen(QPen(QColor("#3498db"), 2));
                QLinearGradient gradient(x, startY, x, startY + boxHeight);
                gradient.setColorAt(0, QColor("#d6eaf8"));
                gradient.setColorAt(1, QColor("#aed6f1"));
                painter.setBrush(QBrush(gradient));
            }
            
            painter.drawRoundedRect(x, startY, boxWidth, boxHeight, 10, 10);
            
            // Draw value
            painter.setPen(QColor("#2c3e50"));
            // Scale font size based on box width
            int fontSize = qMax(10, qMin(12, boxWidth / 8));
            painter.setFont(QFont("Segoe UI", fontSize, QFont::Bold));
            painter.drawText(QRect(x, startY, boxWidth, boxHeight), Qt::AlignCenter, 
                           QString::number(displayQueue[i]));
            
            // Draw labels
            if (i == 0) {
                painter.setPen(QColor("#27ae60"));
                painter.setFont(QFont("Segoe UI", 10, QFont::Bold));
                painter.drawText(x + boxWidth / 2 - 25, startY - 20, "FRONT");
            }
            if (i == displayQueue.size() - 1) {
                painter.setPen(QColor("#f39c12"));
                painter.setFont(QFont("Segoe UI", 10, QFont::Bold));
                painter.drawText(x + boxWidth / 2 - 20, startY + boxHeight + 15, "REAR");
            }
        }
    }
};

QueueVisualizer::QueueVisualizer(QWidget *parent)
    : QWidget(parent),
      m_visualizerWidget(nullptr),
      m_queueTypeButtons(new QButtonGroup(this)),
      m_inputField(nullptr),
      m_sizeSpinBox(nullptr),
      m_statusLabel(nullptr),
      m_queueTypeTitleLabel(nullptr),
      m_queueTypeSubtitleLabel(nullptr),
      m_queueTypeDescriptionLabel(nullptr),
      m_queueStatsLabel(nullptr),
      m_enqueueButton(nullptr),
      m_dequeueButton(nullptr),
      m_enqueueFrontButton(nullptr),
      m_dequeueFrontButton(nullptr),
      m_clearButton(nullptr),
      m_currentQueueType(QueueType::Queue),
      m_front(0),
      m_rear(-1),
      m_capacity(kMaxQueueSize),
      m_vizWidget(nullptr),
      m_queueFront(0),
      m_queueRear(-1),
      m_circularFront(0),
      m_circularRear(-1),
      m_dequeFront(0),
      m_dequeRear(-1)
{
    m_queue.resize(m_capacity, 0);
    m_queueData.clear();
    m_circularQueueData.resize(m_capacity, 0);
    m_dequeData.clear();
    setupUi();
    updateInfoPanel();
    updateQueueType(QueueType::Queue);
    // Start with empty queue
    resetQueueData();
    updateVisualization(tr("Queue is empty. Use Enqueue or Randomize to add elements."));
}

void QueueVisualizer::setupUi()
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

    auto headerLabel = new QLabel(tr("Queue Visualizer"), headerBar);
    headerLabel->setStyleSheet(QStringLiteral("color: #0f3460; font-size: 22px; font-weight: 700;"));
    headerLayout->addWidget(headerLabel);
    headerLayout->addStretch();

    mainLayout->addWidget(headerBar);

    // Queue Type Navigation Bar
    auto navBar = new QFrame(centralContainer);
    navBar->setStyleSheet(QStringLiteral("QFrame { background-color: #ffffff; border-radius: 12px; border: 1px solid #d8e3f0; }"));
    auto algorithmLayout = new QHBoxLayout(navBar);
    algorithmLayout->setSpacing(10);
    algorithmLayout->setContentsMargins(12, 10, 12, 10);
    setupQueueTypeButtons(algorithmLayout);
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
    auto chartCard = new QFrame(this);
    chartCard->setObjectName("chartCard");
    chartCard->setStyleSheet(QStringLiteral(
        "QFrame#chartCard { background-color: #ffffff; border-radius: 16px; border: 1px solid #d8e3f0; }"));
    chartCard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto chartLayout = new QVBoxLayout(chartCard);
    chartLayout->setContentsMargins(20, 16, 20, 20);
    chartLayout->setSpacing(8);

    auto chartTitle = new QLabel(tr("Visualization"), chartCard);
    chartTitle->setStyleSheet(QStringLiteral("color: #0f3460; font-size: 15px; font-weight: 600;"));
    chartLayout->addWidget(chartTitle);

    m_vizWidget = new QueueVizWidget(this);
    m_vizWidget->parentVisualizer = this;
    m_vizWidget->setMinimumHeight(200);
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

    m_enqueueButton = new QPushButton(tr("Enqueue"), this);
    m_dequeueButton = new QPushButton(tr("Dequeue"), this);
    m_enqueueFrontButton = new QPushButton(tr("Enqueue Front"), this);
    m_dequeueFrontButton = new QPushButton(tr("Dequeue Front"), this);
    m_clearButton = new QPushButton(tr("Clear"), this);

    m_enqueueButton->setStyleSheet(controlStyle);
    m_dequeueButton->setStyleSheet(controlStyle);
    m_enqueueFrontButton->setStyleSheet(controlStyle);
    m_dequeueFrontButton->setStyleSheet(controlStyle);
    m_clearButton->setStyleSheet(controlStyle);

    m_enqueueFrontButton->setVisible(false);
    m_dequeueFrontButton->setVisible(false);

    auto controlsCard = new QFrame(this);
    controlsCard->setObjectName("controlsCard");
    controlsCard->setStyleSheet(QStringLiteral(
        "QFrame#controlsCard { background-color: #ffffff; border-radius: 16px; border: 1px solid #d8e3f0; }"));
    auto controlsLayout = new QVBoxLayout(controlsCard);
    controlsLayout->setContentsMargins(20, 16, 20, 18);
    controlsLayout->setSpacing(14);

    auto buttonRow1 = new QHBoxLayout();
    buttonRow1->setSpacing(12);
    buttonRow1->addWidget(m_enqueueButton);
    buttonRow1->addWidget(m_dequeueButton);
    buttonRow1->addWidget(m_clearButton);
    buttonRow1->addStretch();
    controlsLayout->addLayout(buttonRow1);

    auto buttonRow2 = new QHBoxLayout();
    buttonRow2->setSpacing(12);
    buttonRow2->addWidget(m_enqueueFrontButton);
    buttonRow2->addWidget(m_dequeueFrontButton);
    buttonRow2->addStretch();
    controlsLayout->addLayout(buttonRow2);

    leftColumn->addWidget(controlsCard, 1);

    // Input Card (same as StackVisualizer)
    auto inputCard = new QFrame(this);
    inputCard->setObjectName("inputCard");
    inputCard->setStyleSheet(QStringLiteral(
        "QFrame#inputCard { background-color: #ffffff; border-radius: 16px; border: 1px solid #d8e3f0; }"));
    auto inputLayout = new QVBoxLayout(inputCard);
    inputLayout->setContentsMargins(20, 18, 20, 18);
    inputLayout->setSpacing(14);

    m_inputField = new QLineEdit(this);
    m_inputField->setPlaceholderText(tr("Enter numbers separated by spaces"));
    m_inputField->setStyleSheet(QStringLiteral(
        "QLineEdit { background-color: #f9fbff; border: 1px solid #c2d4eb; "
        "border-radius: 8px; color: #2c3e50; padding: 10px 14px; font-size: 13px; min-height: 40px; max-height: 40px; }"
        "QLineEdit:focus { border-color: #0f3460; }"));
    auto applyButton = new QPushButton(tr("Apply"), this);
    applyButton->setStyleSheet(controlStyle);
    applyButton->setMinimumWidth(120);
    connect(applyButton, &QPushButton::clicked,
            this, &QueueVisualizer::handleApplyInput);

    auto manualInputLayout = new QHBoxLayout();
    manualInputLayout->setSpacing(10);
    manualInputLayout->addWidget(m_inputField, 1);
    manualInputLayout->addWidget(applyButton);
    inputLayout->addLayout(manualInputLayout);

    auto randomLayout = new QHBoxLayout();
    randomLayout->setSpacing(10);

    auto sizeLabel = new QLabel(tr("Size"), this);
    sizeLabel->setStyleSheet(QStringLiteral("color: #0f3460; font-size: 13px; font-weight: 600;"));
    randomLayout->addWidget(sizeLabel);

    m_sizeSpinBox = new QSpinBox(this);
    m_sizeSpinBox->setRange(kMinQueueSize, kMaxQueueSize);
    m_sizeSpinBox->setValue(5);
    QString spinBoxInputStyle = QStringLiteral(
        "QSpinBox { background-color: #f9fbff; border: 1px solid #c2d4eb; "
        "border-radius: 8px; color: #2c3e50; padding: 8px; font-size: 13px; min-width: 80px; min-height: 40px; max-height: 40px; "
        "border-top-right-radius: 0; border-bottom-right-radius: 0; }"
        "QSpinBox::up-button, QSpinBox::down-button { width: 0; border: none; background: transparent; }");
    m_sizeSpinBox->setStyleSheet(spinBoxInputStyle);
    m_sizeSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);

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
    customSpinBoxLayout->addWidget(m_sizeSpinBox, 1);
    customSpinBoxLayout->addWidget(buttonContainer);

    randomLayout->addLayout(customSpinBoxLayout);
    randomLayout->addSpacing(6);

    auto generateButton = new QPushButton(tr("Randomize"), this);
    generateButton->setStyleSheet(controlStyle);
    generateButton->setMinimumWidth(130);
    connect(generateButton, &QPushButton::clicked,
            this, &QueueVisualizer::handleGenerateRandom);

    connect(upButton, &QPushButton::clicked, m_sizeSpinBox, &QSpinBox::stepUp);
    connect(downButton, &QPushButton::clicked, m_sizeSpinBox, &QSpinBox::stepDown);

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

    auto infoFrame = new QFrame(this);
    infoFrame->setObjectName("infoCard");
    infoFrame->setStyleSheet(QStringLiteral(
        "QFrame#infoCard { background-color: #ffffff; border-radius: 16px; border: 1px solid #d8e3f0; }"));
    auto infoLayout = new QVBoxLayout(infoFrame);
    infoLayout->setContentsMargins(24, 28, 24, 24);
    infoLayout->setSpacing(16);

    m_queueTypeTitleLabel = new QLabel(this);
    m_queueTypeTitleLabel->setStyleSheet(QStringLiteral(
        "color: #0f3460; font-size: 24px; font-weight: 700;"));
    infoLayout->addWidget(m_queueTypeTitleLabel);

    m_queueTypeSubtitleLabel = new QLabel(this);
    m_queueTypeSubtitleLabel->setStyleSheet(QStringLiteral(
        "color: #7a8ca6; font-size: 13px; font-weight: 600;"));
    infoLayout->addWidget(m_queueTypeSubtitleLabel);

    auto infoDivider = new QFrame(infoFrame);
    infoDivider->setFrameShape(QFrame::HLine);
    infoDivider->setFrameShadow(QFrame::Plain);
    infoDivider->setStyleSheet(QStringLiteral("QFrame { color: #d8e3f0; }"));
    infoLayout->addWidget(infoDivider);

    m_queueTypeDescriptionLabel = new QLabel(this);
    m_queueTypeDescriptionLabel->setWordWrap(true);
    m_queueTypeDescriptionLabel->setStyleSheet(QStringLiteral(
        "color: #2c3e50; font-size: 13px; line-height: 1.6;"));
    infoLayout->addWidget(m_queueTypeDescriptionLabel);

    auto statsContainer = new QFrame(infoFrame);
    statsContainer->setObjectName("statsContainer");
    statsContainer->setStyleSheet(QStringLiteral(
        "QFrame#statsContainer { background-color: #f9fbff; border-radius: 12px; border: 1px solid #d8e3f0; }"));
    auto statsLayout = new QVBoxLayout(statsContainer);
    statsLayout->setContentsMargins(18, 14, 18, 14);
    statsLayout->setSpacing(6);

    m_queueStatsLabel = new QLabel(this);
    m_queueStatsLabel->setStyleSheet(QStringLiteral(
        "color: #0f3460; font-size: 13px; font-weight: 600;"));
    statsLayout->addWidget(m_queueStatsLabel);

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
    connect(m_queueTypeButtons, &QButtonGroup::idClicked,
            this, &QueueVisualizer::handleQueueTypeButton);
    connect(m_enqueueButton, &QPushButton::clicked,
            this, &QueueVisualizer::handleEnqueue);
    connect(m_dequeueButton, &QPushButton::clicked,
            this, &QueueVisualizer::handleDequeue);
    connect(m_enqueueFrontButton, &QPushButton::clicked,
            this, &QueueVisualizer::handleEnqueue);
    connect(m_dequeueFrontButton, &QPushButton::clicked,
            this, &QueueVisualizer::handleDequeue);
    connect(m_clearButton, &QPushButton::clicked,
            this, &QueueVisualizer::handleClear);
    connect(m_inputField, &QLineEdit::returnPressed,
            this, &QueueVisualizer::handleApplyInput);
}

void QueueVisualizer::setupQueueTypeButtons(QHBoxLayout *layout)
{
    m_queueTypeButtons->setExclusive(true);
    int index = 0;
    for (const auto &info : kQueueTypeInfos) {
        auto button = new QPushButton(info.title, this);
        button->setCheckable(true);
        button->setStyleSheet(
            "QPushButton { background-color: #ffffff; color: #0f3460; padding: 10px 18px; "
            "border-radius: 10px; border: 1px solid #d8e3f0; font-size: 13px; font-weight: 600; }"
            "QPushButton:hover { background-color: #e8f1ff; }"
            "QPushButton:checked { background-color: #0f3460; color: #ffffff; border: 1px solid #0f3460; }");
        m_queueTypeButtons->addButton(button, index);
        layout->addWidget(button);

        if (info.type == QueueType::Queue) {
            button->setChecked(true);
        }
        ++index;
    }
    layout->addStretch();
}

void QueueVisualizer::handleQueueTypeButton(int id)
{
    const int clampedId = qBound(0, id, static_cast<int>(kQueueTypeInfos.size()) - 1);
    QueueType newType = kQueueTypeInfos.at(clampedId).type;
    
    if (m_currentQueueType != newType) {
        // Save current state before switching
        saveCurrentQueueState();
        // Load the new queue type's state
        loadQueueState(newType);
        updateQueueType(newType);
    }
}

void QueueVisualizer::updateQueueType(QueueType type)
{
    if (m_currentQueueType == type)
        return;
    m_currentQueueType = type;
    updateInfoPanel();
    
    // Show/hide deque buttons
    bool isDeque = (type == QueueType::Deque);
    m_enqueueFrontButton->setVisible(isDeque);
    m_dequeueFrontButton->setVisible(isDeque);
    
    updateVisualization();
}

void QueueVisualizer::saveCurrentQueueState()
{
    switch (m_currentQueueType) {
    case QueueType::Queue:
        m_queueData = m_queue;
        m_queueFront = m_front;
        m_queueRear = m_rear;
        break;
    case QueueType::CircularQueue:
        m_circularQueueData = m_queue;
        m_circularFront = m_front;
        m_circularRear = m_rear;
        break;
    case QueueType::Deque:
        m_dequeData = m_queue;
        m_dequeFront = m_front;
        m_dequeRear = m_rear;
        break;
    }
}

void QueueVisualizer::loadQueueState(QueueType type)
{
    switch (type) {
    case QueueType::Queue:
        m_queue = m_queueData;
        m_front = m_queueFront;
        m_rear = m_queueRear;
        break;
    case QueueType::CircularQueue:
        m_queue = m_circularQueueData;
        m_front = m_circularFront;
        m_rear = m_circularRear;
        break;
    case QueueType::Deque:
        m_queue = m_dequeData;
        m_front = m_dequeFront;
        m_rear = m_dequeRear;
        break;
    }
}

void QueueVisualizer::resetQueueData()
{
    m_queue.clear();
    m_queue.resize(m_capacity, 0);
    m_front = 0;
    m_rear = -1;
    
    // Reset all queue types
    m_queueData.clear();
    m_circularQueueData.clear();
    m_circularQueueData.resize(m_capacity, 0);
    m_dequeData.clear();
    m_queueFront = 0;
    m_queueRear = -1;
    m_circularFront = 0;
    m_circularRear = -1;
    m_dequeFront = 0;
    m_dequeRear = -1;
}

void QueueVisualizer::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    // Reset to empty when page is shown
    resetQueueData();
    updateVisualization(tr("Queue is empty. Use Enqueue or Randomize to add elements."));
}

void QueueVisualizer::handleBackButton()
{
    emit requestBackToMenu();
}

void QueueVisualizer::handleEnqueue()
{
    bool ok = false;
    const auto values = parseInputValues(&ok);
    if (!ok || values.empty()) {
        setStatusMessage(tr("Invalid input. Please enter a single integer."));
        return;
    }
    if (values.size() > 1) {
        setStatusMessage(tr("Please enter only one value to enqueue."));
        return;
    }
    
    switch (m_currentQueueType) {
    case QueueType::Queue:
        enqueueQueue(values[0]);
        break;
    case QueueType::CircularQueue:
        enqueueCircularQueue(values[0]);
        break;
    case QueueType::Deque:
        enqueueDequeRear(values[0]);
        break;
    }
    m_inputField->clear();
}

void QueueVisualizer::handleDequeue()
{
    int value = -1;
    bool success = false;
    
    switch (m_currentQueueType) {
    case QueueType::Queue:
        value = dequeueQueue();
        success = (value != -1);
        break;
    case QueueType::CircularQueue:
        value = dequeueCircularQueue();
        success = (value != -1);
        break;
    case QueueType::Deque:
        value = dequeueDequeFront();
        success = (value != -1);
        break;
    }
    
    if (!success) {
        setStatusMessage(tr("Queue is empty! Cannot dequeue."));
        return;
    }
    updateVisualization(tr("Dequeued %1 from queue.").arg(value));
}

void QueueVisualizer::handleClear()
{
    m_queue.clear();
    m_queue.resize(m_capacity, 0);
    m_front = 0;
    m_rear = -1;
    // Save state after clearing
    saveCurrentQueueState();
    updateVisualization(tr("Queue cleared."));
}

void QueueVisualizer::handleGenerateRandom()
{
    const auto values = generateRandomValues(m_sizeSpinBox->value());
    setQueue(values);
    // Save state after generating
    saveCurrentQueueState();
    setStatusMessage(tr("Generated random queue of size %1.").arg(values.size()));
}

void QueueVisualizer::handleApplyInput()
{
    bool ok = false;
    const auto values = parseInputValues(&ok);
    if (!ok || values.empty()) {
        setStatusMessage(tr("Invalid input. Please enter integers separated by spaces."));
        return;
    }
    setQueue(values, false);
    // Save state after applying input
    saveCurrentQueueState();
    m_inputField->setText(formatArray(values));
    setStatusMessage(tr("Loaded custom queue of size %1.").arg(values.size()));
}

void QueueVisualizer::updateInfoPanel()
{
    const auto it = std::find_if(kQueueTypeInfos.begin(), kQueueTypeInfos.end(),
                                 [this](const QueueTypeInfo &info) {
                                     return info.type == m_currentQueueType;
                                 });
    if (it == kQueueTypeInfos.end())
        return;

    m_queueTypeTitleLabel->setText(it->title);
    m_queueTypeSubtitleLabel->setText(it->subtitle);
    m_queueTypeDescriptionLabel->setText(it->description);
}

void QueueVisualizer::updateVisualization(const QString &message)
{
    if (m_vizWidget) {
        m_vizWidget->update();
    }
    
    // Get active queue elements for stats
    std::vector<int> displayValues;
    int size = 0;
    if (m_currentQueueType == QueueType::CircularQueue) {
        if (m_rear >= m_front) {
            size = m_rear - m_front + 1;
            for (int i = m_front; i <= m_rear; ++i) {
                if (m_queue[i] != 0) {
                    displayValues.push_back(m_queue[i]);
                }
            }
        } else if (m_rear != -1) {
            size = (m_capacity - m_front) + (m_rear + 1);
            for (int i = m_front; i < m_capacity; ++i) {
                if (m_queue[i] != 0) {
                    displayValues.push_back(m_queue[i]);
                }
            }
            for (int i = 0; i <= m_rear; ++i) {
                if (m_queue[i] != 0) {
                    displayValues.push_back(m_queue[i]);
                }
            }
        }
    } else {
        for (int val : m_queue) {
            if (val != 0) {
                displayValues.push_back(val);
            }
        }
        size = static_cast<int>(displayValues.size());
    }
    
    m_queueStatsLabel->setText(tr("Queue Size: %1%2%3")
                                   .arg(size)
                                   .arg(displayValues.empty() ? "" : tr(" | Front: %1").arg(displayValues[0]))
                                   .arg(displayValues.size() < 2 ? "" : tr(" | Rear: %1").arg(displayValues.back())));
    if (!message.isEmpty()) {
        setStatusMessage(message);
    }
}

void QueueVisualizer::setStatusMessage(const QString &message)
{
    m_lastStatusMessage = message;
    m_statusLabel->setText(message);
}

void QueueVisualizer::setQueue(const std::vector<int> &values, bool updateInputField)
{
    m_queue.clear();
    m_queue.resize(m_capacity, 0);
    for (size_t i = 0; i < values.size() && i < static_cast<size_t>(m_capacity); ++i) {
        m_queue[i] = values[i];
    }
    m_front = 0;
    m_rear = static_cast<int>(values.size()) - 1;
    if (updateInputField) {
        m_inputField->setText(formatArray(values));
    }
    if (m_sizeSpinBox && values.size() >= static_cast<size_t>(kMinQueueSize) &&
        values.size() <= static_cast<size_t>(kMaxQueueSize)) {
        m_sizeSpinBox->setValue(static_cast<int>(values.size()));
    }
    resetState();
}

void QueueVisualizer::resetState()
{
    // Don't clear data, just update visualization
    updateVisualization(tr("Queue ready."));
}

std::vector<int> QueueVisualizer::parseInputValues(bool *ok) const
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

std::vector<int> QueueVisualizer::generateRandomValues(int size) const
{
    std::vector<int> values;
    values.reserve(size);
    for (int i = 0; i < size; ++i) {
        values.push_back(static_cast<int>(QRandomGenerator::global()->bounded(10, 120)));
    }
    return values;
}

void QueueVisualizer::enqueueQueue(int value)
{
    if (m_queue.size() >= static_cast<size_t>(m_capacity)) {
        setStatusMessage(tr("Queue is full! Maximum size is %1.").arg(m_capacity));
        return;
    }
    m_queue.push_back(value);
    m_rear = static_cast<int>(m_queue.size()) - 1;
    // Save state after operation
    saveCurrentQueueState();
    updateVisualization(tr("Enqueued %1 to queue.").arg(value));
}

int QueueVisualizer::dequeueQueue()
{
    if (m_queue.empty()) {
        return -1;
    }
    int value = m_queue[0];
    m_queue.erase(m_queue.begin());
    m_rear = static_cast<int>(m_queue.size()) - 1;
    // Save state after operation
    saveCurrentQueueState();
    return value;
}

void QueueVisualizer::enqueueCircularQueue(int value)
{
    int nextRear = (m_rear + 1) % m_capacity;
    if (nextRear == m_front && m_rear != -1) {
        setStatusMessage(tr("Circular queue is full!"));
        return;
    }
    m_rear = nextRear;
    m_queue[m_rear] = value;
    // Save state after operation
    saveCurrentQueueState();
    updateVisualization(tr("Enqueued %1 to circular queue.").arg(value));
}

int QueueVisualizer::dequeueCircularQueue()
{
    if (m_rear == -1) {
        return -1;
    }
    int value = m_queue[m_front];
    if (m_front == m_rear) {
        m_front = 0;
        m_rear = -1;
    } else {
        m_front = (m_front + 1) % m_capacity;
    }
    // Save state after operation
    saveCurrentQueueState();
    return value;
}

void QueueVisualizer::enqueueDequeFront(int value)
{
    if (m_queue.size() >= static_cast<size_t>(m_capacity)) {
        setStatusMessage(tr("Deque is full!"));
        return;
    }
    m_queue.insert(m_queue.begin(), value);
    m_rear = static_cast<int>(m_queue.size()) - 1;
    // Save state after operation
    saveCurrentQueueState();
    updateVisualization(tr("Enqueued %1 to front of deque.").arg(value));
}

void QueueVisualizer::enqueueDequeRear(int value)
{
    if (m_queue.size() >= static_cast<size_t>(m_capacity)) {
        setStatusMessage(tr("Deque is full!"));
        return;
    }
    m_queue.push_back(value);
    m_rear = static_cast<int>(m_queue.size()) - 1;
    // Save state after operation
    saveCurrentQueueState();
    updateVisualization(tr("Enqueued %1 to rear of deque.").arg(value));
}

int QueueVisualizer::dequeueDequeFront()
{
    if (m_queue.empty()) {
        return -1;
    }
    int value = m_queue[0];
    m_queue.erase(m_queue.begin());
    m_rear = static_cast<int>(m_queue.size()) - 1;
    // Save state after operation
    saveCurrentQueueState();
    return value;
}

int QueueVisualizer::dequeueDequeRear()
{
    if (m_queue.empty()) {
        return -1;
    }
    int value = m_queue.back();
    m_queue.pop_back();
    m_rear = static_cast<int>(m_queue.size()) - 1;
    // Save state after operation
    saveCurrentQueueState();
    return value;
}

