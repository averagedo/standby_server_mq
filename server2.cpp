#include "header.h"
#include <sys/msg.h>
#include <sys/ipc.h>

#define KEY 60
#define PERMS 0644

bool flag_recv;
vector<string> gl_vstr;

mutex mtx_recv, mtx_sto;
condition_variable cv_recv;

struct msg
{
  long msg_type;
  char msg_text[100];
};

int msqid;

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

int main()
{
  if ((msqid = msgget(KEY, PERMS | IPC_CREAT)) == -1)
  {
    perror("msgget");
    exit(1);
  }

  signal(SIGINT, finish);

  while (1)
  {
    msg buf;
    if (msgrcv(msqid, &buf, sizeof(buf.msg_text), 0, 0) == -1)
    {
      perror("msgrcv");
      exit(1);
    }
    cout<< buf.msg_text<<endl;
  }
}