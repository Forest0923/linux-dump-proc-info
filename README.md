# linux-dump-proc-info

This is a simple LKM to dump processe information.

Tested on Linux v5.8.13, v5.17.

## Build

To build the module, run the following command:

```sh
make
```

## Usage

- Specify apprication

```sh
make test_proc APP=[name]
```

- Specify PID

```sh
make test_pid PID=[pid]
```

