#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define FIELD_WIDTH 30
#define FIELD_HEIGHT 20
#define SPEED 200000

typedef struct {
    int x;
    int y;
} Position;

typedef struct drone {
    Position pos;
    struct drone *next;
} Drone;

void initDrone(Drone **head);
void initBaskets(Drone **head, int size);
void init(Drone **head, int baskets_size);
void go(Drone *head, char direction);
void drawField(Drone *head, Position pumpkins[], int pumpkins_count);
void generatePumpkins(Position pumpkins[], int *count);
void collectPumpkin(Drone *head, Position pumpkins[], int *count);
void printLevel(Drone *head);
int isCrush(Drone *head);
void printExit(Drone *head);
void autoChangeDirection(Drone *head, Position pumpkins[], int pumpkins_count);

#endif 
