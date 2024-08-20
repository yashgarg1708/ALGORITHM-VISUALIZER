# Algorithm Visualizer Project

## Overview

The Algorithm Visualizer is a project built with C++ and SFML to visually demonstrate various algorithms in an intuitive and interactive way. It provides real-time visualizations for sorting and graph algorithms, helping users better understand their underlying processes.

## Features

- **Sorting Algorithms Visualized**:
  - Implementation and visualization of 5 different sorting algorithms:
    - Bubble Sort
    - Selection Sort
    - Heap Sort
    - Quick Sort
    - Insertion Sort

- **Graph Searching Algorithms Visualized**:
  - Implementation and visualization of 3 different graph searching algorithms:
    - Breadth-First Search (BFS)
    - Depth-First Search (DFS)
    - A* Search

- **Interactive Interface**:
  - Users can observe the algorithms as they execute step by step.
  - The interface allows users to control the progress of the visualization, providing better insight into each algorithm's operation.

## How It Works

### Graph Visualization

- Once the grid appears:
  - **First Click**: Place the starting point.
  - **Second Click**: Place the ending point.
  - **Subsequent Clicks**: Place blocks to simulate obstacles.
  - **Press the Space Bar**: Start the graph search algorithm to find the optimal path.

### Sorting Visualization

- Select a sorting algorithm and watch the data being sorted visually, with control over the process.

## Requirements

To run this project, ensure you have the following installed:

- C++11 or later
- SFML (Simple and Fast Multimedia Library) installed on your system.

### Install SFML (MacOS)

You can install SFML using Homebrew:

```bash
brew install sfml
```

## Compilation Instructions (MacOS)

To compile the project on MacOS, use the following command in your terminal:

```bash
g++ -std=c++11 main.cpp graph.cpp display_graph.cpp sorting.cpp display.cpp -I/opt/homebrew/Cellar/sfml/2.6.1/include -o prog -L/opt/homebrew/Cellar/sfml/2.6.1/lib -lsfml-graphics -lsfml-window -lsfml-system
```

This command will compile the project and link the necessary SFML libraries.

## Running the Project

Once the `prog` executable is created, you can run the program using:

```bash
./prog
```

## Example Usage

- Upon running the program, you will be prompted to select the type of algorithm you want to visualize.
- Choose between sorting algorithms and graph search algorithms.
- In graph algorithms, place your start point, end point, and blocks, then press the space bar to start the visualization.
- Watch the sorting or searching algorithm in action as it progresses step by step on the selected data set or graph.

## Project Structure

- `main.cpp`: The entry point of the program.
- `graph.cpp`: Contains the implementation of graph algorithms such as BFS, DFS, and A*.
- `display_graph.cpp`: Handles the visualization of graph algorithms.
- `sorting.cpp`: Contains the implementation of sorting algorithms.
- `display.cpp`: Manages the visualization of sorting algorithms.

## Future Enhancements

- **Additional Algorithms**: Expanding the project to include more sorting and graph algorithms.
- **Customization**: Allowing users to control dataset size, sorting speeds, and graph structures dynamically.
- **Performance Metrics**: Adding features to display algorithm performance statistics such as execution time and space complexity.


## Contributing

Contributions are welcome! Feel free to fork the project, create a new branch, and submit pull requests for bug fixes, enhancements, or new features.

---

This project is a great resource for developers, students, and anyone interested in gaining a deeper understanding of algorithms through interactive visualization.

## Author


[YASH GARG](https://github.com/yashgarg170)




![Screenshot 2024-08-20 at 9 23 04 PM](https://github.com/user-attachments/assets/a5696622-983c-4474-84e3-d86a0437eefa)
![Screenshot 2024-08-20 at 9 24 02 PM](https://github.com/user-attachments/assets/4e877fd2-e8f7-4c84-b065-071fbdddeb9b)

![Screenshot 2024-08-20 at 9 26 09 PM](https://github.com/user-attachments/assets/2b059195-126f-4dcf-8574-259324b83209)
