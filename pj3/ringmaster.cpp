#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <stdlib.h>
#include <vector>

#include "socket_lib.h"

using namespace std;

int main(int argc, char *argv[])
{
  if (argc != 4) {
    cout << "Syntax: ringmaster <port_num> <num_players> <num_hops>" << endl;
    return 1;
  }

  if (argv_number_check(argv, 1, "port_num") || argv_number_check(argv, 2, "num_players") || argv_number_check(argv, 3, "num_hops")) {
    return 1;
  }
  
  const char *port = argv[1];
  const int num_players = atoi(argv[2]);
  const int num_hops = atoi(argv[3]);

  if (num_players < 1) {
    cout << "player number must be positive" << endl;
    exit(EXIT_FAILURE);
  }

  if (num_hops < 0) {
    cout << "hops number must be non-negative" << endl;
    exit(EXIT_FAILURE);
  }
  
  vector<int> player_socketfd;
  
  cout << "Potato Ringmaster" << endl;
  cout << "Players = " << num_players << endl;
  cout << "Hops = " << num_hops << endl;

  // build a server socket which is listening
  int socket_fd = build_server(port);

  // manage players connecting in, allocating their ID to them
  for (int i = 0; i < num_players; i++) {
    // accept connection with player
    int connection_socket_fd = server_accept(socket_fd);
    player_socketfd.push_back(connection_socket_fd);

    // send id, total players to connected player
    send(connection_socket_fd, &i, sizeof(i), 0);
    send(connection_socket_fd, &num_players, sizeof(num_players), 0);    

    cout << "Player " << i << " is ready to play" << endl;

    // receive and store address and port of this player
    char player_hostname[128];
    char player_port[5];
    //now only print these two for testing
    recv(connection_socket_fd, player_hostname, 128, 0);    
    recv(connection_socket_fd, player_port, 5, 0);

    cout << player_hostname << endl;
    cout << player_port << endl;
  }

  for (int fd: player_socketfd) {
    close(fd);
  }
  close(socket_fd);
  return 0;
}
