# PA0
## Task 2.1
[Streamlink Twitch Gui](https://github.com/streamlink/streamlink-twitch-gui)
* Consistent format
* Visible headings that separate into logical sections
* Code blocks when necessary

[VS Code Debug Visualizer](https://github.com/hediet/vscode-debug-visualizer/blob/master/extension/README.md)
* Emojis
* Language names as code blocks
* Gifs

[Vortex Mod Manager](https://github.com/Nexus-Mods/Vortex)
* Nested lists
* File locations in italics
* Avoid using code in italics
## Task 3.6
It appears that running `uname -a` prints out the system information. The `-a` flag prints out all information, which makes me assume just running the command without flags would limit the information printed out. 
## Task 4.1
* `cd ~` brings us to the home directory of root
* `pwd` prints out the current directory of the command line
* `mkdir -p /tmp/this/is/a/sub/directory` creates the full tree of directories, including parent directories if they don't already exist. The directories are created in the temp folder on the VM.
* `ln -s /tmp/this/is/a/sub/directory ~/mydir` creates a new directory called `/mydir/` in the root's home directory and symbolically links it to the directory we created previously. This means that the new directory is essentially the old one but easier to find. 
* `ls -al` prints out all contents of the current working directory, including size, permissions, owner, group, and last modified time.
* `env | grep PATH` finds the line in the output of `env` that contains the value of the `PATH` variable and prints it to the console. 
* `curl -O https://raw.githubusercontent.com/traviswpeters/cs460-code/master/week02/info` downloads `info` from the designated address to the current working directory using the curl command.  
* `cat info` prints out the contents of `info` to the command line. 
* `sudo lshw -html > hardwareinfo.html` puts the hardware info found with `lshw` into an html format with `-html`, and stores it in `hardwareinfo.html`. It is run with the `sudo` command to give us elevated privileges to let the command work. 
## Task 4.2
I personally use `man` to quickly find information about an unfamiliar command quickly without tabbing out of the command line. 
## Task 4.3
My favorite command line tool is `less` because it is easy to find what you're looking for in files efficiently without accidentally deleting a part of it.  
## Task 5.3
It is important to add the `.PHONY` target when we don't explicitly create files to improve performance and avoid conflicts in case there are any conflicting commands/programs/files we could run. 
## Task 5.4
I added a `zip` target to add a readme and compress the files into `dress_dyi.zip`. 
```C
zip: 
	echo "This is how I get dressed in the morning!" >> ~/README
	zip ~/dress_dyi.zip ~/README ~/dressed.txt
```
## Task 7.1
```
vagrant@vbox:~/projects/PA0/stategame$ gdb -q stategame
Reading symbols from stategame...done.
(gdb) set args test
(gdb) start
Temporary breakpoint 1 at 0x983: file stategame.c, line 35.
Starting program: /home/vagrant/projects/PA0/stategame/stategame test

Temporary breakpoint 1, main () at stategame.c:35
warning: Source file is more recent than executable.
35      {
(gdb) next
36          struct listnode head = {NULL, NULL}; // dummy node at head of empty list
(gdb) p &head
$1 = (struct listnode *) 0x7fffffffe3e0
```
The address of the head variable is ```0x7fffffffe3e0```.
## Task 7.2
```
(gdb) info locals
head = {name = 0x7ffff7de59f0 <_dl_fini> "UH\211\345AWAVAUATSH\203\354(L\213%X\177!", next = 0x0}
lineLength = 0
line = "H\203"
n = 15775487
```
Following the previous code block, ```info locals``` can be ran to quickly show all variables at the current function which is main
## Task 7.3
```
vagrant@vbox:~/projects/PA0/stategame$ make
gcc -Wall -pedantic -std=c11 -ggdb    -c -o stategame.o stategame.c
gcc -Wall -pedantic -std=c11 -ggdb  stategame.o readline.o -lm -o stategame
vagrant@vbox:~/projects/PA0/stategame$ make testvalgrind
valgrind --leak-check=full --show-leak-kinds=all ./stategame < TEST.txt > guess
==2890== Memcheck, a memory error detector
==2890== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==2890== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==2890== Command: ./stategame
==2890==
==2890== Invalid free() / delete / delete[] / realloc()
==2890==    at 0x4C30D3B: free (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==2890==    by 0x108D14: freeAllNodes (stategame.c:169)
==2890==    by 0x108A86: main (stategame.c:59)
==2890==  Address 0x1fff000310 is on thread 1's stack
==2890==  in frame #2, created by main (stategame.c:35)
==2890==
==2890==
==2890== HEAP SUMMARY:
==2890==     in use at exit: 0 bytes in 0 blocks
==2890==   total heap usage: 62 allocs, 63 frees, 17,177 bytes allocated
==2890==
==2890== All heap blocks were freed -- no leaks are possible
==2890==
==2890== For counts of detected and suppressed errors, rerun with: -v
==2890== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```
All memory was freed successfully. 