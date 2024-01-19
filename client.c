#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#define DELAY 500 // 500 ms entre l'envoi de chaque signal.

/**
 * Envoie un signal spécifié à un PID de serveur donné.
 * Cette fonction est utilisée pour simuler l'envoi de bits en utilisant des signaux.
 * @param server_pid Le PID du processus serveur.
 * @param signal Le signal à envoyer (SIGUSR1 pour un bit '0', SIGUSR2 pour un bit '1').
 */
void send_signal(pid_t server_pid, int signal) {                                                       
    if (kill(server_pid, signal) == -1) { // Tente d'envoyer un signal au PID spécifié.
        perror("Erreur lors de l'envoi du signal"); // Affiche une erreur si `kill` échoue.
        exit(EXIT_FAILURE); // Termine le programme avec un code d'erreur si une erreur se produit.
    }
    usleep(DELAY); // Attend pendant un court délai après avoir envoyé le signal.
}

/**
 * Envoie un caractère au serveur en le décomposant en une séquence de signaux.
 * Chaque bit du caractère est envoyé séquentiellement en commençant par le bit le plus significatif.
 * @param c Le caractère à envoyer.
 * @param server_pid Le PID du processus serveur.
 */
void send_char_as_signals(char c, pid_t server_pid) {
    for (int i = 7; i >= 0; --i) { // Itère sur chaque bit du caractère.
        int bit = (c >> i) & 1; // Isole le bit actuel à envoyer.
        send_signal(server_pid, bit ? SIGUSR2 : SIGUSR1); // Envoie le signal approprié pour le bit.
    }
}

/**
 * Valide les arguments passés au programme.
 * Cette fonction vérifie que le bon nombre d'arguments a été fourni et que le PID est valide.
 * @param argc Le nombre d'arguments passés au programme.
 * @param argv Les arguments du programme.
 * @return 1 si les arguments sont valides, sinon 0.
 */
int validate_arguments(int argc, char *argv[]) {
    if (argc != 3 || atoi(argv[1]) <= 0) { // Vérifie que le nombre d'arguments est correct et que le PID est positif.
        fprintf(stderr, "Usage: %s <Server PID> <Message>\n", argv[0]); // Affiche l'usage correct si les arguments sont invalides.
        return 0;
    }
    return 1; // Retourne 1 si les arguments sont valides.
}

/**
 * Le point d'entrée principal du programme client.
 * Ce programme lit le PID du serveur et le message à envoyer à partir des arguments de la ligne de commande,
 * puis envoie le message au serveur caractère par caractère.
 */
int main(int argc, char *argv[]) {
    if (!validate_arguments(argc, argv)) { // Valide les arguments avant de continuer.
        return 1; // Termine le programme si les arguments ne sont pas valides.
    }

    pid_t server_pid = (pid_t)atoi(argv[1]); // Convertit le PID du serveur de la chaîne en entier.
    char *message = argv[2]; // Pointeur vers le message à envoyer.

    while (*message) { // Boucle à travers chaque caractère du message.
        send_char_as_signals(*message++, server_pid); // Envoie chaque caractère sous forme de signaux.
    }

    send_char_as_signals('\0', server_pid); // Envoie le caractère nul pour indiquer la fin du message.

    // Affiche une confirmation que le message a été envoyé.
    printf("Message envoyé au serveur avec le PID %d\n", server_pid);
    return 0; // Termine le programme avec succès.
}
