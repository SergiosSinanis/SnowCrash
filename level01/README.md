# Level 01

## Description

Again, we find ourselves into an empty account. Let's change the tactic, let's check how many users we have :

```bash
cat /etc/passwd
```

Now we can see all the users of the system (including "levelXX" and "flagXX"), let's try to print the passwords/flags :

```bash
cat /etc/shadow

for i in {00..14}; do cat /home/flag/flag$i; done
```

No permissions... let's just try this again :

```bash
cat /etc/passwd
```

Let's examine the output :

```
...
flag00:x:3000:3000::/home/flag/flag00:/bin/bash
flag01:42hDRfypTqqnw:3001:3001::/home/flag/flag01:/bin/bash
flag02:x:3002:3002::/home/flag/flag02:/bin/bash
...
```

So, we look at all the system users, everything has the normal format :

```
username:password:UID:GID:comment:home_directory:shell
```

But, there is a suspicious line :

```
flag01:42hDRfypTqqnw:3001:3001::/home/flag/flag01:/bin/bash
```

The field ":password" is not normal (normally its hashed like ':x', but here it's "42hDRfypTqqnw")

Let's try to use this as a password for "flag01" :

```
level01@SnowCrash:/$ su flag01
Password: 
su: Authentication failure
```

So, it must be encrypted... We are sure this must be the "flag01" password because it's the only password field in the "/etc/passwd" that is not hidden. So, we need to find a way to decrypt it.

We tried all the simple text decryption techniques possible (shifting letters in the alphabet (cesar, rot, atbash = mirror rotation = 'a' becomes 'z' etc, vigenère, rotating characters, simple substitutions), to no avail :

```
Shift 1: iESgzqUrrox
Shift 2: jFTharVsspy
Shift 3: kGUibsWttqz
Shift 4: lHVjctXuura
Shift 5: mIWkduYvvsb
Shift 6: nJXlevZwwtc
Shift 7: oKYmfwAxxud
Shift 8: pLZngxByyve
Shift 9: qMAohyCzzwf
Shift 10: rNBpizDaaxg
Shift 11: sOCqjaEbbyh
Shift 12: tPDrkbFcczi
Shift 13: uQEslcGddaj
Shift 14: vRFtmdHeebk
Shift 15: wSGuneIffcl
Shift 16: xTHvofJggdm
Shift 17: yUIwpgKhhen
Shift 18: zVJxqhLiifo
Shift 19: aWKyriMjjgp
Shift 20: bXLzsjNkkhq
Shift 21: cYMatkOllir
Shift 22: dZNbulPmmjs
Shift 23: eAOcvmQnnkt
Shift 24: fBPdwnRoolu
Shift 25: gCQexoSppmv
```

Nothing here worked for the "flag01" password, nor it's a readable text (in case it's a hint). So, maybe it's a hash of some sort (maybe an MD5, a SHA1, a SHA256, a DES, etc), we need tools to de-hash :

I think this must be a DES, because it has exactly 13 chars in length, but it could be some other common hash (usually MD5 has 32 chars, SHA1 has 40 chars and SHA256 has 64 chars), let's try them all :

(We used the site https://md5decrypt.net/ to do this) : the MD5, SHA1, SHA256 de-hashing didn't work at all (Hashes are not valid), I suspect why :

- the string doesn't have the right char length
- these hashes use restricted charsets
- hashes usually look random

So, this leaves us with the DES, which is a key-encryption :

```
DES = Data Encryption Standard = A symmetric-key block cipher (same key for encryption & decryption) = Encrypts data in 64-bit blocks
Key = DES key length: 56 bits (7 bytes)
```

but the problem here is that we must have the key to decrypt DES-encrypted data !
Without the key (even if we know it's DES), the ciphertext is practically impossible to recover...

Let's now try using something more powerful like "John the Ripper" (often just "John"), which is a brute-force password cracker tool (it uses common password wordlists and rainbow tables), so I don't need to know the key :

Since we can't install anything in our VM, we will use JTR out of the VM :

```bash
echo "42hDRfypTqqnw" > hash.txt
john hash.txt
```

John will:

- auto-detect the type of your hash
- start cracking with its default wordlist
- if it found a match it will print you the answer on the screen

Output :

```
Loaded 1 password hash (descrypt, traditional crypt(3) [DES 128/128 SSE2])
Will run 8 OpenMP threads
Press 'q' or Ctrl-C to abort, almost any other key for status
abcdefg          (?)
1g 0:00:00:00 100% 2/3 2.127g/s 69719p/s 69719c/s 69719C/s 123456..thebest3
Use the "--show" option to display all of the cracked passwords reliably
Session completed
```

Ok, so the de-hashed value for "42hDRfypTqqnw" is "abcdefg".

So, now let's connect to "flag01" :

```
level01@SnowCrash:~$ su flag01
Password: abcdefg
Don't forget to launch getflag !
flag01@SnowCrash:~$ getflag
Check flag.Here is your token : f2av5il02puano7naaf6adaaf
flag01@SnowCrash:~$ su level02
Password: f2av5il02puano7naaf6adaaf

level02@SnowCrash:~$ 
```
