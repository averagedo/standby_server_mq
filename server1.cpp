#include <header.h>
#include <sys/msg.h>
#include <sys/ipc.h>

#define PORT 10000
#define KEY 50
#define PERMS 0644

struct msg
{
  long msg_type;
  char msg_text[100];
};

int msqid;
bool flag_recv;
vector<string> gl_vstr;

mutex mtx_recv, mtx_sto;
condition_variable cv_recv;

void finish(int sig)
{
  if (msgctl(msqid, IPC_RMID, NULL) == -1)
  {
    perror("msgctl");
    exit(1);
  }
  cout << "Finish!!!" << endl;
  exit(0);
}

string uperstring(string str)
{
  for (int i = 0; i < str.length(); i++)
  {
    str[i] = toupper(str[i]);
  }
  return str;
}

int main()
{

  vector<string> v_str;

  signal(SIGINT, finish);

  if ((msqid = msgget(KEY, PERMS | IPC_CREAT)) == -1)
  {
    perror("msgget");
    exit(1);
  }

  thread thr_recv(recv_mess, PORT);

  while (1)
  {
    msg buf;
    buf.msg_type = 1;

    unique_lock<mutex> lck(mtx_recv);
    cv_recv.wait(lck, []() { return flag_recv; });
    mtx_sto.lock();
    while (gl_vstr.size() != 0)
    {
      v_str.push_back(gl_vstr.back());
      gl_vstr.pop_back();
    }
    mtx_sto.unlock();

    while(v_str.size()!=0)
    {
      string a = v_str.back();
      cout << a << endl;
      v_str.pop_back();
      a = uperstring(a);
      send_mess("127.0.0.1", 9999, a);
      a.push_back('\0');
      memcpy(buf.msg_text, a.c_str(), a.length());
      if (msgsnd(msqid, &buf, a.length(), 0) == -1)
        perror("msgsnd");
    }
  }
  return 0;
}