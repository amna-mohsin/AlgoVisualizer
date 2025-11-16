/**
 * @file StackVisualizer.cpp
 * @brief Implementation of the StackVisualizer widget for stack operations visualization
 * @author AlgoVisualizer Team
 * @date 2025
 * 
 * @details
 * This file implements the StackVisualizer class which provides interactive
 * visualization for stack (LIFO - Last In, First Out) data structure operations:
 * - Push: Add element to top of stack
 * - Pop: Remove element from top of stack
 * - Clear: Remove all elements from stack
 * 
 * Features:
 * - Visual representation of stack as vertical structure
 * - Step-by-step operation visualization
 * - Random value generation
 * - Custom input support
 */

#include "StackVisualizer.h"

// --------------------- Qt Widget Includes -----------------------
#include <QButtonGroup>      ///< Groups related buttons together
#include <QHBoxLayout>        ///< Horizontal layout for arranging widgets
#include <QVBoxLayout>        ///< Vertical layout for stacking widgets
#include <QGridLayout>        ///< Grid layout for complex arrangements
#include <QPushButton>        ///< Clickable button widgets
#include <QLineEdit>          ///< Single-line text input for values
#include <QSpinBox>           ///< Number input for random size
#include <QSlider>            ///< Slider control (if needed)
#include <QLabel>             ///< Text label for displaying information
#include <QFrame>             ///< Container widget with border styling
#include <QScrollArea>        ///< Scrollable container
#include <QSpacerItem>        ///< Flexible spacing in layouts
#include <QStyle>             ///< Qt style system

// --------------------- Qt Graphics Includes ---------------------
#include <QPainter>           ///< 2D painting system for drawing stack
#include <QLinearGradient>    ///< Linear gradient fills
#include <QBrush>             ///< Brush for filling shapes
#include <QPen>               ///< Pen for drawing outlines
#include <QPolygon>           ///< Polygon shapes for arrows

// --------------------- Qt Core Includes -------------------------
#include <QRandomGenerator>   ///< Generates random numbers
#include <QtMath>             ///< Mathematical functions
#include <QStringList>        ///< List of strings for text processing
#include <QShowEvent>         ///< Event fired when widget becomes visible

// --------------------- Standard Library Includes ----------------
#include <algorithm>          ///< STL algorithms

namespace {

constexpr int kMinStackSize = 3;
constexpr int kMaxStackSize = 16;

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

const StackVisualizer::StructureInfo StackVisualizer::kStructureInfo = {
    QObject::tr("Stack"),
    QObject::tr("Last In First Out (LIFO)"),
    QObject::tr("A Stack is a linear data structure that follows the Last In First Out (LIFO) principle. "
                "Elements are added (pushed) and removed (popped) from the same end, called the top. "
                "The most recently added element is always the first to be removed. "
                "Common operations include push (add), pop (remove), peek/top (view top element), and isEmpty.")
};

// Custom visualization widget for Stack
class StackVisualizer::StackVizWidget : public QWidget {
public:
    StackVisualizer* parentVisualizer;
    StackVizWidget(QWidget* parent) : QWidget(parent), parentVisualizer(nullptr) {
        setMinimumHeight(400);
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
        
        int width = this->width();
        int startX = width / 2 - 100;
        int startY = 50;
        int boxWidth = 200;
        int boxHeight = 50;
        int spacing = 5;
        
        // Calculate required height based on stack size
        int totalElements = static_cast<int>(parentVisualizer->m_stack.size());
        int requiredHeight = startY + totalElements * (boxHeight + spacing) + 100; // 100px for labels and margins
        int minRequiredHeight = qMax(400, requiredHeight);
        
        // Update widget minimum height
        if (parentVisualizer && parentVisualizer->m_vizWidget) {
            if (parentVisualizer->m_vizWidget->minimumHeight() != minRequiredHeight) {
                parentVisualizer->m_vizWidget->setMinimumHeight(minRequiredHeight);
                parentVisualizer->m_vizWidget->updateGeometry();
            }
        }
        
        if (parentVisualizer->m_stack.empty()) {
            int centerX = width / 2 - 100;
            int centerY = height() / 2 - 25;
            int boxWidth = 200;
            int boxHeight = 50;
            
            // Draw empty stack box
            painter.setPen(QPen(QColor("#95a5a6"), 2, Qt::DashLine));
            painter.setBrush(QBrush(QColor("#ecf0f1")));
            painter.drawRoundedRect(centerX, centerY, boxWidth, boxHeight, 10, 10);
            
            // Draw NULL text
            painter.setPen(QColor("#7f8c8d"));
            painter.setFont(QFont("Segoe UI", 12, QFont::Bold));
            painter.drawText(QRect(centerX, centerY, boxWidth, boxHeight), Qt::AlignCenter, "NULL");
            
            // Draw TOP pointer pointing to null
            int pointerX = centerX + boxWidth + 20;
            int pointerY = centerY + boxHeight / 2;
            painter.setPen(QPen(QColor("#27ae60"), 3));
            painter.setFont(QFont("Segoe UI", 11, QFont::Bold));
            painter.drawText(pointerX, pointerY - 5, "TOP");
            painter.drawLine(pointerX - 15, pointerY, centerX + boxWidth, pointerY);
            // Draw arrow
            QPolygon arrow;
            arrow << QPoint(centerX + boxWidth, pointerY)
                  << QPoint(centerX + boxWidth - 8, pointerY - 5)
                  << QPoint(centerX + boxWidth - 8, pointerY + 5);
            painter.setBrush(QBrush(QColor("#27ae60")));
            painter.drawPolygon(arrow);
            return;
        }
        
        // Draw stack from bottom to top
        for (int i = 0; i < static_cast<int>(parentVisualizer->m_stack.size()); i++) {
            int y = startY + (static_cast<int>(parentVisualizer->m_stack.size()) - 1 - i) * (boxHeight + spacing);
            
            // Highlight top element
            if (i == static_cast<int>(parentVisualizer->m_stack.size()) - 1) {
                painter.setPen(QPen(QColor("#27ae60"), 3));
                QLinearGradient gradient(startX, y, startX, y + boxHeight);
                gradient.setColorAt(0, QColor("#a8e6cf"));
                gradient.setColorAt(1, QColor("#7dcea0"));
                painter.setBrush(QBrush(gradient));
            } else {
                painter.setPen(QPen(QColor("#3498db"), 2));
                QLinearGradient gradient(startX, y, startX, y + boxHeight);
                gradient.setColorAt(0, QColor("#d6eaf8"));
                gradient.setColorAt(1, QColor("#aed6f1"));
                painter.setBrush(QBrush(gradient));
            }
            
            painter.drawRoundedRect(startX, y, boxWidth, boxHeight, 10, 10);
            
            // Draw value
            painter.setPen(QColor("#2c3e50"));
            painter.setFont(QFont("Segoe UI", 14, QFont::Bold));
            painter.drawText(QRect(startX, y, boxWidth, boxHeight), Qt::AlignCenter, 
                           QString::number(parentVisualizer->m_stack[i]));
            
            // Draw "TOP" label on top element
            if (i == static_cast<int>(parentVisualizer->m_stack.size()) - 1) {
                painter.setPen(QColor("#27ae60"));
                painter.setFont(QFont("Segoe UI", 11, QFont::Bold));
                painter.drawText(startX + boxWidth + 10, y + boxHeight / 2, "← TOP");
            }
        }
    }
};

StackVisualizer::StackVisualizer(QWidget *parent)
    : QWidget(parent),
      m_visualizerWidget(nullptr),
      m_inputField(nullptr),
      m_sizeSpinBox(nullptr),
      m_statusLabel(nullptr),
      m_structureTitleLabel(nullptr),
      m_structureSubtitleLabel(nullptr),
      m_structureDescriptionLabel(nullptr),
      m_stackStatsLabel(nullptr),
      m_pushButton(nullptr),
      m_popButton(nullptr),
      m_clearButton(nullptr),
      m_vizWidget(nullptr)
{
    setupUi();
    updateInfoPanel();
    // Start with empty stack
    m_stack.clear();
    updateVisualization(tr("Stack is empty. Use Push or Randomize to add elements."));
}

void StackVisualizer::setupUi()
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

    auto headerLabel = new QLabel(tr("Stack Visualizer"), headerBar);
    headerLabel->setStyleSheet(QStringLiteral("color: #0f3460; font-size: 22px; font-weight: 700;"));
    headerLayout->addWidget(headerLabel);
    headerLayout->addStretch();

    mainLayout->addWidget(headerBar);

    // Main Content Area
    auto contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(20);
    mainLayout->addLayout(contentLayout, 1);

    // Left Column: Visualization and Controls
    auto leftColumn = new QVBoxLayout();
    leftColumn->setSpacing(16);
    contentLayout->addLayout(leftColumn, 3);

    // Chart Card (Visualization)
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

    m_vizWidget = new StackVizWidget(this);
    m_vizWidget->parentVisualizer = this;
    m_vizWidget->setMinimumHeight(400);
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

    m_pushButton = new QPushButton(tr("Push"), this);
    m_popButton = new QPushButton(tr("Pop"), this);
    m_clearButton = new QPushButton(tr("Clear"), this);

    m_pushButton->setStyleSheet(controlStyle);
    m_popButton->setStyleSheet(controlStyle);
    m_clearButton->setStyleSheet(controlStyle);

    auto controlsCard = new QFrame(centralContainer);
    controlsCard->setObjectName("controlsCard");
    controlsCard->setStyleSheet(QStringLiteral(
        "QFrame#controlsCard { background-color: #ffffff; border-radius: 16px; border: 1px solid #d8e3f0; }"));
    auto controlsLayout = new QVBoxLayout(controlsCard);
    controlsLayout->setContentsMargins(20, 16, 20, 18);
    controlsLayout->setSpacing(14);

    auto buttonRow = new QHBoxLayout();
    buttonRow->setSpacing(12);
    buttonRow->addWidget(m_pushButton);
    buttonRow->addWidget(m_popButton);
    buttonRow->addWidget(m_clearButton);
    buttonRow->addStretch();
    controlsLayout->addLayout(buttonRow);

    leftColumn->addWidget(controlsCard, 1);

    // Input Card
    auto inputCard = new QFrame(centralContainer);
    inputCard->setObjectName("inputCard");
    inputCard->setStyleSheet(QStringLiteral(
        "QFrame#inputCard { background-color: #ffffff; border-radius: 16px; border: 1px solid #d8e3f0; }"));
    auto inputLayout = new QVBoxLayout(inputCard);
    inputLayout->setContentsMargins(20, 18, 20, 18);
    inputLayout->setSpacing(14);

    m_inputField = new QLineEdit(this);
    m_inputField->setPlaceholderText(tr("Enter numbers separated by spaces (e.g., 5 3 8 1 2)"));
    m_inputField->setStyleSheet(QStringLiteral(
        "QLineEdit { background-color: #f9fbff; border: 1px solid #c2d4eb; "
        "border-radius: 8px; color: #2c3e50; padding: 10px 14px; font-size: 13px; min-height: 40px; max-height: 40px; }"
        "QLineEdit:focus { border-color: #0f3460; }"));
    auto applyButton = new QPushButton(tr("Apply"), this);
    applyButton->setStyleSheet(controlStyle);
    applyButton->setMinimumWidth(120);
    connect(applyButton, &QPushButton::clicked,
            this, &StackVisualizer::handleApplyInput);

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
    m_sizeSpinBox->setRange(kMinStackSize, kMaxStackSize);
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
            this, &StackVisualizer::handleGenerateRandom);

    connect(upButton, &QPushButton::clicked, m_sizeSpinBox, &QSpinBox::stepUp);
    connect(downButton, &QPushButton::clicked, m_sizeSpinBox, &QSpinBox::stepDown);

    randomLayout->addWidget(generateButton);
    randomLayout->addStretch();
    inputLayout->addLayout(randomLayout);

    leftColumn->addWidget(inputCard, 1);
    leftColumn->addStretch(1);
    contentLayout->setStretch(0, 3);

    // Right Column: Structure Info and Status
    auto rightColumn = new QVBoxLayout();
    rightColumn->setSpacing(20);
    contentLayout->addLayout(rightColumn, 2);

    // Info Card
    auto infoFrame = new QFrame(centralContainer);
    infoFrame->setObjectName("infoCard");
    infoFrame->setStyleSheet(QStringLiteral(
        "QFrame#infoCard { background-color: #ffffff; border-radius: 16px; border: 1px solid #d8e3f0; }"));
    auto infoLayout = new QVBoxLayout(infoFrame);
    infoLayout->setContentsMargins(24, 28, 24, 24);
    infoLayout->setSpacing(16);

    m_structureTitleLabel = new QLabel(this);
    m_structureTitleLabel->setStyleSheet(QStringLiteral(
        "color: #0f3460; font-size: 24px; font-weight: 700;"));
    infoLayout->addWidget(m_structureTitleLabel);

    m_structureSubtitleLabel = new QLabel(this);
    m_structureSubtitleLabel->setStyleSheet(QStringLiteral(
        "color: #7a8ca6; font-size: 13px; font-weight: 600;"));
    infoLayout->addWidget(m_structureSubtitleLabel);

    auto infoDivider = new QFrame(infoFrame);
    infoDivider->setFrameShape(QFrame::HLine);
    infoDivider->setFrameShadow(QFrame::Plain);
    infoDivider->setStyleSheet(QStringLiteral("QFrame { color: #d8e3f0; }"));
    infoLayout->addWidget(infoDivider);

    m_structureDescriptionLabel = new QLabel(this);
    m_structureDescriptionLabel->setWordWrap(true);
    m_structureDescriptionLabel->setStyleSheet(QStringLiteral(
        "color: #2c3e50; font-size: 13px; line-height: 1.6;"));
    infoLayout->addWidget(m_structureDescriptionLabel);

    // Stats Container
    auto statsContainer = new QFrame(infoFrame);
    statsContainer->setObjectName("statsContainer");
    statsContainer->setStyleSheet(QStringLiteral(
        "QFrame#statsContainer { background-color: #f9fbff; border-radius: 12px; border: 1px solid #d8e3f0; }"));
    auto statsLayout = new QVBoxLayout(statsContainer);
    statsLayout->setContentsMargins(18, 14, 18, 14);
    statsLayout->setSpacing(6);

    m_stackStatsLabel = new QLabel(this);
    m_stackStatsLabel->setStyleSheet(QStringLiteral(
        "color: #0f3460; font-size: 13px; font-weight: 600;"));
    statsLayout->addWidget(m_stackStatsLabel);

    infoLayout->addWidget(statsContainer);

    // Status Panel
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
    connect(m_pushButton, &QPushButton::clicked,
            this, &StackVisualizer::handlePush);
    connect(m_popButton, &QPushButton::clicked,
            this, &StackVisualizer::handlePop);
    connect(m_clearButton, &QPushButton::clicked,
            this, &StackVisualizer::handleClear);
    connect(m_inputField, &QLineEdit::returnPressed,
            this, &StackVisualizer::handleApplyInput);
}

void StackVisualizer::handleBackButton()
{
    emit requestBackToMenu();
}

void StackVisualizer::handlePush()
{
    bool ok = false;
    const auto values = parseInputValues(&ok);
    if (!ok || values.empty()) {
        setStatusMessage(tr("Invalid input. Please enter a single integer."));
        return;
    }
    if (values.size() > 1) {
        setStatusMessage(tr("Please enter only one value to push."));
        return;
    }
    if (m_stack.size() >= static_cast<size_t>(kMaxStackSize)) {
        setStatusMessage(tr("Stack is full! Maximum size is %1.").arg(kMaxStackSize));
        return;
    }
    m_stack.push_back(values[0]);
    m_inputField->clear();
    updateVisualization(tr("Pushed %1 onto stack.").arg(values[0]));
}

void StackVisualizer::handlePop()
{
    if (m_stack.empty()) {
        setStatusMessage(tr("Stack is empty! Cannot pop."));
        return;
    }
    int value = m_stack.back();
    m_stack.pop_back();
    updateVisualization(tr("Popped %1 from stack.").arg(value));
}

void StackVisualizer::handleClear()
{
    m_stack.clear();
    updateVisualization(tr("Stack cleared."));
}

void StackVisualizer::handleGenerateRandom()
{
    const auto values = generateRandomValues(m_sizeSpinBox->value());
    setStack(values);
    setStatusMessage(tr("Generated random stack of size %1.").arg(values.size()));
}

void StackVisualizer::handleApplyInput()
{
    bool ok = false;
    const auto values = parseInputValues(&ok);
    if (!ok || values.empty()) {
        setStatusMessage(tr("Invalid input. Please enter integers separated by spaces."));
        return;
    }
    setStack(values, false);
    m_inputField->setText(formatArray(values));
    setStatusMessage(tr("Loaded custom stack of size %1.").arg(values.size()));
}

void StackVisualizer::updateInfoPanel()
{
    m_structureTitleLabel->setText(kStructureInfo.title);
    m_structureSubtitleLabel->setText(kStructureInfo.subtitle);
    m_structureDescriptionLabel->setText(kStructureInfo.description);
}

void StackVisualizer::updateVisualization(const QString &message)
{
    if (m_vizWidget) {
        m_vizWidget->update();
    }
    m_stackStatsLabel->setText(tr("Stack Size: %1%2")
                                   .arg(m_stack.size())
                                   .arg(m_stack.empty() ? "" : tr(" | Top: %1").arg(m_stack.back())));
    if (!message.isEmpty()) {
        setStatusMessage(message);
    }
}

void StackVisualizer::setStatusMessage(const QString &message)
{
    m_lastStatusMessage = message;
    m_statusLabel->setText(message);
}

void StackVisualizer::setStack(const std::vector<int> &values, bool updateInputField)
{
    m_stack = values;
    if (updateInputField) {
        m_inputField->setText(formatArray(values));
    }
    if (m_sizeSpinBox && values.size() >= static_cast<size_t>(kMinStackSize) &&
        values.size() <= static_cast<size_t>(kMaxStackSize)) {
        m_sizeSpinBox->setValue(static_cast<int>(values.size()));
    }
    resetState();
}

void StackVisualizer::resetState()
{
    updateVisualization(tr("Stack ready."));
}

void StackVisualizer::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    // Reset to empty when page is shown
    m_stack.clear();
    updateVisualization(tr("Stack is empty. Use Push or Randomize to add elements."));
}

std::vector<int> StackVisualizer::parseInputValues(bool *ok) const
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

std::vector<int> StackVisualizer::generateRandomValues(int size) const
{
    std::vector<int> values;
    values.reserve(size);
    for (int i = 0; i < size; ++i) {
        values.push_back(static_cast<int>(QRandomGenerator::global()->bounded(10, 120)));
    }
    return values;
}

