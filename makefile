A ne pas prendre en compte 

Raison des variables:
Client:
La variable current_bit est utilisée pour garder une trace du bit actuel qui est en train d'être traité et envoyé au serveur. Comme le processus d'envoi des signaux est séquentiel et que chaque bit est traité un par un, une seule variable globale suffit pour cette tâche.
L'utilisation d'une seule variable globale minimise l'utilisation de la mémoire et maintient la simplicité du code, ce qui est bénéfique pour les débutants. Il est plus facile de suivre le flux du programme avec une seule variable qui change de valeur plutôt qu'avec de multiples variables ou une structure plus complexe.
Serveur:
La variable globale server_state est utilisée pour maintenir l'état du serveur, y compris la construction du message en cours de réception.

La raison pour laquelle une variable globale est utilisée ici peut être multiple :

Persistance de l'état : Une variable globale est persistante à travers l'exécution du programme. Cela signifie que l'état du serveur, y compris le nombre de bits reçus (bit_count), le buffer du message (message), et l'indice du caractère en cours (message_index), reste intact entre les appels de la fonction gestionnaire de signal signal_handler. Sans cette persistance, il serait impossible de construire le message sur plusieurs signaux reçus.

Accès depuis le gestionnaire de signal : Les gestionnaires de signal ont une signature fixe (ils prennent un seul paramètre int qui est le numéro du signal) et ne peuvent pas accéder aux variables locales à main ou à d'autres fonctions sans un pointeur global ou statique. La variable server_state doit être accessible depuis signal_handler, donc elle est définie comme statique globale.

Simplicité : Utiliser une structure globale simplifie le code en évitant de passer l'état du serveur à travers plusieurs fonctions ou d'utiliser des pointeurs complexes.

Cependant, l'utilisation de variables globales n'est généralement pas recommandée car elle peut conduire à des problèmes de conception, comme le couplage serré entre les composants du programme et des difficultés à gérer l'état dans les environnements multithread. Dans ce cas spécifique, la variable globale est justifiée par la nature du gestionnaire de signal qui a besoin d'accéder à l'état entre les signaux. Il est important de noter que le programme doit être soigneusement conçu pour éviter des conditions de compétition et des problèmes de concurrence, particulièrement si le serveur doit gérer plusieurs sources de signaux ou fonctionner dans un environnement multithread.

Le commentaire dans le code indique qu'il n'est permis d'utiliser qu'une seule variable globale, ce qui suggère que les concepteurs du programme étaient conscients des inconvénients de l'utilisation de variables globales et ont voulu limiter leur utilisation autant que possible.

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
