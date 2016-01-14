Coding Project 3
====================
CS 6233
 
In the last assignment, you have extended the linux kernel’s features by modifying system calls and recompiling the kernel. However, linux supports kernel loadable modules (KLM) which help you avoid to recompile the kernel each time a new function is added. In this assignment, you will get to know Filesystem in Userspace (FUSE), a KLM that let non-privileged users to create a customized file system without editing the kernel.

Implement a file system that manages processes in your system (similar to what you see in /proc), called ProcFS:

####What you should do:

- The root directory is myproc that stores basic information of all processes’ in your system as files. Each file contain information of a process, filename is the same as process’s id. You are free to use any file format to store this information, i.e plain text, json, xml. You would get bonus point if you could create cool feature for this. For example, using multi-levels directories to better organize the processes, or keeping track of both active processes and retired processes.

- “ls myproc” command should return all processes’ names.

- Reading file would yield all information associated with the corresponding process.

####Suggestion to start:

- Start the assignment as soon as possible.
- Download lastest version of FUSE: http://fuse.sourceforge.net [2]
- Run the hello example.
- Understand the hello example: 
http://sourceforge.net/apps/mediawiki/fuse/index.php?title=Hello_World [3]
- Understand this chart: http://en.wikipedia.org/wiki/File:FUSE_structure.svg [4]
- Read FUSE APIs and start implementation.
- You are free to make any assumption to get the task done however be sure to make it clear in your report.
- You could use any language for this project.

 
####What to submit:

- A report describing how you implement the filesystem and how to build/run it. All your assumptions and design should be made clear in the report.
- Source code of the filesystem. All dependency libraries (if any) should be in the same directory of the source code.

+ All must be compressed into one file with this format: 
####CS6223_Fall2015_Assignment3_YourID_YourName
 [1] http://fuse.sourceforge.net
 [2] http://sourceforge.net/apps/mediawiki/fuse/index.php?title=Hello_World
 [3] http://en.wikipedia.org/wiki/File:FUSE_structure.svg
