#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define MAX_MESSAGE_LENGTH 1024 // Longueur maximale d'un message

// Structure globale pour maintenir l'état du serveur
static struct {
    int bit_count; // Nombre de bits reçus pour le caractère en cours
    char message[MAX_MESSAGE_LENGTH]; // Buffer pour stocker le message reçu
    int message_index; // Indice du caractère en cours dans le message
} server_state = {0, {0}, 0};

// Fonction pour écrire le message dans un fichier log
void write_to_log(const char *msg) {
    // Ouvre ou crée un fichier log pour ajouter le message reçu
    FILE *log_file = fopen("conversations.log", "a");
    if (log_file == NULL) {
        perror("Erreur lors de l'ouverture du fichier de log");
        exit(EXIT_FAILURE);
    }
    
    // Obtient l'heure actuelle et la formate pour le log
    time_t now = time(NULL);
    char timestamp[30];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    // Écrit le message avec un horodatage dans le fichier log
    fprintf(log_file, "[%s] %s\n", timestamp, msg);
    fclose(log_file);
}

// Gestionnaire de signal pour SIGUSR1 et SIGUSR2
void signal_handler(int signum) {
    // Décale le message actuel d'un bit et ajoute le nouveau bit reçu
    server_state.message[server_state.message_index] <<= 1;
    if (signum == SIGUSR2) {
        server_state.message[server_state.message_index] |= 1;
    }

    server_state.bit_count++;
    
    // Vérifie si un caractère complet a été reçu
    if (server_state.bit_count == 8) {
        server_state.bit_count = 0;
        // Vérifie si c'est la fin du message
        if (server_state.message[server_state.message_index] == '\0') {
            write_to_log(server_state.message);
            printf("%s\n", server_state.message); // Affiche le message
            memset(server_state.message, 0, MAX_MESSAGE_LENGTH); // Prépare pour le prochain message
            server_state.message_index = 0;
        } else {
            server_state.message_index++;
        }
    }
}

// Configuration de la gestion des signaux
void setup_signal_handling() {
    struct sigaction sa = {0}; // Initialise la structure sigaction
    sa.sa_handler = &signal_handler; // Spécifie la fonction de gestion des signaux
    sigaction(SIGUSR1, &sa, NULL); // Configure le gestionnaire pour SIGUSR1
    sigaction(SIGUSR2, &sa, NULL); // Configure le gestionnaire pour SIGUSR2
}

int main() {
    printf("Serveur prêt. PID: %d\n", getpid()); // Affiche le PID du serveur
    printf("En attente de nouveaux messages\n");
    setup_signal_handling(); // Initialise la gestion des signaux

    // Boucle infinie pour attendre les signaux indéfiniment
    while (1) {
        pause(); // Met le processus en attente jusqu'à ce qu'un signal soit reçu
    }

    // Ce retour ne sera jamais atteint
    return 0;
}
