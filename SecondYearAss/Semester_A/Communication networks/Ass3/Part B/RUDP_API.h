#ifndef RUDP_API_H
#define RUDP_API_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>

#define RETRY 3
#define WINDOW_MAX_SIZE 60000

#define SUCCESS 1
#define FAILURE 0
#define ERROR -1

/**
 * @brief Structure defining the RUDP flags.
 */
typedef struct RUDP_flags {
  unsigned int SYN : 1; /**< Synchronization flag. */
  unsigned int ACK : 1; /**< Acknowledgment flag. */
  unsigned int DATA : 1; /**< Data flag. */
  unsigned int FIN : 1; /**< Finish flag. */
} RUDP_flags;

/**
 * @brief Structure defining the RUDP packet.
 */
typedef struct _RUDP {
  RUDP_flags flags; /**< Flags indicating the packet type. */
  int seq_num; /**< Sequence number of the packet. */
  int checksum; /**< Checksum for error detection. */
  int length; /**< Length of the data payload. */
  char data[WINDOW_MAX_SIZE]; /**< Data payload. */
} RUDP;

/**
 * @brief Create a RUDP socket.
 * @return The socket file descriptor if successful, otherwise -1.
 */
int create_RUDP_socket();

/**
 * @brief Establish a connection with the receiver.
 * This function connects the sender with the receiver and performs
 * a two-way handshake to establish the connection.
 * @param socket The socket to connect with.
 * @param ip The IP address of the receiver.
 * @param port The port number of the receiver.
 * @return 1 on success, 0 if failed.
 */
int connect_with_receiver(int socket, char *ip, int port);

/**
 * @brief Listen for incoming connection requests.
 * This function listens for incoming connection requests from a sender.
 * @param socket The socket to listen to.
 * @param port The port number to listen on.
 * @return 1 on success, 0 if failed.
 */
int listen_for_connection(int socket, int port);

/**
 * @brief Send data to the peer.
 * The function waits for an acknowledgment packet, and if it doesn't
 * receive any, it retransmits the data.
 * @param socket The socket to send data through.
 * @param data The data to send.
 * @param data_length The length of the data.
 * @return 1 on success, 0 if failed.
 */
int send_data(int socket, char *data, int data_length);

/**
 * @brief Receive data from the peer.
 * @param socket The socket to receive data from.
 * @param data A pointer to the received data.
 * @param data_length A pointer to the length of the received data.
 * @return 1 if data received successfully, 0 if no data received, -1 if error occurred.
 */
int receive_data(int socket, char **data, int *data_length);

/**
 * @brief Close the connection.
 * This function closes the connection with the peer.
 * @param socket The socket to close.
 * @return 1 on success, -1 if failed.
 */
int close_connection(int socket);

/**
 * @brief Print the contents of a received RUDP packet.
 * This function prints the details of a received RUDP packet.
 * @param packet The received RUDP packet.
 */
void print_received_RUDP_packet(RUDP *packet);

#endif /* RUDP_API_H */
