#include <fmt/core.h>

#include <condition_variable>
#include <mutex>
#include <thread>
#include <utility>

std::mutex mtx;
std::condition_variable cv;
int num = 0;
int max = 10;

/*
 * std::condition_variable 的wait函数原型:

 * template< class Predicate >
 * void wait( std::unique_lock<std::mutex>& lock, Predicate pred );
 * 会先判断pred的值, 如果为false才会进入wait, 如果为true直接执行不会wait
 *
 * cv.wait(lock, pred) 等价于:
 *    while (!pred()) {
 *     wait(lock);
 *    }
 */

size_t tid()
{
  // thread_local
  // 关键字修饰的变量具有线程（thread）周期，这些变量在线程开始的时候被生成，在线程结束的时候被销毁，并且每一个线程都拥有一个独立的变量实例
  static thread_local size_t tid = std::hash<std::thread::id>{}(std::this_thread::get_id()) % 0xFFFF;
  return tid;
}

void printNum(int flag)
{
  while (true)
  {
    std::unique_lock<std::mutex> locker(mtx);
    cv.wait(locker, [=] { return num % 2 == flag; });  // 会先判断pred的值, 如果为false才会进入wait, 如果为true直接执行不会wait
    fmt::println("thread[{:#06x}] flag = {}: {}", tid(), flag, num++);
    locker.unlock();
    cv.notify_one();
    if (num >= max)
    {
      break;
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
