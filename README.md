*This project has been created as part of the 42 curriculum by mharoud.*

# Get Next Line

## Description

The **Get Next Line** project is an essential milestone in the 42 curriculum. Its primary objective is to implement a C function that returns a single line read from a file descriptor (`fd`), terminating with a newline character (`\n`) if one is present, until reaching the end of the file or encountering an error.

Repeated invocations of `get_next_line(fd)` allow sequential reading of an entire text file or stream (such as standard input) without buffering the whole file into memory. This project introduces key low-level concepts including static variables in C, dynamic memory allocation (`malloc`, `free`), file I/O operations (`read`), and robust leak-free buffer management.

### Prototype

```c
char *get_next_line(int fd);
```

- **Parameters:** `fd` — The file descriptor to read from.
- **Return Value:** 
  - The line read from the file descriptor (including the terminating `\n` if present).
  - `NULL` if there is nothing left to read or if an error occurred.
- **External Functions Allowed:** `read`, `malloc`, `free`.

---

## Instructions

### Compilation

The project can be compiled into a static library archive `get_next_line.a` using the provided `Makefile`:

```bash
make
```

You can define a custom buffer size at compile time using the `-D BUFFER_SIZE=n` flag:

```bash
# Standard buffer size
make BUFFER_SIZE=42

# Stress testing buffer sizes
make BUFFER_SIZE=1
make BUFFER_SIZE=9999
make BUFFER_SIZE=10000000
```

### Clean-up Rules

- `make clean` — Removes intermediate object files (`.o`).
- `make fclean` — Removes intermediate object files and the generated library (`get_next_line.a`).
- `make re` — Performs a full rebuild (`fclean` followed by `all`).

### Testing & Execution

To test the function with your own code, create a `main.c` file:

```c
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "get_next_line.h"

int main(void)
{
    int     fd;
    char    *line;

    fd = open("test.txt", O_RDONLY);
    if (fd < 0)
    {
        perror("Error opening file");
        return (1);
    }
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```

Compile and run with your library or directly with the source files:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 main.c get_next_line.a -o gnl_test
./gnl_test
```

---

## Algorithm Explanation & Technical Choices

The implementation relies on a single **static variable** (`static char *stash`) that retains characters read beyond the latest newline delimiter across successive function calls.

```
       [ get_next_line(fd) ]
                 |
                 v
   +-----------------------------+
   |  1. rts (Read To Stash)     | <--- Reads chunks of BUFFER_SIZE via read()
   |     Appends chunks to stash |      until '\n' is found or EOF is reached.
   +-----------------------------+
                 |
                 v
   +-----------------------------+
   |  2. extract_line            | <--- Slices and allocates from stash[0] up to
   |     Extracts line up to \n  |      and including '\n' (or EOF).
   +-----------------------------+
                 |
                 v
   +-----------------------------+
   |  3. clean_stash             | <--- Frees old stash, preserves leftovers
   |     Preserves remaining     |      after '\n' into new stash for next call.
   +-----------------------------+
                 |
                 v
           [ Return line ]
```

### Detailed Breakdown

1. **Reading into the Stash (`rts`)**:
   - Allocates a temporary buffer of size `BUFFER_SIZE + 1`.
   - Loops while `ft_strchr(stash, '\n')` returns `NULL` and `read()` returns `> 0` bytes.
   - Null-terminates the temporary buffer after each read call and joins it to `stash` using a memory-safe `ft_strjoin` that frees the preceding iteration of `stash`.
   - If `read()` fails (`-1`), both the temporary buffer and the current stash are freed using `ft_free_and_null` to prevent memory leaks before returning `NULL`.

2. **Line Extraction (`extract_line`)**:
   - Calculates the character count up to and including the first `\n` (or string termination `\0`).
   - Allocates the exact memory required for the line plus the null terminator (`\0`).
   - Copies the characters and returns the freshly allocated string to the caller.

3. **Buffer Trimming & Persistence (`clean_stash`)**:
   - Scans to find the position of `\n`.
   - If no newline is present (EOF reached), frees `stash` and returns `NULL`.
   - If a newline is present, calculates the length of the remaining characters (`ft_strlen(stash + i)`), allocates `new_stash`, copies the leftover substring into it, frees the old `stash`, and returns `new_stash`.

4. **Memory Safety & Utility Functions (`get_next_line_utils.c`)**:
   - `ft_free_and_null`: Utility helper that safely frees any dynamically allocated string pointer and resets it to `NULL`, eliminating dangling pointers.
   - `ft_strjoin`: Automatically frees the incoming `s1` pointer upon successful concatenation or failure, preventing memory leaks during iterative reading.

---

## Resources

### References

* [Linux Manual: `read(2)`](https://man7.org/linux/man-pages/man2/read.2.html)
* [Static Variables in C (GeeksforGeeks)](https://www.geeksforgeeks.org/static-variables-in-c/)
* [Managing Dynamic Memory Allocation in C (GNU C Library)](https://www.gnu.org/software/libc/manual/html_node/Memory-Allocation.html)

### AI Usage Statement

* **Tool:** Gemini.
* **Purposes:**
  * Formatting the project `README.md` according to the 42 subject specification (including the mandatory italicized header and structural organization).
  * Documenting the algorithmic workflow (`rts`, `extract_line`, `clean_stash`) corresponding to the actual code implementation.
* All C code files (`get_next_line.c`, `get_next_line_utils.c`, `get_next_line.h`) and algorithmic choices were crafted and verified to conform to 42 school norms and memory leak constraints.