#include "Table.hpp"

std::pair<int, int> Table::get_fork_ids(int id) {
    int left = id;
    int right = (id + 1) % max_philosophers;
    return {left, right};
}

Table::Table(int max_philosophers) {
    this->max_philosophers = max_philosophers;
    this->philosophers_at_table = 0;
    for (int i = 0; i < max_philosophers; i++) {
        this->forks.push_back(std::make_unique<std::mutex>());
        philosopher_states.push_back("MYSLI");
    }
}

void Table::enter_table() {
    std::unique_lock<std::mutex> lock(mtx);
    while (philosophers_at_table >= max_philosophers - 1) {
        cv.wait(lock);
    }
    philosophers_at_table += 1;
}

void Table::leave_table() {
    std::unique_lock<std::mutex> lock(mtx);
    philosophers_at_table -= 1;
    cv.notify_one();
}

void Table::pick_forks(int phil_id) {
    auto [left_fork, right_fork] = get_fork_ids(phil_id);
    this->forks[left_fork]->lock();
    this->forks[right_fork]->lock();
}

void Table::put_forks(int phil_id) {
    auto [left_fork, right_fork] = get_fork_ids(phil_id);
    this->forks[left_fork]->unlock();
    this->forks[right_fork]->unlock();
}
void Table::set_state(int phil_id, std::string s) {
    std::lock_guard<std::mutex> lock(mtx);
    philosopher_states[phil_id] = s; 
}
std::string Table::get_state(int phil_id) {
    return philosopher_states[phil_id];
}