NAME        = get_next_line.a

CC          = cc
CFLAGS      = -Wall -Wextra -Werror

# Optional default buffer size (can be overridden: make BUFFER_SIZE=42)
BUFFER_SIZE ?= 42
CFLAGS      += -D BUFFER_SIZE=$(BUFFER_SIZE)

RM          = rm -f
AR          = ar rcs

SRCS        = get_next_line.c \
              get_next_line_utils.c

OBJS        = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)

%.o: %.c get_next_line.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
