#include <stdio.h> // Standard input/output library
#include <arpa/inet.h> // For the in_addr structure and the inet_pton function
#include <sys/time.h> // For the gettimeofday function
#include <time.h> // For the time function
#include <sys/socket.h> // For the socket function
#include <netinet/in.h> // For the sockaddr_in structure
#include <netinet/tcp.h> // for TCP_CONGESTION
#include <unistd.h> // For the close function
#include <string.h> // For the memset function
#include <stdlib.h> // Standard library
#define BUFFER_SIZE 2097152
#define EXPECTED_BYTES 2097152

// This function is called whenever an error occurs.
// It prints an error message, closes any open sockets, and then terminates the program.
void handleError(int sock, int sender_socket, const char* msg) {
    perror(msg);
    if (sender_socket != -1) close(sender_socket);
    if (sock != -1) close(sock);
    exit(1);
}

// function for setting up sockets while error handling using handleError helper function.
int setupSocket(int receiver_port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) handleError(-1, -1, "socket(2)");

    int opt = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        handleError(sock, -1, "setsockopt(2)");

    struct sockaddr_in receiver;
    memset(&receiver, 0, sizeof(receiver));
    receiver.sin_family = AF_INET;
    receiver.sin_port = htons(receiver_port);
    receiver.sin_addr.s_addr = INADDR_ANY;
    if (receiver.sin_addr.s_addr == INADDR_NONE) 
        handleError(sock, -1, "inet_addr(3)");
    
    
    if (bind(sock, (struct sockaddr *)&receiver, sizeof(receiver)) < 0)
        handleError(sock, -1, "bind(2)");

    if (listen(sock, 1) < 0)
        handleError(sock, -1, "listen(2)");
    // returns the socket descriptor to accept connections from clients.
    return sock;
}


int main(int argc, char *argv[]) {
    clock_t start_t, end_t;
    double total_t;
    double avgTime_t;
    double avgBandwidth = 0;
    double singleBandwidth = 0;
    int times_sent = 1;
    
    if (argc != 5)
    {
        fprintf(stderr, "5 arguments needed !\n");
        return 1;
    }

    const int receiver_port = atoi(argv[2]);
    const char *algo = argv[4];

    
    // create a socket, bind it to the specified port, and start listening for incoming connections.
    int sock = setupSocket(receiver_port);
    fprintf(stdout, "Listening for incoming connections on port %d...\n", receiver_port);
    struct sockaddr_in sender;
    socklen_t sender_len = sizeof(sender);
    // Blocks until a client connects to the Receiver.
    int sender_socket = accept(sock, (struct sockaddr *)&sender, &sender_len);
    // Sets congestion control algos, the user chooses which algo to use by input.
    if (setsockopt(sender_socket, IPPROTO_TCP, TCP_CONGESTION, algo, strlen(algo)) < 0)
    {
        handleError(sock, sender_socket, "setsockopt(2) - TCP_CONGESTION");
    }
    if (sender_socket < 0)
    {
        handleError(sock, -1, "accept(2)");
    }
    fprintf(stdout, "Sender connected, beginning to receive file..\n");

    int still_listen = 1; //
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    // ssize_t instead of int to handle large files
    // open a file to save stats for printing
    FILE *sFile = fopen("stats_file.txt", "w+");
    if (sFile == NULL)
    {
        perror("fopen(3)");
        close(sender_socket);
        close(sock);
        return 1;
    }
    fprintf(sFile,"\n------------------------------------------------\n\t\t * Statistics * \t\t\n\n");
    
    while (still_listen)
    {
        size_t total_bytes_received = 0;
        start_t = clock();
        ssize_t bytes_received = recv(sender_socket, buffer, BUFFER_SIZE, 0);
        total_bytes_received += bytes_received;
        // If the message receiving failed, print an error message and return 1.
        if (bytes_received < 0){
        perror("recv(2)");
        close(sender_socket);
        return 1;
        } else if (bytes_received == 0){
            fprintf(stdout, "Sender disconnected\n\n");
            break;
        }
        if (strcmp(buffer, "EXIT") == 0){
        fprintf(stdout, "Sender sent EXIT message \n\n");
        still_listen = 0;
        break;
        }
        while(total_bytes_received < sizeof(buffer)) {
            bytes_received = recv(sender_socket, buffer + total_bytes_received,sizeof(buffer) - total_bytes_received, 0);
            if (bytes_received < 0){
                perror("recv(2)");
                close(sender_socket);
                return 1;
            } else if (bytes_received == 0){
                fprintf(stdout, "Sender disconnected\n\n");
                break;
            }
            total_bytes_received += bytes_received;
        }
        end_t = clock();
        if (still_listen) {
            total_t = (((double)(end_t - start_t)) / CLOCKS_PER_SEC) * 1000 ;
            avgTime_t += total_t;
            singleBandwidth =  (2 / (total_t / 1000));
            avgBandwidth += singleBandwidth;
            fprintf(sFile, "Run #%d Data: Time=%.2f ms ; Speed=%.2fMB/s \n\n", times_sent, total_t , singleBandwidth);
            times_sent++;
        }
    }
        // Print all of the statistics to the stats file.
        fprintf(sFile, "- Average time: %.2f ms\n",avgTime_t / (times_sent-1));
        fprintf(sFile,"- Average bandwidth: %.2f MB/s\n", avgBandwidth / (times_sent-1));
        fprintf(sFile, "------------------------------------------------\n");
        fprintf(sFile, "Receiver end.\n");
        rewind(sFile);
        // Print the contents of sFile line by line
        rewind(sFile);
        char line[256];
        while (fgets(line, sizeof(line), sFile)) {
            fprintf(stdout, "%s", line);
        }
        fclose(sFile);
        close(sender_socket);
        close(sock);
        remove("stats_file.txt");
        return 0;
    }
 