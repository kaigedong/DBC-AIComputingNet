#include "log/log.h"
#include "server/server.h"
#include <chrono>
#include <csignal>
#include <functional>

Server g_server;

void signal_handler(int sig) { g_server.Exit(); }

void register_signal_function() {
	struct sigaction sa;
	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	sigaction(SIGUSR1, &sa, nullptr);
	sigaction(SIGUSR2, &sa, nullptr);
	sigaction(SIGINT, &sa, nullptr);
	sigaction(SIGTERM, &sa, nullptr);
}

int main(int argc, char *argv[]) {
  register_signal_function();

  srand((int)time(0));
  std::cout << "1.2. ######## argc, argv:" << argc << argv << std::endl;

  // FIXME: libvirt: XML-RPC error :
  // unable to connect to server at 'localhost:16509': Connection refused
  int result = g_server.Init(argc, argv);

  std::cout << "2. ######## argc, argv:" << argc << argv << std::endl;

  if (ERR_SUCCESS != result) {
    std::cout << "server init failed: " << result;
    g_server.Exit();
    return 0;
  }

  // 运行之后，
  // 1. 根据外部请求驱动程序进行;
  // 2. 根据内部定时器（如检测机器是否被租用的逻辑）驱动进行
  // 3. p2p 根据--驱动，与其他程序通讯
  g_server.Idle();
  return 0;
}
