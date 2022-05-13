// To have just one thread handle the signal:
// 1. Block the signal in the """parent""" thread. All new thread will inherit this setting,
// so no thread will handle it with the default handler.
// 2. Explicitly wait for the signal in one thread with sigwait. Handle it and inform others.
inline void BlockSIGINT() {
  sigset_t set;
  sigemptyset(&set);
  sigaddset(&set, SIGINT);
  pthread_sigmask(SIG_BLOCK, &set, NULL);
}

inline void HandleSignal(std::atomic_int& should_stop) {
  sigset_t sigset;
  sigemptyset(&sigset);
  sigaddset(&sigset, SIGINT);
  int sig;

  for (;;) {
    if (sigwait(&sigset, &sig) != 0) {
      exit(0);
    }
    if (sig == SIGINT) {
      should_stop = 1;
      break;
    }
  }
}
/**************************************************************************/
// Another way for the same purpose, set a specific signal handler.
// The bad part is we don't know which thread will run this action.
void SIGINTHandler(int sig) {
  should_stop = 1;
}

inline void SetSignalAction(int signal, void(*funcp)(int)) {
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = funcp;
  if (sigaction(signal, &sa, NULL)) {
    exit(-1);
  }
}
/**************************************************************************/
