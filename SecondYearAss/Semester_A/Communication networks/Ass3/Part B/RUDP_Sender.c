#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "RUDP_API.h"

#define FILE_SIZE 2097152  // 2MB
#define EXIT_MESSAGE "exit"

/**
 * @brief Parse the command line arguments to extract the IP address and port number.
 * 
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line argument strings.
 * @param ip A pointer to store the extracted IP address.
 * @param port A pointer to store the extracted port number.
 * @return int 1 if successful, 0 otherwise.
 */
int parse_command_line(int argc, char *argv[], char **ip, int *port);

/**
 * @brief Connect to the receiver by creating a RUDP socket and establishing a connection.
 * 
 * @param ip The IP address of the receiver.
 * @param port The port number of the receiver.
 * @return int The socket file descriptor if successful, -1 otherwise.
 */
int connect_to_receiver(char *ip, int port);

/**
 * @brief Generate random data of a specified size.
 * 
 * @param size The size of the random data to generate.
 * @return char* A pointer to the buffer containing the generated random data.
 */
char *generate_random_data(unsigned int size);

/**
 * @brief Main function to send a file to the receiver over RUDP.
 * 
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line argument strings.
 * @return int 0 upon successful execution.
 */
int main(int argc, char *argv[]);

// Function definitions

int main(int argc, char *argv[]) {
    // Parse the command line arguments
    char *ip;
    int port;
    if (!parse_command_line(argc, argv, &ip, &port)) {
        return 1;
    }

    // Generate the file data
    char *file_data = generate_random_data(FILE_SIZE);

    // Create a RUDP socket and connect to the receiver
    int socket_fd = connect_to_receiver(ip, port);
    if (socket_fd == -1) {
        return 1;
    }

    int choice;
    do {
        printf("Sending the file\n");
        if (RUDP_send(socket_fd, file_data, FILE_SIZE) < 0) {
            printf("Error sending the file\n");
            RUDP_close(socket_fd);
            return 1;
        }
        printf("Do you want to send the file again? (1/0): ");
        scanf("%d", &choice);
    } while (choice == 1);

    printf("Closing the connection\n");
    RUDP_close(socket_fd);
    printf("Connection closed\n");
    free(file_data);
    return 0;
}

int parse_command_line(int argc, char *argv[], char **ip, int *port) {
    if (argc != 5 || strcmp(argv[1], "-ip") != 0 || strcmp(argv[3], "-p") != 0) {
        printf("Invalid arguments.\nUsage: %s -ip <IP> -p <port> \n", argv[0]);
        return 0;
    }

    *ip = argv[2];
    *port = atoi(argv[4]);
    if (*port < 0 || *port > 65535) {
        printf("Invalid port number\n");
        return 0;
    }
    return 1;
}

int connect_to_receiver(char *ip, int port) {
    int socket_fd = RUDP_socket();
    if (socket_fd == -1) {
        return -1;
    }
    if (RUDP_connect(socket_fd, ip, port) <= 0) {
        return -1;
    }
    return socket_fd;
}

char *generate_random_data(unsigned int size) {
    char *buffer = NULL;
    if (size == 0) return NULL;
    buffer = (char *)calloc(size, sizeof(char));
    if (buffer == NULL) return NULL;
    srand(time(NULL));
    for (unsigned int i = 0; i < size; i++)
        *(buffer + i) = ((unsigned int)rand() % 255) + 1;
    return buffer;
}
