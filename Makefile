NAME=mysh
CC=cc
RM=rm -rf
CFLAG= -Wall -Wextra -Werror -pthread #-g -fsanitize=thread

#Directory
HDRS_DIR = hdrs
SRCS_DIR = srcs
OBJS_DIR = objs

# Sources and objects
SRCS = $(shell find $(SRCS_DIR) -name '*.c')
OBJS = $(patsubst $(SRCS_DIR)/%,$(OBJS_DIR)/%,$(SRCS:%.c=%.o))
HDRS = $(HDRS_DIR)/philosopher.h

all: $(NAME)

$(NAME) : $(OBJS_DIR) $(OBJS)
	$(CC) $(CFLAG) $(OBJS) -o $@

$(OBJS_DIR):
	mkdir $(OBJS_DIR)

$(OBJS_DIR)/%.o : $(SRCS_DIR)/%.c $(HDRS)
	$(CC) $(CFLAG) -o $@ -c $< -I$(HDRS_DIR)

clean:
	$(RM) $(OBJS)
	$(RM) $(OBJS_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean
	make all