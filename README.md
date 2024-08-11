# nanoMPI

A minimal MPI Implementation loosely based on OpenMPI. nanoMPI has a few usecases:

1. First and foremost, nanoMPI is for educational purposes. The vast majority of most MPI libraries' code is dedicated to performance optimization, which makes them hard to parse as a beginner. nanoMPI allows beginners to the field of distributed computing to quickly see answers to questions like "how is a tree allreduce implemented?"
2. Development of distributed code on local devices is super convenient! It works offline and doesn't require job scheduling. Part of this project is to allow the authors to develop distributed code on a local laptop. 

## Quickstart

Here are some basic setup instructions for running socket-based point-to-point. These instructions assume a linux-based environment like Ubuntu or [WSL](https://learn.microsoft.com/en-us/windows/wsl/install).

### Pre-Requisites

Install GNU `make`
```
sudo apt install make
```

<details>
<summary>What is make? What are Makefiles?</summary>
<br>

`make` is a tool that helps compile software. Central to `make`'s operation is the `Makefile`, a text file located in the project's directory. The `Makefile` contains compilation instructions for `make`, including optimization settings, debugging options, and where to install components such as executables, documentation, and configuration files. `make` has a lot of convenience features such as only recompiling parts of the program that have changed since the last build, system-specific instructions, etc. Our favorite resource on all things `Makefile`: https://makefiletutorial.com/

We use `make` to compile nanoMPI

</details>

Install the `ssh` server
```
sudo apt update
sudo apt install openssh-server
```

Enable it to start on boot
```
sudo systemctl enable ssh
```

Generate an ssh keypair using the ed25519 protocol. Use the default location under `~/.ssh/` with no passphrase.
```
ssh-keygen -t ed25519
```

You should now have a private key (`~/.ssh/id_ed25519`) and public key (`~/.ssh/id_ed25519.pub`).

Add your key to the `ssh-agent`:

```
ssh-add ~/.ssh/id_ed25519
```

Test your installation by `ssh`-ing to your local machine:

```
$ ssh localhost
```

<details>
<summary>What is ssh? Why do we need it?</summary>
<br>
ssh (Secure Shell) is a cryptographic network protocol used for secure remote login over an unsecured network. We need SSH for several reasons:

- Secure remote access: It allows users to securely log into and control remote systems.
- File transfer: It enables secure file transfer between local and remote systems.
- Port forwarding: It can securely tunnel other protocols through its encrypted connection.

ssh-agent is a program that holds private keys used for public key authentication in SSH. It's particularly useful because:

- It saves you from typing your passphrase every time you use your SSH key.
- It allows for single sign-on across multiple SSH sessions.

In the context of the provided commands:

- We install the SSH server to allow incoming SSH connections to our machine.
- We generate an ED25519 key pair for secure authentication.
- We add the private key to ssh-agent to manage it securely and conveniently.
- We test the setup by SSH-ing to localhost, which simulates connecting to a remote machine.

</details>

### Repo Setup

Clone and build nanoMPI
```
git clone https://github.com/nsarka/nanoMPI.git
cd nanoMPI
make
```

Add nanoMPI to your linux environment:
```
export LD_LIBRARY_PATH=$PWD:$LD_LIBRARY_PATH
```

<details>
<summary>What are these Linux environment variables?</summary>
<br>
Linux environment variables are editable values that affect programs running on a system. They are part of the environment in which a process runs. There are a few important ones on linux-based systems:

- `LD_LIBRARY_PATH`: Tells the system where to look for shared libraries (e.g. `libmpi.so`) when executing programs at run-time.
- `PATH`: Tells the system where to look for program binaries (e.g. `mpirun`)

And if you're compiling with `gcc` like us:

- `LIBRARY_PATH`: Tells `gcc` where to look for linker files or ordinary libraries at compilation-time. Note that `LD_LIBRARY_PATH` is used by your program *after compilation* to find libraries, and `LIBRARY_PATH` is used by `gcc` *before compilation* to find libraries that need linked to the program.
- `CPATH`: Tells `gcc` where to look for include paths (i.e. header files) at compilation-time

You can access the values of environment variables using the `$` character. Try running `echo $LD_LIBRARY_PATH` to see the library paths already exported!

The command `export LD_LIBRARY_PATH=$PWD:$LD_LIBRARY_PATH` does the following:

- It adds the value of the current directory (`$PWD`, which is the `nanoMPI` directory) to the beginning of the existing `LD_LIBRARY_PATH`.
- This allows the system to find and use shared libraries (`libmpi.so`) in the current directory when running programs later on (`mpirun`).
- The `export` command makes this change available to all child processes of the current shell.

</details>

### Running

Fill in the hostfile with the hosts you will run with, one per line:
```
echo -e "localhost\nlocalhost" > hostfile
```


Run basic [hello world with MPI](tests/test_hello.c):
```
./mpirun ./hostfile ./tests/test_hello
```

Which should output:
```
Hello world from rank 0 out of 2 processors
Hello world from rank 1 out of 2 processors
```

## Roadmap

- [x] Basic launcher
- [ ] Socket implementation of point-to-point
- [x] Basic Collectives
- [ ] Collective algos
- [ ] InfiniBand implementation of point-to-point