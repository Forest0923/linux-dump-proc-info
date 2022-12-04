# linux-proc-debugger

This repository provides the LKM used for kernel development and wrapper shell scripts to load/unload the LKM.

If you are doing kernel development and want to debug a process, leveraging procfs or gdb is very effective.

However, there are situations that require direct access to kernel objects, or that require modifications to the kernel in order to obtain necessary information.

This LKM allows you to obtain information about a specified process without spending time to compile the kernel.

> Tested on Linux v5.8.13, v5.17, v6.10.

## Build

First of all, you need to build LKM.

Clone this repository:

```sh
git@github.com:Forest0923/linux-proc-debugger.git
```

Get into the source directory and make:

```sh
cd src/
make
```

## Usage

All you have to do is run proc_debugger.sh and follow its instructions.

