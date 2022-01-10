# bcf-software-task

Application using multithreading to analyze and present statistics about files and subdirectories of given directory. A pool of threads is being used for thread management. When necessary, additional threads are spawned to avoid deadlock.

## Usage

Application asks for number of threads of execution to spawn in the beginning, and a path to list.

## Behaviour

As an output, list of recursive subdirectories and files is printed. Order is unspecified and typically varies with every execution.