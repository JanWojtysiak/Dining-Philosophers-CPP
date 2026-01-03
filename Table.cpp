#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
using namespace std;

class Table {
    private:
        mutex mtx;
        int max_philosophers;
        int philosophers_at_table;
        vector<unique_ptr<mutex>> forks;
        condition_variable cv;
        pair<int, int> get_fork_ids(int id) {
            int left = id;
            int right = ( id+1 )  %   max_philosophers;
            return {left, right};
        }
    public:
        Table(int max_philosophers) {
            this->max_philosophers = max_philosophers;
            this->philosophers_at_table = 0;
            for (int i = 0; i < max_philosophers; i++) {
                this->forks.push_back(make_unique<mutex>());
            }
        }
    void enter_table() {
        unique_lock<mutex> lock(mtx);
        while (philosophers_at_table >= max_philosophers - 1) {
            cv.wait(lock);
        }
        philosophers_at_table += 1;
    }
    void leave_table() {
        unique_lock<mutex> lock(mtx);
        philosophers_at_table -= 1;
        cv.notify_one();
    }
    void pick_forks(int phil_id) {
        auto [left_fork, right_fork] = get_fork_ids(phil_id);
        this->forks[left_fork]->lock();
        this->forks[right_fork]->lock();
    }
    void put_forks(int phil_id) {
        auto [left_fork, right_fork] = get_fork_ids(phil_id);
        this->forks[left_fork]->unlock();
        this->forks[right_fork]->unlock();
    }

};

int main() {
    
    return 0;
}