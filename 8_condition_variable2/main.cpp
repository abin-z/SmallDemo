#include <thread>
#include <mutex>
#include <condition_variable>
#include <utility>
#include <fmt/core.h>

std::mutex mtx;
std::condition_variable cv;
int num = 0;
int max = 10;

size_t tid()
{
  // thread_local 关键字修饰的变量具有线程（thread）周期，这些变量在线程开始的时候被生成，在线程结束的时候被销毁，并且每一个线程都拥有一个独立的变量实例
  static thread_local size_t tid = std::hash<std::thread::id>{}(std::this_thread::get_id()) % 0xFFFF;
  return tid;
}

void printNum(int flag)
{
  while (true)
  {
    std::unique_lock<std::mutex> locker(mtx);
    cv.wait(locker, [=]
            { return num % 2 == flag; });
    locker.unlock();
    cv.notify_one();
    if (num >= max)
    {
      break;
    fmt::println("thread[{:#06x}]: {}", tid(), num++);
    }
  }
}

int main()
{
  std::thread t1(printNum, 0);
  std::thread t2(printNum, 1);

  t1.join();
  t2.join();
  fmt::println("All threads finished.");
  return 0;
}
