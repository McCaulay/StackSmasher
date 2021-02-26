# StackSmasher

## Description
StackSmasher is an application which automatically performs a stack based buffer overflow on the target application. Currently it only supports attacking a vulnerable application through the application run time arguments. However, future modifications are to support various application inputs such as standard input and tcp input.

It will rerun the application many times to fuzz the input until a segmentation fault occurs. It will then find the offset of EIP, as well as the address of a JMP ESP instructions. Next it tests all characters to find any bad characters. Afterwards, it xor encodes the payload to obfuscate the payload and remove any bad characters. Finally it builds the final payload and outputs it.

## Running StackSmasher
```sh
./smasher vulnerable
```

Example Output:
```
   _____ __             __   _____                      __             
  / ___// /_____ ______/ /__/ ___/____ ___  ____ ______/ /_  ___  _____
  \__ \/ __/ __ `/ ___/ //_/\__ \/ __ `__ \/ __ `/ ___/ __ \/ _ \/ ___/
 ___/ / /_/ /_/ / /__/ ,<  ___/ / / / / / / /_/ (__  ) / / /  __/ /    
/____/\__/\__,_/\___/_/|_|/____/_/ /_/ /_/\__,_/____/_/ /_/\___/_/     
                                                                       
                    By https://github.com/McCaulay                     

[*] Fuzzing EIP and finding JMP ESP...
[+] Found JMP ESP at 0x565561fe
[+] EIP found at offset 0x51e with a padding length of 0x530
[*] Finding bad characters...
[+] Found 4 bad characters: \x00\x0a\x0d\x43
[*] XOR encoding payload...
[+] Executing application with payload...
_________________________________________

Hello AAAAAAAAAAAAAAAAAAAA...(truncated)
$ pwd
/home/mccaulay/StackSmasher/bin
```

## Limitations
Various settings must be disabled for this to work including DEP, NX, and ASLR. ASLR needs to be done manually using the following script. This application also only works against 32 bit applications.

## ASLR
ASLR must be disabled to test this application.

### Disable ASLR
```sh
sudo bash -c "echo 0 > /proc/sys/kernel/randomize_va_space"
```

### Enable ASLR
```sh
sudo bash -c "echo 2 > /proc/sys/kernel/randomize_va_space"
```

## Payloads
There are currently two payloads in this project.

### sh.bin
sh.bin is a simple shell which executes the /bin/sh application. This application can be easily modified in the payloads/sh.asm file, and then recompiled.

### reverse-shell.bin
reverse-shell.bin will connect to a shell listening on ip 1337 and port 1337. This can be changed in payloads/reverse-shell.asm and then recompiled.

## Compiling
To compile the applications as 32 bit, you may need to install gcc-multilib.
```sh
sudo apt-get install gcc-multilib
```

Compile all applications.
```sh
make
```

# Improvements
- [ ] Debug and fix the bug which causes payload not to work on some occasions. If not encoded, sh.bin works but reverse-shell.bin doesnt. Encoding makes results random. Possibly a bad character that isn't picked up on the bad character functionality? Other potential reasons could be bad character at a specific index only?
- [ ] Support multiple output languages (Golang / Ruby etc)
- [ ] Move shellcode to inside padding then decrease ESP, JMP ESP?
- [ ] Validate there is enough space for payload
- [ ] Support more application input types (interactive, stdin, network).
- [ ] Support more encoders.
- [ ] Support x64
- [ ] ROP Chains
- [ ] Ret2Libc
- [ ] Windows
- [ ] Search library memory for JMP ESP
- [ ] Support alternatives to JMP ESP
- [ ] Support payload prefix and suffix (--template file?)
