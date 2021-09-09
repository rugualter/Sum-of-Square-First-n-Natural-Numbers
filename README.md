# Sum-of-Square-First-n-Natural-Numbers
C - CLI program to calculate the Sum of the square of the first n natural numbers using multiple process

The program follows a basic structure where there is a main function and two others that are called to main, the process and final function.

The function processes, makes the sum s(n) of the square of the first n natural numbers, with the chain of processes 1 to np dedicated to calculating the terms of the sum in equal parts. It starts by defining all the variables, testing a function right from the start if to check if it is the last process then you will have to complete the remaining numbers, if not, it will do the normal process, and do the partial divisions in the processes that have been entered. Below is the piece of code that creates the data.aux file where the results of the partial sums are inserted.

The final function, given by the sum of the partial sums, fetches the information from data.aux and makes the sum.

The main function starts by imposing conditions, error messages if the numbers entered in the console are not as required. Afterwards, the process is identified and printed to the screen, then the number of processes chosen by the user is created through a “for” that uses the “wait” command to make sure that the parent process waits for the child to finish, when the child process created is called the process function.

After the for ends, the final function is called to give the result of the summation.
