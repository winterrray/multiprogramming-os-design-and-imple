# multiprogramming-os-design-and-imple

## Introduction
This project implements a basic operating system (OS) with functionalities divided into two phases. The OS manages processes, memory, system interrupts, and input/output operations to execute user programs efficiently.

## Phase 1
### Overview
Phase 1 focuses on building the foundational components of the operating system, including process management, instruction execution, and basic input/output operations.

### Implementation
#### Data Structures
- **Instruction Register (IR)**: Stores the current instruction being executed.
- **General Purpose Register (R)**: Used for general data manipulation.
- **Memory**: Represents main memory divided into 100 blocks, each containing 4 characters.
- **Head Pointer (ptr)**: Points to the current page in memory.
- **System Interrupt (SI)**: Indicates the type of system interrupt.
- **Toggle (C)**: Toggle register for conditional branching.

#### Functions
- **init()**: Initializes memory, registers, and flags.
- **mos()**: Handles system interrupts (GD, PD, Halt).
- **ExecuteUserProgram()**: Executes user programs loaded into memory.
- **Load()**: Loads user programs into memory and executes them.

### Phase 2
#### Overview
Phase 2 extends the functionalities of Phase 1 by introducing memory allocation, address mapping, error handling, and time limit enforcement mechanisms.

#### Implementation
##### Data Structures
- **Process Control Block (PCB)**: Stores information about each process, including job ID, time limits, and counters.
- **Page Table Register (ptr)**: Points to the current page table entry.
- **Visited[]**: Tracks visited memory blocks for page allocation.

##### Functions
- **ALLOCATE()**: Allocates memory for new pages.
- **ADDRESSMAP()**: Maps virtual addresses to physical addresses.
- **MOS()**: Handles system interrupts (GD, PD, Halt) and error conditions.
- **EXECUTE()**: Executes instructions and enforces time limits and error handling.
- **LOAD()**: Loads user programs into memory, manages page allocation, and executes them.

## Usage
1. Compile and run the program using a C++ compiler.
2. Provide input file containing user programs (e.g., input2.txt).
3. The program will execute the loaded programs, handle errors, and generate an output file (Output2.txt).

## Conclusion
The Operating System Project demonstrates the development of a basic operating system with features such as process management, memory allocation, and error handling. It provides a foundation for building more advanced operating systems with enhanced functionalities.

