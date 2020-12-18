#include"header.h"


void recv_mess(int port)
{
  int server_fd, new_socket;
  sockaddr_in address;
  int addrlen = sizeof(address);
  char buffer[LEN_BUFF] = {0};

  // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
    std::cout << "Socket creation error\n";
    //exit(1);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  if (bind(server_fd, (sockaddr *)&address, sizeof(address)) < 0)
  {
    std::cout << "Bind failed\n";
    //exit(1);
  }

  if (listen(server_fd, 100) < 0)
  {
    std::cout << "Listen ERROR\n";
  }

  int i = 0;
  while (1)
  {
    if ((new_socket = accept(server_fd, (sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
      std::cout << "ERROR accept\n";
    }

    int recv_len;
    memset(buffer, 0, LEN_BUFF);
    if ((recv_len = recv(new_socket, buffer, LEN_BUFF, 0)) < 0)
    {
      std::cout << "ERROR recv\n";
      //exit(1);
    }

    std::string str = buffer;
    mtx_sto.lock();
    gl_vstr.push_back(str);
    mtx_sto.unlock();
    flag_recv=true;
    cv_recv.notify_one();
  }
}

void send_mess(std::string ip, int port, std::string mess)
{
  int sock;
  sockaddr_in serv_addr;
  char buffer[LEN_BUFF] = {0};
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    std::cout << "Socket creation error\n";
    //exit(1);
  }

  int enable = 1;
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    cout << "setsockopt(SO_REUSEADDR) failed" << endl;
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) < 0)
    cout << "setsockopt(SO_REUSEPORT) failed" << endl;

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);

  if (inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr) <= 0)
  {
    std::cout << "Invalid address\n";
    fprintf(stderr, "%m\n");
    //exit(1);
  }

  if (connect(sock, (sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
    std::cout << "Connection Failed\n";
    fprintf(stderr, "%m\n");
    //exit(1);
  }

  if (send(sock, mess.c_str(), mess.length(), 0) < 0)
  {
    std::cout << "ERROR send\n";
    //exit(1);
  }

  close(sock);
}