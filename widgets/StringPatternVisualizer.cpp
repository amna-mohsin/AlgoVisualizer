/**
 * @file StringPatternVisualizer.cpp
 * @brief Implementation of the StringPatternVisualizer widget for string matching algorithms
 * @author AlgoVisualizer Team
 * @date 2025
 * 
 * @details
 * This file implements the StringPatternVisualizer class which provides interactive
 * visualization for string pattern matching algorithms:
 * - Naive String Search (brute force approach)
 * - KMP Algorithm (Knuth-Morris-Pratt, linear time)
 * 
 * Features:
 * - Step-by-step pattern matching visualization
 * - Highlight matching characters and positions
 * - Display all match positions
 * - Random example generation
 * - State preservation when switching algorithms
 */

#include "StringPatternVisualizer.h"

// --------------------- Qt Widget Includes -----------------------
#include <QButtonGroup>      ///< Groups algorithm selection buttons
#include <QHBoxLayout>        ///< Horizontal layout for arranging widgets
#include <QVBoxLayout>        ///< Vertical layout for stacking widgets
#include <QPushButton>        ///< Clickable button widgets
#include <QLineEdit>          ///< Single-line text input for text and pattern
#include <QLabel>             ///< Text label for displaying information
#include <QFrame>             ///< Container widget with border styling
#include <QScrollArea>        ///< Scrollable container
#include <QMessageBox>        ///< Dialog boxes for user notifications
#include <QApplication>       ///< Main application object

// --------------------- Qt Graphics Includes ---------------------
#include <QPainter>           ///< 2D painting system for drawing text visualization
#include <QBrush>             ///< Brush for filling shapes
#include <QPen>               ///< Pen for drawing outlines

// --------------------- Qt Core Includes -------------------------
#include <QThread>            ///< Thread management for animations
#include <QShowEvent>         ///< Event fired when widget becomes visible

// --------------------- Standard Library Includes ----------------
#include <algorithm>          ///< STL algorithms
#include <random>             ///< Random number generation for examples

namespace {
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

const std::vector<StringPatternVisualizer::AlgorithmInfo> StringPatternVisualizer::kAlgorithmInfos = {
    {Algorithm::Naive,
     QObject::tr("Naive Search"),
     QObject::tr("Brute force pattern matching"),
     QObject::tr("The Naive pattern matching algorithm checks every possible position in the text for the pattern. "
                 "For each position, it compares characters one by one. Time complexity: O(n*m) where n is text length and m is pattern length.")},
    {Algorithm::KMP,
     QObject::tr("KMP Search"),
     QObject::tr("Knuth-Morris-Pratt algorithm"),
     QObject::tr("The KMP algorithm uses a preprocessed prefix table (LPS) to skip characters that cannot match. "
                 "This avoids unnecessary comparisons and improves efficiency. Time complexity: O(n+m).")}
};

// Custom visualization widget
class StringPatternVisualizer::StringVizWidget : public QWidget {
public:
    StringPatternVisualizer* parentVisualizer;
    StringVizWidget(QWidget* parent) : QWidget(parent), parentVisualizer(nullptr) {
        setMinimumHeight(300);
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
        
        int startX = 50;
        int startY = 50;
        int charWidth = 30;
        int charHeight = 40;
        int spacing = 5;
        
        if (parentVisualizer->m_text.empty()) {
            painter.setPen(QPen(QColor("#95a5a6"), 2, Qt::DashLine));
            painter.setBrush(QBrush(QColor("#ecf0f1")));
            int centerX = width() / 2 - 100;
            int centerY = height() / 2;
            painter.drawRect(centerX, centerY, 200, 50);
            painter.setPen(QColor("#7f8c8d"));
            painter.setFont(QFont("Segoe UI", 12));
            painter.drawText(QRect(centerX, centerY, 200, 50), Qt::AlignCenter, "NO TEXT");
            return;
        }
        
        // Draw text
        for (size_t i = 0; i < parentVisualizer->m_text.length(); i++) {
            int x = startX + static_cast<int>(i) * (charWidth + spacing);
            bool isHighlighted = std::find(parentVisualizer->m_highlightIndices.begin(), 
                                          parentVisualizer->m_highlightIndices.end(), 
                                          static_cast<int>(i)) != parentVisualizer->m_highlightIndices.end();
            bool isMatch = std::find(parentVisualizer->m_matches.begin(), 
                                    parentVisualizer->m_matches.end(), 
                                    static_cast<int>(i)) != parentVisualizer->m_matches.end();
            
            if (isMatch) {
                painter.setPen(QPen(QColor("#27ae60"), 2));
                painter.setBrush(QBrush(QColor("#a8e6cf")));
            } else if (isHighlighted) {
                painter.setPen(QPen(QColor("#e74c3c"), 2));
                painter.setBrush(QBrush(QColor("#fadbd8")));
            } else {
                painter.setPen(QPen(QColor("#3498db"), 1));
                painter.setBrush(QBrush(QColor("#d6eaf8")));
            }
            
            painter.drawRoundedRect(x, startY, charWidth, charHeight, 5, 5);
            painter.setPen(QColor("#2c3e50"));
            painter.setFont(QFont("Segoe UI", 14, QFont::Bold));
            painter.drawText(QRect(x, startY, charWidth, charHeight), Qt::AlignCenter, QString(parentVisualizer->m_text[i]));
            painter.setPen(QColor("#7f8c8d"));
            painter.setFont(QFont("Segoe UI", 8));
            painter.drawText(x, startY + charHeight + 15, QString::number(static_cast<int>(i)));
        }
        
        // Draw pattern
        if (!parentVisualizer->m_pattern.empty()) {
            int patternY = startY + charHeight + 40;
            painter.setPen(QColor("#2c3e50"));
            painter.setFont(QFont("Segoe UI", 12, QFont::Bold));
            painter.drawText(startX, patternY - 20, "Pattern:");
            for (size_t i = 0; i < parentVisualizer->m_pattern.length(); i++) {
                int x = startX + static_cast<int>(i) * (charWidth + spacing);
                bool isHighlighted = (parentVisualizer->m_currentPatternPos >= 0 && 
                                     static_cast<int>(i) == parentVisualizer->m_currentPatternPos);
                if (isHighlighted) {
                    painter.setPen(QPen(QColor("#f39c12"), 2));
                    painter.setBrush(QBrush(QColor("#fdeaa7")));
                } else {
                    painter.setPen(QPen(QColor("#9b59b6"), 1));
                    painter.setBrush(QBrush(QColor("#e8daef")));
                }
                painter.drawRoundedRect(x, patternY, charWidth, charHeight, 5, 5);
                painter.setPen(QColor("#2c3e50"));
                painter.setFont(QFont("Segoe UI", 14, QFont::Bold));
                painter.drawText(QRect(x, patternY, charWidth, charHeight), Qt::AlignCenter, QString(parentVisualizer->m_pattern[i]));
            }
        }
    }
};

StringPatternVisualizer::StringPatternVisualizer(QWidget *parent)
    : QWidget(parent),
      m_visualizerWidget(nullptr),
      m_algorithmButtons(new QButtonGroup(this)),
      m_textInput(nullptr),
      m_patternInput(nullptr),
      m_statusLabel(nullptr),
      m_algorithmTitleLabel(nullptr),
      m_algorithmSubtitleLabel(nullptr),
      m_algorithmDescriptionLabel(nullptr),
      m_searchButton(nullptr),
      m_clearButton(nullptr),
      m_randomButton(nullptr),
      m_currentAlgorithm(Algorithm::Naive),
      m_currentTextPos(-1),
      m_currentPatternPos(-1),
      m_vizWidget(nullptr),
      m_naiveText(QString()),
      m_naivePattern(QString()),
      m_kmpText(QString()),
      m_kmpPattern(QString())
{
    setupUi();
    updateInfoPanel();
    updateAlgorithm(Algorithm::Naive);
    updateVisualization();
}

void StringPatternVisualizer::setupUi()
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
    auto headerLabel = new QLabel(tr("String Pattern Matching Visualizer"), headerBar);
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

    m_vizWidget = new StringVizWidget(this);
    m_vizWidget->parentVisualizer = this;
    m_vizWidget->setMinimumHeight(300);
    chartLayout->addWidget(m_vizWidget);
    m_visualizerWidget = m_vizWidget;
    leftColumn->addWidget(chartCard, 3);

    const QString controlStyle = "QPushButton { background-color: #0f3460; color: #ffffff; font-size: 14px; padding: 10px 18px; border-radius: 8px; border: none; font-weight: 600; min-height: 40px; max-height: 40px; min-width: 100px; max-width: 120px; } QPushButton:hover { background-color: #1a5fb4; }";

    m_searchButton = new QPushButton(tr("Search"), this);
    m_clearButton = new QPushButton(tr("Clear"), this);
    m_randomButton = new QPushButton(tr("Random"), this);
    m_searchButton->setStyleSheet(controlStyle);
    m_clearButton->setStyleSheet(controlStyle);
    m_randomButton->setStyleSheet(controlStyle);

    auto controlsCard = new QFrame(centralContainer);
    controlsCard->setObjectName("controlsCard");
    controlsCard->setStyleSheet(QStringLiteral("QFrame#controlsCard { background-color: #ffffff; border-radius: 16px; border: 1px solid #d8e3f0; }"));
    auto controlsLayout = new QVBoxLayout(controlsCard);
    controlsLayout->setContentsMargins(20, 16, 20, 18);
    controlsLayout->setSpacing(14);
    auto buttonRow = new QHBoxLayout();
    buttonRow->setSpacing(12);
    buttonRow->addWidget(m_searchButton);
    buttonRow->addWidget(m_clearButton);
    buttonRow->addWidget(m_randomButton);
    buttonRow->addStretch();
    controlsLayout->addLayout(buttonRow);
    leftColumn->addWidget(controlsCard, 1);

    auto inputCard = new QFrame(centralContainer);
    inputCard->setObjectName("inputCard");
    inputCard->setStyleSheet(QStringLiteral("QFrame#inputCard { background-color: #ffffff; border-radius: 16px; border: 1px solid #d8e3f0; }"));
    auto inputLayout = new QVBoxLayout(inputCard);
    inputLayout->setContentsMargins(20, 18, 20, 18);
    inputLayout->setSpacing(14);

    auto textLayout = new QHBoxLayout();
    auto textLabel = new QLabel(tr("Text"), this);
    textLabel->setStyleSheet(QStringLiteral("color: #0f3460; font-size: 13px; font-weight: 600;"));
    m_textInput = new QLineEdit(this);
    m_textInput->setPlaceholderText(tr("Enter text to search in"));
    m_textInput->setStyleSheet(QStringLiteral("QLineEdit { background-color: #f9fbff; border: 1px solid #c2d4eb; border-radius: 8px; color: #2c3e50; padding: 10px 14px; font-size: 13px; min-height: 40px; max-height: 40px; } QLineEdit:focus { border-color: #0f3460; }"));
    textLayout->addWidget(textLabel);
    textLayout->addWidget(m_textInput, 1);
    inputLayout->addLayout(textLayout);

    auto patternLayout = new QHBoxLayout();
    auto patternLabel = new QLabel(tr("Pattern"), this);
    patternLabel->setStyleSheet(QStringLiteral("color: #0f3460; font-size: 13px; font-weight: 600;"));
    m_patternInput = new QLineEdit(this);
    m_patternInput->setPlaceholderText(tr("Enter pattern to search for"));
    m_patternInput->setStyleSheet(QStringLiteral("QLineEdit { background-color: #f9fbff; border: 1px solid #c2d4eb; border-radius: 8px; color: #2c3e50; padding: 10px 14px; font-size: 13px; min-height: 40px; max-height: 40px; } QLineEdit:focus { border-color: #0f3460; }"));
    patternLayout->addWidget(patternLabel);
    patternLayout->addWidget(m_patternInput, 1);
    inputLayout->addLayout(patternLayout);
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

    connect(m_algorithmButtons, &QButtonGroup::idClicked, this, &StringPatternVisualizer::handleAlgorithmButton);
    connect(m_searchButton, &QPushButton::clicked, this, &StringPatternVisualizer::handleSearch);
    connect(m_clearButton, &QPushButton::clicked, this, &StringPatternVisualizer::handleClear);
    connect(m_randomButton, &QPushButton::clicked, this, &StringPatternVisualizer::handleRandomExample);
}

void StringPatternVisualizer::setupAlgorithmButtons(QHBoxLayout *layout)
{
    m_algorithmButtons->setExclusive(true);
    int index = 0;
    for (const auto &info : kAlgorithmInfos) {
        auto button = new QPushButton(info.title, this);
        button->setCheckable(true);
        button->setStyleSheet("QPushButton { background-color: #ffffff; color: #0f3460; padding: 10px 18px; border-radius: 10px; border: 1px solid #d8e3f0; font-size: 13px; font-weight: 600; } QPushButton:hover { background-color: #e8f1ff; } QPushButton:checked { background-color: #0f3460; color: #ffffff; border: 1px solid #0f3460; }");
        m_algorithmButtons->addButton(button, index);
        layout->addWidget(button);
        if (info.type == Algorithm::Naive) button->setChecked(true);
        ++index;
    }
    layout->addStretch();
}

void StringPatternVisualizer::handleAlgorithmButton(int id)
{
    const int clampedId = qBound(0, id, static_cast<int>(kAlgorithmInfos.size()) - 1);
    updateAlgorithm(kAlgorithmInfos.at(clampedId).type);
}

void StringPatternVisualizer::updateAlgorithm(Algorithm algorithm)
{
    if (m_currentAlgorithm == algorithm) return;
    
    // Save current state before switching
    saveCurrentState();
    
    // Switch algorithm
    m_currentAlgorithm = algorithm;
    
    // Load state for the new algorithm
    loadAlgorithmState(algorithm);
    
    updateInfoPanel();
    updateVisualization();
}

void StringPatternVisualizer::updateInfoPanel()
{
    const auto it = std::find_if(kAlgorithmInfos.begin(), kAlgorithmInfos.end(),
                                 [this](const AlgorithmInfo &info) { return info.type == m_currentAlgorithm; });
    if (it == kAlgorithmInfos.end()) return;
    m_algorithmTitleLabel->setText(it->title);
    m_algorithmSubtitleLabel->setText(it->subtitle);
    m_algorithmDescriptionLabel->setText(it->description);
}

void StringPatternVisualizer::updateVisualization()
{
    if (m_vizWidget) m_vizWidget->update();
    if (m_text.empty() && m_pattern.empty()) {
        setStatusMessage(tr("Enter text and pattern, then click Search."));
    }
}

void StringPatternVisualizer::setStatusMessage(const QString &message)
{
    m_lastStatusMessage = message;
    if (m_statusLabel) m_statusLabel->setText(message);
}

void StringPatternVisualizer::handleSearch()
{
    QString text = m_textInput->text();
    QString pattern = m_patternInput->text();
    if (text.isEmpty() || pattern.isEmpty()) {
        QMessageBox::warning(this, tr("Input Error"), tr("Please enter both text and pattern!"));
        return;
    }
    
    // Save the current input to the algorithm's storage
    saveCurrentState();
    
    m_text = text.toStdString();
    m_pattern = pattern.toStdString();
    m_matches.clear();
    m_highlightIndices.clear();
    m_currentTextPos = -1;
    m_currentPatternPos = -1;
    
    if (m_currentAlgorithm == Algorithm::Naive) {
        performNaiveSearch();
    } else {
        performKMPSearch();
    }
}

void StringPatternVisualizer::handleClear()
{
    m_text.clear();
    m_pattern.clear();
    m_matches.clear();
    m_highlightIndices.clear();
    m_textInput->clear();
    m_patternInput->clear();
    
    // Clear the stored state for current algorithm
    if (m_currentAlgorithm == Algorithm::Naive) {
        m_naiveText.clear();
        m_naivePattern.clear();
    } else if (m_currentAlgorithm == Algorithm::KMP) {
        m_kmpText.clear();
        m_kmpPattern.clear();
    }
    
    updateVisualization();
    setStatusMessage(tr("Cleared."));
}

void StringPatternVisualizer::handleRandomExample()
{
    std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 25);
    
    std::string text, pattern;
    for (int i = 0; i < 20; i++) text += chars[dis(gen)];
    for (int i = 0; i < 4; i++) pattern += chars[dis(gen)];
    
    m_textInput->setText(QString::fromStdString(text));
    m_patternInput->setText(QString::fromStdString(pattern));
    setStatusMessage(tr("Random example generated."));
}

void StringPatternVisualizer::handleBackButton()
{
    emit requestBackToMenu();
}

void StringPatternVisualizer::performNaiveSearch()
{
    m_matches.clear();
    setStatusMessage(tr("Performing Naive Search..."));
    
    int n = static_cast<int>(m_text.length());
    int m = static_cast<int>(m_pattern.length());
    
    for (int i = 0; i <= n - m; i++) {
        m_currentTextPos = i;
        m_highlightIndices.clear();
        for (int j = 0; j < m; j++) {
            m_highlightIndices.push_back(i + j);
        }
        updateVisualization();
        QApplication::processEvents();
        QThread::msleep(400);
        
        bool match = true;
        for (int j = 0; j < m; j++) {
            m_currentPatternPos = j;
            updateVisualization();
            QApplication::processEvents();
            QThread::msleep(200);
            if (m_text[i + j] != m_pattern[j]) {
                match = false;
                break;
            }
        }
        
        if (match) {
            m_matches.push_back(i);
        }
    }
    
    if (m_matches.empty()) {
        setStatusMessage(tr("Pattern not found!"));
    } else {
        setStatusMessage(tr("Pattern found at %1 position(s).").arg(m_matches.size()));
    }
    
    m_currentTextPos = -1;
    m_currentPatternPos = -1;
    m_highlightIndices.clear();
    updateVisualization();
}

void StringPatternVisualizer::performKMPSearch()
{
    m_matches.clear();
    setStatusMessage(tr("Performing KMP Search..."));
    
    std::vector<int> lps = computeLPS(m_pattern);
    int n = static_cast<int>(m_text.length());
    int m = static_cast<int>(m_pattern.length());
    
    int i = 0, j = 0;
    while (i < n) {
        m_currentTextPos = i;
        m_currentPatternPos = j;
        m_highlightIndices.clear();
        if (j >= 0 && j < m) {
            for (int k = 0; k <= j; k++) {
                if (i - j + k >= 0 && i - j + k < n) {
                    m_highlightIndices.push_back(i - j + k);
                }
            }
        }
        updateVisualization();
        QApplication::processEvents();
        QThread::msleep(300);
        
        if (m_text[i] == m_pattern[j]) {
            i++;
            j++;
        }
        if (j == m) {
            m_matches.push_back(i - j);
            j = lps[j - 1];
        } else if (i < n && m_text[i] != m_pattern[j]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
    
    if (m_matches.empty()) {
        setStatusMessage(tr("Pattern not found!"));
    } else {
        setStatusMessage(tr("Pattern found at %1 position(s).").arg(m_matches.size()));
    }
    
    m_currentTextPos = -1;
    m_currentPatternPos = -1;
    m_highlightIndices.clear();
    updateVisualization();
}

std::vector<int> StringPatternVisualizer::computeLPS(const std::string& pattern)
{
    int m = static_cast<int>(pattern.length());
    std::vector<int> lps(m, 0);
    int len = 0;
    int i = 1;
    
    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

void StringPatternVisualizer::saveCurrentState()
{
    // Save current input fields to the current algorithm's storage
    if (m_textInput && m_patternInput) {
        QString text = m_textInput->text();
        QString pattern = m_patternInput->text();
        
        if (m_currentAlgorithm == Algorithm::Naive) {
            m_naiveText = text;
            m_naivePattern = pattern;
        } else if (m_currentAlgorithm == Algorithm::KMP) {
            m_kmpText = text;
            m_kmpPattern = pattern;
        }
    }
}

void StringPatternVisualizer::loadAlgorithmState(Algorithm algorithm)
{
    // Load the stored state for the selected algorithm
    QString text, pattern;
    
    if (algorithm == Algorithm::Naive) {
        text = m_naiveText;
        pattern = m_naivePattern;
    } else if (algorithm == Algorithm::KMP) {
        text = m_kmpText;
        pattern = m_kmpPattern;
    }
    
    // Update input fields
    if (m_textInput) {
        m_textInput->setText(text);
    }
    if (m_patternInput) {
        m_patternInput->setText(pattern);
    }
    
    // Update internal state if inputs exist
    if (!text.isEmpty() && !pattern.isEmpty()) {
        m_text = text.toStdString();
        m_pattern = pattern.toStdString();
    } else {
        m_text.clear();
        m_pattern.clear();
    }
    
    // Clear visualization state when switching
    m_matches.clear();
    m_highlightIndices.clear();
    m_currentTextPos = -1;
    m_currentPatternPos = -1;
}

void StringPatternVisualizer::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    // Reset inputs when page is shown (don't restore previous state)
    if (m_textInput) {
        m_textInput->clear();
    }
    if (m_patternInput) {
        m_patternInput->clear();
    }
    m_text.clear();
    m_pattern.clear();
    m_matches.clear();
    m_highlightIndices.clear();
    m_currentTextPos = -1;
    m_currentPatternPos = -1;
    
    // Also clear stored states
    m_naiveText.clear();
    m_naivePattern.clear();
    m_kmpText.clear();
    m_kmpPattern.clear();
    
    updateVisualization();
    setStatusMessage(tr("Enter text and pattern, then click Search."));
}

