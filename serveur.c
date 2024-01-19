#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define MAX_MESSAGE_LENGTH 1024

// Structure pour maintenir l'état des signaux reçus et construire le message.
typedef struct {
    int bit_count;                      // Compteur de bits reçus pour le caractère actuel
    char message[MAX_MESSAGE_LENGTH];   // Buffer pour accumuler le message reçu
    int current_bit;                    // Valeur du bit actuel (0 ou 1)
    int current_char_index;             // Position actuelle dans le buffer du message
} SignalState;

// Variable globale unique pour stocker l'état des signaux.
SignalState signal_state = {0, {0}, 0, 0};

// Prototypes de fonctions pour une meilleure lisibilité.
void handle_signal(int signum);
void setup_signal_handling(void);
void log_message(const char *received_message);
void reset_message(void);

int main() {
    // Tentative d'ouverture du fichier de log pour écrire les messages reçus.
    FILE *log_file = fopen("conversations.log", "a");
    if (log_file == NULL) {
        perror("Erreur lors de l'ouverture du fichier de log");
        exit(EXIT_FAILURE); // Quitter le programme si le fichier ne peut pas être ouvert
    }

    // Configuration des gestionnaires de signaux pour SIGUSR1 et SIGUSR2.
    setup_signal_handling();

    // Affichage du PID du serveur pour que le client sache où envoyer les signaux.
    printf("Serveur prêt. PID: %d\n", getpid());

    // Boucle principale du serveur en attente de signaux.
    while (1) {
        pause(); // Pause jusqu'à la réception d'un signal
    }

    // Le code ci-dessous ne sera jamais atteint. Si nécessaire, fermer le fichier log.
    fclose(log_file);
    return 0;
}

// Fonction pour écrire un message reçu dans le fichier de log avec horodatage.
void log_message(const char *received_message) {
    // Ouvre le fichier de log en mode ajout.
    FILE *log_file = fopen("conversations.log", "a");
    if (log_file == NULL) {
        perror("Erreur lors de l'ouverture du fichier de log");
        exit(EXIT_FAILURE);
    }

    // Obtient l'horodatage actuel et le formate.
    time_t now = time(NULL);
    char timestamp[30];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    // Écrit le message et l'horodatage dans le fichier de log.
    fprintf(log_file, "[%s] %s\n", timestamp, received_message);
    fflush(log_file); // Force l'écriture des données sur le disque

    // Ferme le fichier de log après chaque écriture pour garantir la mise à jour du fichier.
    fclose(log_file);
}

// Fonction pour réinitialiser le buffer du message après la réception d'un message complet.
void reset_message() {
    // Réinitialise le buffer du message à zéro.
    memset(signal_state.message, 0, MAX_MESSAGE_LENGTH);
    signal_state.current_char_index = 0; // Remet l'index à zéro pour le prochain message
}

// Gestionnaire de signaux pour traiter SIGUSR1 et SIGUSR2.
void handle_signal(int signum) {
    // Détermine le bit actuel en fonction du signal reçu et l'ajoute au message.
    signal_state.current_bit = (signum == SIGUSR2) ? 1 : 0;
    signal_state.message[signal_state.current_char_index] = (signal_state.message[signal_state.current_char_index] << 1) | signal_state.current_bit;
    signal_state.bit_count++; // Incrémente le compteur de bits

    // Si un octet complet a été reçu, passe au caractère suivant.
    if (signal_state.bit_count == 8) {
        signal_state.bit_count = 0; // Réinitialise le compteur de bits
        signal_state.current_char_index++; // Passe à la position suivante dans le buffer

        // Si le caractère nul est atteint, le message est complet.
        if (signal_state.message[signal_state.current_char_index - 1] == '\0') {
            log_message(signal_state.message); // Écrit le message dans le fichier de log
            reset_message(); // Réinitialise le buffer pour le prochain
