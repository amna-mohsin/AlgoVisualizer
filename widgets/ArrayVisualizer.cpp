/**
 * @file ArrayVisualizer.cpp
 * @brief Implementation of the base array visualization widget
 * 
 * This widget provides a bar chart visualization of arrays. It's used by
 * SortingVisualizer to display arrays with different states (comparing,
 * swapping, sorted, etc.). Uses QPainter for custom drawing.
 * 
 * @details
 * The ArrayVisualizer draws arrays as vertical bars where:
 * - Bar height represents the value
 * - Bar color represents the state (red=unsorted, yellow=comparing, green=sorted, etc.)
 * - Supports custom messages displayed below the visualization
 */

#include "ArrayVisualizer.h"

// --------------------- Qt Graphics Includes ---------------------
#include <QPainter>         ///< 2D painting system for drawing bars
#include <QLinearGradient>  ///< Gradient fills for visual appeal
#include <QPen>             ///< Drawing pen for outlines and borders

// --------------------- Standard Library Includes ----------------
#include <algorithm>        ///< STL algorithms (std::max_element, etc.)

/**
 * @brief Constructor - Initializes the array visualizer widget
 * @param parent Parent widget
 * 
 * Sets up the widget with minimum height and expanding size policy.
 * This widget will be used to display arrays as bar charts.
 */
ArrayVisualizer::ArrayVisualizer(QWidget *parent)
    : QWidget(parent)
{
    setMinimumHeight(280);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

/**
 * @brief Sets the array values to be displayed
 * @param values Vector of integers representing array elements
 * 
 * Updates the internal array and ensures state vector matches size.
 * Triggers a repaint to show the new array.
 */
void ArrayVisualizer::setArray(const std::vector<int> &values)
{
    m_values = values;
    ensureStateSize();
    update(); // Triggers paintEvent to redraw
}

/**
 * @brief Sets the visual state for each bar
 * @param states Vector of BarState enum values (one per array element)
 * 
 * Each bar can have a different state (Default, Comparing, Swapping,
 * Sorted, Pivot) which determines its color. Used to highlight algorithm
 * operations during visualization.
 */
void ArrayVisualizer::setStates(const std::vector<BarState> &states)
{
    m_states = states;
    ensureStateSize();
    update(); // Triggers paintEvent to redraw
}

/**
 * @brief Sets a status message to display below the chart
 * @param message Text message to display
 * 
 * Shows algorithm status or step information to the user.
 */
void ArrayVisualizer::setMessage(const QString &message)
{
    m_message = message;
    update(); // Triggers paintEvent to redraw
}

/**
 * @brief Paint event handler - Draws the array as a bar chart
 * @param event Paint event (unused)
 * 
 * This is the core rendering function. It:
 * 1. Draws background and grid lines
 * 2. Calculates bar positions and heights based on values
 * 3. Draws each bar with color based on state
 * 4. Displays values and indices
 * 5. Shows status message if present
 * 
 * Uses QPainter for all drawing operations. Bar height is proportional
 * to value, and color indicates the current state (comparing, sorted, etc.).
 */
void ArrayVisualizer::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const QRectF bounds = rect().adjusted(20, 20, -20, -40);

    // Background
    painter.setBrush(QColor(255, 255, 255));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(bounds.adjusted(-12, -12, 12, 20), 16, 16);

    if (m_values.empty()) {
        painter.setPen(QColor(200, 200, 200));
        painter.drawText(bounds, Qt::AlignCenter, tr("Generate or enter array values to visualize"));
        return;
    }

    ensureStateSize();

    const int barCount = static_cast<int>(m_values.size());
    const int maxValue = std::max(1, *std::max_element(m_values.begin(), m_values.end()));
    const qreal barWidth = bounds.width() / std::max(1, barCount);

    // Grid lines
    painter.setPen(QPen(QColor(209, 220, 235), 1, Qt::DashLine));
    const int gridLines = 4;
    for (int i = 1; i < gridLines; ++i) {
        const qreal y = bounds.bottom() - (bounds.height() / gridLines) * i;
        painter.drawLine(QPointF(bounds.left(), y), QPointF(bounds.right(), y));
    }

    // Draw bars
    for (int i = 0; i < barCount; ++i) {
        const qreal heightRatio = static_cast<qreal>(m_values[i]) / maxValue;
        const qreal barHeight = bounds.height() * heightRatio;
        const qreal x = bounds.left() + i * barWidth;
        const qreal y = bounds.bottom() - barHeight;

        const QRectF barRect(x + barWidth * 0.1, y, barWidth * 0.8, barHeight);

        const QColor baseColor = colorForState(m_states.at(i));
        QLinearGradient gradient(barRect.topLeft(), barRect.bottomLeft());
        gradient.setColorAt(0.0, baseColor.lighter(105));
        gradient.setColorAt(1.0, baseColor.darker(130));
        painter.setBrush(gradient);
        painter.setPen(QPen(QColor(70, 90, 120), 1.2));
        painter.drawRoundedRect(barRect, 6, 6);

        // Value text
        painter.setPen(QColor(44, 62, 80));
        painter.setFont(QFont("Segoe UI", 10, QFont::Bold));
        const QString valueText = QString::number(m_values[i]);
        painter.drawText(barRect.adjusted(0, -22, 0, -barRect.height()), Qt::AlignHCenter | Qt::AlignBottom, valueText);

        // Index text
        painter.setFont(QFont("Segoe UI", 9));
        painter.setPen(QColor(96, 125, 139));
        painter.drawText(QRectF(barRect.left(), bounds.bottom() + 6, barRect.width(), 18),
                         Qt::AlignHCenter | Qt::AlignTop,
                         QString::number(i));
    }

    if (!m_message.isEmpty()) {
        painter.setPen(QColor(15, 52, 96));
        painter.setFont(QFont("Segoe UI", 10, QFont::Medium));
        painter.drawText(QRectF(bounds.left(), rect().bottom() - 24, bounds.width(), 20),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         m_message);
    }
}

/**
 * @brief Returns the color corresponding to a bar state
 * @param state The BarState enum value to get color for
 * @return QColor The color that should be used for this state
 * 
 * @details
 * Maps each visual state to a specific color for algorithm visualization:
 * - Default: Red (#FF636E) - Used for unsorted elements or elements being searched
 * - Comparing: Yellow (#F1C40F) - Highlights elements currently being compared
 * - Swapping: Red (#DC3545) - Shows elements in the process of being swapped
 * - Sorted: Green (#2ECC71) - Indicates elements in their final sorted position
 * - Pivot: Purple (#9B59B6) - Special marker for pivot element in Quick Sort
 * - Found: Blue (#3498DB) - Target element found in search algorithms
 * 
 * @note
 * Colors are chosen for high contrast and visual clarity during algorithm execution.
 */
QColor ArrayVisualizer::colorForState(ArrayVisualizer::BarState state) const
{
    switch (state) {
    case BarState::Comparing:
        return QColor(241, 196, 15);  // yellow
    case BarState::Swapping:
        return QColor(220, 53, 69);   // red
    case BarState::Sorted:
        return QColor(46, 204, 113);  // green
    case BarState::Pivot:
        return QColor(155, 89, 182);  // purple
    case BarState::Found:
        return QColor(52, 152, 219);  // blue
    case BarState::Default:
    default:
        return QColor(255, 99, 110);  // unsorted/searching red
    }
}

/**
 * @brief Ensures the state vector size matches the array size
 * 
 * @details
 * This safety function prevents out-of-bounds access errors. If the state
 * vector size doesn't match the array size (e.g., after setting a new array),
 * it resizes the state vector and initializes all states to Default.
 * 
 * @note
 * Called automatically whenever setArray() or setStates() is invoked.
 * This ensures data consistency between values and their visual states.
 */
void ArrayVisualizer::ensureStateSize()
{
    if (m_states.size() != m_values.size()) {
        m_states.assign(m_values.size(), BarState::Default);
    }
}

