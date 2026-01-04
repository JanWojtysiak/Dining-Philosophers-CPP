#ifndef TABLE_HPP
#define TABLE_HPP

#include <vector>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <utility>
#include <iostream>
#include <string>

class Table {
    private:
        std::mutex mtx;
        int max_philosophers;
        int philosophers_at_table;
        std::vector<std::unique_ptr<std::mutex>> forks;
        std::condition_variable cv;
        std::pair<int, int> get_fork_ids(int id);
        std::vector<std::string> philosopher_states;

    public:
        Table(int max_philosophers);
        void enter_table();
        void leave_table();
        void pick_forks(int phil_id);
        void put_forks(int phil_id);
        void set_state(int phil_id, std::string s);
        std::string get_state (int phil_id);
};
#endif