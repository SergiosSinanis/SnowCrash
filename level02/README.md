# Level 02

## Description

Ok, so now we are logged in the account "level02", let's see what's in there :

```
level02@SnowCrash:~$ ls
level02.pcap
level02@SnowCrash:~$ 
```

Interesting... this time we have a file named "level02.pcap", let's try to read it to see what's in this network traffic recording to see if we can find something interesting :

To read it, we must use a tool like "Wireshark" (that is not installed in the VM, so we need to use our own machine), or "tcpdump" like "tcpdump -r level02.pcap" (that is installed in the VM) :

I prefer using Wireshark, so we need to copy the ".pcap" file in our host (using "scp") :

So, in our host machine we go to the directory that we want to copy the file (like "Downloads") and we open a new terminal to launch a new "ssh/scp" connection with our VM :

```
scp -P 4243 level02@127.0.0.1:/home/user/level02/level02.pcap .
	   _____                      _____               _     
	  / ____|                    / ____|             | |    
	 | (___  _ __   _____      _| |     _ __ __ _ ___| |__  
	  \___ \| '_ \ / _ \ \ /\ / / |    | '__/ _` / __| '_ \ 
	  ____) | | | | (_) \ V  V /| |____| | | (_| \__ \ | | |
	 |_____/|_| |_|\___/ \_/\_/  \_____|_|  \__,_|___/_| |_|
                                                        
  Good luck & Have fun

          10.0.2.15 
level02@127.0.0.1's password: <flag01>
level02.pcap  100% 8302     6.2MB/s   00:00  
```

Bingo, now we have the ".pcap" file in our host, so now we can open it with Wireshark :

We see the packets panel, let's try a simple filtering :

```
tcp contains "login" 
```

Ok, nothing interesting yet, let's see the full TCP stream : Analyze → Follow → TCP Stream

In a Telnet connection (like the one displayed here in the TCP packets), every key the user presses is sent across the network in plaintext.

We get this :

```
...
Linux 2.6.38-8-generic-pae (::ffff:10.1.1.2) (pts/10)

..wwwbugs login: l.le.ev.ve.el.lX.X
..
Password: ft_wandr...NDRel.L0L
.
..
Login incorrect
wwwbugs login: 
...
```

So, the password is "Password: ft_wandr...NDRel.L0L", but the 3 dots here are non-printable Ascii values, let's see what their Ascii value is using the window's "Show data as: C Arrays" option :

Examining the packets payload we quickly find this section :

```c
char peer0_13[] = { /* Packet 45 */
0x66 };
char peer0_14[] = { /* Packet 47 */
0x74 };
char peer0_15[] = { /* Packet 49 */
0x5f };
char peer0_16[] = { /* Packet 51 */
0x77 };
char peer0_17[] = { /* Packet 53 */
0x61 };
char peer0_18[] = { /* Packet 55 */
0x6e };
char peer0_19[] = { /* Packet 57 */
0x64 };
char peer0_20[] = { /* Packet 59 */
0x72 };
char peer0_21[] = { /* Packet 61 */
0x7f };
char peer0_22[] = { /* Packet 63 */
0x7f };
char peer0_23[] = { /* Packet 65 */
0x7f };
char peer0_24[] = { /* Packet 67 */
0x4e };
char peer0_25[] = { /* Packet 69 */
0x44 };
char peer0_26[] = { /* Packet 71 */
0x52 };
char peer0_27[] = { /* Packet 73 */
0x65 };
char peer0_28[] = { /* Packet 75 */
0x6c };
char peer0_29[] = { /* Packet 77 */
0x7f };
char peer0_30[] = { /* Packet 79 */
0x4c };
char peer0_31[] = { /* Packet 81 */
0x30 };
char peer0_32[] = { /* Packet 83 */
0x4c };
char peer0_33[] = { /* Packet 85 */
0x0d };
```

So, we "translate" each Ascii for each packet :

```
0x66 = f
0x74 = t
0x5f = _
0x77 = w
0x61 = a
0x6e = n
0x64 = d
0x72 = r
0x7f = DEL (the non-printable chars)
0x7f = DEL
0x7f = DEL
0x4e = N
0x44 = D
0x52 = R
0x65 = e
0x6c = l
0x7f = DEL
0x4c = L
0x30 = 0
0x4c = L
0x0d = CR (carriage return)
```

It seems that the user first typed "ft_wandr", then pressed backspace three times, deleting the last three characters, he then continued by typing "NDRel". After that, he deleted the final "l" and replaced it with "L0L", before pressing Enter to submit the password.

So the final result is: "ft_waNDReL0L"

Let's try this for the "flag02" :

```
level02@SnowCrash:~$ su flag02
Password: ft_waNDReL0L
Don't forget to launch getflag !
flag02@SnowCrash:~$ getflag
Check flag.Here is your token : kooda2puivaav1idi4f57q8iq
flag02@SnowCrash:~$ su level03
Password: kooda2puivaav1idi4f57q8iq

level03@SnowCrash:~$ 
```
