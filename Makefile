NAME=mysh
CC=gcc
RM=rm -rf

#Directory
SRCS_DIR = srcs
OBJS_DIR = objs

# Sources and objects
SRCS = $(shell find $(SRCS_DIR) -name '*.c')
OBJS = $(patsubst $(SRCS_DIR)/%,$(OBJS_DIR)/%,$(SRCS:%.c=%.o))

all: $(NAME)

$(NAME) : $(OBJS_DIR) $(OBJS)
	$(CC) $(OBJS) -o $@

$(OBJS_DIR):
	mkdir $(OBJS_DIR)

$(OBJS_DIR)/%.o : $(SRCS_DIR)/%.c
	$(CC)  -o $@ -c $<

clean:
	$(RM) $(OBJS)
	$(RM) $(OBJS_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean
	make all