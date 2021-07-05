/* Jake Whitebread
   Shell Interpreter
   CST-315*/
   //Code reference to https://www.geeksforgeeks.org/making-linux-shell-c/
// C Program to design a shell in Linux
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>

#define MAXCOM 1000 // max number of letters to be supported
#define MAXLIST 100 // max number of commands to be supported

// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J")

// Greeting shell during startup
void init_shell()
{
    clear();
    printf("\n\n\n\n******************"
        "************************");
    printf("\n\n\n\t****MY SHELL****");
    printf("\n\n\t-USE AT YOUR OWN RISK-");
    printf("\n\n\n\n*******************"
        "***********************");
    char* username = getenv("USER");
    printf("\n\n\nUSER is: @%s", username);
    printf("\n");
    sleep(1);
    clear();
}

// Function to take input
int takeInput(char* str)
{
    char* buf;

    buf = readline("\n>>> ");
    if (strlen(buf) != 0) {
        add_history(buf);
        strcpy(str, buf);
        return 0;
    } else {
        return 1;
    }
}

// Function to print Current Directory.
void printDir()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\nDir: %s", cwd);
}

// Function where the system command is executed
void execArgs(char** parsed)
{
    // Forking a child
    pid_t pid = fork();

    if (pid == -1) {
        printf("\nFailed forking child..");
        return;
    } else if (pid == 0) {
        if (execvp(parsed[0], parsed) < 0) {
            printf("\nCould not execute command..");
        }
        exit(0);
    } else {
        // waiting for child to terminate
        wait(NULL);
        return;
    }
}


//
void execMult(char** parsed, char** parsedMult) {
       // Forking a child

    for (int i = 0; i < 2; i++) {
        if (i == 0){
        pid_t pid = fork();

        if (pid == -1) {
            printf("\nFailed forking child..");
            return;
        } else if (pid == 0) {
            if (execvp(parsed[0], parsed) < 0) {
                printf("\nCould not execute command..");
            }
            exit(0);
        } else {
            // waiting for child to terminate
            wait(NULL);
            }
        }
        if (i == 1) {
            pid_t pid = fork();

            if (pid == -1) {
                printf("\nFailed forking child..");
                return;
            } else if (pid == 0) {
                if (execvp(parsedMult[0], parsedMult) < 0) {
                    printf("\nCould not execute command..");
                }
                exit(0);
            } else {
                // waiting for child to terminate
                wait(NULL);
            }
        }
    }
//    pid_t pid1, pid2;
//    pid1 = fork();
//
//    if (pid1 == -1) {
//        printf("\nFailed forking child..");
//        return;
//    }
//    if (pid1 == 0) {
//        if (execvp(parsed[0], parsed) < 0) {
//            printf("\nCould not execute command1..");
//            exit(0);
//        }
//    }
//    else {
//        pid2 = fork();
//
//        if (pid2 == -1) {
//            printf("\nFailed forking child2..");
//            return;
//        }
//        if (pid2 == 0) {
//            if (execvp(parsedMult[0], parsedMult) < 0) {
//                printf("\nCould not execute command2..");
//                exit(0);
//            }
//        }
//        else {
//            // waiting for child to terminate
//            wait(NULL);
//            wait(NULL);
//        }
//    }
}

// Function where the piped system commands is executed
void execArgsPiped(char** parsed, char** parsedpipe)
{
    // 0 is read end, 1 is write end
    int pipefd[2];
    pid_t p1, p2;

    if (pipe(pipefd) < 0) {
        printf("\nPipe could not be initialized");
        return;
    }
    p1 = fork();
    if (p1 < 0) {
        printf("\nCould not fork");
        return;
    }

    if (p1 == 0) {
        // Child 1 executing..
        // It only needs to write at the write end
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        if (execvp(parsed[0], parsed) < 0) {
            printf("\nCould not execute command 1..");
            exit(0);
        }
    } else {
        // Parent executing
        p2 = fork();

        if (p2 < 0) {
            printf("\nCould not fork");
            return;
        }

        // Child 2 executing..
        // It only needs to read at the read end
        if (p2 == 0) {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            if (execvp(parsedpipe[0], parsedpipe) < 0) {
                printf("\nCould not execute command 2..");
                exit(0);
            }
        } else {
            // parent executing, waiting for two children
            wait(NULL);
            wait(NULL);
        }
    }
}

void factorial() {
    int n;
    int ans = 1;
    printf("What number do you want to use? ");
    scanf("%d", &n);

    for (int i = 1; i<=n; ++i) {
        ans *= i;
    }
    printf("The Factorial of %d is %d",n,ans);
    return;
}

void roundRobin() {
    int i, max, x, timeQ;
    int counter = 0;
    int total = 0;
    int waitTime = 0, turnaround = 0, arrivalT[10], burstT[10], temp[10];
    float avgWait, avgTurnaround;
    printf("nHow many processes do you want to use?:    ");
    scanf("%d", &max);
    x = max;
    //lets user input details of processes
    for(i = 0; i < max; i++) {
        printf("\nEnter Details of Process[%d]\n", i + 1);
        printf("Arrival Time:   ");
        scanf("%d", &arrivalT[i]);
        printf("Burst Time: ");
        scanf("%d", &burstT[i]);
        temp[i] = burstT[i];
    }
    //lets user decide time quantum
    printf("\nEnter Time Quantum:    ");
    scanf("%d", &timeQ);
    printf("\nProcess ID     Burst Time   Turnaround Time     Waiting Time\n");
    //schedules processes using Round Robin
    for(total = 0, i = 0; x != 0;) {
        if(temp[i] <= timeQ && temp[i] > 0) {
            total = total + temp[i];
            temp[i] = 0;
            counter = 1;
        }
        else if(temp[i] > 0) {
            temp[i] = temp[i] - timeQ;
            total = total + timeQ;
        }
        if(temp[i] == 0 && counter == 1) {
            x--;
            printf("\nProcess[%d]        %d         %d                      %d", i + 1, burstT[i], total - arrivalT[i], total - arrivalT[i] - burstT[i]);
            waitTime = waitTime + total - arrivalT[i] - burstT[i];
            turnaround = turnaround + total - arrivalT[i];
            counter = 0;
        }
        if(i == max - 1){
            i = 0;
        }
        else if (arrivalT[i + 1] <= total) {
            i++;
        }
        else  {
            i = 0;
        }
    }
    avgWait = waitTime * 1.0 / max;
    avgTurnaround = turnaround * 1.0 / max;
    printf("\n\nAverage Waiting time: %f", avgWait);
    printf("\nAverage Tunaround Time:    %f\n", avgTurnaround);
    return;
}

void memoryAllo() {
    int *arr;
    int max,i;
    int sum=0;

    printf("Enter total number of elements: ");
    scanf("%d",&max);

    /*allocate memory for limit elements dynamically*/
    arr=(int*)malloc(max*sizeof(int));

    if(arr==NULL)
    {
        printf("Insufficient Memory, Exiting... \n");
        return;
    }

    printf("Enter %d elements:\n",max);
    for(i=0; i<max; i++)
    {
        printf("Enter element %3d: ",i+1);
        scanf("%d",(arr+i));
        /*calculate sum*/
        sum=sum + *(arr+i);
    }

    printf("Array elements are:");
    for(i=0; i<max; i++)
        printf("%3d ",*(arr+i));


    printf("\nSum of all the elements is: %d\n",sum);

    return;
}

// Help command builtin
void openHelp()
{
    puts("\n***WELCOME TO MY SHELL HELP***"
        "\nCopyright @ Suprotik Dey"
        "\n-Use the shell at your own risk..."
        "\nList of Commands supported:"
        "\n>cd"
        "\n>ls"
        "\n>exit"
        "\n>all other general commands available in UNIX shell"
        "\n>pipe handling"
        "\n>improper space handling");

    return;
}

// Function to execute builtin commands
int ownCmdHandler(char** parsed)
{
    int NoOfOwnCmds = 7, i, switchOwnArg = 0;
    char* ListOfOwnCmds[NoOfOwnCmds];
    char* username;

    ListOfOwnCmds[0] = "exit";
    ListOfOwnCmds[1] = "cd";
    ListOfOwnCmds[2] = "help";
    ListOfOwnCmds[3] = "hello";
    ListOfOwnCmds[4] = "fct";
    ListOfOwnCmds[5] = "mem";
    ListOfOwnCmds[6] = "rr";

    for (i = 0; i < NoOfOwnCmds; i++) {
        if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) {
            switchOwnArg = i + 1;
            break;
        }
    }

    switch (switchOwnArg) {
    case 1:
        printf("\nGoodbye\n");
        exit(0);
    case 2:
        chdir(parsed[1]);
        return 1;
    case 3:
        openHelp();
        return 1;
    case 4:
        username = getenv("USER");
        printf("\nHello %s.\nMind that this is "
            "not a place to play around."
            "\nUse help to know more..\n",
            username);
        return 1;
    case 5:
        factorial();
        return 1;
    case 6:
        memoryAllo();
        return 1;
    case 7:
        roundRobin();
        return 1;
    default:
        break;
    }

    return 0;
}

// function for finding pipe
int parsePipe(char* str, char** strpiped)
{
    int i;
    for (i = 0; i < 2; i++) {
        strpiped[i] = strsep(&str, "|");
        if (strpiped[i] == NULL)
            break;
    }

    if (strpiped[1] == NULL)
        return 0; // returns zero if no pipe is found.
    else {
        return 1;
    }
}

//new portion to parse multiple commands seperated by ;
int parseMultiple (char* str, char** strMult) {
    int i;
    for (i = 0; i < 2; i++){        //runs through input
        strMult[i] = strsep(&str, ";"); //seperates strign before and afer ";"
        if (strMult[i]  == NULL){
            break;      //breaks if no semicolon
        }
    }

    if (strMult[1] == NULL) {
        return 0;       //returns 0 if no semicolon
    }
    else {
        return 1;       //returns 1 if parsed correctly
    }
}

// function for parsing command words
void parseSpace(char* str, char** parsed)
{
    int i;

    for (i = 0; i < MAXLIST; i++) {     //runs through input
        parsed[i] = strsep(&str, " ");  //seperates command words by seperating string after space

        if (parsed[i] == NULL)
            break;
        if (strlen(parsed[i]) == 0)
            i--;
    }
}
//decides appropriate parsing function for type of input and sends to execution
int processString(char* str, char** parsed, char** parsedpipe)
{

    char* strpiped[2];  //array for input with pipe
    //new
    char* strMult[2];   //array for input with ";"

    int piped = 0;      //acts as boolean, either 1 or 0
    int mult = 0;

    piped = parsePipe(str, strpiped);   //if input has pipe, parses it
    //new
    mult = parseMultiple(str, strMult); //if input has ";", parses it

    if (piped) {    //seperates commands from piped input
        parseSpace(strpiped[0], parsed);    //comand 1
        parseSpace(strpiped[1], parsedpipe);    //command 2

    }
    //new
    else if (mult) {    //seperates input from multi commands
        parseSpace(strMult[0], parsed);     //command 1
        parseSpace(strMult[1], parsedpipe); //command 2
    }

    else {

        parseSpace(str, parsed);    //parses single command
    }
    //returns number based on type of input to run proper execution function
    if (ownCmdHandler(parsed))
        return 0;       //0 for local command
    else if (piped)
        return 1 + piped;   //2 for piped command
    else if (mult)
        return 2 + mult;    //3 for multi command
    else return 1;          //1 for single command
}

int main()
{
    char inputString[MAXCOM], *parsedArgs[MAXLIST]; //allocates arrays to store input
    char* parsedArgsPiped[MAXLIST];     //allocates array for multi/piped inputs
    int execFlag = 0;   //variable to store output of processString()
    init_shell();   //runs initializing function

    while (1) {
        // print shell line
        printDir();
        // take input
        if (takeInput(inputString))
            continue;
        // process
        execFlag = processString(inputString,
        parsedArgs, parsedArgsPiped);
        // execflag returns zero if there is no command
        // or it is a builtin command,
        // 1 if it is a simple command
        // 2 if it is including a pipe.
        // 3 if it has multi commands

        // execute
        if (execFlag == 1)
            execArgs(parsedArgs);

        if (execFlag == 2)
            execArgsPiped(parsedArgs, parsedArgsPiped);

        if (execFlag == 3)
            execMult(parsedArgs, parsedArgsPiped);
    }
    return 0;
    }
