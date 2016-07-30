# What is this?

This is a project that was developed to solve the C++ Embedded exercise proposed by Music-Group.

# What was asked for?

Produce a C++ template FIFO class for a Linux-based system which can be used to pass work items (for example, a struct defining some work to be done) from a high priority, timing critical thread (the writer) to a low-priority background thread (the reader) where the work is done. You should make use of the STL and compile with C++11.

If you need more details the full description of the exercise is saved in 'docs' folder.

# What do you need?

you need to install:
 - git
 - cmake
 - make
 - Eclipse (Optional, the eclipse environment is available, but it is not mandatory).
 - valgrind (Optional, just to check memory leak)

# How do I build?

The project has two targets (`debug` and `debug-test`) and you can build the project in two different ways:
 - Entering in the folder: `/music-group/scripts` and executing `./build.sh <target>`
 - Open the Eclipse program, import the project and run the respective target.

# Where are the results ?

All the output files, including the executable file, are inserted in the respective builder folder of the project:
 - debug:
	`/music-group/build/debug/`
 - debug-test:
	`/music-group/build/debug-test/`

# How to run the programs?

You should enter in the respective output folder and execute the binary file.

	`./music-group-debug`
	`./music-group-debug-test`

# What do they do?

The *debug-test* program uses the google-test environment to test the FIFO class. All the unit test code are located into the `/music-group/test` folder. You can test if the class has problems with memory leak or sintaxe running the following command:
 - `valgrind ./music-group-debug-test`

The *debug* program uses the FIFO class to feed some animals, run the program and have a lot of fun.







