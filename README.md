# Snow Crash

A Linux security project focused on privilege escalation, binary exploitation, system analysis, and vulnerability discovery. This project consists of a series of security challenges where each level requires analyzing the environment, identifying vulnerabilities, and exploiting them to obtain credentials for the next level.

## Authors

Snow Crash is developed as an individual project at 42 École, showcasing practical understanding of Linux systems, Unix permissions, process execution, shell environments, binary analysis, privilege escalation, and common system-level vulnerabilities.

## Key Features

### Linux Security & Privilege Escalation

- User Privilege Analysis: Analyze users, groups, permissions, and execution contexts
- Privilege Escalation: Exploit vulnerabilities to gain access to privileged accounts
- Account Analysis: Investigate `levelXX` and `flagXX` accounts
- File Permissions: Analyze Unix file and directory permissions
- SUID Analysis: Identify and exploit privileged executables
- Process Analysis: Inspect running processes and their execution context
- Environment Analysis: Investigate environment variables and shell configurations
- Command Execution: Analyze how system commands are executed and manipulated

### Binary Analysis & Exploitation

- Binary Inspection: Analyze executable files and their behavior
- Program Analysis: Understand the relationship between binaries, users, and permissions
- Command Injection: Exploit vulnerable command execution mechanisms
- Argument Manipulation: Analyze and manipulate program arguments
- Environment Manipulation: Exploit unsafe environment configurations
- Privileged Binary Analysis: Investigate binaries executed with elevated privileges
- Exploitation Techniques: Develop appropriate techniques to bypass intended restrictions

### Unix & System Administration

- Linux Command Line: Extensive use of standard Unix utilities
- File System Analysis: Search and inspect files and directories
- Permission Management: Analyze ownership and access permissions
- Process Management: Inspect and interact with running processes
- User Management: Switch between system accounts using `su`
- SSH Access: Connect to the challenge environment remotely
- Shell Scripting: Use shell commands and scripts during exploitation
- System Configuration: Analyze system configuration and exposed resources

### Level-Based Security Challenges

The project consists of multiple progressively difficult security levels.

Each level requires:

1. Connect to the current `levelXX` account
2. Analyze the available files, binaries, processes, and environment
3. Identify the vulnerability or weakness
4. Develop an exploitation technique
5. Obtain access to the corresponding `flagXX` account or retrieve the flag directly
6. Execute the `getflag` binary when required
7. Obtain the password for the next `levelXX` account
8. Repeat the process until the final level

The challenges progressively introduce different concepts involving Linux security, permissions, binaries, shell behavior, environment variables, and privilege escalation.

## Authentication & Level Progression

The challenge environment contains two main types of accounts:

```text
levelXX
   ↓
Find vulnerability
   ↓
flagXX
   ↓
getflag
   ↓
Password for levelXX+1
   ↓
levelXX+1
```
