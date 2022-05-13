/**
 * There needs to be "wait, wakeup, check if ok to run"(by checking f1_run...).
 * Only wait and notify the program may miss the notification.
 *
 * For example, (assume there are only 2 threads)
 *
void func1() {
  int counter = 0;
  std::unique_lock<std::mutex> lk(m);
  while (counter++ < 100) {
    cout << "1 ";
    cout.flush();
    cv1.notify_one();
    cv2.wait(lk);
  }
  exit(0);
}

void func2() {
  std::unique_lock<std::mutex> lk(m);
  while (true) {
    cv1.wait(lk);
    cout << "2 ";
    cout.flush();
    cv2.notify_one();
  }
}

int main() {
  thread t1(func1);
  std::this_thread::sleep_for(1000ms);
  thread t2(func2);

  t1.join();
  return 0;
}

 * For the above code, if func1 run first, then the signal will be lost
 * and func2 will never run. This is verified by the commented code below.
 * One way to fix this problem is make sure thread 2 run first(by a sleep).
 * With this fix, the program will work fine. This approach can also be
 * extended to the case with 3 robots.
 * Note that in the code above notification is done with the lock held.
 * This is not necessary, just a nature way to write the code above.
 *
 * Another point to note is that making sure threads start in a sequence
 * by sleep is not clean. The "wait, wakeup, check if ok to run" approach
 * should be favored.
 *
 *
 * For the current(not commented) approach,
 * another way is to let thread 1, 2, 3 wait at the
 * beginning, then notify thread 1 to start, then the wait_for can be replaced
 * by simply wait.
 */
#include <iostream>
#include <thread>
#include <condition_variable>
#include <chrono>

using namespace std;

mutex m;
condition_variable cv1, cv2, cv3;
int f1_run, f2_run, f3_run;

void func1() {
  int counter = 0;
  std::unique_lock<std::mutex> lk(m);
  while (counter++ < 100) {
    cv3.wait_for(lk, 100ms, []{return f3_run == 1;});
    f3_run--;
    cout << "1 ";
    f1_run++;
    cv1.notify_one();
  }
  exit(0);
}

void func2() {
  std::unique_lock<std::mutex> lk(m);
  while (true) {
    cv1.wait_for(lk, 100ms, []{return f1_run == 1;});
    f1_run--;
    cout << "2 ";
    f2_run++;
    cv2.notify_one();
  }
}

void func3() {
  std::unique_lock<std::mutex> lk(m);
  while (true) {
    cv2.wait_for(lk, 100ms, []{return f2_run == 1;});
    f2_run--;
    cout << "3 ";
    f3_run++;
    cv3.notify_one();
  }
}

int main() {
  f1_run = 0;
  f2_run = 0;
  f3_run = 1;

  thread t1(func1);
  thread t2(func2);
  thread t3(func3);

  t1.join();
  return 0;
}


//void func1() {
//  int counter = 0;
//  std::unique_lock<std::mutex> lk(m);
//  while (counter++ < 100) {
//    cout << "1 ";
//    cout.flush();
//    cv1.notify_one();
//    cv2.wait(lk);
//  }
//  exit(0);
//}
//
//void func2() {
//  std::unique_lock<std::mutex> lk(m);
//  while (true) {
//    cv1.wait(lk);
//    cout << "2 ";
//    cout.flush();
//    cv2.notify_one();
//  }
//}
//
//int main() {
//  thread t1(func1);
//  std::this_thread::sleep_for(1000ms);
//  thread t2(func2);
//
//  t1.join();
//  return 0;
//}
