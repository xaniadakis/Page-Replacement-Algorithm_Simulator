# Page Replacement Algorithm Simulator

In this project, I  examine the behaviour of applications in relation to
replacement algorithms. 

My application simulates the behaviour of the Memory Management System based on real reports' trace. There are two real trace 
files present in the repository. Each file contains a number of lines
where hexadecimal memory reports followed by them are presented
R or W indicators that indicate whether it is for writing or reading. The
simulator reads the file and runs the virtual memory mechanism
based on a hashed page table. The simulator knows which pages are 
loaded into main memory. When a report arises from the report trace
the simulator checks if the specific page is in main memory. 
If this is not the case and the memory is already saturated, the simluator
decides which page to remove (page replacement). 
If the page to be removed has been modified it gets "saved" to disk. 
When a new page loads, the page table is appropriately updated.
All pages and frames in the current simulation are 4096 bytes in size 
and the algorithms implemented are the LRU and Second Chance algorithms.
When the need arises to write to (or read from) the disc, the event 
is recorded in the appropriate metrics of the simulation. The
program alternately reads sets of reports (number q) from each traces' file
so that in the main memory there are pages from both processes.


The program accepts as arguments:

⚫ the replacement algorithm  [ 1 :LRU, 2 :Second Chance ]

⚫ the number of frames in the main memory,

⚫ the number of reports q and

⚫ an optional argument: the maximum number of reports to be examined from the traces' files. 


Upon completion of the simulation statistics such as: 

⚫ writes to disk, 

⚫ reads from disk, 

⚫ page faults, 

⚫ entries that got examined from each of the traces' file and 

⚫ memory frames 

are displayed.

The application gets compiled as follows:

    make clean all

The application can be run as follows:

    ./project <replacementAlgorithm[1/2]> <memoryFrames> <q_traces> [<maxTraces>]
    
{ example of execution parameters: ./a.out 1 10 10 10 }


