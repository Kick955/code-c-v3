#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define MAX_MESSAGE_LENGTH 1024

// Structure pour maintenir l'état du signal.
typedef struct {
    int bit_count;                  // Compteur de bits pour le caractère en cours
    char message[MAX_MESSAGE_LENGTH];    // Buffer pour stocker le message reçu
    int current_bit;                // Dernier bit reçu (0 ou 1)
    int current_char_index;         // Index du caractère actuel dans le message
} SignalState;

// Déclaration d'une variable globale unique conformément aux instructions
SignalState signal_state = {0, {0}, 0, 0};

// Déclaration anticipée des fonctions
void handle_signal(int signum);
void setup_signal_handling(void);
void log_message(const char *received_message);
void reset_message(void);

int main() {
    FILE *log_file = fopen("conversations.log", "a");
    if (log_file == NULL) {
        perror("Erreur lors de l'ouverture du fichier de log");
        exit(EXIT_FAILURE);
    }

    setup_signal_handling();

    printf("Serveur prêt. PID: %d\n", getpid());

    while (1) {
        pause(); // Met le processus en pause jusqu'à ce qu'un signal soit reçu
    }

    // Cette ligne ne sera jamais atteinte
    fclose(log_file);
    return 0;
}

void log_message(const char *received_message) {
    FILE *log_file = fopen("conversations.log", "a");
    if (log_file == NULL) {
        perror("Erreur lors de l'ouverture du fichier de log");
        exit(EXIT_FAILURE);
    }

    time_t now = time(NULL);
    char timestamp[30];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    fprintf(log_file, "[%s] %s\n", timestamp, received_message);
    fflush(log_file);

    fclose(log_file);
}

void reset_message() {
    memset(signal_state.message, 0, signal_state.current_char_index + 1);
    signal_state.current_char_index = 0;
}

void handle_signal(int signum) {
    signal_state.current_bit = (signum == SIGUSR2) ? 1 : 0;
    signal_state.message[signal_state.current_char_index] = (signal_state.message[signal_state.current_char_index] << 1) | signal_state.current_bit;
    signal_state.bit_count++;

    if (signal_state.bit_count == 8) {
        signal_state.bit_count = 0;
        signal_state.current_char_index++;

        if (signal_state.message[signal_state.current_char_index - 1] == '\0') {
            log_message(signal_state.message);
            reset_message();
        }
    }
}

void setup_signal_handling() {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_signal;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
}
