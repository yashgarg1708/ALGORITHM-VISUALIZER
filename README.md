# Algorithm Visualizer Project

## Overview

Algorithm Visualizer is a C++ and SFML project that demonstrates sorting and graph pathfinding algorithms in an interactive, real-time UI.

## Features

### Sorting Algorithms Visualized

- Bubble Sort
- Selection Sort
- Heap Sort
- Quick Sort
- Insertion Sort

### Graph Algorithms Visualized

- Breadth-First Search (BFS)
- Depth-First Search (DFS)
- Dijkstra

### Interactive Interface

- Real-time animation for both sorting and graph traversal.
- Pause/resume, rerun, speed controls, and randomization.
- Visual state cues for comparisons, visited nodes, and final path.

## How It Works

### Graph Visualization

- Open graph mode from the console menu.
- First left click places the **Start** node.
- Second left click places the **End** node.
- Further left clicks toggle **Block** cells.
- Right click erases a cell (also removes Start/End if clicked).
- Press `Space` to start traversal.

### Sorting Visualization

- Choose a sorting algorithm from the sorting window (`1..5`).
- Press `Space` to start/pause/resume.
- Press `Enter` to rerun on the same data.
- Press `R` to randomize the dataset.

## Controls

### Sorting Window

- `1..5`: choose algorithm
- `Space`: start / pause / resume
- `Enter`: rerun current algorithm
- `R`: randomize data
- `Up/Down`: adjust speed
- `Esc`: close window

### Graph Window

- `Left click`: Start -> End -> toggle Blocks
- `Right click`: erase cell
- `1/2/3`: BFS / DFS / Dijkstra
- `Space`: start / pause / resume
- `Enter`: rerun traversal
- `R`: randomize blocks
- `C`: clear grid
- `Up/Down`: adjust speed
- `Esc`: close window

## Requirements

- C++17 or later
- SFML 3.x
- CMake 3.16+

### Install SFML (macOS)

```bash
brew install sfml
```

## Build Instructions

```bash
cmake -S . -B build
cmake --build build
```

## Run

```bash
./build/algoviz
```

## Project Structure

- `main.cpp`: entry point + menu routing
- `display.cpp`: sorting renderer and controls
- `display_graph.cpp`: graph renderer and controls
- `sorting.cpp` / `sorting.hpp`: sorting logic and operation generation
- `graph.cpp` / `graph.hpp`: BFS/DFS/Dijkstra logic and traversal result model

## Future Enhancements

- Add more algorithms.
- Add UI options for custom grid size and dataset size.
- Show runtime/operation metrics per algorithm.

## Contributing

Contributions are welcome via issues and pull requests.

## Author

[Yash Garg](https://github.com/yashgarg1708)

<img width="1512" height="901" alt="Screenshot 2026-03-08 at 4 02 03 PM" src="https://github.com/user-attachments/assets/bdb10ca2-a083-48e9-b751-583fa9558a57" />
<img width="1512" height="900" alt="Screenshot 2026-03-08 at 4 02 32 PM" src="https://github.com/user-attachments/assets/283d912e-bc88-44b3-85ba-a4c08db42d6c" />
<img width="1512" height="913" alt="Screenshot 2026-03-08 at 4 02 53 PM" src="https://github.com/user-attachments/assets/242e84ab-57e9-4f8c-8806-936dac9c689e" />
<img width="1512" height="951" alt="Screenshot 2026-03-08 at 4 08 13 PM" src="https://github.com/user-attachments/assets/87f0bcae-d5ba-40a6-a57f-eda63f3211d0" />
<img width="1509" height="952" alt="Screenshot 2026-03-08 at 4 08 57 PM" src="https://github.com/user-attachments/assets/601b4580-46bc-4c54-bb54-7cda5391b8bf" />


