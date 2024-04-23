#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "RUDP_API.h"

/**
 * Main function for the RUDP Receiver program.
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return 0 on successful execution, -1 on failure.
 */
int main(int argc, char* argv[]) {
  // Parse the command line arguments
  if (argc != 3 || strcmp(argv[1], "-p") != 0) {
    printf("Usage: %s -p <port>\n", argv[0]);
    return -1;
  }
  
  // Extract port number from command line arguments
  int port = atoi(argv[2]);
  
  // Print receiver information
  printf("~~~~~~~~ RUDP Receiver ~~~~~~~~\n");
  
  // Create a RUDP socket
  int socket_fd = create_RUDP_socket();
  if (socket_fd == -1) {
    printf("Could not create socket\n");
    return -1;
  }
  printf("Socket created\n");

  // Attempt to establish connection
  int rval = listen_for_connection(socket_fd, port);
  if (rval == 0) {
    printf("Could not establish connection\n");
    return -1;
  }
  printf("Connection established\n");
  
  // Open a file for writing statistics
  FILE* file = fopen("stats", "w+");
  if (file == NULL) {
    printf("Error opening stats file!\n");
    return -1;
  }
  fprintf(file, "\n\n~~~~~~~~ Statistics ~~~~~~~~\n");
  
  // Variables for measuring time and speed
  double average_time = 0;
  double average_speed = 0;
  clock_t start, end;

  // Variables for receiving data
  char* data_received = NULL;
  int data_length = 0;
  char total_data[2097152] = {0};  // 2MB
  int run = 1;

  // Start receiving data
  start = clock();
  do {
    // Receive data from sender
    rval = receive_data(socket_fd, &data_received, &data_length);

    if (rval == -2) {  // Connection closed by sender
      break;
    }
    if (rval == -1) {
      printf("Error receiving data\n");
      return -1;
    }

    // If the received data is the first one, start the timer
    if (rval == 1 && start < end) {
      start = clock();
    }
    
    // If the received data is not the last one, add it to the total data
    if (rval == 1) {
      strcat(total_data, data_received);
    }
    
    // If the received data is the last one, take it and write the stats to the file
    if (rval == 2) {
      strcat(total_data, data_received);
      end = clock();
      double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
      average_time += time_taken;
      double speed = 2 / time_taken;
      average_speed += speed;
      fprintf(file, "Run #%d Data: Time=%f S ; Speed=%f MB/S\n", run,
              time_taken, speed);
      memset(total_data, 0, sizeof(total_data));
      run++;
    }

  } while (rval >= 0);  // Keep the loop until the connection is closed

  // Print connection status
  printf("Connection closed\n");
  
  // Add average time and speed to the file
  fprintf(file, "\n");
  fprintf(file, "Average time: %f S\n", average_time / (run - 1));
  fprintf(file, "Average speed: %f MB/S\n", average_speed / (run - 1));

  // End the file with a message
  fprintf(file, "\n\n-----------------------------\n");
  fprintf(file, "Thank you for using our RUDP service\n");
  
  // Rewind and print the file content
  rewind(file);
  char print_buffer[100];
  while (fgets(print_buffer, 100, file) != NULL) {
    printf("%s", print_buffer);
  }

  // Close and delete the file
  fclose(file);
  remove("stats");

  return 0;
}
