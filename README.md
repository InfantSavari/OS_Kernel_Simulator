# OS_Kernel_Simulator


## Overview
The OS Kernel Simulator is a command-line tool designed to simulate basic kernel functionalities, including process management, resource allocation, scheduling, and memory management. The simulator provides an interactive menu for users to create processes, allocate resources, and monitor system behavior.

## Features
- **Process Management**: Create and manage processes with attributes such as process ID, arrival time, execution time, and priority.
- **Resource Allocation**: Simulate resource allocation using a resource allocation table (RAT) with support for multiple resource types.
- **Scheduling Algorithms**: Implement different scheduling policies (FCFS, Round Robin, Priority Scheduling, etc.).
- **Memory Management**: Simulate paging and segmentation with real-time tracking.
- **Deadlock Detection**: Analyze resource dependencies to detect circular wait conditions.

## Installation
### Prerequisites
- C++ compiler (GCC, Clang, or MSVC)
- CMake (optional, for building)

### Build and Run
1. Clone the repository:
   ```sh
   git clone https://github.com/yourusername/os-kernel-simulator.git
   cd os-kernel-simulator
   ```
2. Compile the code using g++:
   ```sh
   g++ -o kernel_simulator main.cpp
   ```
3. Run the executable:
   ```sh
   ./kernel_simulator
   ```

## Usage
Upon running the simulator, you will see a menu with options like:
```
1. Create Process
2. Allocate Resources
3. Display Process Table
4. Run Scheduler
5. Exit
```
Select an option by entering the corresponding number and follow the prompts.

## Code Structure
```
├── src
│   ├── main.cpp                  # Main entry point
│   ├── Process.cpp                # Process management logic
│   ├── Process.h                  # Process class header
│   ├── ResourceAllocationTable.cpp # Resource allocation logic
│   ├── ResourceAllocationTable.h   # Resource allocation header
│   ├── Scheduler.cpp               # Scheduling algorithms
│   ├── Scheduler.h                 # Scheduler class header
│   └── MemoryManager.cpp           # Memory management logic
├── README.md
├── Makefile (if using Make)
└── CMakeLists.txt (if using CMake)
```

## Future Enhancements
- Implement graphical UI with Qt C++.
- Add logging and performance monitoring.
- Extend memory management with virtual memory support.




