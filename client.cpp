#include "header.h"

#define PORT 9999

bool flag_recv=false;
vector<string> gl_vstr;

mutex mtx_recv, mtx_sto;
condition_variable cv_recv;

void finish(int sig)
{
  cout << "Finish!!!" << endl;
  exit(0);
}

int main()
{
  signal(SIGINT, finish);

  thread thr_recv(recv_mess, PORT);

  while (1)
  {
    string a;
    cout << "Typing something: " << endl;
    getline(cin,a);
    fflush(stdin);

    send_mess("127.0.0.1", 10000, a);

    unique_lock<mutex> lck(mtx_recv);
    cv_recv.wait(lck, []() { return flag_recv; });
    mtx_sto.lock();
    while (gl_vstr.size() != 0)
    {
      cout<<gl_vstr.back()<<endl;
      gl_vstr.pop_back();
    }
    mtx_sto.unlock();
  }
}
