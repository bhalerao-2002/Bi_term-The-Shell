#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>

#define MAX_CMD_LINE 80
#define ROOT_DIRECTORY "Bi_term-The-Shell"

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

void changeDirectory(const std::string& path) {
    if (chdir(path.c_str()) != 0) {
        std::cerr << "Error: " << strerror(errno) << std::endl;
    }
}

std::string getCurrentDirectory() {
    char cwd[1024];
    std::string result = "";

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        std::string currentPath(cwd);
        std::string rootDir = ROOT_DIRECTORY;

        // Find the position of ROOT_DIRECTORY in the path
        std::size_t pos = currentPath.find(rootDir);
        if (pos != std::string::npos) {
            // Add length of ROOT_DIRECTORY to the position to skip it
            std::size_t start = pos + rootDir.length();
            if (start < currentPath.length()) {
                // Get the remaining path after ROOT_DIRECTORY
                result = currentPath.substr(start);
            }
        } else {
            result = currentPath;
        }
    } else {
        std::cerr << "Error getting current directory: " << strerror(errno) << std::endl;
    }

    return result;
}


int main() {
    printWelcomeMessage();
    char *args[MAX_CMD_LINE/2 + 1];
    int should_run = 1; // Control the main loop
    int argc = 0;       // Argument count

    std::vector<std::string> entered_input;
    std::vector<std::string> command_history;

    std::string currentDirName = ""; // Initialize with an empty string

    while (should_run) {
        if (!currentDirName.empty() && currentDirName != "Bi_term-The-Shell") {
            std::cout << "Biterm>" << currentDirName << "> "; // Display current directory name in prompt
        } else {
            std::cout << "Biterm> "; // Display basic prompt if no directory name is set
        }

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

        // Handle `cd` command
        if (entered_input[0] == "cd") {
            if (argc < 2) {
                std::cerr << "Error: Missing directory argument" << std::endl;
            } else {
                changeDirectory(entered_input[1]);
                currentDirName = getCurrentDirectory(); // Update the current directory name
            }
            continue;
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
