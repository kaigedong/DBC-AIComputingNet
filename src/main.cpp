#include "log/log.h"
#include "server/server.h"
#include <chrono>
#include <csignal>
#include <functional>

Server g_server;

// // 当接受到信号时调用退出
// void signal_handler(int sig) { g_server.Exit(); }

// // NOTE: sigaction: 查询或设置信号处理方式,释放资源
// // NOTE: 注册一个信号处理
// void register_signal_function() {
//   struct sigaction sa;
//   sa.sa_handler = signal_handler;
//   sigemptyset(&sa.sa_mask);
//   sa.sa_flags = 0;

//   // sigaction: 获取或者设置sig action
//   sigaction(SIGUSR1, &sa, nullptr);
//   sigaction(SIGUSR2, &sa, nullptr);
//   sigaction(SIGINT, &sa, nullptr);
//   sigaction(SIGTERM, &sa, nullptr);
// }

// argc 为argument count; 参数个数
// argv[0] 是程序的名称，并且包含了路径
int main(int argc, char *argv[]) {
  // register_signal_function();

  // 根据当前时间设置随机种子，随后调用rand产生随机数
  // srand((int)time(0));

  // FIXME: libvirt: XML-RPC error :
  // unable to connect to server at 'localhost:16509': Connection refused
  int result = g_server.Init(argc, argv);

  // REVIEW: 如果初始化出错，则退出程序，释放资源
  // if (ERR_SUCCESS != result) {
  //   std::cout << "server init failed: " << result;
  //   g_server.Exit();
  //   return 0;
  // }

  // 运行之后，
  // 1. 根据外部请求驱动程序进行;
  // 2. 根据内部定时器（如检测机器是否被租用的逻辑）驱动进行
  // 3. p2p 根据--驱动，与其他程序通讯
  // std::cout << "In (main func) will run until end" << std::endl;
  g_server.Idle();
  return 0;
}
