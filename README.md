# Findex

Findex is a search utility that finds files and folders by name across indexed storage locations.

It is a small C program for Linux: you type a name, Findex searches for it and saves the matching paths to a file, and helper functions let you pick a result and open it.

## Features

- Prompts for a file or folder name and searches for it
- Saves every matching path to `text.txt`, one per line
- `get_path` returns the Nth result from the saved list
- `open_path` opens a path with the system default application via `xdg-open`
- Tiny codebase with a simple `Makefile` build

## Requirements

- Linux
- `gcc` and `make`
- `find` at `/bin/find`
- `xdg-open` (only needed for opening results)

## Build

```bash
git clone https://github.com/voidl0op/Findex.git
cd Findex
make
```

This produces a `Findex` binary in the project directory. To remove it:

```bash
make clean
```

## Usage

```bash
./Findex
```

```
Enter the file wanted: report.pdf
```

Findex searches under `/home` and writes the results to `text.txt` in the directory you ran it from:

```
/home/alice/Documents/report.pdf
/home/alice/Downloads/report.pdf
```

## Project structure

| File | Purpose |
| --- | --- |
| `main.c` | Entry point; reads the name to search for |
| `file.c` | `find_path`, `get_path` and `open_path` |
| `file.h` | Declarations for the functions in `file.c` |
| `Makefile` | Build configuration (`make`, `make clean`) |

## API

| Function | Description |
| --- | --- |
| `void find_path(const char* file)` | Runs `find /home -name <file>` and writes the matching paths to `text.txt` |
| `char* get_path(FILE* fptr, int choice)` | Returns the `choice`-th line (starting at 1) of the results file as a newly allocated string, or `NULL` if there is no such line. The caller frees it |
| `void open_path(const char* path)` | Opens `path` with `xdg-open` |

## Current limitations

- Results are written to `text.txt` in the current directory, overwriting any previous results

## Security

Findex builds shell command strings and runs them with `popen()` and `system()`. Both functions pass the string to `/bin/sh -c`, so any character the shell treats specially is interpreted rather than treated as plain text.

### Shell injection in `find_path`

```c
snprintf(cmd, sizeof(cmd), "/bin/find /home -name %s", file);
FILE *fp = popen(cmd, "r");
```

The name typed at the prompt is inserted into the command unquoted. Examples of what the shell does with it:

| Input | Effect |
| --- | --- |
| `x; rm -rf ~` | `;` ends the `find` command and runs a second one |
| `$(whoami)` or `` `whoami` `` | The command is executed and its output is substituted |
| `foo -delete` | Not shell syntax: the extra word becomes an option to `find`, which deletes every match |
| `*.txt` | The shell expands the glob against the current directory before `find` runs, so the search no longer matches what was typed |

A trailing newline (kept by `fgets` in `main`) also acts as a command separator in `sh`.

### Shell injection in `open_path`

```c
snprintf(cmd, sizeof(cmd), "xdg-open \"%s\"", path);
system(cmd);
```

Double quotes do not stop `$(...)` or backtick substitution. The path comes from search results, and filenames are controlled by whoever can create files under `/home`. A file named `$(some command).pdf` would run that command when it is opened with `open_path`.

### Format string in `find_path`

```c
while(fgets(buffer, 4096, fp) != NULL) fprintf(fptr, buffer);
```

`buffer` holds filenames returned by `find` and is used as the `fprintf` format string. A filename containing `%s` or `%n` is interpreted as a format specifier, which can crash the program or write to memory.

### Related issues

- `fgets` returns `NULL` on end of input (Ctrl-D), and `main` passes that result to `find_path` without checking it.
- The trailing newline from `fgets` is passed to `find_path` unstripped.

### Why this happens

`popen()` and `system()` always go through the shell. Running `find` and `xdg-open` with `fork` and `exec`, passing each argument separately, keeps user input from being parsed as shell syntax or as extra `find` options.

## Future scope

1. Add a GUI
2. Fix the security issues described above

## Contributing

Issues and pull requests are welcome. For larger changes, please open an issue first to discuss what you'd like to change.