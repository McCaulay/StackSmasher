# StackSmasher

## Disclaimer
This project was built to advance my knowledge of buffer overflows, debuggers, registers, assembly and programming languages. I am sure this sure this software will contain vulnerabiities itself, as I do not have lots of experience with C or C++. I would appreciate any pull requests to fix any bugs or vulnerabilities.

## Description
StackSmasher is an application which automatically performs a stack based buffer overflow on the target application. Currently it only supports attacking a vulnerable application through the application run time arguments. However, future modifications are to support various application inputs such as standard input and tcp input.

It will rerun the application many times to fuzz the input until a segmentation fault occurs. It will then find the offset of EIP, as well as the address of a JMP ESP instructions. Next it tests all characters to find any bad characters. Afterwards, it xor encodes the payload to obfuscate the payload and remove any bad characters. Finally it builds the final payload and outputs it.

## Running StackSmasher
```sh
./smasher vulnerable -e
```

![StackSmasher](https://user-images.githubusercontent.com/31845045/109390832-ca443180-790b-11eb-904c-67c6b34204ff.png)

Help:
```
Usage: StackSmasher [options] application 

Positional arguments:
application           	The target application being tested

Optional arguments:
-h --help             	Shows help message and exists
--version             	Show program's version number and exit
-p --payload          	The built-in payload to use. [execute, bind-shell, reverse-shell, shutdown] [default: "execute"]
-pf --payload-file    	The binary payload file to be sent instead of built-in payload. [default: ""]
-v --verbose          	Verbosity level: 0-4. [default: 1]
-n --nops             	The number of NOPs to put in the NOP sled before the shell. [default: 20]
-q --quiet            	Do not print the start header. [default: false]
-e --exploit          	Automatically run the full payload against the application. [default: false]
-s --script           	Save the exploit as a script for the given language. [python, go, c, ruby] [default: ""]
-sf --script-filename 	The filename without extension the script should be saved as if --script is passed. [default: ""]
--binary              	The binary the application should execute if a built-in payload is selected. [default: "/bin/sh"]
--shell-ip            	The IP address a reverse shell should connect to. [default: "127.0.0.1"]
--shell-port          	The port number a bind shoud listen on or a reverse shell should connect to. [default: 1337]
--skip-aslr-check     	Skip checking if ASLR is enabled and proceed anyway. [default: false]
--skip-encoding       	Skip encoding the payload and checking bad characters. [default: false]
--no-colour           	Do not output ANSI escape codes to show colours. [default: false]
```

## Limitations
Various settings must be disabled for this to work including DEP, NX, and ASLR. ASLR needs to be done manually using the following script. This application also only works against 32 bit applications.

## Payloads
There are currently four payloads in this project.

### sh.bin
sh.bin is a simple shell which executes the /bin/sh application. This application can be easily modified in the payloads/sh.asm file, and then recompiled.

### bind-shell.bin
bind-shell.bin will listen for a connection port 1337. This can be changed in payloads/bind-shell.asm and then recompiled.

### reverse-shell.bin
reverse-shell.bin will connect to a shell listening on ip 127.0.0.1 and port 1337. This can be changed in payloads/reverse-shell.asm and then recompiled.

### shutdown.bin
shutdown.bin will automatically shut down the computer when executed.

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
- [ ] Search library memory for JMP ESP
- [ ] Support alternatives to JMP ESP
- [ ] Support payload prefix and suffix (--template file?)
- [ ] Support more application input types (interactive, stdin, network).
- [ ] Validate there is enough space for payload
- [ ] Move shellcode to inside padding then decrease ESP, JMP ESP?
- [ ] Support x64
- [ ] ROP Chains
- [ ] Support more encoders.
- [ ] Ret2Libc
- [ ] Windows
