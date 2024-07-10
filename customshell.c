#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

void space();

int main()
{
    char command[1024];
    bool promptOn = true;
    char cwd[PATH_MAX];
    char *path = getcwd(cwd, sizeof(cwd));
    char dirPath[1024];
    DIR *directory;
    struct dirent *entry;
    int max_length = 0;
    int current_length;
    int files = 0;

    printf("Welcome to CustomShell!\n");
    printf("If you require help run command 'help' to learn about commands!\n");
    for (int i = 0; i < 4; i++)
    {
        space();
    }

    while (promptOn)
    {
        printf("%s $ myshell> ", path);
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0';

        char *cmd = strtok(command, " ");
        char *dirPath = strtok(NULL, " ");

        if (cmd == NULL)
        {
            continue;
        }

        if (strcmp(cmd, "exit") == 0)
        {
            promptOn = false;
        }
        else if (strcmp(cmd, "help") == 0)
        {
            printf("+-----------------------------------------------------------------------+\n");
            printf("| This is the list of valid Shell commands:                             |\n");
            printf("+-----------------------------------------------------------------------+\n");
            printf("| cd - changes current working directory to a specified one.            |\n");
            printf("| clear - clears the terminal's display.                                |\n");
            printf("| exit - exits Shell.                                                   |\n");
            printf("| help - displays all valid Shell commands and their explanations.      |\n");
            printf("| ls - displays all existing files within current directory.            |\n");
            printf("| mkdir - creates new directory within current directory.               |\n");
            printf("| rm - removes a specified file within current directory.               |\n");
            printf("| rmdir - removes an empty specified directory within current directory.|\n");
            printf("| touch - creates new file within current directory.                    |\n");
            printf("+-----------------------------------------------------------------------+\n");
        }
        else if (cmd != NULL && strcmp(cmd, "cd") == 0)
        {
            if (chdir(dirPath) == 0)
            {
                path = getcwd(cwd, sizeof(cwd));
                dirPath[0] = '\0';
            }
            else
            {
                printf("Invalid directory name: %s ! Try again.\n", dirPath);
            }
        }
        else if (cmd != NULL && strcmp(cmd, "clear") == 0)
        {
            system(cmd);
        }
        else if (strcmp(cmd, "ls") == 0)
        {
            directory = opendir(path);
            if (directory == NULL)
            {
                perror("Unable to read directory! Try again.\n");
            }

            while ((entry = readdir(directory)))
            {
                current_length = strlen(entry->d_name);
                if (current_length > max_length)
                {
                    max_length = current_length;
                }
            }
            rewinddir(directory);

            int total_width = 7 + max_length;

            char border[total_width + 3];
            memset(border, '-', total_width + 2);
            border[0] = '+';
            border[total_width + 1] = '+';
            border[total_width + 2] = '\0';

            printf("List contains existing files of directory '%s'.\n", path);
            printf("%s\n", border);

            while ((entry = readdir(directory)))
            {
                files++;
                printf("| %3d: %-*s |\n", files, max_length, entry->d_name);
            }
            printf("%s\n", border);

            closedir(directory);
            files = 0;
            dirPath[0] = '\0';
        }
        else if (strcmp(cmd, "mkdir") == 0)
        {
            if (mkdir(dirPath) == 0)
            {
                printf("Directory '%s' created successfully.\n", dirPath);
                dirPath[0] = '\0';
            }
            else
            {
                perror("Error creating directory!");
            }
        }
        else if (strcmp(cmd, "rm") == 0)
        {
            if (remove(dirPath) == 0)
            {
                printf("File '%s' deleted successfully.\n", dirPath);
                dirPath[0] = '\0';
            }
            else
            {
                perror("Error deleting file!");
            }
        }
        else if (strcmp(cmd, "rmdir") == 0)
        {
            if (rmdir(dirPath) == 0)
            {
                printf("Directory '%s' deleted successfully.\n", dirPath);
                dirPath[0] = '\0';
            }
            else
            {
                perror("Error deleting directory!");
            }
        }
        else if (strcmp(cmd, "touch") == 0)
        {
            if (open(dirPath, O_CREAT) != -1)
            {
                printf("File '%s' created successfuly.\n", dirPath);
                dirPath[0] = '\0';
            }
            else
            {
                perror("Error creating file!");
            }
        }
        else
        {
            printf("Invalid Shell command '%s'. Try other commands.\n", command);
        }
    }

    return 0;
}

void space()
{
    printf("*\n");
}