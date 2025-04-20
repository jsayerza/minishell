# Variables
NAME = libget_next_line.a
CC = gcc
CFLAGS = -Wall -Wextra -Werror -I./ -g -O0

HEADER = get_next_line.h
#HEADER = get_next_line_bonus.h

SRCS =	get_next_line.c main.c
#get_next_line_utils.c 
#SRCS =	get_next_line_bonus.c get_next_line_utils_bonus.c #main.c

BONUS_SRCS = 

# Objetos
OBJ = $(SRCS:.c=.o)
BONUS_OBJ = $(BONUS_SRCS:.c=.o)

EXE = programa

# Regla por defecto: compila el archivo estático y el ejecutable
all: $(NAME)

# Regla para construir el archivo estático
$(NAME): $(OBJ) $(HEADER)
	@ar rcs $(NAME) $(OBJ)

# Regla para construir el ejecutable
$(EXE): $(OBJ) $(NAME)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ) -L. -lget_next_line
	
# Regla para compilar con las funciones bonus e incluirlas en el archivo estático
bonus: $(OBJ) $(BONUS_OBJ)
	@ar rcs $(NAME) $(OBJ) $(BONUS_OBJ)

# Regla para limpiar archivos objeto y el archivo estático
clean:
	@rm -f $(OBJ) $(BONUS_OBJ)

# Regla para limpiar todos los archivos generados por la compilación
fclean: clean
	@rm -f $(NAME) $(EXE)

# Regla para ejecutar el programa
run: $(EXE)
	@./$(EXE)

re: fclean all

# Regla para compilar con información de depuración y ejecutar el programa
gdb: fclean $(EXE)
	gdb $(EXE)
	
# Regla para construir el archivo objeto
%.o: %.c Makefile get_next_line.h
	$(CC) $(CFLAGS) -c $< -o $@ 

# Phony targets
.PHONY: all clean fclean run gdb re bonus
