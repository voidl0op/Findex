# Findex

Findex is a small GTK4 search launcher for Linux: a borderless, undecorated
window with a search box and a search button, styled via a CSS file. It's
backed by a few small C helpers that find files under `/home` by name and
open the one you pick with the system default application.

## Features

- Borderless launcher window (no title bar, fixed size), styled with
  `include/style.css`
- Escape closes the window
- Search entry and "search" button in the UI
- Backend helpers (`find_path`, `get_path`, `open_path`) for searching
  `/home` and opening a result with `xdg-open`

## Status

The UI isn't wired up to the backend yet — typing in the search box doesn't
currently trigger a search. That's the next piece of work, along with fixing
the shell-injection issues described below, which need to be closed before
user input reaches `find_path`/`open_path`.

## Requirements

- Linux
- `gcc` and `make`
- GTK4 development libraries (e.g. `libgtk-4-dev` on Debian/Ubuntu)
- `find` at `/bin/find`
- `xdg-open` (only needed for opening results)

## Build

```bash
git clone git@github.com:voidl0op/Findex.git
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

Opens the launcher window. Type a name in the search box and press the
search button — search isn't wired up to results yet, so nothing happens
after that for now.

## Project structure

| Path | Purpose |
| --- | --- |
| `src/main.c` | GTK application entry point |
| `src/ui.c` / `include/ui.h` | Window setup, CSS loading, Escape-to-close |
| `src/file.c` / `include/file.h` | `find_path`, `get_path`, `open_path` backend helpers |
| `include/style.css` | GTK CSS theme for the launcher window |
| `Makefile` | Build configuration (`make`, `make clean`) |

## API

| Function | Description |
| --- | --- |
| `void find_path(const char* file)` | Runs `find /home -name <file>` and writes the matching paths to `text.txt` |
| `char* get_path(FILE* fptr, int choice)` | Returns the `choice`-th line (starting at 1) of the results file as a newly allocated string, or `NULL` if there is no such line. The caller frees it |
| `void open_path(const char* path)` | Opens `path` with `xdg-open` |

## Current limitations

- Results are written to `text.txt` in the current directory, overwriting any previous results
- The search UI doesn't call `find_path`/`open_path` yet

## Security

Findex builds shell command strings and runs them with `popen()` and `system()`. Both functions pass the string to `/bin/sh -c`, so any character the shell treats specially is interpreted rather than treated as plain text. The UI doesn't feed user input into these functions yet, but they need to be fixed before it does.

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

A trailing newline also acts as a command separator in `sh`.

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

### Why this happens

`popen()` and `system()` always go through the shell. Running `find` and `xdg-open` with `fork`/`exec` (or GLib's `GSubprocess`), passing each argument separately, keeps user input from being parsed as shell syntax or as extra `find` options.

## Future scope

1. Wire the search UI to the backend
2. Fix the security issues described above

## Contributing

Issues and pull requests are welcome. For larger changes, please open an issue first to discuss what you'd like to change.