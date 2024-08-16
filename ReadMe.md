# Biterm - C++ Shell

## Overview

Biterm is a simple yet functional command-line shell developed in C++. It provides a basic interactive command-line environment for executing common shell commands and managing file operations. Designed for educational purposes and to simulate basic shell functionalities, Biterm offers a customizable and extendable platform for learning about shell command execution and process management in C++.




## Features

Biterm includes the following functionalities:

1. **Command History**:
   - View previously executed commands using the `history` command.
   - Re-run previous commands using `!n` (where `n` is the command number) or `!!` (most recent command).

2. **File and Directory Management**:
   - Create directories using the `mkdir` command.
   - Recognize and handle commands like `vi` as placeholders for text editors.

3. **Command Execution**:
   - Execute common shell commands, including file operations and process control.
   - Integrated handling for basic commands like `ls`, `ps`, and `cal`.

4. **Pipeline Functionality**:
   - Support for piping commands using the `|` symbol to pass the output of one command as input to another.

5. **Welcome Message**:
   - Displays a visually appealing ASCII art welcome message when the shell starts.


## Demo walkthrough

https://github.com/user-attachments/assets/e877d603-af44-4d42-8237-fc71dde40b90



## Running the Biterm Project

### Prerequisites

1. **Install a C++ Compiler**:
   Ensure you have a C++ compiler like `g++` installed on your system. For Ubuntu or WSL, you can install it using:

   ```bash
   sudo apt update
   sudo apt install g++
   ```

2. **Install Git**:
   Make sure Git is installed to clone the repository. If not, install it using:

   ```bash
   sudo apt install git
   ```

### Step-by-Step Instructions

1. **Clone the Repository**:
   Open your terminal and navigate to the directory where you want to clone the project. Run the following command to clone the Biterm repository:

   ```bash
   git clone https://github.com/bhalerao-2002/Bi_term---the-shell.git
   ```

2. **Navigate to the Project Directory**:
   Change into the project directory with:

   ```bash
   cd biterm
   ```

3. **Compile the Code**:
   Use `g++` to compile the `main.cpp` file and create the executable. Run:

   ```bash
   g++ --std=c++11 -o run.out main.cpp
   ```

4. **Run the Biterm Shell**:
   Execute the compiled program with:

   ```bash
   ./run.out
   ```

### Usage

Once Biterm is running, you can use the following commands:

- **View Command History**:
  Type `history` and press Enter to display a list of previously executed commands.

- **Re-run Previous Commands**:
  - Use `!n` (where `n` is the command number) to re-run a command from the history.
  - Use `!!` to re-run the most recent command.

- **Create a Directory**:
  Use `mkdir <directory-name>` to create a new directory. For example:

  ```bash
  mkdir my_new_directory
  ```

- **Simulate Text Editor**:
  Use `vi <file-name>` to simulate opening a file with a text editor. For example:

  ```bash
  vi example.txt
  ```

- **Pipeline Commands**:
  Use `command1 | command2` to pass the output of `command1` as input to `command2`. For example:

  ```bash
  ls | grep .cpp
  ```





                                    Developed with ❤️ by Bi
