#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "RUDP_API.h"

int main(int argc, char* argv[]) {
  // parse the command line arguments
  if (argc != 3 || strcmp(argv[1], "-p") != 0) {
    printf("Usage: %s -p <port>\n", argv[0]);
  }
  int port = atoi(argv[2]);
  printf("~~~~~~~~ RUDP Receiver ~~~~~~~~\n");
  int socket = RUDP_socket();
  if (socket == -1) {
    printf("Could not create socket\n");
    return -1;
  }
  printf("Socket created\n");

  int rval = RUDP_get_connection(socket, port);
  if (rval == 0) {
    printf("Could not get connection\n");
    return -1;
  }
  printf("Connection established\n");
  FILE* file = fopen("stats", "w+");
  if (file == NULL) {
    printf("Error opening  stats file!\n");
    return 1;
  }
  fprintf(file, "\n\n~~~~~~~~ Statistics ~~~~~~~~\n");
  double average_time = 0;
  double average_speed = 0;
  clock_t start, end;

  char* date_received = NULL;
  int data_length = 0;
  char total_date[2097152] = {0};  // 2MB
  rval = 0;
  int run = 1;

  start = clock();
  end = clock();

  do {
    rval = RUDP_receive(socket, &date_received, &data_length);

    if (rval == -2) {  // if the connection was closed by the sender
      break;
    }
    if (rval == -1) {
      printf("Error receiving data\n");
      return -1;
    }
    // printf("Data received: %s\n", date_received);

    // if the data received is the first one, start the timer
    if (rval == 1 && start < end) {
      start = clock();
    }
    // if the data received is not the last one, add it to the total data
    if (rval == 1) {
      strcat(total_date, date_received);
    }
    // if the data received is the last one, take it and write the stats it to
    // the file
    if (rval == 2) {
      strcat(total_date, date_received);
      printf("Received total date: %zu\n", sizeof(total_date));
      end = clock();
      double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
      average_time += time_taken;
      double speed = 2 / time_taken;
      average_speed += speed;
      fprintf(file, "Run #%d Data: Time=%f S ; Speed=%f MB/S\n", run,
              time_taken, speed);
      memset(total_date, 0, sizeof(total_date));
      run++;
    }

  } while (rval >= 0);  //   keep the loop until the connection is closed

  // check if the connection was closed by the sender and print stats
  printf("connection closed\n");
  // add the average time and speed to the file
  fprintf(file, "\n");
  fprintf(file, "Average time: %f S\n", average_time / (run - 1));
  fprintf(file, "Average speed: %f MB/S\n", average_speed / (run - 1));

  // end the file with nice message
  fprintf(file, "\n\n-----------------------------\n");
  fprintf(file, "Thank you for using our RUDP service\n");
  rewind(file);
  char print_buffer[100];
  while (fgets(print_buffer, 100, file) != NULL) {
    printf("%s", print_buffer);
  }

  // Close the file
  fclose(file);
  remove("stats");

  return 0;
}
