#include "RUDP_API.h"

#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define TIMEOUT 1  // Timeout in seconds

static int calculate_checksum(RUDP *packet);
static int wait_for_acknowledgment(int socket, int seq_num, clock_t start_time, int timeout);
static int send_acknowledgment(int socket, RUDP *packet);
static int set_socket_timeout(int socket, int time);

static int sequence_number = 0;

/**
 * @brief Print the contents of a received RUDP packet.
 * @param packet The received RUDP packet.
 */
void print_received_RUDP_packet(RUDP *packet) {
  printf("Received RUDP Packet:\n");
  printf("  Flags:\n");
  printf("    SYN: %u\n", packet->flags.SYN);
  printf("    ACK: %u\n", packet->flags.ACK);
  printf("    DATA: %u\n", packet->flags.DATA);
  printf("    FIN: %u\n", packet->flags.FIN);
  printf("  Sequence Number: %d\n", packet->seq_num);
  printf("  Checksum: %d\n", packet->checksum);
  printf("  Length: %d\n", packet->length);
  printf("\n");
}

/**
 * @brief Create a RUDP socket.
 * @return The socket file descriptor if successful, otherwise -1.
 */
int create_RUDP_socket() {
  // Create UDP socket
  int socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (socket_fd == -1) {
    perror("Socket creation failed");
    return ERROR;
  }
  return socket_fd;
}

/**
 * @brief Establish a connection with the receiver.
 * @param socket The socket to connect with.
 * @param ip The IP address of the receiver.
 * @param port The port number of the receiver.
 * @return 1 on success, 0 if failed.
 */
int connect_with_receiver(int socket, char *ip, int port) {
  // Set up timeout for the socket
  if (set_socket_timeout(socket, TIMEOUT) == ERROR) {
    return FAILURE;
  }

  // Set up the server address structure
  struct sockaddr_in server_address;
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port);
  int rval = inet_pton(AF_INET, (const char *)ip, &server_address.sin_addr);
  if (rval <= 0) {
    perror("inet_pton() failed");
    return FAILURE;
  }

  // Connect to the receiver
  if (connect(socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
    perror("Connect failed");
    return FAILURE;
  }

  // Send SYN message
  RUDP *packet = malloc(sizeof(RUDP));
  memset(packet, 0, sizeof(RUDP));
  packet->flags.SYN = 1;
  int total_tries = 0;
  RUDP *recv_packet = NULL;
  while (total_tries < RETRY) {
    int send_result = sendto(socket, packet, sizeof(RUDP), 0, NULL, 0);
    if (send_result == -1) {
      perror("sendto failed");
      free(packet);
      return FAILURE;
    }
    clock_t start_time = clock();
    do {
      // Receive SYN-ACK message
      recv_packet = malloc(sizeof(RUDP));
      memset(recv_packet, 0, sizeof(RUDP));
      int recv_result = recvfrom(socket, recv_packet, sizeof(RUDP), 0, NULL, 0);
      if (recv_result == -1) {
        perror("recvfrom failed");
        free(packet);
        free(recv_packet);
        return ERROR;
      }
      if (recv_packet->flags.SYN && recv_packet->flags.ACK) {
        printf("Connected\n");
        free(packet);
        free(recv_packet);
        return SUCCESS;
      } else {
        printf("Received wrong packet when trying to connect");
      }
    } while ((double)(clock() - start_time) / CLOCKS_PER_SEC < TIMEOUT);
    total_tries++;
  }

  printf("Could not connect to receiver");
  free(packet);
  free(recv_packet);
  return FAILURE;
}

/**
 * @brief Listen for incoming connection requests.
 * @param socket The socket to listen to.
 * @param port The port number to listen on.
 * @return 1 on success, 0 if failed.
 */
int listen_for_connection(int socket, int port) {
  // Set up the server address structure
  struct sockaddr_in server_address;
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port);
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);

  // Bind the socket to the server address
  int bind_result = bind(socket, (struct sockaddr *)&server_address, sizeof(server_address));
  if (bind_result == -1) {
    perror("Bind failed");
    close(socket);
    return ERROR;
  }

  // Receive SYN message
  struct sockaddr_in client_address;
  socklen_t client_address_len = sizeof(client_address);
  memset((char *)&client_address, 0, sizeof(client_address));

  RUDP *packet = malloc(sizeof(RUDP));
  memset(packet, 0, sizeof(RUDP));
  int recv_len = recvfrom(socket, packet, sizeof(RUDP) - 1, 0, (struct sockaddr *)&client_address, &client_address_len);

  if (recv_len == -1) {
    perror("recvfrom() failed on receiving SYN");
    free(packet);
    return ERROR;
  }

  // Make the connection for saving the client address
  if (connect(socket, (struct sockaddr *)&client_address, client_address_len) == -1) {
    perror("Connect failed");
    free(packet);
    return ERROR;
  }

  // Send SYN-ACK message
  if (packet->flags.SYN == 1) {
    RUDP *packet_reply = malloc(sizeof(RUDP));
    memset(packet_reply, 0, sizeof(RUDP));
    packet_reply->flags.SYN = 1;
    packet_reply->flags.ACK = 1;
    int send_result = sendto(socket, packet_reply, sizeof(RUDP), 0, NULL, 0);
    if (send_result == -1) {
      perror("sendto() failed");
      free(packet);
      free(packet_reply);
      return ERROR;
    }
    set_socket_timeout(socket, TIMEOUT * 10);
    free(packet);
    free(packet_reply);
    return SUCCESS;
  }

  return FAILURE;
}

/**
 * @brief Send data to the peer.
 * The function waits for an acknowledgment packet, and if it doesn't
 * receive any, it retransmits the data.
 * @param socket The socket to send data through.
 * @param data The data to send.
 * @param data_length The length of the data.
 * @return 1 on success, 0 if failed.
 */
int send_data(int socket, char *data, int data_length) {
  int packets_num = data_length / WINDOW_MAX_SIZE;
  int last_packet_size = data_length % WINDOW_MAX_SIZE;

  RUDP *packet = malloc(sizeof(RUDP));

  for (int i = 0; i < packets_num; i++) {
    memset(packet, 0, sizeof(RUDP));
    packet->seq_num = i;
    packet->flags.DATA = 1;
    if (i == packets_num - 1 && last_packet_size == 0) {
      packet->flags.FIN = 1;
    }
    memcpy(packet->data, data + i * WINDOW_MAX_SIZE, WINDOW_MAX_SIZE);
    packet->length = WINDOW_MAX_SIZE;
    packet->checksum = calculate_checksum(packet);
    do {
      int send_result = sendto(socket, packet, sizeof(RUDP), 0, NULL, 0);
      if (send_result == -1) {
        perror("sendto() failed");
        return FAILURE;
      }
    } while (wait_for_acknowledgment(socket, i, clock(), TIMEOUT) <= 0);
  }

  if (last_packet_size > 0) {
    memset(packet, 0, sizeof(RUDP));
    packet->seq_num = packets_num;
    packet->flags.DATA = 1;
    packet->flags.FIN = 1;
    memcpy(packet->data, data + packets_num * WINDOW_MAX_SIZE, last_packet_size);
    packet->length = last_packet_size;
    packet->checksum = calculate_checksum(packet);
    do {
      int send_result = sendto(socket, packet, sizeof(RUDP), 0, NULL, 0);
      if (send_result == -1) {
        perror("sendto() failed");
        free(packet);
        return FAILURE;
      }
    } while (wait_for_acknowledgment(socket, packets_num, clock(), TIMEOUT) <= 0);
    free(packet);
  }
  return SUCCESS;
}

/**
 * @brief Receive data from the peer.
 * @param socket The socket to receive data from.
 * @param data A pointer to the received data.
 * @param data_length A pointer to the length of the received data.
 * @return 1 if data received successfully, 0 if no data received, -1 if error occurred.
 */
int receive_data(int socket, char **data, int *data_length) {
  RUDP *packet = malloc(sizeof(RUDP));
  memset(packet, 0, sizeof(RUDP));

  int recv_len = recvfrom(socket, packet, sizeof(RUDP) - 1, 0, NULL, 0);
  if (recv_len == -1) {
    perror("recvfrom() failed on receiving data");
    free(packet);
    return ERROR;
  }

  if (calculate_checksum(packet) != packet->checksum) {
    free(packet);
    return ERROR;
  }

  if (send_acknowledgment(socket, packet) == -1) {
    free(packet);
    return ERROR;
  }

  if (packet->flags.SYN == 1) {
    printf("Received connection request\n");
    free(packet);
    return 0;
  }

  if (packet->seq_num == sequence_number) {
    if (packet->seq_num == 0 && packet->flags.DATA == 1) {
      set_socket_timeout(socket, TIMEOUT * 10);
    }

    if (packet->flags.FIN == 1 && packet->flags.DATA == 1) {
      *data = malloc(packet->length);
      memcpy(*data, packet->data, packet->length);
      *data_length = packet->length;
      free(packet);
      sequence_number = 0;
      set_socket_timeout(socket, 10000000);
      return 2;
    }

    if (packet->flags.DATA == 1) {
      *data = malloc(packet->length);
      memcpy(*data, packet->data, packet->length);
      *data_length = packet->length;
      free(packet);
      sequence_number++;
      return 1;
    }
  } else if (packet->flags.DATA == 1) {
    free(packet);
    return 0;
  }

  if (packet->flags.FIN == 1) {
    free(packet);
    printf("Received close request\n");

    set_socket_timeout(socket, TIMEOUT * 10);

    packet = malloc(sizeof(RUDP));
    time_t FIN_send_time = time(NULL);
    printf("Waiting for close\n");
    while ((double)(time(NULL) - FIN_send_time) < TIMEOUT) {
      memset(packet, 0, sizeof(RUDP));
      recvfrom(socket, packet, sizeof(RUDP) - 1, 0, NULL, 0);
      if (packet->flags.FIN == 1) {
        if (send_acknowledgment(socket, packet) == -1) {
          free(packet);
          return ERROR;
        }
        FIN_send_time = time(NULL);
      }
    }
    free(packet);
    close(socket);
    return -2;
  }
  free(packet);
  return 0;
}

/**
 * @brief Close the connection.
 * @param socket The socket to close.
 * @return 1 on success, -1 if failed.
 */
int close_connection(int socket) {
  RUDP *close_packet = malloc(sizeof(RUDP));
  memset(close_packet, 0, sizeof(RUDP));
  close_packet->flags.FIN = 1;
  close_packet->seq_num = -1;
  close_packet->checksum = calculate_checksum(close_packet);
  do {
    int send_result = sendto(socket, close_packet, sizeof(RUDP), 0, NULL, 0);
    if (send_result == -1) {
      perror("sendto() failed");
      free(close_packet);
      return FAILURE;
    }
  } while (wait_for_acknowledgment(socket, -1, clock(), TIMEOUT) <= 0);
  close(socket);
  free(close_packet);
  return SUCCESS;
}

/**
 * @brief Calculate the checksum of the packet.
 * @param packet The RUDP packet.
 * @return The checksum value.
 */
static int calculate_checksum(RUDP *packet) {
  int sum = 0;
  for (int i = 0; i < 10 && i < WINDOW_MAX_SIZE; i++) {
    sum += packet->data[i];
  }
  return sum;
}

/**
 * @brief Wait for acknowledgment from the receiver.
 * @param socket The socket to wait on.
 * @param seq_num The sequence number of the packet.
 * @param start_time The start time of waiting.
 * @param timeout The timeout duration.
 * @return 1 on success, 0 if no acknowledgment received, -1 if error occurred.
 */
static int wait_for_acknowledgment(int socket, int seq_num, clock_t start_time, int timeout) {
  RUDP *packet_reply = malloc(sizeof(RUDP));
  while ((double)(clock() - start_time) / CLOCKS_PER_SEC < timeout) {
    int recv_len = recvfrom(socket, packet_reply, sizeof(RUDP) - 1, 0, NULL, 0);
    if (recv_len == -1) {
      free(packet_reply);
      return FAILURE;
    }
    if (packet_reply->seq_num == seq_num && packet_reply->flags.ACK == 1) {
      free(packet_reply);
      return SUCCESS;
    }
  }
  free(packet_reply);
  return FAILURE;
}

/**
 * @brief Send acknowledgment to the sender.
 * @param socket The socket to send acknowledgment through.
 * @param packet The received packet.
 * @return 1 on success, -1 if failed.
 */
static int send_acknowledgment(int socket, RUDP *packet) {
  RUDP *ack_packet = malloc(sizeof(RUDP));
  memset(ack_packet, 0, sizeof(RUDP));
  ack_packet->flags.ACK = 1;
  if (packet->flags.FIN == 1) {
    ack_packet->flags.FIN = 1;
  }
  if (packet->flags.SYN == 1) {
    ack_packet->flags.SYN = 1;
  }
  if (packet->flags.DATA == 1) {
    ack_packet->flags.DATA = 1;
  }
  ack_packet->seq_num = packet->seq_num;
  ack_packet->checksum = calculate_checksum(ack_packet);
  int send_result = sendto(socket, ack_packet, sizeof(RUDP), 0, NULL, 0);
  if (send_result == -1) {
    perror("sendto() failed");
    free(ack_packet);
    return FAILURE;
  }
  free(ack_packet);
  return SUCCESS;
}

/**
 * @brief Set timeout for the socket.
 * @param socket The socket to set timeout for.
 * @param time The timeout duration in seconds.
 * @return 1 on success, -1 if failed.
 */
static int set_socket_timeout(int socket, int time) {
  struct timeval timeout;
  timeout.tv_sec = time;
  timeout.tv_usec = 0;

  if (setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
    perror("Error setting timeout for socket");
    return FAILURE;
  }
  return SUCCESS;
}
