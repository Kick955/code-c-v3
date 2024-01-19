CC=gcc
CFLAGS=-Wall -Wextra -Werror
NAME=serveur

all: $(NAME)

$(NAME):
    $(CC) $(CFLAGS) -o $(NAME) serveur.c

clean:
    rm -f *.o

fclean: clean
    rm -f $(NAME)

re: fclean all

CC=gcc
CFLAGS=-Wall -Wextra -Werror
CLIENT=client

all: $(CLIENT)

$(CLIENT):
    $(CC) $(CFLAGS) -o $(CLIENT) client.c

clean:
    rm -f *.o

fclean: clean
    rm -f $(CLIENT)

re: fclean all
