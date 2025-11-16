/**
 * @file TreeVisualizer.cpp
 * @brief Implementation of the TreeVisualizer widget for binary tree visualization
 * @author AlgoVisualizer Team
 * @date 2025
 * 
 * @details
 * This file implements the TreeVisualizer class which provides interactive
 * visualization for binary tree traversals including:
 * - Preorder Traversal (Root, Left, Right)
 * - Inorder Traversal (Left, Root, Right)
 * - Postorder Traversal (Left, Right, Root)
 * - Level Order Traversal (BFS)
 * 
 * Features:
 * - Build trees from input or random generation
 * - Step-by-step traversal visualization
 * - Visual tree structure rendering
 * - Algorithm information display
 */

#include "TreeVisualizer.h"

// --------------------- Qt Widget Includes -----------------------
#include <QButtonGroup>      ///< Groups traversal type selection buttons
#include <QHBoxLayout>        ///< Horizontal layout for arranging widgets
#include <QVBoxLayout>        ///< Vertical layout for stacking widgets
#include <QPushButton>        ///< Clickable button widgets
#include <QLineEdit>          ///< Single-line text input for tree values
#include <QSpinBox>           ///< Number input for random tree size
#include <QLabel>             ///< Text label for displaying information
#include <QFrame>             ///< Container widget with border styling
#include <QScrollArea>        ///< Scrollable container for large trees
#include <QMessageBox>        ///< Dialog boxes for user notifications
#include <QApplication>       ///< Main application object

// --------------------- Qt Graphics Includes ---------------------
#include <QPainter>           ///< 2D painting system for drawing trees
#include <QLinearGradient>    ///< Linear gradient fills for nodes
#include <QRadialGradient>    ///< Radial gradient fills for nodes
#include <QBrush>             ///< Brush for filling shapes
#include <QPen>               ///< Pen for drawing outlines

// --------------------- Qt Core Includes -------------------------
#include <QRandomGenerator>   ///< Generates random numbers for tree generation
#include <QShowEvent>         ///< Event fired when widget becomes visible
#include <QThread>            ///< Thread management for animations

// --------------------- Standard Library Includes ----------------
#include <algorithm>          ///< STL algorithms
#include <functional>         ///< Function objects and lambdas
#include <queue>              ///< FIFO queue for level order traversal

namespace {
constexpr int kMinTreeSize = 3;
constexpr int kMaxTreeSize = 15;

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

const std::vector<TreeVisualizer::TraversalTypeInfo> TreeVisualizer::kTraversalTypeInfos = {
    {TraversalType::Preorder,
     QObject::tr("Pre-order Traversal"),
     QObject::tr("Root → Left → Right"),
     QObject::tr("Pre-order traversal visits the root node first, then recursively traverses the left subtree, "
                 "and finally the right subtree. This is useful for copying trees or prefix expressions.")},
    {TraversalType::Inorder,
     QObject::tr("In-order Traversal"),
     QObject::tr("Left → Root → Right"),
     QObject::tr("In-order traversal visits the left subtree, then the root, and finally the right subtree. "
                 "For binary search trees, this produces values in sorted order.")},
    {TraversalType::Postorder,
     QObject::tr("Post-order Traversal"),
     QObject::tr("Left → Right → Root"),
     QObject::tr("Post-order traversal visits the left subtree, then the right subtree, and finally the root. "
                 "This is useful for deleting trees or postfix expressions.")},
    {TraversalType::LevelOrder,
     QObject::tr("Level-order Traversal"),
     QObject::tr("Breadth-first level by level"),
     QObject::tr("Level-order traversal visits nodes level by level from top to bottom, left to right. "
                 "This uses a queue and is also known as breadth-first traversal.")}
};

// Custom visualization widget
class TreeVisualizer::TreeVizWidget : public QWidget {
public:
    TreeVisualizer* parentVisualizer;
    TreeVizWidget(QWidget* parent) : QWidget(parent), parentVisualizer(nullptr) {
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
        
        if (parentVisualizer->m_root == nullptr) {
            painter.setPen(QPen(QColor("#95a5a6"), 2, Qt::DashLine));
            painter.setBrush(QBrush(QColor("#ecf0f1")));
            int centerX = width() / 2 - 100;
            int centerY = height() / 2;
            painter.drawRect(centerX, centerY, 200, 50);
            painter.setPen(QColor("#7f8c8d"));
            painter.setFont(QFont("Segoe UI", 12));
            painter.drawText(QRect(centerX, centerY, 200, 50), Qt::AlignCenter, "EMPTY TREE");
            return;
        }
        
        parentVisualizer->drawTree(painter, parentVisualizer->m_root, width() / 2, 50, 0, width() / 4);
    }
};

TreeVisualizer::TreeVisualizer(QWidget *parent)
    : QWidget(parent),
      m_visualizerWidget(nullptr),
      m_traversalTypeButtons(new QButtonGroup(this)),
      m_treeInput(nullptr),
      m_sizeSpinBox(nullptr),
      m_statusLabel(nullptr),
      m_traversalTypeTitleLabel(nullptr),
      m_traversalTypeSubtitleLabel(nullptr),
      m_traversalTypeDescriptionLabel(nullptr),
      m_treeStatsLabel(nullptr),
      m_buildButton(nullptr),
      m_runButton(nullptr),
      m_stopButton(nullptr),
      m_clearButton(nullptr),
      m_randomButton(nullptr),
      m_currentTraversalType(TraversalType::Preorder),
      m_root(nullptr),
      m_vizWidget(nullptr),
      m_isRunning(false)
{
    setupUi();
    updateInfoPanel();
    updateTraversalType(TraversalType::Preorder);
    updateVisualization();
}

void TreeVisualizer::setupUi()
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
    auto headerLabel = new QLabel(tr("Tree Traversal Visualizer"), headerBar);
    headerLabel->setStyleSheet(QStringLiteral("color: #0f3460; font-size: 22px; font-weight: 700;"));
    headerLayout->addWidget(headerLabel);
    headerLayout->addStretch();
    mainLayout->addWidget(headerBar);

    auto navBar = new QFrame(centralContainer);
    navBar->setStyleSheet(QStringLiteral("QFrame { background-color: #ffffff; border-radius: 12px; border: 1px solid #d8e3f0; }"));
    auto algorithmLayout = new QHBoxLayout(navBar);
    algorithmLayout->setSpacing(10);
    algorithmLayout->setContentsMargins(12, 10, 12, 10);
    setupTraversalTypeButtons(algorithmLayout);
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

    m_vizWidget = new TreeVizWidget(this);
    m_vizWidget->parentVisualizer = this;
    m_vizWidget->setMinimumHeight(400);
    chartLayout->addWidget(m_vizWidget);
    m_visualizerWidget = m_vizWidget;
    leftColumn->addWidget(chartCard, 3);

    const QString controlStyle = "QPushButton { background-color: #0f3460; color: #ffffff; font-size: 14px; padding: 10px 18px; border-radius: 8px; border: none; font-weight: 600; min-height: 40px; max-height: 40px; min-width: 100px; max-width: 120px; } QPushButton:hover { background-color: #1a5fb4; }";

    m_runButton = new QPushButton(tr("Run"), this);
    m_stopButton = new QPushButton(tr("Stop"), this);
    m_clearButton = new QPushButton(tr("Clear"), this);
    m_runButton->setStyleSheet(controlStyle);
    m_stopButton->setStyleSheet(controlStyle);
    m_stopButton->setEnabled(false);
    m_clearButton->setStyleSheet(controlStyle);

    auto controlsCard = new QFrame(centralContainer);
    controlsCard->setObjectName("controlsCard");
    controlsCard->setStyleSheet(QStringLiteral("QFrame#controlsCard { background-color: #ffffff; border-radius: 16px; border: 1px solid #d8e3f0; }"));
    auto controlsLayout = new QVBoxLayout(controlsCard);
    controlsLayout->setContentsMargins(20, 16, 20, 18);
    controlsLayout->setSpacing(14);
    auto buttonRow1 = new QHBoxLayout();
    buttonRow1->setSpacing(12);
    buttonRow1->addWidget(m_runButton);
    buttonRow1->addWidget(m_stopButton);
    buttonRow1->addWidget(m_clearButton);
    buttonRow1->addStretch();
    controlsLayout->addLayout(buttonRow1);
    leftColumn->addWidget(controlsCard, 1);

    auto inputCard = new QFrame(centralContainer);
    inputCard->setObjectName("inputCard");
    inputCard->setStyleSheet(QStringLiteral("QFrame#inputCard { background-color: #ffffff; border-radius: 16px; border: 1px solid #d8e3f0; }"));
    auto inputLayout = new QVBoxLayout(inputCard);
    inputLayout->setContentsMargins(20, 18, 20, 18);
    inputLayout->setSpacing(14);

    auto manualInputLayout = new QHBoxLayout();
    auto manualInputLabel = new QLabel(tr("Values"), this);
    manualInputLabel->setStyleSheet(QStringLiteral("color: #0f3460; font-size: 13px; font-weight: 600;"));
    m_treeInput = new QLineEdit(this);
    m_treeInput->setPlaceholderText(tr("Enter numbers separated by spaces"));
    m_treeInput->setStyleSheet(QStringLiteral("QLineEdit { background-color: #f9fbff; border: 1px solid #c2d4eb; border-radius: 8px; color: #2c3e50; padding: 10px 14px; font-size: 13px; min-height: 40px; max-height: 40px; } QLineEdit:focus { border-color: #0f3460; }"));
    m_buildButton = new QPushButton(tr("Build Tree"), this);
    m_buildButton->setStyleSheet(controlStyle);
    m_buildButton->setMinimumWidth(120);
    manualInputLayout->addWidget(manualInputLabel);
    manualInputLayout->addWidget(m_treeInput, 1);
    manualInputLayout->addWidget(m_buildButton);
    inputLayout->addLayout(manualInputLayout);

    auto randomLayout = new QHBoxLayout();
    auto sizeLabel = new QLabel(tr("Size"), this);
    sizeLabel->setStyleSheet(QStringLiteral("color: #0f3460; font-size: 13px; font-weight: 600;"));
    m_sizeSpinBox = new QSpinBox(this);
    m_sizeSpinBox->setRange(kMinTreeSize, kMaxTreeSize);
    m_sizeSpinBox->setValue(7);
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

    connect(upButton, &QPushButton::clicked, m_sizeSpinBox, &QSpinBox::stepUp);
    connect(downButton, &QPushButton::clicked, m_sizeSpinBox, &QSpinBox::stepDown);

    m_randomButton = new QPushButton(tr("Random Tree"), this);
    m_randomButton->setStyleSheet(controlStyle);
    m_randomButton->setMinimumWidth(130);
    randomLayout->addWidget(sizeLabel);
    randomLayout->addLayout(customSpinBoxLayout);
    randomLayout->addSpacing(6);
    randomLayout->addWidget(m_randomButton);
    randomLayout->addStretch();
    inputLayout->addLayout(randomLayout);
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

    m_traversalTypeTitleLabel = new QLabel(this);
    m_traversalTypeTitleLabel->setStyleSheet(QStringLiteral("color: #0f3460; font-size: 24px; font-weight: 700;"));
    infoLayout->addWidget(m_traversalTypeTitleLabel);

    m_traversalTypeSubtitleLabel = new QLabel(this);
    m_traversalTypeSubtitleLabel->setStyleSheet(QStringLiteral("color: #7a8ca6; font-size: 13px; font-weight: 600;"));
    infoLayout->addWidget(m_traversalTypeSubtitleLabel);

    auto infoDivider = new QFrame(infoFrame);
    infoDivider->setFrameShape(QFrame::HLine);
    infoDivider->setStyleSheet(QStringLiteral("QFrame { color: #d8e3f0; }"));
    infoLayout->addWidget(infoDivider);

    m_traversalTypeDescriptionLabel = new QLabel(this);
    m_traversalTypeDescriptionLabel->setWordWrap(true);
    m_traversalTypeDescriptionLabel->setStyleSheet(QStringLiteral("color: #2c3e50; font-size: 13px; line-height: 1.6;"));
    infoLayout->addWidget(m_traversalTypeDescriptionLabel);

    auto statsContainer = new QFrame(infoFrame);
    statsContainer->setObjectName("statsContainer");
    statsContainer->setStyleSheet(QStringLiteral("QFrame#statsContainer { background-color: #f9fbff; border-radius: 12px; border: 1px solid #d8e3f0; }"));
    auto statsLayout = new QVBoxLayout(statsContainer);
    statsLayout->setContentsMargins(18, 14, 18, 14);
    m_treeStatsLabel = new QLabel(this);
    m_treeStatsLabel->setStyleSheet(QStringLiteral("color: #0f3460; font-size: 13px; font-weight: 600;"));
    statsLayout->addWidget(m_treeStatsLabel);
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

    connect(m_traversalTypeButtons, &QButtonGroup::idClicked, this, &TreeVisualizer::handleTraversalTypeButton);
    connect(m_buildButton, &QPushButton::clicked, this, &TreeVisualizer::handleBuildTree);
    connect(m_runButton, &QPushButton::clicked, this, &TreeVisualizer::handleRun);
    connect(m_stopButton, &QPushButton::clicked, this, &TreeVisualizer::handleStop);
    connect(m_clearButton, &QPushButton::clicked, this, &TreeVisualizer::handleClear);
    connect(m_randomButton, &QPushButton::clicked, this, &TreeVisualizer::handleRandomTree);
}

void TreeVisualizer::setupTraversalTypeButtons(QHBoxLayout *layout)
{
    m_traversalTypeButtons->setExclusive(true);
    int index = 0;
    for (const auto &info : kTraversalTypeInfos) {
        auto button = new QPushButton(info.title, this);
        button->setCheckable(true);
        button->setStyleSheet("QPushButton { background-color: #ffffff; color: #0f3460; padding: 10px 18px; border-radius: 10px; border: 1px solid #d8e3f0; font-size: 13px; font-weight: 600; } QPushButton:hover { background-color: #e8f1ff; } QPushButton:checked { background-color: #0f3460; color: #ffffff; border: 1px solid #0f3460; }");
        m_traversalTypeButtons->addButton(button, index);
        layout->addWidget(button);
        if (info.type == TraversalType::Preorder) button->setChecked(true);
        ++index;
    }
    layout->addStretch();
}

void TreeVisualizer::handleTraversalTypeButton(int id)
{
    const int clampedId = qBound(0, id, static_cast<int>(kTraversalTypeInfos.size()) - 1);
    updateTraversalType(kTraversalTypeInfos.at(clampedId).type);
}

void TreeVisualizer::updateTraversalType(TraversalType type)
{
    if (m_currentTraversalType == type) return;
    m_currentTraversalType = type;
    updateInfoPanel();
}

void TreeVisualizer::updateInfoPanel()
{
    const auto it = std::find_if(kTraversalTypeInfos.begin(), kTraversalTypeInfos.end(),
                                 [this](const TraversalTypeInfo &info) { return info.type == m_currentTraversalType; });
    if (it == kTraversalTypeInfos.end()) return;
    m_traversalTypeTitleLabel->setText(it->title);
    m_traversalTypeSubtitleLabel->setText(it->subtitle);
    m_traversalTypeDescriptionLabel->setText(it->description);
}

void TreeVisualizer::updateVisualization()
{
    if (m_vizWidget) m_vizWidget->update();
    if (m_root == nullptr) {
        m_treeStatsLabel->setText(tr("Tree: Empty"));
        setStatusMessage(tr("Tree is empty. Build a tree first."));
    } else {
        int nodeCount = 0;
        std::function<void(TreeNode*)> count = [&](TreeNode* node) {
            if (!node) return;
            nodeCount++;
            count(node->left);
            count(node->right);
        };
        count(m_root);
        m_treeStatsLabel->setText(tr("Tree: %1 nodes").arg(nodeCount));
    }
}

void TreeVisualizer::setStatusMessage(const QString &message)
{
    m_lastStatusMessage = message;
    if (m_statusLabel) m_statusLabel->setText(message);
}

void TreeVisualizer::handleBuildTree()
{
    bool ok = false;
    const auto values = parseInputValues(&ok);
    if (!ok || values.empty()) {
        setStatusMessage(tr("Invalid input. Please enter integers separated by spaces."));
        return;
    }
    clearTree(m_root);
    m_root = buildTreeFromArray(values);
    m_traversalResult.clear();
    m_highlightedNodes.clear();
    updateVisualization();
    setStatusMessage(tr("Tree built successfully with %1 nodes.").arg(values.size()));
}

void TreeVisualizer::handleRandomTree()
{
    clearTree(m_root);
    m_root = nullptr;
    const auto values = generateRandomValues(m_sizeSpinBox->value());
    m_root = buildTreeFromArray(values);
    m_traversalResult.clear();
    m_highlightedNodes.clear();
    updateVisualization();
    setStatusMessage(tr("Random tree generated with %1 nodes.").arg(values.size()));
}

void TreeVisualizer::handleRun()
{
    if (m_root == nullptr) {
        QMessageBox::warning(this, tr("Empty Tree"), tr("Please build a tree first!"));
        return;
    }
    if (m_isRunning) return;
    
    m_isRunning = true;
    m_runButton->setEnabled(false);
    m_stopButton->setEnabled(true);
    
    m_traversalResult.clear();
    m_highlightedNodes.clear();
    
    switch (m_currentTraversalType) {
    case TraversalType::Preorder: {
        m_currentTraversal = "Preorder";
        std::function<void(TreeNode*)> preorder = [&](TreeNode* node) {
            if (node == nullptr || !m_isRunning) return;
            m_traversalResult.push_back(node->data);
            m_highlightedNodes.push_back(node->data);
            updateVisualization();
            QApplication::processEvents();
            QThread::msleep(500);
            preorder(node->left);
            preorder(node->right);
        };
        setStatusMessage(tr("Pre-order Traversal (Root → Left → Right)"));
        preorder(m_root);
        break;
    }
    case TraversalType::Inorder: {
        m_currentTraversal = "Inorder";
        std::function<void(TreeNode*)> inorder = [&](TreeNode* node) {
            if (node == nullptr || !m_isRunning) return;
            inorder(node->left);
            if (!m_isRunning) return;
            m_traversalResult.push_back(node->data);
            m_highlightedNodes.push_back(node->data);
            updateVisualization();
            QApplication::processEvents();
            QThread::msleep(500);
            inorder(node->right);
        };
        setStatusMessage(tr("In-order Traversal (Left → Root → Right)"));
        inorder(m_root);
        break;
    }
    case TraversalType::Postorder: {
        m_currentTraversal = "Postorder";
        std::function<void(TreeNode*)> postorder = [&](TreeNode* node) {
            if (node == nullptr || !m_isRunning) return;
            postorder(node->left);
            postorder(node->right);
            if (!m_isRunning) return;
            m_traversalResult.push_back(node->data);
            m_highlightedNodes.push_back(node->data);
            updateVisualization();
            QApplication::processEvents();
            QThread::msleep(500);
        };
        setStatusMessage(tr("Post-order Traversal (Left → Right → Root)"));
        postorder(m_root);
        break;
    }
    case TraversalType::LevelOrder: {
        m_currentTraversal = "Level Order";
        std::queue<TreeNode*> q;
        q.push(m_root);
        setStatusMessage(tr("Level-order Traversal (BFS)"));
        while (!q.empty() && m_isRunning) {
            TreeNode* node = q.front();
            q.pop();
            m_traversalResult.push_back(node->data);
            m_highlightedNodes.push_back(node->data);
            updateVisualization();
            QApplication::processEvents();
            QThread::msleep(500);
            if (node->left != nullptr) q.push(node->left);
            if (node->right != nullptr) q.push(node->right);
        }
        break;
    }
    }
    
    if (m_isRunning) {
        QStringList resultList;
        for (int val : m_traversalResult) {
            resultList << QString::number(val);
        }
        setStatusMessage(tr("Result: %1").arg(resultList.join(" → ")));
    } else {
        setStatusMessage(tr("Traversal stopped."));
    }
    
    m_highlightedNodes.clear();
    updateVisualization();
    m_isRunning = false;
    m_runButton->setEnabled(true);
    m_stopButton->setEnabled(false);
}

void TreeVisualizer::handleStop()
{
    m_isRunning = false;
    m_runButton->setEnabled(true);
    m_stopButton->setEnabled(false);
    setStatusMessage(tr("Stopping traversal..."));
}

void TreeVisualizer::handleClear()
{
    clearTree(m_root);
    m_root = nullptr;
    m_traversalResult.clear();
    m_highlightedNodes.clear();
    m_currentTraversal = "";
    updateVisualization();
    setStatusMessage(tr("Tree cleared."));
}

void TreeVisualizer::handleApplyInput()
{
    handleBuildTree();
}

void TreeVisualizer::handleBackButton()
{
    emit requestBackToMenu();
}

void TreeVisualizer::clearTree(TreeNode* node)
{
    if (node == nullptr) return;
    clearTree(node->left);
    clearTree(node->right);
    delete node;
}

TreeNode* TreeVisualizer::buildTreeFromArray(const std::vector<int>& values)
{
    if (values.empty()) return nullptr;
    
    TreeNode* root = new TreeNode(values[0]);
    std::queue<TreeNode*> q;
    q.push(root);
    
    for (size_t i = 1; i < values.size(); i += 2) {
        TreeNode* current = q.front();
        q.pop();
        
        if (i < values.size()) {
            current->left = new TreeNode(values[i]);
            q.push(current->left);
        }
        
        if (i + 1 < values.size()) {
            current->right = new TreeNode(values[i + 1]);
            q.push(current->right);
        }
    }
    
    return root;
}

void TreeVisualizer::drawTree(QPainter& painter, TreeNode* node, int x, int y, int level, int width)
{
    if (node == nullptr) return;
    
    int nodeRadius = 25;
    int verticalSpacing = 80;
    int horizontalSpacing = width;
    
    bool isHighlighted = std::find(m_highlightedNodes.begin(), m_highlightedNodes.end(), node->data) != m_highlightedNodes.end();
    
    if (node->left != nullptr) {
        int childX = x - horizontalSpacing;
        int childY = y + verticalSpacing;
        painter.setPen(QPen(QColor("#34495e"), 2));
        painter.drawLine(x, y + nodeRadius, childX, childY - nodeRadius);
        drawTree(painter, node->left, childX, childY, level + 1, horizontalSpacing / 2);
    }
    
    if (node->right != nullptr) {
        int childX = x + horizontalSpacing;
        int childY = y + verticalSpacing;
        painter.setPen(QPen(QColor("#34495e"), 2));
        painter.drawLine(x, y + nodeRadius, childX, childY - nodeRadius);
        drawTree(painter, node->right, childX, childY, level + 1, horizontalSpacing / 2);
    }
    
    if (isHighlighted) {
        painter.setPen(QPen(QColor("#e74c3c"), 3));
        QRadialGradient gradient(x, y, nodeRadius);
        gradient.setColorAt(0, QColor("#ff6b6b"));
        gradient.setColorAt(1, QColor("#ee5a6f"));
        painter.setBrush(QBrush(gradient));
    } else {
        painter.setPen(QPen(QColor("#3498db"), 2));
        QRadialGradient gradient(x, y, nodeRadius);
        gradient.setColorAt(0, QColor("#85c1e2"));
        gradient.setColorAt(1, QColor("#5dade2"));
        painter.setBrush(QBrush(gradient));
    }
    
    painter.drawEllipse(x - nodeRadius, y - nodeRadius, nodeRadius * 2, nodeRadius * 2);
    
    painter.setPen(QColor("#2c3e50"));
    painter.setFont(QFont("Segoe UI", 12, QFont::Bold));
    painter.drawText(QRect(x - nodeRadius, y - nodeRadius, nodeRadius * 2, nodeRadius * 2), Qt::AlignCenter, QString::number(node->data));
}

std::vector<int> TreeVisualizer::parseInputValues(bool *ok) const
{
    if (ok) *ok = false;
    const QString text = m_treeInput->text();
    const QStringList parts = text.split(' ', Qt::SkipEmptyParts);
    std::vector<int> values;
    values.reserve(parts.size());
    for (const QString &part : parts) {
        bool localOk = false;
        const int value = part.toInt(&localOk);
        if (!localOk) return {};
        values.push_back(value);
    }
    if (values.empty()) return {};
    if (ok) *ok = true;
    return values;
}

std::vector<int> TreeVisualizer::generateRandomValues(int size) const
{
    std::vector<int> values;
    values.reserve(size);
    for (int i = 0; i < size; ++i) {
        values.push_back(static_cast<int>(QRandomGenerator::global()->bounded(10, 120)));
    }
    return values;
}

