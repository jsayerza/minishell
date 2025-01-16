TARGET = libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes -g -O0
SRCS = \
	ft_atoi.c ft_bzero.c ft_calloc.c \
	ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c \
	ft_itoa.c \
	ft_lstadd_back.c ft_lstadd_front.c ft_lstdelone.c ft_lstlast.c ft_lstnew.c ft_lstsize.c \
	ft_memset.c ft_memcpy.c ft_memmove.c ft_memchr.c ft_memcmp.c \
	ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c \
	ft_split.c \
	ft_strchr.c ft_strdup.c ft_striteri.c ft_strjoin.c ft_strlcat.c ft_strlcpy.c ft_strlen.c \
	ft_strmapi.c ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_strtrim.c \
	ft_substr.c ft_tolower.c ft_toupper.c \
	
OBJ = $(SRCS:.c=.o)
EXE = programa
MAIN = main.c
MAIN_OBJ = $(MAIN:.c=.o)

all: $(EXE)

$(TARGET): $(OBJ)
	@echo "Construint $(TARGET)..."
	@ar rcs $(TARGET) $(OBJ)
	@echo "$(TARGET) construit!"

$(EXE): $(MAIN_OBJ) $(TARGET)
	@echo "Construint executable $(EXE)..."
	$(CC) $(CFLAGS) -o $(EXE) $(MAIN_OBJ) -L. -lft
	@echo "$(EXE) construit!"

clean:
	@echo "Cleaning object files..."
	@rm -f $(OBJ) $(MAIN_OBJ)

fclean: clean
	@echo "Removing $(TARGET) and $(EXE)..."
	@rm -f $(TARGET) $(EXE)

run: $(EXE)
#	@echo "Running $(EXE)..."
	@./$(EXE)

lldb: fclean $(EXE)
	lldb ./$(EXE)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean fclean run lldb
