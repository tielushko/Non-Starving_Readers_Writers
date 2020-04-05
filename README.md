This Program is an Implementation of the Readers-Writers Lock created by Oleg Tielushko U48136789
The requirements of the lock are the following:

1. Any number of readers can be in the critical section simultaneously.
2. Writers must have exclusive access to critical section: no 2 writers at the same time, no writers and readers together. 

To Run the Program:
Compile with:
    
    make rwmain
or alternatively: 
    
    gcc -pthread main.c

Execute with: 
    
    ./rwmain 
or if compiled using alternative way:
    
    ./a.out

Clean up by invoking:
    
    make clean
If you are adding your own scenario, make sure to hit enter when you are finished typing. E.G. for 6 scenarios, there should be 7 lines. Otherwise, the last scenario will get ignored as there is no '\n' character.

Scenario 1: Shows that no more than one writer can be in the execution at the same time.

Scenario 2: Shows that writers cannot enter the execution when readers are present.

Scenario 3: Trying to starve the writer, by having many readers scheduled right after the first writer.

Scenario 4, 5, 6: Testing starvation and the presence of only 1 writer in the room.

Scenario 7: No writer to enter while readers are in execution.

Scenario 8, 9: Testing starvation.

