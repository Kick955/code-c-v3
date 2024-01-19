A ne pas prendre en compte 

Raison des variables:
Client
La variable current_bit est utilisée pour garder une trace du bit actuel qui est en train d'être traité et envoyé au serveur. Comme le processus d'envoi des signaux est séquentiel et que chaque bit est traité un par un, une seule variable globale suffit pour cette tâche.
L'utilisation d'une seule variable globale minimise l'utilisation de la mémoire et maintient la simplicité du code, ce qui est bénéfique pour les débutants. Il est plus facile de suivre le flux du programme avec une seule variable qui change de valeur plutôt qu'avec de multiples variables ou une structure plus complexe.
Serveur:


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
