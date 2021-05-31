To build : just run make. 

Required tools : 
- A cross compiler to a generic i686 target. Change the CC and AS vars in the makefile for the names.
- xorriso to create the iso
- qemu to run the OS


The repository in organised in the following manner : 
- arch-specific files are in arch/i686
- kernel-specific files are in kernel/
- files related to "external" programs (only the shell) are in modules/
- files that should be included in a kind of standard lib are in lib/
