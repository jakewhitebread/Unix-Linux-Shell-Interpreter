# Unix-Linux-Shell-Interpreter
This is the shell interpreter I made in my operating systems class. The goal of it is to take in linux commands and run them just as the command window would. It has multiple different features, such as pipelining, executing multiple commands, built-in commands that execute directly, as well as different scheduling algorithms. We built our interpreters from scratch throughout the class using C.
# Installation:
1. To run this you will need a machine running linux, preferably Ubuntu.
2. You will also need at least 8gb of ram.
3. To run the program, download the .zip file. Find the .cbp, .layout, and main.c files and place them in
   whatever directory you want.
4. Next open up your terminal and navigate to that directory.
5. Compile the main.c file using command >>gcc main.c -lreadline
6. After that runs use commmand >>./a.out to run the shell
7. Now run commands as you would in the command window or try out built-in commands.
# Built-in commands
1. exit
2. cd
3. help
4. hello
5. "fct" (factorial)
6. "mem" (simple memory allocation)
7. "rr" (simple round robin scheduler)
