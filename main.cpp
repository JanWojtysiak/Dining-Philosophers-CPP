#include <ncurses.h> 
#include <thread>
#include <vector>
#include <chrono>
#include <string>
#include "Table.hpp"

void philosopher_life(int id, Table& table) {
    for(int i = 0; i < 3; ++i) { 
        table.set_state(id, "MYSLI");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        table.set_state(id, "CZEKA");
        table.enter_table();

        table.pick_forks(id);

        table.set_state(id, "JE");
        std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // Skróciłem czas dla testu

        table.put_forks(id);
        table.leave_table();
    }
    table.set_state(id, "KONIEC");
}

int main() {
    initscr();           
    noecho();             
    curs_set(0);         
    start_color();        
    init_pair(1, COLOR_GREEN, COLOR_BLACK);  
    init_pair(2, COLOR_RED, COLOR_BLACK);     
    init_pair(3, COLOR_CYAN, COLOR_BLACK);    
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK); 

    int n = 5;
    Table table(n);
    std::vector<std::thread> philosophers;

    for (int i = 0; i < n; ++i) {
        philosophers.emplace_back(philosopher_life, i, std::ref(table));
    }

    while(true) {
        erase();
        
        mvprintw(0, 0, "=== PROBLEM UCZTUJACYCH FILOZOFOW ===");
        mvprintw(1, 0, "Nacisnij Ctrl+C aby przerwac silowo");

        int finished_count = 0;

        for(int i = 0; i < n; i++) {
            std::string state = table.get_state(i);
            int color_pair = 0;
            if (state == "JE") color_pair = 1;
            else if (state == "CZEKA") color_pair = 2;
            else if (state == "MYSLI") color_pair = 3;
            else if (state == "KONIEC") {
                color_pair = 4;
                finished_count++;
            }
            if(color_pair > 0) attron(COLOR_PAIR(color_pair));
            mvprintw(3 + i, 0, "Filozof %d: %s", i, state.c_str());
            if(color_pair > 0) attroff(COLOR_PAIR(color_pair));
        }

        refresh();

        if (finished_count == n) {
            mvprintw(3 + n + 1, 0, "Wszyscy skonczyli! Zamykanie za 2 sekundy...");
            refresh();
            std::this_thread::sleep_for(std::chrono::seconds(2));
            break; 
        }
    }

    for (auto& t : philosophers) {
        if(t.joinable()) t.join();
    }

    endwin();
    return 0;
}