#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>
#include <ctype.h>

#define BUFFER_LENGTH 1024
#define MAX_HISTORY 200

// color codes
#define RESET   "\033[0m"
#define BLUE    "\033[0;34m"
#define GREEN   "\033[0;32m"
#define CYAN    "\033[0;36m"

// structure to hold command history
typedef struct {
    char command[BUFFER_LENGTH];
    pid_t pid;
    time_t execution_time;
    int is_background;
} CommandHistory;

CommandHistory history[MAX_HISTORY];
int current_history_index = 0;
int history_full = 0;



void initialize_signal_handling();

void handle_exit_signal(int signal);

void trim_string(char *str);

void print_command_history();

int process_piped_commands(char *input);

int execute_command(char *input, int is_background);

void start_shell();

void execute_single_command(char *command);

void print_colored(const char *text, const char *color);

void print_ascii_art();


// function to print text in a specified color
void print_colored(const char *text, const char *color) {
    printf("%s%s%s", color, text, RESET); // Reset color after printing
}

// function to print ASCII art in blue
void print_ascii_art() {
    print_colored("     _                 _               _          _ _    \n", GREEN);
    print_colored(" ___(_)_ __ ___  _ __ | | ___      ___| |__   ___| | |   \n", GREEN);
    print_colored("/ __| | '_ ` _ \\| '_ \\| |/ _ \\    / __| '_ \\ / _ \\ | |   \n", GREEN);
    print_colored("\\__ \\ | | | | | | |_) | |  __/    \\__ \\ | | |  __/ | |   \n", BLUE);
    print_colored("|___/_|_| |_| |_| .__/|_|\\___|    |___/_| |_|\\___|_|_|   \n", CYAN);
    print_colored("                |_|                                       \n", CYAN);
}


void start_shell(){
    char input_buffer[BUFFER_LENGTH];
    print_ascii_art();  // to print the ASCII art

    while (1) 
    {
        printf(GREEN "2023467_2023325@Hello_World" RESET ":" BLUE "~" RESET "$ ");
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            perror("Input error");
            continue;
        }

        input_buffer[strcspn(input_buffer, "\n")] = '\0'; // remove newline character

        if (strchr(input_buffer, '|')) {
            process_piped_commands(input_buffer);
        } else {
            if (execute_command(input_buffer, 0) == 0) {
                break; // exit
            }
        }
    }
}

// execute command and handle history
int execute_command(char *input, int is_background){
    if(strcmp(input, "history") == 0){
        print_command_history();
        return 1;
    }
    if(strcmp(input, "exit") == 0){
        print_command_history();
        return 0; // exit shell
    }

   
    char command_copy[BUFFER_LENGTH];
   
    strncpy(command_copy, input, sizeof(command_copy));

    char *sub_command = strtok(command_copy, "&");
   
    int foreground_mode = 0;

    while(sub_command != NULL){
        trim_string(sub_command);
        pid_t child_pid = fork();

        if(child_pid < 0){
            perror("Process creation failed");
            return 1;
        }
        else if(child_pid == 0){ // Child process
            
            execute_single_command(sub_command);
            exit(EXIT_SUCCESS); // Ensure child exits after executing the command
        }
        else{ // Parent process
            if(!foreground_mode){
                waitpid(child_pid, NULL, 0); // Wait for child process to finish
            }


            strncpy(history[current_history_index].command,sub_command,BUFFER_LENGTH);
            history[current_history_index].pid=child_pid;
            time(&history[current_history_index].execution_time);
            history[current_history_index].is_background=foreground_mode;

            // Update index and check if history is full
            current_history_index=(current_history_index + 1) % MAX_HISTORY;
            
            if(current_history_index==0) 
            {
                history_full=1; // Mark history as full when the index wraps around
            }
            foreground_mode=1; // Set subsequent commands to background
        }
        sub_command=strtok(NULL, "&");
    }
    return 1;
}

void execute_single_command(char *command) {
    char *arguments[BUFFER_LENGTH];
    int arg_index=0;

    char *argument=strtok(command," ");
    while(argument){
        arguments[arg_index++]=argument;
        argument=strtok(NULL," ");
    }
    arguments[arg_index]=NULL;

    execvp(arguments[0],arguments);
    perror("Command execution failed");
}

int process_piped_commands(char *input) {
    char *pipe_command=strtok(input, "|");
    
    int num_commands=0;
    
    char *commands[10]; // Support for up to 10 piped commands
    
    int pipe_fds[2], in_fd = 0;

    // Parse the pipe commands
    while(pipe_command !=NULL){
        
        commands[num_commands++]= strdup(pipe_command);
        
        pipe_command=strtok(NULL,"|");
    }

    for(int i=0;i<num_commands;i++){
        trim_string(commands[i]);

        // Create pipe
        if(i<num_commands-1){
            if(pipe(pipe_fds)==-1) {


                perror("Pipe creation error");
                exit(EXIT_FAILURE);

            }
        }

        // Fork to create a process for each command
        pid_t process_id=fork();
       
        if(process_id <0){
            perror("Fork error");

            exit(EXIT_FAILURE);
        }
        else if(process_id==0){ // Child process
           
            if(in_fd!=0){ // If there's input from a previous pipe
                dup2(in_fd, STDIN_FILENO); // Read from the previous command's output
                close(in_fd);
            }
            if
            (
                i<num_commands-1){ // If not the last command, pipe output
                dup2(pipe_fds[1], STDOUT_FILENO); // Write to the pipe for the next command
                close(pipe_fds[0]); // Close read end
            }

            execute_single_command(commands[i]);
            
            exit(EXIT_SUCCESS); // Ensure child exits after executing the command
        }
        else{ // Parent process
            wait(NULL); // Wait for the current command to finish

            if(in_fd!=0){
                close(in_fd); // Close the previous input
            }
            if (i<num_commands-1){
                close(pipe_fds[1]); // Close the write end of the pipe after forking
            }
            in_fd=pipe_fds[0]; // Store the read end for the next command
        }
    }
    return 1;
}

void print_command_history(){
    printf("\nCommand History:\n");
    for (int i=0;i<(history_full ? MAX_HISTORY :current_history_index); i++) {
        printf("[%d] PID: %d\n",i + 1,history[i].pid);
        printf("Command: %s\n",history[i].command);
        printf("Execution Time: %s",ctime(&history[i].execution_time));
        
        
        if (history[i].is_background) {
            printf("Status: Background Process\n");
        }

        else
        {
            printf("Status: Foreground Process\n");
        }
        printf("\n");
    }
}

void trim_string(char *str){
    char *start=str;
    char *end=str+strlen(str)-1;

    while(isspace((unsigned char)*start))start++;

    while(end>start && isspace((unsigned char)*end))end--;


    *(end + 1)='\0'; // Null-terminate the trimmed string
}

void handle_exit_signal(int signal){

   
    printf("\nShell interrupted by signal %d.\n", signal);
   
    print_command_history();
    exit(0);
}

void initialize_signal_handling(){
    struct sigaction action;
    
    action.sa_handler= handle_exit_signal;
    sigemptyset(&action.sa_mask);
    action.sa_flags=0;
    sigaction(SIGINT,&action,NULL);
}

int main() 
{
    initialize_signal_handling();
    start_shell();
    return 0;
}
