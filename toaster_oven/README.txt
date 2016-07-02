// Ric Rodriguez
// CMPE 13
// Lab 7 
// README.txt

In this lab I implemented a finite state machine called a toaster oven. This machine had a number of states aside from the ones visible to the user. Typically, all the use would see is the bake, toast, and broil modes. From an engineering perspective however, we need more modes to keep track of system states and changes. The most important aspect of this lab was the state machine concept. Additionally, working with the timers was very fruitful in terms of learning how to use them and convert to seconds.

My approach was to read the lab manual first and study the state machine. The rest of the coding was easy. Some pseudocode here and there but replaced with macros and functions from header files was all that was needed. The one thing that went wrong was my shifting of the timers. I did a right shift to half the time and modify it, specifically decrementing it, but I had forgotten to shift back left and set it back, resulting in really quick timers. The one thing that worked well was the string formatting.

The lab ended up working perfectly and I spend around 12 hours working on it. I liked the end result, looking back all that effort to program such a useful device made me really feel like an engineer. This was by far a worthwhile lab. The lab manual for this lab was very good at explaining what needed to be done, specifically the section where it told you how to implement the different steps.