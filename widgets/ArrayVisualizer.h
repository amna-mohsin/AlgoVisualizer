/**
 * @file ArrayVisualizer.h
 * @brief Header file for the ArrayVisualizer widget class
 * @author AlgoVisualizer Team
 * @date 2025
 * 
 * This file declares the ArrayVisualizer class, which provides a visual
 * representation of arrays using bar charts. It supports different visual
 * states for algorithm visualization (comparing, swapping, sorted, etc.).
 */

#ifndef ALGOVISUALIZER_WIDGETS_ARRAYVISUALIZER_H
#define ALGOVISUALIZER_WIDGETS_ARRAYVISUALIZER_H

// --------------------- Qt Includes ------------------------------
#include <QWidget>    ///< Base class for all Qt widgets
#include <QColor>     ///< Color representation for bar states

// --------------------- Standard Library Includes ----------------
#include <vector>     ///< Dynamic array container for storing values and states

/**
 * @class ArrayVisualizer
 * @brief A QWidget-based component for visualizing array operations.
 *
 * @details
 * This class is responsible for drawing bars representing array values
 * and highlighting them based on their current state (comparison, swap, sorted, etc.).
 * It is used by various algorithm visualizers such as sorting algorithms.
 */
class ArrayVisualizer : public QWidget
{
    Q_OBJECT

public:
    /**
     * @enum BarState
     * @brief Visual state of each bar in the array.
     */
    enum class BarState {
        Default,    ///< Normal display — no special state (red for unsorted/searching).
        Comparing,  ///< The value is being compared with another (yellow).
        Swapping,   ///< The value is currently being swapped (red).
        Sorted,     ///< The value has reached its final sorted position (green for sorted/searched).
        Pivot,      ///< Special marker used by algorithms like QuickSort (purple).
        Found       ///< Target element found in search algorithms (blue).
    };

    /**
     * @brief Constructs an ArrayVisualizer widget.
     * @param parent Optional parent QWidget.
     */
    explicit ArrayVisualizer(QWidget *parent = nullptr);

    /**
     * @brief Sets the array values to display.
     * @param values The integer array to store and draw.
     */
    void setArray(const std::vector<int> &values);

    /**
     * @brief Sets the visual state of each bar in the array.
     * @param states A list of BarState values corresponding to each bar.
     */
    void setStates(const std::vector<BarState> &states);

    /**
     * @brief Sets a message to be displayed beneath the visualization.
     * @param message A string describing the current algorithm step.
     */
    void setMessage(const QString &message);

protected:
    /**
     * @brief Handles the widget's painting logic.
     * @param event The paint event triggered by Qt.
     */
    void paintEvent(QPaintEvent *event) override;

private:
    /**
     * @brief Returns the appropriate color for a given bar state.
     * @param state Current visual state of a bar.
     * @return Corresponding QColor.
     */
    QColor colorForState(BarState state) const;

    /**
     * @brief Ensures internal state vector matches the array size.
     *
     * @details
     * Prevents out-of-range access by resizing the state vector
     * whenever a new array is set.
     */
    void ensureStateSize();

    std::vector<int> m_values;         ///< Stored array values.
    std::vector<BarState> m_states;    ///< State of each bar.
    QString m_message;                 ///< Display message.
};

#endif  // ALGOVISUALIZER_WIDGETS_ARRAYVISUALIZER_H
