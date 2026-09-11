## Project introduction :

### Virtual Machine Setup

Create a VM with VirtualBox, using the project's ".iso" :

- Name : snow-crash
- Type : Linux
- Version : Ubuntu (64-bit)
- RAM : 1024 MB (whatever)
- CPU : 1 (whatever)
- Network access mode : NAT
- Port forwarding :

| Name | Protocol | Host IP | Host Port | Guest IP | Guest Port |
|---|---|---|---|---|---|
| ssh | TCP | 127.0.0.1 | 4243 | empty | 4242 |

Now, launch your VM and connect to it from your host using "ssh" :

ssh level00@127.0.0.1 -p 4243 (this is like doing "su level00")

password : level00

Once connected to this level's account, we have to find the password that will log us in the "flagXX" account (XX = current level number). Yes, in this project there are 2 kind of user accounts in the system : the "levelXX" and the corresponding "flagXX" accounts. In every level, you'll have to find the password to log into the "flagXX" account first (that corresponds to the current levelXX). So, if we are in level00, then we have to find the password to login into the account "flag00" and once we have successfully connected to that account (by somehow finding its password), we have to run the "getflag" binary that is inside it to get the password for the next account (level01).

Let's try to connect to the first "flagXX" account it :

level00@SnowCrash:~$ su flag00

Password : (I don't know it)

Here we have to do something to get this "flag00" password...

Once you find it and you log in successfully to the "flagXX" account, launch the "getflag" binary/command inside it (the "getflag" binary is found in "/bin/" path of every "levelXX" account). This program will give you the password to connect to the next level account, with "su level01" :

flag00@SnowCrash:~$ getflag
Check flag.Here is your token : <password>

Now, this is the password for "level01" !

flag00@SnowCrash:~$ su level01
Password: <password>

level01@SnowCrash:~$

Level00 passed !

Now, we're connected on the "level01" account ! Now, we must successfully log into "su flag02" to find the password of the next level account "level03"... and so on.

You won't have to log into the account "flagXX" for every level to get the flag/password of the next level. Sometimes the flag is found directly in the "levelXX" account, but sometimes you won't even have to call the program "getflag" (or you may not even be able to connect to a "flagXX" account, you'll have to find a method to work around this, like a command injection on the "getflag" program depending on its rights, depending on the level).

 
## The SCP command :

The scp command (short for secure copy) is used to transfer files or directories between machines over SSH :

scp [options] source destination

- Uses SSH encryption (secure)
- No extra setup if SSH already works
- Simple and built-in on most systems
- scp is non-interactive (just copies, no syncing)
- For more advanced transfers (resume, sync), people often use "rsync"

1. Copy a file from your current directory (host) to a remote server :

scp file.txt level00@127.0.0.1:/home/user/level00

2. Copy a file from a remote server to the current directory of your host computer :

scp level00@127.0.0.1:/home/user/level00 .

3. Copy a directory (use -r (recursive)) from your current directory (host) to a remote server :

scp -r myfolder level00@127.0.0.1:/home/user/level00

4. Copy a file from a remote server to the current directory of your host computer, using a specific SSH port :

scp -P 4243 level00@127.0.0.1:/home/user/level00/file.txt .

5. Use an SSH key (no need here)

scp -i ~/.ssh/id_rsa file.txt level00@127.0.0.1:/home/user/level00
