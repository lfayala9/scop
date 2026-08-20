CC = g++

NAME = scop

FLAGS = -Wall -Wextra -Werror

SRCS = main.cpp window.cpp

OBJ = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
		$(CC) $(SRCS) -I./include -L./build/src -lglfw  -lvulkan -o $(NAME)

%.o: %.cpp
		$(CC) $(FLAGS) -c $< -o $@

clean:
		rm -rf $(OBJ)

fclean:
		rm -rf $(OBJ) $(NAME)

re: fclean all

.PHONY = all clean fclean re
