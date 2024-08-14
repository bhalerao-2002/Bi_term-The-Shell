#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
// #include <cstdlib>
// #include <string>


// Define the maximum command line size
#define MAX_CMD_LINE 80

// void launchDetached() {
//     std::string batchFilePath = "D:\\biterm.bat";  // Update with the correct Windows path

//     // Command to open a new Windows Command Prompt window using PowerShell and execute the batch file
//     std::string command = "start cmd.exe /c \"" + batchFilePath + "\"";    
//     // Execute the command to open a new terminal window
//     std::system(command.c_str());
// }

void printWelcomeMessage() {
    std::string message = R"(
                                                    Welcome to 

                                    ████████████████████████████████████████████
                                    █▄─▄─▀█▄─▄█▀▀▀▀▀██─▄─▄─█▄─▄▄─█▄─▄▄▀█▄─▀█▀─▄█
                                    ██─▄─▀██─███████████─████─▄█▀██─▄─▄██─█▄█─██
                                    ▀▄▄▄▄▀▀▄▄▄▀▀▀▀▀▀▀▀▀▄▄▄▀▀▄▄▄▄▄▀▄▄▀▄▄▀▄▄▄▀▄▄▄▀                                               

                                                Future of Shell
                                      (⚡ Powered by BhaleraoInnovations ⚡)

   )";

    std::cout << message << std::endl;
}

void executeCommand(char **args) {
    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Fork failed!" << std::endl;
        exit(1);
    } else if (pid == 0) {
        execvp(args[0], args); // Execute the command
        std::cerr << "Error: Command not found" << std::endl;
        exit(1);
    } else {
        wait(NULL); // Parent process waits for the child to complete
    }
}

void executePipedCommands(char **args1, char **args2) {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        std::cerr << "Pipe failed!" << std::endl;
        exit(1);
    }

    pid_t pid1 = fork();
    if (pid1 < 0) {
        std::cerr << "Fork failed!" << std::endl;
        exit(1);
    } else if (pid1 == 0) {
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe's write end
        close(pipefd[0]); // Close unused read end
        close(pipefd[1]);
        execvp(args1[0], args1); // Execute the first command
        std::cerr << "Error: Command not found" << std::endl;
        exit(1);
    }

    pid_t pid2 = fork();
    if (pid2 < 0) {
        std::cerr << "Fork failed!" << std::endl;
        exit(1);
    } else if (pid2 == 0) {
        dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to pipe's read end
        close(pipefd[1]); // Close unused write end
        close(pipefd[0]);
        execvp(args2[0], args2); // Execute the second command
        std::cerr << "Error: Command not found" << std::endl;
        exit(1);
    }

    close(pipefd[0]);
    close(pipefd[1]);
    wait(NULL); // Parent process waits for the first child to complete
    wait(NULL); // Parent process waits for the second child to complete
}

int main() {
    // launchDetached();
    printWelcomeMessage();
    char *args[MAX_CMD_LINE/2 + 1];
    int should_run = 1; // Control the main loop
    int argc = 0;       // Argument count

    std::vector<std::string> entered_input;
    std::vector<std::string> command_history;

    while (should_run) {
        std::cout << "Biterm>"; // Custom shell prompt
        std::string input;
        std::getline(std::cin, input); // Get user input

        if (input == "exit") {
            std::cout << "Exiting the program..." << std::endl;
            break;
        }

        if (input == "history") {
            int start = std::max(0, static_cast<int>(command_history.size()) - 10);
            for (int i = start; i < command_history.size(); ++i) {
                std::cout << i + 1 << ": " << command_history[i] << std::endl;
            }
            continue;
        }

        command_history.push_back(input); // Save command to history

        std::istringstream iss(input);
        std::string token;
        argc = 0;
        entered_input.clear();

        while (iss >> token) {
            entered_input.push_back(token);
            argc++;
        }

        bool isPipe = false;
        int pipeIndex = -1;

        // Check if the input contains a pipe '|'
        for (int i = 0; i < entered_input.size(); ++i) {
            if (entered_input[i] == "|") {
                isPipe = true;
                pipeIndex = i;
                break;
            }
        }

        if (isPipe) {
            char *args1[MAX_CMD_LINE/2 + 1];
            char *args2[MAX_CMD_LINE/2 + 1];

            for (int i = 0; i < pipeIndex; ++i) {
                args1[i] = const_cast<char*>(entered_input[i].c_str());
            }
            args1[pipeIndex] = NULL;

            for (int i = pipeIndex + 1; i < argc; ++i) {
                args2[i - pipeIndex - 1] = const_cast<char*>(entered_input[i].c_str());
            }
            args2[argc - pipeIndex - 1] = NULL;

            executePipedCommands(args1, args2);
        } else {
            for (int i = 0; i < argc; ++i) {
                args[i] = const_cast<char*>(entered_input[i].c_str());
            }
            args[argc] = NULL;

            executeCommand(args);
        }
    }

    return 0;
}
