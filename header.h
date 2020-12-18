#include <iostream>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <thread>
#include<mutex>
#include<condition_variable>
#include<vector>
#include<signal.h>
#include<errno.h>
#include<string.h>
using namespace std;

#define LEN_BUFF 10000


extern vector<string> gl_vstr;

extern mutex mtx_recv, mtx_sto;
extern condition_variable cv_recv;

extern bool flag_recv;

void send_mess(std::string ip, int port, std::string mess);

void recv_mess(int port);