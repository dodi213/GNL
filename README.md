*This project has been created as part of the 42 curriculum by <login>.*

# get_next_line

## Description

`get_next_line(int fd)` returns the next line read from a file descriptor,
including the terminating `\n` (unless the file ends without one). Repeated
calls read the file one line at a time. It returns `NULL` when there is nothing
left to read or when an error occurs.

The bonus version handles several file descriptors at the same time while
using a single static variable.

## Instructions

Compile the function with your own `main`:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c
```

Bonus (multiple file descriptors):

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c main.c
```

`BUFFER_SIZE` is optional and defaults to 42. A value `<= 0` makes the function
return `NULL`. In the bonus, `FD_MAX` (default 1024) bounds the accepted fds.

## Algorithm

Each fd has a **stash**: a heap string holding what was read but not yet
returned.

1. **Read**: while the stash has no `\n` and `read()` doesn't hit EOF, read
   `BUFFER_SIZE` bytes into a heap buffer and append them to the stash. This
   reads only as much as needed for the next line, never the whole file.
2. **Split**: cut the stash after the first `\n` (or at its end). The first
   part is returned as the line. The rest becomes the new stash, or `NULL` if
   empty.
3. **Cleanup**: on EOF with an empty stash, on a `read()` error, or on a failed
   `malloc`, the stash is freed and set to `NULL`, so nothing leaks and later
   calls start clean.

Why this design:
- The read buffer is `malloc`ed, not a stack array, so a very large
  `BUFFER_SIZE` (e.g. 10000000) cannot overflow the stack.
- The mandatory part uses `static char *stash`. The bonus uses
  `static char *stash[FD_MAX]`, which is still one static variable, indexed by
  fd so each descriptor keeps its own reading state.

## Resources

- `man 2 read`, `man 3 malloc`
- [Static variables in C](https://en.wikipedia.org/wiki/Static_variable)
- [File descriptors](https://en.wikipedia.org/wiki/File_descriptor)

**AI usage:** AI (Claude) was used to write the implementation and this
README, and to test it (norminette, multiple `BUFFER_SIZE` values,
stdin, multiple fds, and Valgrind leak checks).
