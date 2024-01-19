#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#define DELAY 700 // Délai en microsecondes entre l'envoi de chaque signal.

// Variable globale pour stocker le bit courant envoyé. On choisit cette variable comme globale
// car elle représente l'état actuel de l'envoi des signaux et est utilisée à travers les fonctions.
static int current_bit = 0;

/**
 * Envoie un signal spécifié à un PID de serveur donné.
 * @param server_pid Le PID du processus serveur.
 * @param signal Le signal à envoyer (SIGUSR1 pour un '0', SIGUSR2 pour un '1').
 */
void send_signal(pid_t server_pid, int signal) {
    if (kill(server_pid, signal) == -1) { // En cas d'erreur lors de l'envoi du signal
        perror("Erreur lors de l'envoi du signal");
        exit(EXIT_FAILURE); // On arrête le programme avec un code d'erreur.
    }
    usleep(DELAY); // Attendre un peu après avoir envoyé le signal pour donner le temps au serveur de réagir.
}

/**
 * Envoie un caractère au serveur en le décomposant en une séquence de signaux.
 * @param c Le caractère à envoyer.
 * @param server_pid Le PID du processus serveur.
 */
void send_char_as_signals(char c, pid_t server_pid) {
    // Parcourt chaque bit du caractère, du plus significatif au moins significatif.
    for (int i = 7; i >= 0; --i) {
        current_bit = (c >> i) & 1; // Isoler le bit courant à envoyer.
        // Envoie le signal correspondant au bit actuel.
        send_signal(server_pid, current_bit ? SIGUSR2 : SIGUSR1);
    }
}

/**
 * Valide les arguments passés au programme.
 * @param argc Le nombre d'arguments passés au programme.
 * @param argv Les arguments du programme.
 * @return 1 si les arguments sont valides, sinon 0.
 */
int validate_arguments(int argc, char *argv[]) {
    if (argc != 3 || atoi(argv[1]) <= 0) { // Vérifie la validité du PID et la présence de trois arguments.
        fprintf(stderr, "Usage: %s <Server PID> <Message>\n", argv[0]);
        return 0; // Renvoie 0 si les arguments ne sont pas valides.
    }
    return 1; // Renvoie 1 si les arguments sont valides.
}

/**
 * Le point d'entrée principal du programme client.
 * Le programme envoie un message au serveur caractère par caractère.
 */
int main(int argc, char *argv[]) {
    if (!validate_arguments(argc, argv)) { // Valider les arguments avant de continuer.
        return 1; // Termine le programme si les arguments ne sont pas valides.
    }

    pid_t server_pid = (pid_t)atoi(argv[1]); // Convertir le PID du serveur en entier.
    char *message = argv[2]; // Pointer vers le message à envoyer.

    // Envoyer chaque caractère du message, caractère par caractère.
    while (*message != '\0') { // Continuer jusqu'à la fin de la chaîne.
        send_char_as_signals(*message, server_pid); // Envoyer le caractère actuel.
        message++; // Passer au caractère suivant.
    }

    // Envoyer le caractère nul pour indiquer la fin du message.
    send_char_as_signals('\0', server_pid);

    printf("Message envoyé au serveur avec le PID %d\n", server_pid); // Confirmer l'envoi du message.
    return 0; // Terminer le programme avec succès.
}
