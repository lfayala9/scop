CC = g++
NAME = scop
FLAGS = -Wall -Wextra -Werror -std=c++20
INCLUDE = -I./include
LIBS = -L./build/src -lglfw -lvulkan

SRCS = main.cpp window.cpp
OBJ = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -std=c++20 $(OBJ) $(INCLUDE) $(LIBS) -o $(NAME)

%.o: %.cpp
	$(CC) -std=c++20 $(INCLUDE) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re