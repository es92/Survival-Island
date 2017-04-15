
#include <mutex>
#include <condition_variable>
#include <deque>
#include <vector>

using std::vector;
using std::pair;

template <typename T>
class Blocking_Queue {
  public:
    void push(T const& value) {
      {
        std::unique_lock<std::mutex> lock(this->d_mutex);
        d_queue.push_front(value);
      }
      this->d_condition.notify_one();
    }

    T pop() {
      std::unique_lock<std::mutex> lock(this->d_mutex);
      this->d_condition.wait(lock, [=]{ return !this->d_queue.empty(); });
      T rc(std::move(this->d_queue.back()));
      this->d_queue.pop_back();
      return rc;
    }


    pair<bool, T> pop_nowait(){
      bool locked = d_mutex.try_lock();

      T rc;

      if (locked && !this->d_queue.empty()){
        rc(std::move(this->d_queue.back()));
        this->d_queue.pop_back();
        d_mutex.unlock();
        return { true, rc };
      }
      else {
        return { false, rc };
      }

    }

    vector<T> multi_pop_nowait(){
      bool locked = d_mutex.try_lock();

      T rc;

      vector<T> v;

      if (locked){
        while (!this->d_queue.empty()){
          rc = this->d_queue.back();
          this->d_queue.pop_back();
          v.push_back(rc);
        }
        d_mutex.unlock();
      }
      return v;

    }

  private:
    std::mutex d_mutex;
    std::condition_variable d_condition;
    std::deque<T> d_queue;
};

