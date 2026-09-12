# Level 00

## Description

When we try to log into "level00" :

```bash
su level00
```

password : `level00`

we find ourselves into an empty account. If we try to do things like :

```bash
ls
ls -la
ls -lRa
```

we get :

```
total 12
dr-xr-x---+ 1 level00 level00  100 Mar  5  2016 .
d--x--x--x  1 root    users    340 Aug 30  2015 ..
-r-xr-x---+ 1 level00 level00  220 Apr  3  2012 .bash_logout
-r-xr-x---+ 1 level00 level00 3518 Aug 30  2015 .bashrc
-r-xr-x---+ 1 level00 level00  675 Apr  3  2012 .profile
```

Nothing there... so the answer is not in our "home" directory. Let's try to navigate to other typical system directories (for example the "/usr") :

```
level00@SnowCrash:~$ ls -a
.  ..  .bash_logout  .bashrc  .profile
level00@SnowCrash:~$ cd /
level00@SnowCrash:/$ ls
bin  boot  cdrom  dev  etc  home  initrd.img  lib  media  mnt  opt  proc  rofs  root  run  sbin  selinux  srv  sys  tmp  usr  var  vmlinuz
level00@SnowCrash:~$ cd /usr
level00@SnowCrash:/usr$ ls
bin  games  include  lib  local  sbin  share  src
```

Let's try to search all the files starting from the root '/' directory that are owned by "flag00" (-user flag00) :

```bash
find / -user flag00
```

Here we have a full list of files that are "Permission denied" to us, let's try to hide the error (2) lines from our search, to clean the output and see in which ones we have the access rights to read :

```bash
find / -user flag00 2>/dev/null
```

"2>" redirects stderr (error messages) to the "/dev/null", which is the "black hole" device, anything sent here is discarded.

This gives us 2 files that are owned by "flag00" AND we have access :

```
/usr/sbin/john
/rofs/usr/sbin/john
```

If we try to look inside them with "cat" we can see that both have the same content :

```
cat /usr/sbin/john
cdiiddwpgswtgt
```

I copy it.

```
level00@SnowCrash:~$ su flag00
Password: cdiiddwpgswtgt

su: Authentification failure
```

The password is not right, so this means it must be encoded. From the look of it, it looks like a simple cipher (all lowercase letters, no numbers or symbols, length matches what you'd expect for a password/flag to be, no meaningful english words, but it looks like an anagram = no random choice of letters and their number of appearence), like an "atbash", or a "base64" or another substitution cipher like a Caesar with "ROT" encryption rotation (shift).

(We tried everything, only the Cesar ROT15 got us an english message)

What is ROT = "Rotate" ?

In the Caesar cipher, each letter of the alphabet is "rotated" forward by a fixed number of letters :

```
a = 0
b = 1
c = 2
...
z = 25
```

Here, "ROT15" means: shift 15 positions forward = take a letter, convert it to a number, add 15 to it, then wrap around the alphabet (mod 26).

Example :

| Original | a | b | c | d | e |
|---|---|---|---|---|---|
| ROT15 | p | q | r | s | t |

Explanation :

```
a → number 0 → add 15 → 15 → letter p
b → number 1 → add 15 → 16 → letter q
z → number 25 → add 15 → 40 → 40 mod 26 = 14 → letter o
```

This wrap-around modulo is what keeps it in the alphabet.

Encrypt vs Decrypt :

```
Encrypt (encode): move letters forward by 15 → ROT15
Decrypt (decode): move letters backward by 15 → ROT-15
```

Using ROT15 backward (decrypt) gives the real readable string. Let's use an online decoding tool like "https://cryptii.com" to decode it (we selected Ceasar Cipher, Shift : -15) :

```
nottoohardhere
```

Let's try it :

```
level00@SnowCrash:~$ su flag00
Password: nottoohardhere
Don't forget to launch getflag !
flag00@SnowCrash:~$ getflag
Check flag.Here is your token : x24ti5gi3x0ol2eh4esiuxias
```

Now, this is the password for "level01" !

```
flag00@SnowCrash:~$ su level01
Password: x24ti5gi3x0ol2eh4esiuxias

level01@SnowCrash:~$
```
