/**
 * @file SortingVisualizer.cpp
 * @brief Implementation of sorting algorithm visualizer
 * 
 * This class visualizes 5 sorting algorithms:
 * - Bubble Sort: O(n²) - Repeatedly swaps adjacent elements
 * - Selection Sort: O(n²) - Finds minimum and swaps
 * - Insertion Sort: O(n²) - Builds sorted array incrementally
 * - Merge Sort: O(n log n) - Divide and conquer, recursive
 * - Quick Sort: O(n log n) average - Partition around pivot
 * 
 * KEY DATA STRUCTURES USED:
 * - std::vector<int> - Stores the array being sorted
 * - std::queue<std::function<void()>> - Command queue pattern for algorithm steps
 * - std::vector<BarState> - Tracks visual state of each element
 * 
 * ALGORITHM IMPLEMENTATION:
 * Each sorting algorithm is pre-computed into a queue of steps.
 * Each step is a lambda function that updates the visualization.
 * This allows step-by-step execution and replay functionality.
 * 
 * C++ FEATURES DEMONSTRATED:
 * - Lambda expressions (std::function<void()>)
 * - STL containers (vector, queue)
 * - Enum classes for type safety
 * - Qt signals/slots for event handling
 * - Custom painting with QPainter
 */

#include "SortingVisualizer.h"
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QSlider>
#include <QLabel>
#include <QFrame>
#include <QRandomGenerator>
#include <QSpacerItem>
#include <QStyle>
#include <QtMath>
#include <QStringList>
#include <QScrollArea>
#include <algorithm>

namespace {

constexpr int kMinArraySize = 3;
constexpr int kMaxArraySize = 16;
constexpr int kMinSliderValue = 80;
constexpr int kMaxSliderValue = 1000;

QString formatArray(const std::vector<int> &values)
{
    QStringList parts;
    parts.reserve(static_cast<int>(values.size()));
    for (int value : values) {
        parts << QString::number(value);
    }
    return parts.join(' ');
}

QWidget *makeLegendItem(const QString &text, const QColor &color, QWidget *parent)
{
    auto container = new QWidget(parent);
    auto layout = new QHBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(6);

    auto swatch = new QFrame(container);
    swatch->setFixedSize(14, 14);
    swatch->setStyleSheet(QStringLiteral("QFrame { background-color: %1; border-radius: 7px; border: 1px solid #c2d4eb; }")
                              .arg(color.name()));
    layout->addWidget(swatch);

    auto label = new QLabel(text, container);
    label->setStyleSheet(QStringLiteral("color: #2c3e50; font-size: 12px; font-weight: 600;"));
    layout->addWidget(label);

    layout->addStretch();
    return container;
}

}  // namespace

const std::vector<SortingVisualizer::AlgorithmInfo> SortingVisualizer::kAlgorithmInfos = {
    {Algorithm::LinearSearch,
     QObject::tr("Linear Search"),
     QObject::tr("Sequentially checks each element"),
     QObject::tr("Linear Search sequentially checks each element in the array from the beginning "
                 "until it finds the target value or reaches the end. Time complexity: O(n).")},
    {Algorithm::BinarySearch,
     QObject::tr("Binary Search"),
     QObject::tr("Searches in a sorted array by dividing in half"),
     QObject::tr("Binary Search works on sorted arrays by repeatedly dividing the search interval in half. "
                 "It compares the target with the middle element and eliminates half of the remaining elements. "
                 "Time complexity: O(log n). Note: Array must be sorted first.")},
    {Algorithm::Bubble,
     QObject::tr("Bubble Sort"),
     QObject::tr("Repeatedly swaps adjacent elements"),
     QObject::tr("Bubble Sort iterates through the list, comparing adjacent elements and swapping them "
                 "if they are in the wrong order. After each pass the largest remaining element "
                 "bubbles to its correct position at the end of the array.")},
    {Algorithm::Selection,
     QObject::tr("Selection Sort"),
     QObject::tr("Repeatedly selects the minimum element"),
     QObject::tr("Selection Sort partitions the array into a sorted and unsorted region. "
                 "It repeatedly selects the minimum element from the unsorted region and swaps "
                 "it with the first unsorted element.")},
    {Algorithm::Insertion,
     QObject::tr("Insertion Sort"),
     QObject::tr("Builds the sorted array one item at a time"),
     QObject::tr("Insertion Sort iterates through each element and inserts it into its correct position "
                 "within the already sorted prefix, shifting larger elements to the right.")},
    {Algorithm::Merge,
     QObject::tr("Merge Sort"),
     QObject::tr("Divide and conquer, merges sorted halves"),
     QObject::tr("Merge Sort recursively splits the array into halves, sorts each half, "
                 "and then merges the sorted halves back together.")},
    {Algorithm::Quick,
     QObject::tr("Quick Sort"),
     QObject::tr("Partition around a pivot"),
     QObject::tr("Quick Sort selects a pivot element, partitions the array into values less than and "
                 "greater than the pivot, and recursively sorts the partitions.")}};

/**
 * @brief Constructor - Initializes the sorting visualizer
 * @param parent Parent widget
 * 
 * Sets up the UI, initializes data structures, and connects signals/slots.
 * Creates an initial random array and sets up the default algorithm (Bubble Sort).
 * 
 * DATA STRUCTURES INITIALIZED:
 * - m_originalArray: std::vector<int> - Original unsorted array
 * - m_currentArray: std::vector<int> - Current state during sorting
 * - m_barStates: std::vector<BarState> - Visual state of each element
 * - m_stepQueue: std::queue<std::function<void()>> - Queue of algorithm steps
 */
SortingVisualizer::SortingVisualizer(QWidget *parent)
    : QWidget(parent),
      m_visualizer(new ArrayVisualizer(this)),
      m_algorithmButtons(new QButtonGroup(this)),
      m_inputField(nullptr),
      m_sizeSpinBox(nullptr),
      m_targetSpinBox(nullptr),
      m_targetLayout(nullptr),
      m_targetWidget(nullptr),
      m_speedSlider(nullptr),
      m_speedValueLabel(nullptr),
      m_stepLabel(nullptr),
      m_statusLabel(nullptr),
      m_algorithmTitleLabel(nullptr),
      m_algorithmSubtitleLabel(nullptr),
      m_algorithmDescriptionLabel(nullptr),
      m_arrayStatsLabel(nullptr),
      m_playButton(nullptr),
      m_stepButton(nullptr),
      m_resetButton(nullptr),
      m_currentAlgorithm(Algorithm::LinearSearch),
      m_currentStep(0),
      m_isPrepared(false),
      m_isFinished(false),
      m_isAutoPlaying(false),
      m_autoTimer(new QTimer(this)),
      m_searchTarget(0)
{
    // Initialize UI components
    setupUi();
    updateInfoPanel();
    updatePlayButtonState();

    // Connect Qt signals to slots for event handling
    connect(m_algorithmButtons, &QButtonGroup::idClicked,
            this, &SortingVisualizer::handleAlgorithmButton);
    connect(m_playButton, &QPushButton::clicked,
            this, &SortingVisualizer::handlePlay);
    connect(m_stepButton, &QPushButton::clicked,
            this, &SortingVisualizer::handleStep);
    connect(m_resetButton, &QPushButton::clicked,
            this, &SortingVisualizer::handleReset);
    connect(m_autoTimer, &QTimer::timeout,
            this, &SortingVisualizer::handleAutoAdvance);
    connect(m_speedSlider, &QSlider::valueChanged,
            this, &SortingVisualizer::handleSpeedChanged);
    connect(m_inputField, &QLineEdit::returnPressed,
            this, &SortingVisualizer::handleApplyInput);

    // Initialize with default algorithm and random array
    updateAlgorithm(Algorithm::LinearSearch);
    setArray(generateRandomValues(8));
    updateVisualization(tr("Ready. Use Play or Step to begin."));
}

void SortingVisualizer::setupUi()
{
    // --- 1. Base Styling and Outer Layout ---
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet(QStringLiteral("background-color: #f0f4f8; color: #2c3e50; font-family: 'Segoe UI', Arial, sans-serif;"));

    auto outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->setSpacing(0);

    // --- NEW: Add a Scroll Area for the Main Content ---
    auto scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Disable horizontal scrolling
    scrollArea->setFrameShape(QFrame::NoFrame); // Remove the scroll area border
    scrollArea->setStyleSheet(QStringLiteral("QScrollArea { border: none; background-color: #f0f4f8; }"));
    outerLayout->addWidget(scrollArea);

    // This centralContainer will be the scrollable widget
    auto centralContainer = new QWidget();
    centralContainer->setObjectName("centralContainer");
    centralContainer->setStyleSheet(QStringLiteral("QWidget#centralContainer { background-color: #f0f4f8; }"));
    scrollArea->setWidget(centralContainer);
    // --- END NEW ---

    auto mainLayout = new QVBoxLayout(centralContainer);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(20);

    // --- 2. Header Bar ---
    auto headerBar = new QFrame(centralContainer);
    headerBar->setStyleSheet(QStringLiteral("QFrame { background-color: #ffffff; border-radius: 12px; border: 1px solid #d8e3f0; }"));
    auto headerLayout = new QHBoxLayout(headerBar);
    headerLayout->setContentsMargins(16, 12, 16, 12);
    headerLayout->setSpacing(16);

    auto headerLabel = new QLabel(tr("Sorting Visualizer"), headerBar);
    headerLabel->setStyleSheet(QStringLiteral("color: #0f3460; font-size: 22px; font-weight: 700;"));
    headerLayout->addWidget(headerLabel);
    headerLayout->addStretch();

    mainLayout->addWidget(headerBar);

    // --- 3. Algorithm Navigation Bar ---
    auto navBar = new QFrame(centralContainer);
    navBar->setStyleSheet(QStringLiteral("QFrame { background-color: #ffffff; border-radius: 12px; border: 1px solid #d8e3f0; }"));
    auto algorithmLayout = new QHBoxLayout(navBar);
    algorithmLayout->setSpacing(10);
    algorithmLayout->setContentsMargins(12, 10, 12, 10);
    setupAlgorithmButtons(algorithmLayout);
    mainLayout->addWidget(navBar);

    // --- 4. Main Content Area (Left and Right Columns) ---
    auto contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(20);
    // Do NOT set a stretch factor on contentLayout here (mainLayout->addLayout handles the vertical stretch)
    mainLayout->addLayout(contentLayout, 1);

    // --- 4.1. Left Column: Visualization and Controls ---
    auto leftColumn = new QVBoxLayout();
    leftColumn->setSpacing(16);
    contentLayout->addLayout(leftColumn, 3);

    // Chart Card (Visualization)
    auto chartCard = new QFrame(centralContainer);
    chartCard->setObjectName("chartCard");
    chartCard->setStyleSheet(QStringLiteral(
        "QFrame#chartCard { background-color: #ffffff; border-radius: 16px; border: 1px solid #d8e3f0; }"));
    auto chartLayout = new QVBoxLayout(chartCard);
    chartLayout->setContentsMargins(20, 16, 20, 20);
    chartLayout->setSpacing(8);

    auto chartTitle = new QLabel(tr("Visualization"), chartCard);
    chartTitle->setStyleSheet(QStringLiteral("color: #0f3460; font-size: 15px; font-weight: 600;"));
    chartLayout->addWidget(chartTitle);

    m_visualizer->setMinimumHeight(240);
    m_visualizer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum); // Use Minimum for vertical policy
    chartLayout->addWidget(m_visualizer);

    // Stretch 3/5 of left column space for chart
    leftColumn->addWidget(chartCard, 3);

    // Define Control Button Style (DEFENSIVE: Fixed size for consistency, generous padding)
    // DEFENSIVE BUTTON STYLE: Enforce min/max width in CSS to prevent text clipping
    const QString controlStyle =
        "QPushButton { background-color: #0f3460; color: #ffffff; font-size: 14px; padding: 10px 18px;" // Slightly larger font/padding
        " border-radius: 8px; border: none; font-weight: 600;"
        " min-height: 40px; max-height: 40px;"
        " min-width: 100px; max-width: 120px; }" // <--- KEY FIX: Enforcing size in CSS prevents most layout issues
        "QPushButton:hover { background-color: #1a5fb4; }"
        "QPushButton:disabled { background-color: #d0d7e2; color: #7a8ca6; }";

    m_playButton = new QPushButton(tr("Play"), this);
    m_stepButton = new QPushButton(tr("Step"), this);
    m_resetButton = new QPushButton(tr("Reset"), this);

    m_playButton->setStyleSheet(controlStyle);
    m_stepButton->setStyleSheet(controlStyle);
    m_resetButton->setStyleSheet(controlStyle);
    // Controls Card (Play/Step/Reset/Speed)
    auto controlsCard = new QFrame(centralContainer);
    controlsCard->setObjectName("controlsCard");
    controlsCard->setStyleSheet(QStringLiteral(
        "QFrame#controlsCard { background-color: #ffffff; border-radius: 16px; border: 1px solid #d8e3f0; }"));
    auto controlsLayout = new QVBoxLayout(controlsCard);
    controlsLayout->setContentsMargins(20, 16, 20, 18);
    controlsLayout->setSpacing(14);

    auto buttonRow = new QHBoxLayout();
    buttonRow->setSpacing(12);
    buttonRow->addWidget(m_playButton);
    buttonRow->addWidget(m_stepButton);
    buttonRow->addWidget(m_resetButton);
    buttonRow->addStretch();
    controlsLayout->addLayout(buttonRow);

    auto speedLayout = new QHBoxLayout();
    speedLayout->setSpacing(8);
    auto speedLabel = new QLabel(tr("Speed"), this);
    speedLabel->setStyleSheet(QStringLiteral("color: #0f3460; font-size: 13px; font-weight: 600;"));
    m_speedSlider = new QSlider(Qt::Horizontal, this);
    m_speedSlider->setRange(kMinSliderValue, kMaxSliderValue);
    m_speedSlider->setValue(420);
    // MOST ROBUST QSlider CSS
    m_speedSlider->setStyleSheet(QStringLiteral(
        "QSlider::groove:horizontal { background: #d0d7e2; height: 8px; border-radius: 4px; }"
        "QSlider::handle:horizontal { background: #0f3460; width: 16px; margin: -4px 0; border-radius: 8px; }"
        "QSlider::sub-page:horizontal { background: #1a5fb4; border-radius: 4px; }"));
    m_speedValueLabel = new QLabel(tr("%1 ms").arg(m_speedSlider->value()), this);
    m_speedValueLabel->setStyleSheet(QStringLiteral("color: #0f3460; font-size: 13px; font-weight: 600; min-width: 45px;"));

    speedLayout->addWidget(speedLabel);
    speedLayout->addWidget(m_speedSlider, 1);
    speedLayout->addWidget(m_speedValueLabel);
    controlsLayout->addLayout(speedLayout);

    leftColumn->addWidget(controlsCard, 1);

    // Input Card (Manual Input, Random Generation, Legend)
    auto inputCard = new QFrame(centralContainer);
    inputCard->setObjectName("inputCard");
    inputCard->setStyleSheet(QStringLiteral(
        "QFrame#inputCard { background-color: #ffffff; border-radius: 16px; border: 1px solid #d8e3f0; }"));
    auto inputLayout = new QVBoxLayout(inputCard);
    inputLayout->setContentsMargins(20, 18, 20, 18);
    inputLayout->setSpacing(14);

    m_inputField = new QLineEdit(this);
    m_inputField->setPlaceholderText(tr("Enter numbers separated by spaces (e.g., 5 3 8 1 2)"));
    // Clean QLineEdit style (Fixed height for consistency)
    m_inputField->setStyleSheet(QStringLiteral(
        "QLineEdit { background-color: #f9fbff; border: 1px solid #c2d4eb; "
        "border-radius: 8px; color: #2c3e50; padding: 10px 14px; font-size: 13px; min-height: 40px; max-height: 40px; }"
        "QLineEdit:focus { border-color: #0f3460; }"));
    auto applyButton = new QPushButton(tr("Apply"), this);
    applyButton->setStyleSheet(controlStyle);
    applyButton->setMinimumWidth(120);
    connect(applyButton, &QPushButton::clicked,
            this, &SortingVisualizer::handleApplyInput);

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

    // --- START: Manual SpinBox Replacement ---

    // Re-use m_sizeSpinBox pointer but treat it as a QLineEdit visually,
    // and hide its default buttons, which are causing the visual corruption.
    m_sizeSpinBox = new QSpinBox(this);
    m_sizeSpinBox->setRange(kMinArraySize, kMaxArraySize);
    m_sizeSpinBox->setValue(8);

    // Base style for the input field part
    QString spinBoxInputStyle = QStringLiteral(
        "QSpinBox { background-color: #f9fbff; border: 1px solid #c2d4eb; "
        "border-radius: 8px; color: #2c3e50; padding: 8px; font-size: 13px; min-width: 80px; min-height: 40px; max-height: 40px; "
        "border-top-right-radius: 0; border-bottom-right-radius: 0; }" // Keep left rounded, make right straight
        "QSpinBox::up-button, QSpinBox::down-button { width: 0; border: none; background: transparent; }" // Hide default corrupted buttons
        );

    // Create a small widget to hold the custom buttons
    auto buttonContainer = new QWidget(this);
    buttonContainer->setStyleSheet(QStringLiteral(
        "QWidget { border: 1px solid #c2d4eb; border-left: none; background-color: #d0d7e2; "
        "border-top-right-radius: 8px; border-bottom-right-radius: 8px; }"
        ));
    buttonContainer->setMaximumWidth(20); // Width of the buttons

    auto buttonLayout = new QVBoxLayout(buttonContainer);
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->setSpacing(0);

    // Style for the '+' button (Up)
    auto upButton = new QPushButton(tr("+"), buttonContainer);
    upButton->setStyleSheet(QStringLiteral(
        "QPushButton { background: transparent; color: #0f3460; font-size: 16px; font-weight: 700; border: none; padding: 0; margin: 0; height: 20px; }"
        "QPushButton:hover { background: #b5c1d3; }"
        ));

    // Style for the '-' button (Down)
    auto downButton = new QPushButton(tr("\u2212"), buttonContainer); // Unicode minus sign
    downButton->setStyleSheet(QStringLiteral(
        "QPushButton { background: transparent; color: #0f3460; font-size: 16px; font-weight: 700; border: none; padding: 0; margin: 0; height: 20px; }"
        "QPushButton:hover { background: #b5c1d3; }"
        ));

    // Add buttons to container
    buttonLayout->addWidget(upButton);
    buttonLayout->addWidget(downButton);

    // Set the stylesheet after button container is created
    m_sizeSpinBox->setStyleSheet(spinBoxInputStyle);
    m_sizeSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons); // Ensure no buttons are drawn by Qt

    // Add QSpinBox and custom buttons to the layout horizontally
    auto customSpinBoxLayout = new QHBoxLayout();
    customSpinBoxLayout->setSpacing(0);
    customSpinBoxLayout->addWidget(m_sizeSpinBox, 1);
    customSpinBoxLayout->addWidget(buttonContainer);

    randomLayout->addLayout(customSpinBoxLayout);
    randomLayout->addSpacing(6);

    // --- END: Manual SpinBox Replacement ---

    auto generateButton = new QPushButton(tr("Randomize"), this);
    generateButton->setStyleSheet(controlStyle);
    generateButton->setMinimumWidth(130);
    connect(generateButton, &QPushButton::clicked,
            this, &SortingVisualizer::handleGenerateRandom);

    // Connect the custom buttons to the QSpinBox's public slots for increment/decrement
    connect(upButton, &QPushButton::clicked, m_sizeSpinBox, &QSpinBox::stepUp);
    connect(downButton, &QPushButton::clicked, m_sizeSpinBox, &QSpinBox::stepDown);


    randomLayout->addWidget(generateButton);
    randomLayout->addStretch();
    inputLayout->addLayout(randomLayout);

    // Target Value Input (for search algorithms) - with +/- buttons like size field
    auto targetWidget = new QWidget(this);
    auto targetLayout = new QHBoxLayout(targetWidget);
    targetLayout->setSpacing(10);
    targetLayout->setContentsMargins(0, 0, 0, 0);

    auto targetLabel = new QLabel(tr("Target"), targetWidget);
    targetLabel->setStyleSheet(QStringLiteral("color: #0f3460; font-size: 13px; font-weight: 600;"));
    targetLayout->addWidget(targetLabel);

    m_targetSpinBox = new QSpinBox(this);
    m_targetSpinBox->setRange(-1000, 1000);
    m_targetSpinBox->setValue(5);
    
    // Base style for the input field part
    QString targetSpinBoxInputStyle = QStringLiteral(
        "QSpinBox { background-color: #f9fbff; border: 1px solid #c2d4eb; "
        "border-radius: 8px; color: #2c3e50; padding: 8px; font-size: 13px; min-width: 80px; min-height: 40px; max-height: 40px; "
        "border-top-right-radius: 0; border-bottom-right-radius: 0; }"
        "QSpinBox::up-button, QSpinBox::down-button { width: 0; border: none; background: transparent; }");

    // Create a small widget to hold the custom buttons
    auto targetButtonContainer = new QWidget(this);
    targetButtonContainer->setStyleSheet(QStringLiteral(
        "QWidget { border: 1px solid #c2d4eb; border-left: none; background-color: #d0d7e2; "
        "border-top-right-radius: 8px; border-bottom-right-radius: 8px; }"
        ));
    targetButtonContainer->setMaximumWidth(20);

    auto targetButtonLayout = new QVBoxLayout(targetButtonContainer);
    targetButtonLayout->setContentsMargins(0, 0, 0, 0);
    targetButtonLayout->setSpacing(0);

    // Style for the '+' button (Up)
    auto targetUpButton = new QPushButton(tr("+"), targetButtonContainer);
    targetUpButton->setStyleSheet(QStringLiteral(
        "QPushButton { background: transparent; color: #0f3460; font-size: 16px; font-weight: 700; border: none; padding: 0; margin: 0; height: 20px; }"
        "QPushButton:hover { background: #b5c1d3; }"
        ));

    // Style for the '-' button (Down)
    auto targetDownButton = new QPushButton(tr("\u2212"), targetButtonContainer);
    targetDownButton->setStyleSheet(QStringLiteral(
        "QPushButton { background: transparent; color: #0f3460; font-size: 16px; font-weight: 700; border: none; padding: 0; margin: 0; height: 20px; }"
        "QPushButton:hover { background: #b5c1d3; }"
        ));

    // Add buttons to container
    targetButtonLayout->addWidget(targetUpButton);
    targetButtonLayout->addWidget(targetDownButton);

    // Set the stylesheet after button container is created
    m_targetSpinBox->setStyleSheet(targetSpinBoxInputStyle);
    m_targetSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);

    // Add QSpinBox and custom buttons to the layout horizontally
    auto customTargetSpinBoxLayout = new QHBoxLayout();
    customTargetSpinBoxLayout->setSpacing(0);
    customTargetSpinBoxLayout->addWidget(m_targetSpinBox, 1);
    customTargetSpinBoxLayout->addWidget(targetButtonContainer);

    // Connect the custom buttons to the QSpinBox's public slots for increment/decrement
    connect(targetUpButton, &QPushButton::clicked, m_targetSpinBox, &QSpinBox::stepUp);
    connect(targetDownButton, &QPushButton::clicked, m_targetSpinBox, &QSpinBox::stepDown);

    targetLayout->addLayout(customTargetSpinBoxLayout);
    targetLayout->addStretch();
    inputLayout->addWidget(targetWidget);
    
    // Store target widget for showing/hiding
    m_targetLayout = targetLayout;
    m_targetWidget = targetWidget;

    // Legend
    auto legendLayout = new QHBoxLayout();
    legendLayout->setSpacing(16);
    legendLayout->setContentsMargins(0, 6, 0, 0);
    legendLayout->addWidget(makeLegendItem(tr("Unsorted/Searching"), QColor(255, 99, 110), this), 0, Qt::AlignLeft);
    legendLayout->addWidget(makeLegendItem(tr("Comparing"), QColor(241, 196, 15), this), 0, Qt::AlignLeft);
    legendLayout->addWidget(makeLegendItem(tr("Mid"), QColor(52, 152, 219), this), 0, Qt::AlignLeft);
    legendLayout->addWidget(makeLegendItem(tr("Sorted/Searched"), QColor(46, 204, 113), this), 0, Qt::AlignLeft);
    legendLayout->addStretch();
    inputLayout->addLayout(legendLayout);
    
    // Initially hide target field (will be shown when search algorithm is selected)
    if (m_targetWidget) {
        m_targetWidget->setVisible(false);
    }

    leftColumn->addWidget(inputCard, 1);

    // Add a stretch at the bottom of the left column to ensure the cards don't vertically expand unless necessary
    leftColumn->addStretch(1);
    contentLayout->setStretch(0, 3);

    // --- 4.2. Right Column: Algorithm Info and Status ---
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

    m_algorithmTitleLabel = new QLabel(this);
    m_algorithmTitleLabel->setStyleSheet(QStringLiteral(
        "color: #0f3460; font-size: 24px; font-weight: 700;"));
    infoLayout->addWidget(m_algorithmTitleLabel);

    m_algorithmSubtitleLabel = new QLabel(this);
    m_algorithmSubtitleLabel->setStyleSheet(QStringLiteral(
        "color: #7a8ca6; font-size: 13px; font-weight: 600;"));
    infoLayout->addWidget(m_algorithmSubtitleLabel);

    auto infoDivider = new QFrame(infoFrame);
    infoDivider->setFrameShape(QFrame::HLine);
    infoDivider->setFrameShadow(QFrame::Plain);
    infoDivider->setStyleSheet(QStringLiteral("QFrame { color: #d8e3f0; }"));
    infoLayout->addWidget(infoDivider);

    m_algorithmDescriptionLabel = new QLabel(this);
    m_algorithmDescriptionLabel->setWordWrap(true);
    m_algorithmDescriptionLabel->setStyleSheet(QStringLiteral(
        "color: #2c3e50; font-size: 13px; line-height: 1.6;"));
    infoLayout->addWidget(m_algorithmDescriptionLabel);

    // Stats Container
    auto statsContainer = new QFrame(infoFrame);
    statsContainer->setObjectName("statsContainer");
    statsContainer->setStyleSheet(QStringLiteral(
        "QFrame#statsContainer { background-color: #f9fbff; border-radius: 12px; border: 1px solid #d8e3f0; }"));
    auto statsLayout = new QVBoxLayout(statsContainer);
    statsLayout->setContentsMargins(18, 14, 18, 14);
    statsLayout->setSpacing(6);

    m_arrayStatsLabel = new QLabel(this);
    m_arrayStatsLabel->setStyleSheet(QStringLiteral(
        "color: #0f3460; font-size: 13px; font-weight: 600;"));
    statsLayout->addWidget(m_arrayStatsLabel);

    m_stepLabel = new QLabel(tr("Step: 0"), this);
    m_stepLabel->setStyleSheet(QStringLiteral(
        "color: #0f3460; font-size: 13px; font-weight: 600;"));
    statsLayout->addWidget(m_stepLabel);
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
}
void SortingVisualizer::handleBackButton()
{
    emit requestBackToMenu();
}

void SortingVisualizer::setupAlgorithmButtons(QHBoxLayout *layout)
{
    m_algorithmButtons->setExclusive(true);
    int index = 0;
    for (const auto &info : kAlgorithmInfos) {
        auto button = new QPushButton(info.title, this);
        button->setCheckable(true);
        button->setStyleSheet(
            "QPushButton { background-color: #ffffff; color: #0f3460; padding: 10px 18px; "
            "border-radius: 10px; border: 1px solid #d8e3f0; font-size: 13px; font-weight: 600; }"
            "QPushButton:hover { background-color: #e8f1ff; }"
            "QPushButton:checked { background-color: #0f3460; color: #ffffff; border: 1px solid #0f3460; }");
        m_algorithmButtons->addButton(button, index);
        layout->addWidget(button);

        if (info.type == Algorithm::LinearSearch) {
            button->setChecked(true);
        }
        ++index;
    }
    layout->addStretch();
}

void SortingVisualizer::handleAlgorithmButton(int id)
{
    const int clampedId = qBound(0, id, static_cast<int>(kAlgorithmInfos.size()) - 1);
    updateAlgorithm(kAlgorithmInfos.at(clampedId).type);
    resetState(false);
    setStatusMessage(tr("Switched to %1. Resetting array.")
                         .arg(kAlgorithmInfos.at(clampedId).title));
}

void SortingVisualizer::updateAlgorithm(Algorithm algorithm)
{
    if (m_currentAlgorithm == algorithm)
        return;
    m_currentAlgorithm = algorithm;
    
    // Show/hide target field based on algorithm type
    if (m_targetWidget) {
        bool isSearchAlgorithm = (algorithm == Algorithm::LinearSearch || algorithm == Algorithm::BinarySearch);
        m_targetWidget->setVisible(isSearchAlgorithm);
    }
    
    updateInfoPanel();
}

void SortingVisualizer::updateInfoPanel()
{
    const auto it = std::find_if(kAlgorithmInfos.begin(), kAlgorithmInfos.end(),
                                 [this](const AlgorithmInfo &info) {
                                     return info.type == m_currentAlgorithm;
                                 });
    if (it == kAlgorithmInfos.end())
        return;

    m_algorithmTitleLabel->setText(it->title);
    m_algorithmSubtitleLabel->setText(it->subtitle);
    m_algorithmDescriptionLabel->setText(it->description);
}

void SortingVisualizer::updatePlayButtonState()
{
    if (m_isAutoPlaying) {
        m_playButton->setText(tr("Pause"));
    } else {
        m_playButton->setText(tr("Play"));
    }
}

void SortingVisualizer::handlePlay()
{
    if (m_isFinished) {
        resetState(false);
    }

    if (!m_isPrepared) {
        if (!prepareAlgorithm()) {
            return;
        }
    }

    if (m_isAutoPlaying) {
        m_autoTimer->stop();
        m_isAutoPlaying = false;
        setStatusMessage(tr("Paused."));
    } else {
        m_autoTimer->start(currentInterval());
        m_isAutoPlaying = true;
        setStatusMessage(tr("Auto playback running..."));
    }
    updatePlayButtonState();
}

void SortingVisualizer::handleStep()
{
    if (m_isFinished) {
        resetState(false);
        if (!prepareAlgorithm()) {
            return;
        }
    }

    if (!m_isPrepared) {
        if (!prepareAlgorithm()) {
            return;
        }
    }

    executeNextStep();
}

void SortingVisualizer::handleReset()
{
    resetState(true);
}

void SortingVisualizer::handleAutoAdvance()
{
    if (!executeNextStep()) {
        m_autoTimer->stop();
        m_isAutoPlaying = false;
        updatePlayButtonState();
    }
}

void SortingVisualizer::handleGenerateRandom()
{
    const auto values = generateRandomValues(m_sizeSpinBox->value());
    setArray(values);
    setStatusMessage(tr("Generated random array of size %1.").arg(values.size()));
}

void SortingVisualizer::handleApplyInput()
{
    bool ok = false;
    const auto values = parseInputValues(&ok);
    if (!ok || values.empty()) {
        setStatusMessage(tr("Invalid input. Please enter integers separated by spaces."));
        return;
    }
    setArray(values, false);
    m_inputField->setText(formatArray(values));
    setStatusMessage(tr("Loaded custom array of size %1.").arg(values.size()));
}

void SortingVisualizer::handleSpeedChanged(int value)
{
    m_speedValueLabel->setText(tr("%1 ms").arg(value));
    if (m_autoTimer->isActive()) {
        m_autoTimer->start(currentInterval());
    }
}

int SortingVisualizer::currentInterval() const
{
    const int sliderValue = m_speedSlider->value();
    return sliderValue;
}

void SortingVisualizer::setArray(const std::vector<int> &values, bool updateInputField)
{
    m_originalArray = values;
    if (updateInputField) {
        m_inputField->setText(formatArray(values));
    }
    if (m_sizeSpinBox && values.size() >= static_cast<size_t>(kMinArraySize) &&
        values.size() <= static_cast<size_t>(kMaxArraySize)) {
        m_sizeSpinBox->setValue(static_cast<int>(values.size()));
    }
    resetState(true);
}

void SortingVisualizer::resetState(bool hardReset)
{
    m_autoTimer->stop();
    m_isAutoPlaying = false;
    updatePlayButtonState();

    m_currentArray = m_originalArray;
    m_barStates.assign(m_currentArray.size(), ArrayVisualizer::BarState::Default);
    m_sortedFlags.assign(m_currentArray.size(), false);
    m_currentStep = 0;
    m_isPrepared = false;
    m_isFinished = false;
    m_stepLabel->setText(tr("Step: %1").arg(m_currentStep));

    while (!m_stepQueue.empty()) {
        m_stepQueue.pop();
    }

    updateVisualization(hardReset ? tr("Array reset.") : tr("Ready to visualize."));
}

void SortingVisualizer::updateVisualization(const QString &message)
{
    m_visualizer->setArray(m_currentArray);
    m_visualizer->setStates(m_barStates);
    m_arrayStatsLabel->setText(tr("Array Size: %1").arg(m_currentArray.size()));
    m_stepLabel->setText(tr("Step: %1").arg(m_currentStep));
    if (!message.isEmpty()) {
        setStatusMessage(message);
    } else {
        m_visualizer->setMessage(m_lastStatusMessage);
    }
}

void SortingVisualizer::setStatusMessage(const QString &message)
{
    m_lastStatusMessage = message;
    m_statusLabel->setText(message);
    m_visualizer->setMessage(message);
}

/**
 * @brief Prepares the algorithm by pre-computing all steps into a queue
 * @return true if preparation successful, false otherwise
 * 
 * This function implements the COMMAND QUEUE PATTERN:
 * 1. Takes the original array
 * 2. Runs the selected algorithm and captures each step as a lambda function
 * 3. Stores all steps in m_stepQueue (std::queue<std::function<void()>>)
 * 4. Each step is a lambda that updates the visualization when executed
 * 
 * This allows:
 * - Step-by-step execution (user can see each operation)
 * - Replay functionality (can reset and run again)
 * - Speed control (timer controls how fast steps execute)
 * 
 * DATA STRUCTURE: std::queue<std::function<void()>>
 * - Queue ensures steps execute in order (FIFO)
 * - std::function allows storing lambda expressions
 * - Each lambda captures necessary indices/values by value
 */
bool SortingVisualizer::prepareAlgorithm()
{
    if (m_originalArray.empty()) {
        setStatusMessage(tr("Array is empty. Generate or enter values first."));
        return false;
    }

    // Reset to initial state
    resetState(false);

    // Get target value for search algorithms
    m_searchTarget = m_targetSpinBox->value();

    // Pre-compute algorithm steps based on selected algorithm
    // Each enqueue* function fills m_stepQueue with lambda functions
    switch (m_currentAlgorithm) {
    case Algorithm::LinearSearch:
        enqueueLinearSearch(m_originalArray, m_searchTarget);
        break;
    case Algorithm::BinarySearch:
        enqueueBinarySearch(m_originalArray, m_searchTarget);
        break;
    case Algorithm::Bubble:
        enqueueBubbleSort(m_originalArray);
        break;
    case Algorithm::Selection:
        enqueueSelectionSort(m_originalArray);
        break;
    case Algorithm::Insertion:
        enqueueInsertionSort(m_originalArray);
        break;
    case Algorithm::Merge:
        enqueueMergeSort(m_originalArray);
        break;
    case Algorithm::Quick:
        enqueueQuickSort(m_originalArray);
        break;
    }

    // Check if any steps were generated
    if (m_stepQueue.empty()) {
        setStatusMessage(tr("No steps generated. Array might already be sorted."));
        markAllSorted();
        m_isFinished = true;
        return false;
    }

    m_isPrepared = true;
    setStatusMessage(tr("Starting %1...").arg(m_algorithmTitleLabel->text()));
    return true;
}

/**
 * @brief Executes the next step from the algorithm queue
 * @return true if step executed, false if queue is empty
 * 
 * This is the core execution function that:
 * 1. Gets the next step (lambda function) from the queue
 * 2. Executes it (updates array/visualization)
 * 3. Increments step counter
 * 4. Updates the display
 * 
 * C++ FEATURES:
 * - std::queue::front() - Gets next step without removing
 * - std::queue::pop() - Removes step after execution
 * - Lambda execution - step() calls the stored function
 * - RAII - Automatic cleanup when step goes out of scope
 */
bool SortingVisualizer::executeNextStep()
{
    // Auto-prepare if not already prepared
    if (!m_isPrepared) {
        if (!prepareAlgorithm()) {
            return false;
        }
    }

    // Check if queue is empty (algorithm finished)
    if (m_stepQueue.empty()) {
        finalizeRun();
        return false;
    }

    // Get and execute next step from queue
    auto step = m_stepQueue.front();  // Get lambda function
    m_stepQueue.pop();                 // Remove from queue (FIFO)
    step();                            // Execute the lambda
    ++m_currentStep;                   // Increment step counter
    updateVisualization();             // Refresh display

    // Check if algorithm is complete
    if (m_stepQueue.empty()) {
        finalizeRun();
    }

    return true;
}

void SortingVisualizer::finalizeRun()
{
    if (m_isFinished)
        return;
    
    // For search algorithms, don't mark all as sorted
    if (m_currentAlgorithm == Algorithm::LinearSearch || m_currentAlgorithm == Algorithm::BinarySearch) {
        m_isFinished = true;
        m_isAutoPlaying = false;
        m_autoTimer->stop();
        updatePlayButtonState();
        // Message is set in the search algorithm itself
        return;
    }
    
    markAllSorted();
    m_isFinished = true;
    m_isAutoPlaying = false;
    m_autoTimer->stop();
    updatePlayButtonState();
    updateVisualization(tr("Array sorted using %1.").arg(m_algorithmTitleLabel->text()));
}

void SortingVisualizer::clearTransientStates()
{
    for (int i = 0; i < static_cast<int>(m_barStates.size()); ++i) {
        if (i < static_cast<int>(m_sortedFlags.size()) && m_sortedFlags[i]) {
            m_barStates[i] = ArrayVisualizer::BarState::Sorted;
        } else {
            m_barStates[i] = ArrayVisualizer::BarState::Default;
        }
    }
}

void SortingVisualizer::markSortedIndices(const std::vector<int> &indices)
{
    for (int idx : indices) {
        if (idx < 0 || idx >= static_cast<int>(m_barStates.size()))
            continue;
        if (idx < static_cast<int>(m_sortedFlags.size())) {
            m_sortedFlags[idx] = true;
        }
        m_barStates[idx] = ArrayVisualizer::BarState::Sorted;
    }
}

void SortingVisualizer::markAllSorted()
{
    for (int i = 0; i < static_cast<int>(m_barStates.size()); ++i) {
        m_barStates[i] = ArrayVisualizer::BarState::Sorted;
    }
    m_sortedFlags.assign(m_barStates.size(), true);
}

void SortingVisualizer::setStatesForIndices(const std::vector<int> &indices, ArrayVisualizer::BarState state)
{
    for (int idx : indices) {
        if (idx < 0 || idx >= static_cast<int>(m_barStates.size()))
            continue;
        if (state != ArrayVisualizer::BarState::Sorted &&
            idx < static_cast<int>(m_sortedFlags.size()) && m_sortedFlags[idx]) {
            continue;
        }
        m_barStates[idx] = state;
    }
}

/**
 * @brief Pre-computes Linear Search algorithm steps into the queue
 * @param arr Copy of the array to search
 * @param target The value to search for
 * 
 * LINEAR SEARCH ALGORITHM:
 * - Time Complexity: O(n) worst case
 * - Space Complexity: O(1)
 * - Algorithm: Sequentially checks each element from start to end
 * 
 * IMPLEMENTATION DETAILS:
 * - Each comparison is captured as a lambda function
 * - Highlights current element being checked
 * - Marks found element as Sorted (green)
 */
void SortingVisualizer::enqueueLinearSearch(std::vector<int> arr, int target)
{
    const int n = static_cast<int>(arr.size());
    bool found = false;

    for (int i = 0; i < n; ++i) {
        const int currentIndex = i;
        const int currentValue = arr[i];

        // Step: Check current element
        m_stepQueue.push([this, currentIndex, currentValue, target]() {
            clearTransientStates();
            setStatesForIndices({currentIndex}, ArrayVisualizer::BarState::Comparing);
            setStatusMessage(tr("Checking index %1: value %2 (target: %3)")
                                .arg(currentIndex).arg(currentValue).arg(target));
        });

        if (arr[i] == target) {
            found = true;
            const int finalIndex = i;
            // Step: Found the target
            m_stepQueue.push([this, finalIndex, target]() {
                clearTransientStates();
                setStatesForIndices({finalIndex}, ArrayVisualizer::BarState::Sorted);
                setStatusMessage(tr("Found %1 at index %2!").arg(target).arg(finalIndex));
            });
            break;
        }
    }

    if (!found) {
        // Step: Target not found
        m_stepQueue.push([this, target]() {
            clearTransientStates();
            setStatusMessage(tr("Target %1 not found in the array.").arg(target));
        });
    }
}

/**
 * @brief Pre-computes Binary Search algorithm steps into the queue
 * @param arr Copy of the array to search (must be sorted)
 * @param target The value to search for
 * 
 * BINARY SEARCH ALGORITHM:
 * - Time Complexity: O(log n)
 * - Space Complexity: O(1)
 * - Algorithm: Repeatedly divides search interval in half
 * - Requires: Array must be sorted
 * 
 * IMPLEMENTATION DETAILS:
 * - First checks if array is sorted, if not, sorts it first
 * - Each comparison is captured as a lambda function
 * - Highlights left, right, and middle indices
 * - Marks found element as Sorted (green)
 */
void SortingVisualizer::enqueueBinarySearch(std::vector<int> arr, int target)
{
    const int n = static_cast<int>(arr.size());
    
    // Check if array is sorted, if not, sort it first
    bool isSorted = true;
    for (int i = 1; i < n; ++i) {
        if (arr[i - 1] > arr[i]) {
            isSorted = false;
            break;
        }
    }

    if (!isSorted) {
        // Sort the array first using std::sort (we'll show this as a step)
        m_stepQueue.push([this]() {
            clearTransientStates();
            setStatusMessage(tr("Array is not sorted. Sorting first..."));
        });
        std::sort(arr.begin(), arr.end());
        
        // Update the current array to sorted version
        m_stepQueue.push([this, arr]() {
            clearTransientStates();
            for (int i = 0; i < static_cast<int>(arr.size()); ++i) {
                m_currentArray[i] = arr[i];
            }
            markAllSorted();
            setStatusMessage(tr("Array sorted. Starting binary search..."));
        });
    }

    int left = 0;
    int right = n - 1;
    bool found = false;

    while (left <= right) {
        const int mid = left + (right - left) / 2;
        const int midValue = arr[mid];
        const int currentLeft = left;
        const int currentRight = right;
        const int currentMid = mid;

        // Step: Show search range and middle element
        m_stepQueue.push([this, currentLeft, currentRight, currentMid, midValue, target]() {
            clearTransientStates();
            // Highlight the search range
            for (int i = currentLeft; i <= currentRight; ++i) {
                if (i == currentMid) {
                    setStatesForIndices({i}, ArrayVisualizer::BarState::Mid);
                } else {
                    setStatesForIndices({i}, ArrayVisualizer::BarState::Default);
                }
            }
            setStatusMessage(tr("Searching range [%1, %2], checking middle index %3: value %4 (target: %5)")
                                .arg(currentLeft).arg(currentRight).arg(currentMid).arg(midValue).arg(target));
        });

        if (arr[mid] == target) {
            found = true;
            const int finalIndex = mid;
            // Step: Found the target
            m_stepQueue.push([this, finalIndex, target]() {
                clearTransientStates();
                setStatesForIndices({finalIndex}, ArrayVisualizer::BarState::Sorted);
                setStatusMessage(tr("Found %1 at index %2!").arg(target).arg(finalIndex));
            });
            break;
        } else if (arr[mid] < target) {
            // Target is in right half
            left = mid + 1;
            m_stepQueue.push([this, midValue, target]() {
                clearTransientStates();
                setStatusMessage(tr("Value %1 < target %2, searching right half...")
                                    .arg(midValue).arg(target));
            });
        } else {
            // Target is in left half
            right = mid - 1;
            m_stepQueue.push([this, midValue, target]() {
                clearTransientStates();
                setStatusMessage(tr("Value %1 > target %2, searching left half...")
                                    .arg(midValue).arg(target));
            });
        }
    }

    if (!found) {
        // Step: Target not found
        m_stepQueue.push([this, target]() {
            clearTransientStates();
            setStatusMessage(tr("Target %1 not found in the array.").arg(target));
        });
    }
}

/**
 * @brief Pre-computes Bubble Sort algorithm steps into the queue
 * @param arr Copy of the array to sort (we modify a copy, not original)
 * 
 * BUBBLE SORT ALGORITHM:
 * - Time Complexity: O(n²) worst case, O(n) best case (if already sorted)
 * - Space Complexity: O(1)
 * - Algorithm: Repeatedly compares adjacent elements and swaps if in wrong order
 * - After each pass, largest element "bubbles" to the end
 * 
 * IMPLEMENTATION DETAILS:
 * - Uses nested loops: outer loop for passes, inner loop for comparisons
 * - Each comparison and swap is captured as a lambda function
 * - Lambdas are pushed into m_stepQueue for later execution
 * - Optimized: breaks early if no swaps occur (array already sorted)
 * 
 * C++ FEATURES USED:
 * - Lambda expressions with capture [this, idx1, idx2]
 * - std::swap for efficient element swapping
 * - std::vector for array storage
 * - std::queue for step storage
 */
void SortingVisualizer::enqueueBubbleSort(std::vector<int> arr)
{
    const int n = static_cast<int>(arr.size());
    
    // Outer loop: number of passes (n-1 passes needed)
    for (int pass = 0; pass < n - 1; ++pass) {
        bool swapped = false;
        
        // Inner loop: compare adjacent elements
        // After each pass, one more element is in correct position
        for (int i = 0; i < n - pass - 1; ++i) {
            const int idx1 = i;
            const int idx2 = i + 1;
            
            // Step 1: Compare two adjacent elements
            // Lambda captures indices by value, executes later
            m_stepQueue.push([this, idx1, idx2]() {
                clearTransientStates();
                setStatesForIndices({idx1, idx2}, ArrayVisualizer::BarState::Comparing);
                setStatusMessage(tr("Comparing values %1 and %2")
                                     .arg(m_currentArray[idx1]).arg(m_currentArray[idx2]));
            });

            // Step 2: Swap if elements are in wrong order
            if (arr[i] > arr[i + 1]) {
                std::swap(arr[i], arr[i + 1]);  // Swap in local copy
                
                // Create step to swap in visualization
                m_stepQueue.push([this, idx1, idx2]() {
                    clearTransientStates();
                    std::swap(m_currentArray[idx1], m_currentArray[idx2]);  // Swap in displayed array
                    setStatesForIndices({idx1, idx2}, ArrayVisualizer::BarState::Swapping);
                    setStatusMessage(tr("Swapped positions %1 and %2").arg(idx1).arg(idx2));
                });
                swapped = true;
            }
        }

        // Mark the last element of this pass as sorted
        const int sortedIdx = n - pass - 1;
        m_stepQueue.push([this, sortedIdx]() {
            clearTransientStates();
            markSortedIndices({sortedIdx});
            setStatusMessage(tr("Position %1 confirmed sorted").arg(sortedIdx));
        });

        // Optimization: If no swaps occurred, array is sorted
        if (!swapped) {
            // Mark all remaining elements as sorted
            for (int k = 0; k < n - pass - 1; ++k) {
                const int idx = k;
                m_stepQueue.push([this, idx]() {
                    clearTransientStates();
                    markSortedIndices({idx});
                });
            }
            break;  // Early exit
        }
    }
    
    // Final step: Mark all as sorted and show completion message
    m_stepQueue.push([this]() {
        markAllSorted();
        setStatusMessage(tr("Bubble sort complete."));
    });
}

void SortingVisualizer::enqueueSelectionSort(std::vector<int> arr)
{
    const int n = static_cast<int>(arr.size());
    for (int i = 0; i < n - 1; ++i) {
        int minIndex = i;
        for (int j = i + 1; j < n; ++j) {
            const int currentMin = minIndex;
            const int currentJ = j;
            m_stepQueue.push([this, currentMin, currentJ]() {
                clearTransientStates();
                setStatesForIndices({currentMin}, ArrayVisualizer::BarState::Sorted);
                setStatesForIndices({currentJ}, ArrayVisualizer::BarState::Comparing);
                setStatusMessage(tr("Comparing new minimum (%1) with value %2")
                                     .arg(m_currentArray[currentMin]).arg(m_currentArray[currentJ]));
            });
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            const int swapI = i;
            const int swapMin = minIndex;
            std::swap(arr[i], arr[minIndex]);
            m_stepQueue.push([this, swapI, swapMin]() {
                clearTransientStates();
                std::swap(m_currentArray[swapI], m_currentArray[swapMin]);
                setStatesForIndices({swapI, swapMin}, ArrayVisualizer::BarState::Swapping);
                setStatusMessage(tr("Swapped index %1 with new minimum at %2").arg(swapI).arg(swapMin));
            });
        }
        const int sortedIdx = i;
        m_stepQueue.push([this, sortedIdx]() {
            clearTransientStates();
            markSortedIndices({sortedIdx});
            setStatusMessage(tr("Position %1 locked in place").arg(sortedIdx));
        });
    }
    m_stepQueue.push([this]() {
        markAllSorted();
        setStatusMessage(tr("Selection sort complete."));
    });
}

void SortingVisualizer::enqueueInsertionSort(std::vector<int> arr)
{
    const int n = static_cast<int>(arr.size());
    for (int i = 1; i < n; ++i) {
        const int key = arr[i];
        int j = i - 1;

        const int insertIndex = i;
        m_stepQueue.push([this, insertIndex]() {
            clearTransientStates();
            setStatesForIndices({insertIndex}, ArrayVisualizer::BarState::Comparing);
            setStatusMessage(tr("Inserting value at index %1").arg(insertIndex));
        });

        while (j >= 0 && arr[j] > key) {
            const int from = j;
            const int to = j + 1;
            arr[j + 1] = arr[j];
            m_stepQueue.push([this, from, to]() {
                clearTransientStates();
                m_currentArray[to] = m_currentArray[from];
                setStatesForIndices({from, to}, ArrayVisualizer::BarState::Swapping);
                setStatusMessage(tr("Shifting %1 from %2 to %3")
                                     .arg(m_currentArray[to]).arg(from).arg(to));
            });
            --j;
        }

        const int finalPos = j + 1;
        m_stepQueue.push([this, finalPos, key]() {
            clearTransientStates();
            m_currentArray[finalPos] = key;
            setStatesForIndices({finalPos}, ArrayVisualizer::BarState::Swapping);
            setStatusMessage(tr("Placed %1 at position %2").arg(key).arg(finalPos));
        });
    }
    m_stepQueue.push([this]() {
        markAllSorted();
        setStatusMessage(tr("Insertion sort complete."));
    });
}

void SortingVisualizer::enqueueMergeSort(std::vector<int> arr)
{
    enqueueMergeSortRecursive(arr, 0, static_cast<int>(arr.size()) - 1);
    m_stepQueue.push([this]() {
        markAllSorted();
        setStatusMessage(tr("Merge sort complete."));
    });
}

void SortingVisualizer::enqueueMergeSortRecursive(std::vector<int> &arr, int left, int right)
{
    if (left >= right)
        return;

    const int mid = left + (right - left) / 2;
    const int leftBound = left;
    const int rightBound = right;
    const int midBound = mid;
    m_stepQueue.push([this, leftBound, midBound, rightBound]() {
        clearTransientStates();
        setStatusMessage(tr("Dividing array segment [%1, %2]").arg(leftBound).arg(rightBound));
    });

    enqueueMergeSortRecursive(arr, left, mid);
    enqueueMergeSortRecursive(arr, mid + 1, right);

    std::vector<int> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];

    const auto mergedSegment = temp;
    m_stepQueue.push([this, left, mergedSegment]() {
        clearTransientStates();
        for (int idx = 0; idx < static_cast<int>(mergedSegment.size()); ++idx) {
            m_currentArray[left + idx] = mergedSegment[idx];
            setStatesForIndices({left + idx}, ArrayVisualizer::BarState::Swapping);
        }
        setStatusMessage(tr("Merging sorted segment starting at %1").arg(left));
    });

    std::copy(temp.begin(), temp.end(), arr.begin() + left);
}

void SortingVisualizer::enqueueQuickSort(std::vector<int> arr)
{
    enqueueQuickSortRecursive(arr, 0, static_cast<int>(arr.size()) - 1);
    m_stepQueue.push([this]() {
        markAllSorted();
        setStatusMessage(tr("Quick sort complete."));
    });
}

void SortingVisualizer::enqueueQuickSortRecursive(std::vector<int> &arr, int low, int high)
{
    if (low >= high)
        return;

    const int pivotValue = arr[high];
    int i = low - 1;
    const int pivotIndex = high;

    m_stepQueue.push([this, pivotIndex]() {
        clearTransientStates();
        setStatesForIndices({pivotIndex}, ArrayVisualizer::BarState::Pivot);
        setStatusMessage(tr("Pivot at index %1").arg(pivotIndex));
    });

    for (int j = low; j < high; ++j) {
        const int currentJ = j;
        m_stepQueue.push([this, currentJ, pivotIndex]() {
            clearTransientStates();
            setStatesForIndices({pivotIndex}, ArrayVisualizer::BarState::Pivot);
            setStatesForIndices({currentJ}, ArrayVisualizer::BarState::Comparing);
            setStatusMessage(tr("Comparing value %1 with pivot")
                                 .arg(m_currentArray[currentJ]));
        });

        if (arr[j] < pivotValue) {
            ++i;
            if (i != j) {
                std::swap(arr[i], arr[j]);
                const int swapI = i;
                const int swapJ = j;
                m_stepQueue.push([this, swapI, swapJ, pivotIndex]() {
                    clearTransientStates();
                    setStatesForIndices({pivotIndex}, ArrayVisualizer::BarState::Pivot);
                    std::swap(m_currentArray[swapI], m_currentArray[swapJ]);
                    setStatesForIndices({swapI, swapJ}, ArrayVisualizer::BarState::Swapping);
                    setStatusMessage(tr("Swapped %1 and %2").arg(swapI).arg(swapJ));
                });
            }
        }
    }

    const int finalPivotPos = i + 1;
    std::swap(arr[finalPivotPos], arr[high]);
    m_stepQueue.push([this, finalPivotPos, high]() {
        clearTransientStates();
        std::swap(m_currentArray[finalPivotPos], m_currentArray[high]);
        markSortedIndices({finalPivotPos});
        setStatusMessage(tr("Pivot placed at index %1").arg(finalPivotPos));
    });

    enqueueQuickSortRecursive(arr, low, finalPivotPos - 1);
    enqueueQuickSortRecursive(arr, finalPivotPos + 1, high);
}

std::vector<int> SortingVisualizer::parseInputValues(bool *ok) const
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

std::vector<int> SortingVisualizer::generateRandomValues(int size) const
{
    std::vector<int> values;
    values.reserve(size);
    for (int i = 0; i < size; ++i) {
        values.push_back(static_cast<int>(QRandomGenerator::global()->bounded(10, 120)));
    }
    return values;
}

