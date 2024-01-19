#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#define DELAY 500 // Définir un délai en microsecondes entre les envois de signaux

/**
 * Envoie un signal spécifié à un PID de serveur donné.
 * @param server_pid Le PID du processus serveur.
 * @param signal Le signal à envoyer.
 */
void send_signal(pid_t server_pid, int signal) {
    if (kill(server_pid, signal) == -1) {
        perror("Erreur lors de l'envoi du signal");
        exit(EXIT_FAILURE);
    }
    usleep(DELAY);
}

/**
 * Envoie un caractère sous forme de signaux au serveur.
 * @param c Le caractère à envoyer.
 * @param server_pid Le PID du processus serveur.
 */
void send_char_as_signals(char c, pid_t server_pid) {
    for (int i = 7; i >= 0; --i) {
        int bit = (c >> i) & 1;
        send_signal(server_pid, bit ? SIGUSR2 : SIGUSR1);
    }
}

/**
 * Valide les arguments du programme.
 * @param argc Le nombre d'arguments.
 * @param argv Les arguments du programme.
 * @return 1 si les arguments sont valides, sinon 0.
 */
int validate_arguments(int argc, char *argv[]) {
    if (argc != 3 || atoi(argv[1]) <= 0) {
        fprintf(stderr, "Usage: %s <Server PID> <Message>\n", argv[0]);
        return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (!validate_arguments(argc, argv)) {
        return 1;
    }

    pid_t server_pid = (pid_t)atoi(argv[1]);
    char *message = argv[2];

    while (*message) {
        send_char_as_signals(*message++, server_pid);
    }

    send_char_as_signals('\0', server_pid);

    printf("Message envoyé au serveur avec le PID %d\n", server_pid);
    return 0;
}
