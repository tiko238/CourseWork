#include <ncurses.h>
#include <stdlib.h>
#include "functions.h"

int main() {
    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);

    Drone *head;
    int baskets_size = 3;
    init(&head, baskets_size);

    Position pumpkins[10];
    int pumpkins_count = 10;
    generatePumpkins(pumpkins, &pumpkins_count);

    char direction = 'd';

    while (1) {
        timeout(SPEED / 1000);
        int ch = getch();
        if (ch == 'q' || isCrush(head)) break;
        if (ch == 'w' || ch == 's' || ch == 'a' || ch == 'd') {
            direction = ch;
        }

        if (ch == ' ') { 
            autoChangeDirection(head, pumpkins, pumpkins_count);
        } else {
            go(head, direction);
        }
        collectPumpkin(head, pumpkins, &pumpkins_count);
        drawField(head, pumpkins, pumpkins_count);
        printLevel(head);
    }

    printExit(head);
    refresh();
    getch();

    endwin();

    
    while (head) {
        Drone *temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}
