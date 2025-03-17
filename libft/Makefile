TARGET = libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes -g -O0 -fno-stack-protector
SRCS = \
	ft_atoi.c ft_bzero.c ft_calloc.c \
	ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c \
	ft_itoa.c \
	ft_lstadd_back.c ft_lstadd_front.c ft_lstdelone.c ft_lstlast.c ft_lstnew.c ft_lstsize.c \
	ft_memset.c ft_memcpy.c ft_memmove.c ft_memchr.c ft_memcmp.c \
	ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c \
	ft_split.c \
	ft_strchr.c ft_strdup.c ft_striteri.c ft_strjoin.c ft_strlcat.c ft_strlcpy.c ft_strlen.c \
	ft_strndup.c \
	ft_strmapi.c ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_strtrim.c \
	ft_strcmp.c \
	ft_substr.c ft_tolower.c ft_toupper.c \
	
OBJ = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	@echo "Construint $(TARGET)..."
	@ar rcs $(TARGET) $(OBJ)
	@echo "$(TARGET) construit!"

clean:
	@echo "Cleaning object files..."
	@rm -f $(OBJ)

fclean: clean
	@echo "Removing $(TARGET)..."
	@rm -f $(TARGET)

re: fclean all

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean fclean
