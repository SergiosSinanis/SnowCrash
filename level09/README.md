# Level 09

## Description

Ok, let's see what we have here :

```
level09@SnowCrash:~$ ls
level09  token
level09@SnowCrash:~$ ls -la
total 24
dr-x------ 1 level09 level09  140 Mar  5  2016 .
d--x--x--x 1 root    users    340 Aug 30  2015 ..
-r-x------ 1 level09 level09  220 Apr  3  2012 .bash_logout
-r-x------ 1 level09 level09 3518 Aug 30  2015 .bashrc
-rwsr-sr-x 1 flag09  level09 7640 Mar  5  2016 level09
-r-x------ 1 level09 level09  675 Apr  3  2012 .profile
----r--r-- 1 flag09  level09   26 Mar  5  2016 token
```

Ok, this seems fairly similar to "level08"... let's try to run it :

```
level09@SnowCrash:~$ ./level09 token
tpmhr
```

The binary gives some output but it's not the flag :

```
level09@SnowCrash:~$ su flag09
Password: tpmhr
su: Authentication failure
level09@SnowCrash:~$ su level10
Password: tpmhr
su: Authentication failure
```

Let's try to do the same thing we did to level 8 with a symlink :

```
level09@SnowCrash:~$ ln -s /home/user/level09/token /tmp/test
level09@SnowCrash:~$ ./level09 /tmp/test
/uos3ykz|
```

Ok, is this "/uos3ykz|" the flag ? Let's try :

```
level09@SnowCrash:~$ su flag09
Password: /uos3ykz|
su: Authentication failure
level09@SnowCrash:~$ su level10
Password: /uos3ykz|
su: Authentication failure
level09@SnowCrash:~$ cat /tmp/test
f4kmm6p|=�p�n��DB�Du{��
```

Nothing... let's try to pass to the binary "level09" a new file :

```
level09@SnowCrash:~$ echo hello > /tmp/ok
level09@SnowCrash:~$ cat /tmp/ok
hello

level09@SnowCrash:~$ ./level09 /tmp/ok
/uos3tq
```

Ok, so the binary changed our "hello" string into "/uos3tq". So, maybe its an encryption of some kind (like a simple Ceasar/offset cipher calculation) ?

Let's try to find out what the pattern is (since we know that "hello" gives "/uos3tq") :

```
Position:   0    1    2    3    4    5    6 
Input:      h    e    l    l    o    "\n" 
Output:     /    u    o    s    3    t    q
```

The output is longer than the input (7 chars vs 5 + new line). Let's look at the ASCII values :

```
Position:  0     1     2     3     4     5     6
Input:     h=104 e=101 l=108 l=108 o=111
Output:    /=47  u=117 o=111 s=115 3=51  t=116 q=113
```

Let's try to find the Delta (output - input) for each position :

```
pos 0 = /=47 - h=104 = -57
pos 1 = u=117 - e=101 = +16 

pos 0:  output - input =  47 - 104 = -57
pos 1:  output - input = 117 - 101 = +16
pos 2:  output - input = 111 - 108 = +3
pos 3:  output - input = 115 - 108 = +7
pos 4:  output - input =  51 - 111 = -60
```

Ok, so this is not a fixed offset ASCII value mapping, maybe every ASCII char has a different offset ?

That can't be because in our example "hello", we have 2 "L"s but the offset is still different :

```
pos 2:  l=108  →  o=111  delta = +2
pos 3:  l=108  →  s=115  delta = +7
```

So, there are 2 possibilities :

1. the offset is just random for every char on our string

2. the offset is position-dependent (and not just character-dependent)

To confirm if it's 2, let's try this again without the new line :

```
level09@SnowCrash:~$ echo -n hello > /tmp/test
level09@SnowCrash:~$ ./level09 /tmp/test
/uos3j

level09@SnowCrash:~$ echo -n hemlo > /tmp/test2
level09@SnowCrash:~$ ./level09 /tmp/test2
/uos3i
```

Ok, so now the input and the output have the same length, perfect. Let's see :

```
Position:  0    1    2    3    4    5
	       h    e    l/m  l    o
hello:     /    u    o    s    3    j
hemlo:     /    u    o    s    3    i
```

Positions from 0 to 4 are identical, only position 5 differs, even if "hello" and "hemlo' actually differ at position 2 (l vs m)...

Let's try a single character input AND let's try to give the binary the input directly as an argument (not through a file = the file may complex things in this case) :

```
level09@SnowCrash:~$ ./level09 "a"
a
```

Interesting... by not passing through a file I have 1 output char for 1 input char... nice, let's continue :

```
level09@SnowCrash:~$ ./level09 "abcdefg"
acegikm
level09@SnowCrash:~$ ./level09 "abcdefghijklmnopqrstuvwxyz"
acegikmoqsuwy{}����������
```

The 'a' always remains 'a'... so this is not random, it's position-determined !

Now we can see the pattern clearly :

```
Position:  0    1    2    3    4    5    6
Input:     a    b    c    d    e    f    g
Output:    a    c    e    g    i    k    m
```

And we have these ASCII deltas :

```
pos 0:  a=97  → a=97   delta = +0
pos 1:  b=98  → c=99   delta = +1
pos 2:  c=99  → e=101  delta = +2
pos 3:  d=100 → g=103  delta = +3
pos 4:  e=101 → i=105  delta = +4
pos 5:  f=102 → k=107  delta = +5
pos 6:  g=103 → m=109  delta = +6
```

So, the formula is :

```
output_letter = input[ascii_value] + position_number
```

And to decode it, we nned to reverse it :

```
input[i] = ascii(output[i]) - i
```

Ok, now let's try to decode our token manually :

Here, we must decode and try 2 things :

1. the token that the binary "level09" returns when we pass the file "token" to it
2. the "token" file contents

1 :

```
level09@SnowCrash:~$ ./level09 token
tpmhr
```

```
pos 0:  t = 116 - 0 = 116 → 't'
pos 1:  p = 112 - 1 = 111 → 'o'
pos 2:  m = 109 - 2 = 107 → 'k'
pos 3:  h = 104 - 3 = 101 → 'e'
pos 4:  r = 114 - 4 = 110 → 'n'
```

So the decoded token is... "token" ! But it's not the flag :

```
level09@SnowCrash:~$ su flag09
Password: token
su: Authentication failure
level09@SnowCrash:~$ su level10
Password: token
su: Authentication failure
```

So, what is it ? Let's try the method 2 :

```
level09@SnowCrash:~$ cat token
f4kmm6p|=�p�n��DB�Du{��
```

Ok, here we can't do it manually because there are some not-printable chars that we don't know their ASCII value, so let's use our script to do it :

This is a simple "C" program that takes an encoded string as its program argument and decodes it and prints the result :

```c
# include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
        return 1;

    int i;
    char *buf = NULL;
    
    i = 0;
    buf = argv[1];
    
    while (buf[i] != '\0')
    {
        printf("%c", buf[i] - i); // %c = char, this prints the char "input_letter_ascii - i", and i increments 0 ... end of string
        i++;
    }

    printf("\n");
    return 0;
}
```

- Let's create our script with our code inside the "/tmp" directory :

```bash
cat > /tmp/script.c << 'EOF'
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
        return 1;
    int i;
    char *buf = NULL;

    i = 0;
    buf = argv[1];

    while (buf[i] != '\0')
    {
        printf("%c", buf[i] - i);
        i++;
    }
    printf("\n");
    return 0;
}
EOF
```

- Let's compile it :

```bash
gcc /tmp/script.c -o /tmp/script
```

But there is a problem :

```
level09@SnowCrash:~$ gcc /tmp/script.c -o /tmp/script
Cannot create temporary file in ./: Permission denied
Aborted (core dumped)
```

This is because we are currently at the "/home/usr/level09" directory and since the "gcc" also creates hidden temporary files in the current working directory during compilation (and because our current directory is read-only, those temp files couldn't be created, so it crashes), but since the directory "/tmp" is writable by everyone, let's navigate there to compile our file :

```
level09@SnowCrash:~$ cd /tmp
level09@SnowCrash:/tmp$ gcc /tmp/script.c -o /tmp/script
```

- Let's run our script with the encrypted contents of the "token" file :

```
level09@SnowCrash:/tmp$ ./script "$(cat ~/token)"
f3iji1ju5yuevaus41q1afiuq
```

We got a flag, let's use it :

```
level09@SnowCrash:/tmp$ su level10
Password: f3iji1ju5yuevaus41q1afiuq
su: Authentication failure
level09@SnowCrash:/tmp$ su flag09
Password: f3iji1ju5yuevaus41q1afiuq
Don't forget to launch getflag !
flag09@SnowCrash:~$ getflag
Check flag.Here is your token : s5cAJpM8ev6XHw998pRWG728z
flag09@SnowCrash:~$ su level10
Password: s5cAJpM8ev6XHw998pRWG728z

level10@SnowCrash:~$ 
```
