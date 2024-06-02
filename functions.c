#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <ncurses.h>
#include "functions.h"

void initDrone(Drone **head) {
    *head = (Drone*)malloc(sizeof(Drone));
    (*head)->pos.x = FIELD_WIDTH / 2;
    (*head)->pos.y = FIELD_HEIGHT / 2;
    (*head)->next = NULL;
}

void initBaskets(Drone **head, int size) {
    Drone *current = *head;
    for (int i = 0; i < size; i++) {
        Drone *new_basket = (Drone*)malloc(sizeof(Drone));
        new_basket->pos.x = current->pos.x - 1;
        new_basket->pos.y = current->pos.y;
        new_basket->next = NULL;
        current->next = new_basket;
        current = new_basket;
    }
}

void init(Drone **head, int baskets_size) {
    initDrone(head);
    initBaskets(head, baskets_size);
}

void go(Drone *head, char direction) {
    int new_x = head->pos.x;
    int new_y = head->pos.y;

    if (direction == 'w') new_y--;
    else if (direction == 's') new_y++;
    else if (direction == 'a') new_x--;
    else if (direction == 'd') new_x++;

    
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    
    if (new_x < 0) new_x = max_x - 1;         
    else if (new_x >= max_x) new_x = 0;       
    if (new_y < 0) new_y = max_y - 1;         
    else if (new_y >= max_y) new_y = 0;       

    Drone *current = head;
    int temp_x, temp_y;
    while (current) {
        temp_x = current->pos.x;
        temp_y = current->pos.y;
        current->pos.x = new_x;
        current->pos.y = new_y;
        new_x = temp_x;
        new_y = temp_y;
        current = current->next;
    }
}

void drawField(Drone *head, Position pumpkins[], int pumpkins_count) {
    clear();
    for (int y = 0; y < FIELD_HEIGHT; y++) {
        for (int x = 0; x < FIELD_WIDTH; x++) {
            if (y == head->pos.y && x == head->pos.x) {
                mvprintw(y, x, "D");
            } else {
                int is_basket = 0;
                Drone *current = head->next;
                while (current) {
                    if (y == current->pos.y && x == current->pos.x) {
                        mvprintw(y, x, "B");
                        is_basket = 1;
                        break;
                    }
                    current = current->next;
                }
                if (!is_basket) {
                    int is_pumpkin = 0;
                    for (int i = 0; i < pumpkins_count; i++) {
                        if (y == pumpkins[i].y && x == pumpkins[i].x) {
                            mvprintw(y, x, "P");
                            is_pumpkin = 1;
                            break;
                        }
                    }
                    if (!is_pumpkin) {
                        mvprintw(y, x, ".");
                    }
                }
            }
        }
    }
    refresh();
}

void generatePumpkins(Position pumpkins[], int *count) {
    srand(time(NULL));
    for (int i = 0; i < *count; i++) {
        pumpkins[i].x = rand() % FIELD_WIDTH;
        pumpkins[i].y = rand() % FIELD_HEIGHT;
    }
}

void collectPumpkin(Drone *head, Position pumpkins[], int *count) {
    for (int i = 0; i < *count; i++) {
        if (head->pos.x == pumpkins[i].x && head->pos.y == pumpkins[i].y) {
            for (int j = i; j < *count - 1; j++) {
                pumpkins[j] = pumpkins[j + 1];
            }
            (*count)--;
            Drone *new_basket = (Drone*)malloc(sizeof(Drone));
            new_basket->pos.x = head->pos.x;
            new_basket->pos.y = head->pos.y;
            new_basket->next = head->next;
            head->next = new_basket;
            break;
        }
    }
}

void printLevel(Drone *head) {
    int count = 0;
    Drone *current = head->next; 
    while (current) {
        count++;
        current = current->next;
    }
    mvprintw(0, 0, "Harvest Level: %d", count);
}

int isCrush(Drone *head) {
    Drone *current = head->next; 
    while (current) {
        if (head->pos.x == current->pos.x && head->pos.y == current->pos.y) {
            return 1; 
        }
        current = current->next;
    }
    return 0; 
}

void printExit(Drone *head) {
    int count = 0;
    Drone *current = head->next; 
    while (current) {
        count++;
        current = current->next;
    }
    mvprintw(FIELD_HEIGHT / 2, FIELD_WIDTH / 2, "Game Over! Final Harvest Level: %d", count);
}

void autoChangeDirection(Drone *head, Position pumpkins[], int pumpkins_count) {
    int min_distance = FIELD_WIDTH + FIELD_HEIGHT; 
    char new_direction = 'd'; 

    for (int i = 0; i < pumpkins_count; i++) {
        int distance = abs(pumpkins[i].x - head->pos.x) + abs(pumpkins[i].y - head->pos.y);
        if (distance < min_distance) {
            min_distance = distance;
            if (pumpkins[i].x > head->pos.x) new_direction = 'd';
            else if (pumpkins[i].x < head->pos.x) new_direction = 'a';
            else if (pumpkins[i].y > head->pos.y) new_direction = 's';
            else if (pumpkins[i].y < head->pos.y) new_direction = 'w';
        }
    }

    go(head, new_direction);
}
