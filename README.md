# 🎯 AlgoVisualizer

<div align="center">

**An Interactive Data Structures and Algorithms Visualization Platform**

*Visualize algorithms step-by-step and understand data structures interactively*

[Features](#-features) • [Installation](#-installation) • [Usage](#-usage) • [Algorithms](#-algorithms) • [Screenshots](#-screenshots) • [Contributing](#-contributing)

</div>

---

## 📖 About

AlgoVisualizer is an educational application built with Qt and C++ that provides interactive visualizations of data structures and algorithms. It helps students, educators, and developers understand complex algorithms through real-time, step-by-step animations.

### Why AlgoVisualizer?

- 🎓 **Educational**: Learn algorithms visually with step-by-step execution
- 🎮 **Interactive**: Control algorithm speed and step through operations manually
- 🎨 **Visual**: Color-coded states make algorithm operations clear
- 📚 **Comprehensive**: 7 data structures and 10+ algorithms in one platform
- 🚀 **Modern**: Built with modern C++ and Qt framework

---

## ✨ Features

### 🗂️ Data Structures
- **Array/Vector** - Base visualization for sorting and searching
- **Stack** - LIFO (Last In First Out) operations
- **Queue** - FIFO operations (Regular, Circular, Deque)
- **Linked List** - Singly, Doubly, and Circular implementations
- **Binary Tree** - Tree traversals and operations
- **Graph** - Node-edge representation with algorithms
- **String** - Text pattern matching visualizations

### 🔢 Algorithms

#### Sorting Algorithms
- **Bubble Sort** - O(n²) - Repeatedly swaps adjacent elements
- **Selection Sort** - O(n²) - Finds minimum and swaps
- **Insertion Sort** - O(n²) - Builds sorted array incrementally
- **Merge Sort** - O(n log n) - Divide and conquer
- **Quick Sort** - O(n log n) average - Partition around pivot

#### Searching Algorithms
- **Linear Search** - O(n) - Sequential search through array
- **Binary Search** - O(log n) - Efficient search in sorted arrays

#### Graph Algorithms
- **BFS** (Breadth-First Search) - Level-order traversal
- **DFS** (Depth-First Search) - Recursive exploration
- **Dijkstra's Algorithm** - Shortest path finding

#### String Algorithms
- **Naive Pattern Matching** - O(n*m) - Brute force approach
- **KMP Algorithm** - O(n+m) - Optimized pattern matching

---

## 🚀 Installation

### Prerequisites

  * **C++ Compiler**: GCC/G++ or Clang (or MSVC on Windows).
  * **Qt Framework**: Version 5.12 or higher.
  * **Git**: To clone the repository.

### Step 1: Clone the Repository

Open your terminal (Command Prompt/PowerShell on Windows) and run:

```bash
git clone https://github.com/yourusername/AlgoVisualizer.git
cd AlgoVisualizer/DSA/AlgoVisualizer
```

### Step 2 & 3: Install Dependencies, Build, and Run

#### 🐧 Ubuntu/Debian (Linux) Setup

1.  **Install Qt and Build Tools:**
    ```bash
    sudo apt update
    sudo apt install qt5-default qtbase5-dev build-essential
    ```
2.  **Build and Run:**
    ```bash
    qmake AlgoVisualizer.pro
    make
    ./AlgoVisualizer
    ```

#### 🍏 macOS Setup

1.  **Install Homebrew** (if you haven't already):
    ```bash
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    ```
2.  **Install Qt and Build Tools (using Homebrew):**
    ```bash
    brew install qt@5 gcc
    ```
    *Note: Qt is often installed to a version-specific path. You might need to adjust your PATH or use the full path to `qmake`.*
3.  **Build and Run:**
    ```bash
    # Ensure you are using the correct qmake version
    /usr/local/opt/qt@5/bin/qmake AlgoVisualizer.pro 
    make
    ./AlgoVisualizer.app/Contents/MacOS/AlgoVisualizer
    ```

#### 🖥️ Windows Setup (Recommended: Qt Creator)

Using the **Qt Online Installer** is the simplest method for Windows, as it bundles the compiler (MinGW) and the IDE (Qt Creator).

1.  **Download and Install Qt:**
      * Download the official **Qt Online Installer**.
      * During installation, select:
          * **Qt 5.15.x** (or 5.12+).
          * The appropriate compiler kit, e.g., **MinGW 8.1.0** or **MSVC 2019**.
2.  **Open Project in Qt Creator:**
      * Open **Qt Creator**.
      * Go to `File` → `Open File or Project` and select the `AlgoVisualizer.pro` file from the cloned repository.
3.  **Configure and Build:**
      * Qt Creator should automatically detect your installed Kit (e.g., Desktop Qt 5.15.2 MinGW 64-bit).
      * Click **Build** → **Build Project** (or press `Ctrl+B`).
4.  **Run:**
      * Click **Run** (or press `Ctrl+R`) to launch the application.

-----


## 📖 Usage

### Getting Started

1. **Launch the Application**
   - Run the executable from terminal or Qt Creator
   - Welcome screen will appear

2. **Navigate to Main Menu**
   - Click "Start" button on welcome screen
   - Main menu displays all available data structures

3. **Select a Data Structure**
   - Click on any card (Sorting, Stack, Queue, etc.)
   - Visualizer page will open

4. **Input Data**
   - **Manual Input**: Enter values separated by spaces (e.g., `5 3 8 1 2`)
   - **Random Generation**: Set size and click "Randomize"
   - Click "Apply" to load data

5. **Visualize Algorithm**
   - **Select Algorithm**: Choose from available algorithms (if multiple)
   - **Play**: Click "Play" for automatic execution
   - **Step**: Click "Step" to execute one step at a time
   - **Speed**: Adjust slider to control animation speed
   - **Reset**: Click "Reset" to restart

6. **Navigate Back**
   - Click "Back" button to return to main menu

---

## 🏗️ Project Structure

```
AlgoVisualizer/
├── main.cpp                    # Application entry point
├── StartScreen.h/cpp/ui       # Welcome screen
├── MainMenu.h/cpp/ui          # Main menu with navigation
│
└── widgets/                    # Data structure visualizers
    ├── ArrayVisualizer.h/cpp  # Base visualization widget
    ├── SortingVisualizer.h/cpp # 5 sorting algorithms
    ├── StackVisualizer.h/cpp  # Stack (LIFO)
    ├── QueueVisualizer.h/cpp  # Queue, Circular Queue, Deque
    ├── LinkedListVisualizer.h/cpp # Singly, Doubly, Circular
    ├── TreeVisualizer.h/cpp   # Binary tree traversals
    ├── GraphVisualizer.h/cpp  # BFS, DFS, Dijkstra
    └── StringPatternVisualizer.h/cpp # Naive, KMP
```

---

## 🛠️ Tech Stack

| Component | Technology | Version |
|-----------|-----------|---------|
| **Language** | C++ | 17 |
| **GUI Framework** | Qt | 5.12+ |
| **Build System** | qmake | - |
| **Compiler** | GCC/G++ | 7.0+ |
| **UI Design** | Qt Designer | - |

### Key Libraries

- **Qt5 Core**: Application framework, signals/slots
- **Qt5 Widgets**: UI components, layouts
- **Qt5 GUI**: Custom painting, graphics
- **STL**: Standard containers (vector, queue, map, function)

---

## 📊 Algorithms Overview

### Time Complexity Comparison

| Algorithm | Best Case | Average Case | Worst Case | Space Complexity |
|-----------|-----------|--------------|------------|------------------|
| Bubble Sort | O(n) | O(n²) | O(n²) | O(1) |
| Selection Sort | O(n²) | O(n²) | O(n²) | O(1) |
| Insertion Sort | O(n) | O(n²) | O(n²) | O(1) |
| Merge Sort | O(n log n) | O(n log n) | O(n log n) | O(n) |
| Quick Sort | O(n log n) | O(n log n) | O(n²) | O(log n) |
| Linear Search | O(1) | O(n) | O(n) | O(1) |
| Binary Search | O(1) | O(log n) | O(log n) | O(1) |
| BFS | O(V+E) | O(V+E) | O(V+E) | O(V) |
| DFS | O(V+E) | O(V+E) | O(V+E) | O(V) |
| Dijkstra | O((V+E)log V) | O((V+E)log V) | O((V+E)log V) | O(V) |
| Naive String | O(n) | O(n*m) | O(n*m) | O(1) |
| KMP | O(n+m) | O(n+m) | O(n+m) | O(m) |

*V = Vertices, E = Edges, n = text length, m = pattern length*

---

## 📸 Screenshots

### Main Menu
![Main Menu](<img width="1365" height="689" alt="image" src="https://github.com/user-attachments/assets/5f6559cb-cae8-470f-809c-8877290d4536" />
)
*Main menu displaying all data structure cards*

### Sorting - Bubble Sort
![Bubble Sort](<img width="1365" height="689" alt="image" src="https://github.com/user-attachments/assets/af3d346c-45bc-4ee7-90fe-dfd86917d334" />
)
*Bubble Sort visualization showing comparison and swapping states*

### Sorting - Quick Sort
![Quick Sort](<img width="1365" height="689" alt="image" src="https://github.com/user-attachments/assets/e602041c-a370-41dc-b600-987593f56c20" />
)
*Quick Sort showing pivot element and partitioning*

### Stack Visualization
![Stack](<img width="1365" height="689" alt="image" src="https://github.com/user-attachments/assets/713ba814-92ce-4700-bf5f-bc8e368c964a" />
)
*Stack operations showing LIFO structure*

### Queue Visualization
![Queue](<img width="1365" height="689" alt="image" src="https://github.com/user-attachments/assets/1cda84c1-11ed-48e4-a948-5a4e46d2dcd0" />
)
*Queue showing FIFO structure with front/rear pointers*

### Linked List
![Linked List](<img width="1365" height="689" alt="image" src="https://github.com/user-attachments/assets/10739c7b-adf8-4910-9e19-8232234ccee2" />
)
*Doubly Linked List with node connections*

### Binary Tree
![Binary Tree](<img width="1365" height="689" alt="image" src="https://github.com/user-attachments/assets/1c0b1eda-9a0b-40f3-9256-41766bac0178" />
)
*Binary tree with preorder traversal*


### Graph - Dijkstra
![Dijkstra](<img width="1365" height="689" alt="image" src="https://github.com/user-attachments/assets/133f54b5-b9fe-4c3c-93df-42e3a5e89983" />
)
*Shortest path calculation using Dijkstra's algorithm*

### Search
![Linear Search](<img width="1365" height="689" alt="image" src="https://github.com/user-attachments/assets/e0b65221-1824-4d4a-8541-a9e855be1010" />
)
*Linear Search showing sequential element checking*


### String Pattern Matching - KMP
![KMP](<img width="1365" height="689" alt="image" src="https://github.com/user-attachments/assets/e3c186b5-1320-43d3-9e76-5283a9be86ab" />
)
*KMP algorithm visualization*

---

## 🎯 Key Features Explained

### Command Queue Pattern
Algorithms are pre-computed into a queue of steps, each step being a lambda function. This enables:
- Step-by-step execution
- Replay functionality
- Speed control
- Smooth animations

### State-Based Visualization
Each element has a visual state:
- 🔴 **Default/Unsorted**: Red
- 🟡 **Comparing**: Yellow (elements being compared)
- 🔴 **Swapping**: Red (elements being swapped)
- 🟢 **Sorted**: Green (elements in final position)
- 🟣 **Pivot**: Purple (pivot element in Quick Sort)

### Interactive Controls
- **Play/Pause**: Automatic execution with adjustable speed
- **Step**: Manual step-by-step execution
- **Reset**: Restart algorithm from beginning
- **Speed Slider**: Control animation speed (80ms - 1000ms)

---

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

### How to Contribute

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

### Areas for Contribution

- 🐛 Bug fixes
- ✨ New algorithms
- 🎨 UI/UX improvements
- 📚 Documentation improvements
- 🧪 Additional test cases
- 🌐 Multi-language support

---

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## 👥 Authors

* Developed and maintained by **Amna Mohsin**
* Contributors: [Hamna ALi Khan](https://github.com/HamnaAliKhan) ,[Afeerah Shafqat](https://github.com/Afeerah-S), [Arsalna Shaikh](https://github.com/Arsalna-Shaikh),[Ayesha Amir](https://github.com/AYESHAAMIR01)
* For collaborations or inquiries, connect on [LinkedIn](https://www.linkedin.com/in/amna-m98/)


---

## 🐛 Known Issues

- Large datasets (>16 elements) may cause performance issues in sorting visualizations
- Graph layout may need manual adjustment for complex graphs
- Some algorithms may not visualize optimally on very small screens

---

## 🔮 Roadmap

### Upcoming Features
- [ ] Heap Sort algorithm
- [ ] Radix Sort algorithm
- [ ] Hash Table visualization
- [ ] AVL Tree operations
- [ ] Dark mode theme
- [ ] Export visualization as video/GIF
- [ ] Algorithm comparison mode
- [ ] Performance benchmarking

### Future Enhancements
- [ ] Web version (Qt for WebAssembly)
- [ ] Mobile app (Android/iOS)
- [ ] Multi-language support
- [ ] Cloud synchronization
- [ ] Collaborative learning features

---

## ⭐ Star History

If you find this project helpful, please consider giving it a star ⭐!

---

<div align="center">

**Made with ❤️ using Qt and C++**

[⬆ Back to Top](#-algovisualizer)

</div>





