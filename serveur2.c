#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define MAX_MESSAGE_LENGTH 1024

// Annonce d'une seule variable globale 
static struct {
    int bit_count;
    char message[MAX_MESSAGE_LENGTH];
    int message_index;
} server_state = {0, {0}, 0};


// Fonction pour écrire le message dans un fichier log
void write_to_log(const char *msg) {
    FILE *log_file = fopen("conversations.log", "a");
    if (log_file == NULL) {
        perror("Erreur lors de l'ouverture du fichier de log");
        exit(EXIT_FAILURE);
    }
    
    time_t now = time(NULL);
    char timestamp[30];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    fprintf(log_file, "[%s] %s\n", timestamp, msg);
    fclose(log_file);
}

// Gestionnaire de signal pour SIGUSR1 et SIGUSR2
void signal_handler(int signum) {
    // Ajoute le bit au message actuel
    message[message_index] <<= 1;
    if (signum == SIGUSR2) {
        message[message_index] |= 1;
    }

    bit_count++;
    
    // Vérifie si un caractère est complet
    if (bit_count == 8) {
        bit_count = 0;
        if (message[message_index] == '\0') { // Fin du message
            write_to_log(message);
            printf("%s\n", message); // Affiche le message
            memset(message, 0, MAX_MESSAGE_LENGTH); // Réinitialise le message pour le prochain
            message_index = 0;
        } else {
            message_index++;
        }
    }
}

// Configuration de la gestion des signaux
void setup_signal_handling() {
    struct sigaction sa = {0};
    sa.sa_handler = &signal_handler;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
}

int main() {
    printf("Serveur prêt. PID: %d\n", getpid());
    setup_signal_handling();

    // Boucle infinie pour attendre les signaux
    while (1) {
        pause();
    }

    return 0; // Cette ligne ne sera jamais atteinte
}


