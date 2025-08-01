#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//info about doors
struct door {
    int door_id;
    int room1_id;
    int room2_id;
    int locked;
};

//info about keys
struct key {
    int key_id;
    int location;
    int door_id;
};

//info about creature
struct creature {
    int creature_id;
    int location;
    int starting_location;
    int type;
    int is_following;
};

//game conditions
struct game_info {
    struct creature * pc;
    int carrying_capacity;
    int leadership;
    int caught_count;
    int exit_door;
};

//4 global variables
struct door * doors;
struct key * keys;
struct creature * creatures;
struct game_info *game;

//functions
void initialize(int d, int k, int c);
void gameloop(int d, int k, int c);
void look(int d, int k, int c);
void pickup(int pickup_id, int k);
void drop(int drop_id, int k);
void unlock(int unlock_id, int d, int k);
void inv(int k);
void pass(int pass_id, int d, int c);
void check_room(int c);
void lead(int lead_id, int c);
void abandon(int abandon_id, int c);

int main() {
//input
    int r,d,k,c;
    scanf("%d %d %d %d",&r,&d,&k,&c);
//memory allocation
    doors = malloc (d * sizeof(struct door));
    keys = malloc (k * sizeof(struct key));
    creatures = malloc (c * sizeof(struct creature));
    game = malloc (sizeof(struct game_info));
//initialize and start game
    initialize(d,k,c);
    gameloop(d,k,c);
//free heap
    free(doors);
    doors=NULL;
    free(keys);
    keys = NULL;
    free(creatures);
    creatures = NULL;
    free(game);
    game = NULL;
    return 0;
}
//input and assign values to struct
void initialize(int d, int k, int c) {
    for (int i = 0; i < d; i++) {
        int room1_id,room2_id,locked;

        scanf("%d %d %d",&room1_id,&room2_id,&locked);
        doors[i].door_id = i;
        doors[i].room1_id = room1_id;
        doors[i].room2_id = room2_id;
        doors[i].locked = locked;
    }
    for (int i = 0; i < k; i++) {
        int location,door_id;
        scanf("%d %d",&location,&door_id);

        keys[i].key_id = i;
        keys[i].location = location;
        keys[i].door_id = door_id;
    }
//pointer to pc
    struct creature * pc;

    for (int i = 0; i < c; i++) {
        int location, type;
        scanf("%d %d",&location,&type);

        creatures[i].creature_id = i;
        creatures[i].location = location;
        creatures[i].starting_location = location;
        creatures[i].type = type;
//check if creature is pc
        if (creatures[i].type == 0) {
            pc = &creatures[i];
            game->pc = pc;
        }
    }

    int carrying_capacity, leadership;
    scanf("%d %d",&carrying_capacity, &leadership);
//input pc's info
    game->carrying_capacity = carrying_capacity;
    game->leadership = leadership;
    game->caught_count = 0;

    int exit_door;
    scanf("%d",&exit_door);
    game->exit_door = exit_door;
}

void gameloop(int d, int k, int c) {
    char command[10]; //user input
    int arg;//argument
    for (;;) { //infinite loop
        printf("Enter command: ");
        scanf("%s", command);
        if (strcmp(command, "look") == 0) {
            look(d,k,c);
        } else if (strcmp(command, "pickup") == 0) {
            scanf("%d", &arg);
            pickup(arg, k);
        } else if (strcmp(command, "drop") == 0) {
            scanf("%d", &arg);
            drop(arg, k);
        } else if (strcmp(command, "inv") == 0) {
            inv(k);
        } else if (strcmp(command, "unlock") == 0) {
            scanf("%d", &arg);
            unlock(arg, d, k);
        } else if (strcmp(command, "pass") == 0) {
            scanf("%d", &arg);
            pass(arg, d ,c);
        } else if (strcmp(command, "lead") == 0) {
            scanf("%d", &arg);
            lead(arg, c);
        } else if (strcmp(command, "abandon") == 0) {
            scanf("%d", &arg);
            abandon(arg, c);
        } else if (strcmp(command, "quit") == 0) {
            printf("Quitting the game.\n");
            break;//exit loop
        } else {
            printf("Unknown command:\n");
        }
    }
}

void look(int d, int k, int c) {
    // print current room
    printf("You are in room %d.\n", game->pc->location);

    // info about creatures in the room
    int creature_count = 0;
    for (int i = 0; i < c; i++) {
        if (creatures[i].location == game->pc->location) {
            if (creatures[i].type == 1) {
                printf("Escapee %d is in this room.\n", creatures[i].creature_id);
            } else if (creatures[i].type == 2) {
                printf("Guard %d is in this room.\n", creatures[i].creature_id);
            }
            creature_count++;
        }
    }
    if (creature_count == 0) {
        printf("No creatures except you in this room.\n");
    }

    // info abour  keys in the room
    int key_count = 0;
    for (int i = 0; i < k; i++) {
        if (keys[i].location == game->pc->location) {
            printf("Key %d (unlocks door %d) is in this room.\n", keys[i].key_id, keys[i].door_id);
            key_count++;
        }
    }
    if (key_count == 0) {
        printf("No keys in this room.\n");
    }

    //info about doors in the room
    int door_count = 0;
    for (int i = 0; i < d; i++) {
        if (doors[i].room1_id == game->pc->location || doors[i].room2_id == game->pc->location) {
            //check where the door takes
            int next_room;
            if (doors[i].room1_id == game->pc->location) {
                next_room = doors[i].room2_id;
            } else {
                next_room = doors[i].room1_id;
            }
            //check if door is locked
            if (doors[i].locked == 0) {
                printf("Locked door %d leads to room %d.\n", doors[i].door_id, next_room);
                if (doors[i].door_id == game->exit_door) {
                    printf("Door %d leads to exit.\n", doors[i].door_id);
                }
            } else if (doors[i].locked == 1) {
                printf("Unlocked door %d leads to room %d.\n", doors[i].door_id, next_room);
                if (doors[i].door_id == game->exit_door) {
                    printf("Door %d leads to exit.\n", doors[i].door_id);
                }
            }
            door_count++;
        }
    }
    if (door_count == 0) {
        printf("No doors in this room.\n");
    }
}

void pickup(int pickup_id, int k) {
    if (game->carrying_capacity <= 0) {//check if inventory full
        printf("Inventory full.\n");
        return;
    }
//search for keys in room
    for (int i = 0; i < k; i++) {
        if (keys[i].location == game->pc->location && keys[i].key_id == pickup_id) {
            keys[i].location = -1;// key in inventory
            game->carrying_capacity--;
            printf("Key %d picked up from room %d.\n", keys[i].key_id, game->pc->location);
            return;
        }
    }
    printf("Cannot pickup key %d.\n", pickup_id);
}

void drop(int drop_id, int k) {
    for (int i = 0; i < k; i++) {
//search for key in inventory
        if (keys[i].key_id == drop_id && keys[i].location == -1) {
            keys[i].location = game->pc->location;//drop key
            game->carrying_capacity++;
            printf("Key %d dropped in room %d.\n", keys[i].key_id, game->pc->location);
            return;
        }
    }
    printf("Cannot drop key %d.\n", drop_id);
}

void unlock(int unlock_id, int d, int k) {
    for (int i = 0; i < d; i++) {//search for door in room
        if ((doors[i].room1_id == game->pc->location || doors[i].room2_id == game->pc->location) && doors[i].door_id == unlock_id) {//check if player is in either room connected by a door and unlock id
            for (int j = 0; j < k; j++) {
//check if key is in inventory and unlock if so
                if (keys[j].door_id == unlock_id && keys[j].location == -1) {
                    doors[i].locked = 1;
                    printf("Door %d unlocked.\n", unlock_id);
                    return;
                }
            }
            printf("You do not have the key to unlock door %d.\n", unlock_id);
            return;
        }
    }
    printf("Door %d is not in this room.\n", unlock_id);
}

void inv(int k) {
    for (int i = 0; i < k; i++) {
        if (keys[i].location == -1) {//check inventory '-1'
            printf("You have Key %d that unlocks Door %d.\n", keys[i].key_id, keys[i].door_id);
        } else {
            printf("No keys in inventory.\n");
        }
    }
    printf("Your carrying capacity is %d.\n", game->carrying_capacity);
}

void check_room(int c) {
    int escapee_count = 0;
    int guard_count = 0;
//count escapees and guards in the current room
    for (int i = 0; i < c; i++) {
        if (creatures[i].location == game->pc->location) {
            if (creatures[i].type == 1) {
                escapee_count++;
            } else if (creatures[i].type == 2 && creatures[i].is_following == 0) {
                guard_count++;
            }
        }
    }
//if more guards than escapee in the room
    if (guard_count >= escapee_count + 1) {
        game->caught_count++;
        printf("You have been caught by the guards!\n");
        printf("You were caught %d times.\n", game->caught_count);
        printf("You have %d more attempts left.\n", 5 - game->caught_count);
        game->pc->location = game->pc->starting_location;
        printf("You were sent back to your starting room %d.\n",game->pc->starting_location);

        for (int i = 0; i < c; i++) {
            creatures[i].location = creatures[i].starting_location;
            creatures[i].is_following = 0;//creatures dont follow pc anymore
        }
        printf("All creatures are back in their starting room.\n");
//lose condition
        if (game->caught_count >= 5) {
            printf("You have been caught 5 times. Game over!\n");
            exit(0);
        }
//if pc+escapee is more than guards
    } else if (escapee_count + 1 > guard_count) {
        for (int i = 0; i < c; i++) {
            if (creatures[i].type == 2 && creatures[i].location == game->pc->location) {
                creatures[i].is_following = 1;//guards follow escapees until they find more guards
            }
        }
    }
}

void pass(int pass_id, int d, int c) {
    int next_room;//stores next room pc will move tp
    for (int i = 0; i < d; i++) {
        if (doors[i].door_id == pass_id &&  (doors[i].room1_id == game->pc->location || doors[i].room2_id == game->pc->location)) {
            if (doors[i].locked == 0) {
                printf("Door %d is locked.\n", pass_id);
                return;
            }
//next room based on door
            if (game->pc->location == doors[i].room1_id){
                next_room = doors[i].room2_id;
            } else {
                next_room = doors[i].room1_id;
            }

            for (int j =0; j < c; j++) {
//escapees follow pc
                if (creatures[j].is_following == 1 && creatures[j].location == game->pc->location && creatures[j].type == 1 ) {
                    creatures[j].location = next_room;
                    printf("Escapee %d followed you to Room %d.\n", creatures[j].creature_id, next_room);
//guards follow escapees until they find more guards
                } else if (creatures[j].type == 2 &&creatures[j].location == game->pc->location && creatures[j].is_following == 1) {
                    creatures[j].location = next_room;
                    printf("Guard %d followed you to Room %d.\n", creatures[j].creature_id, next_room);
                }
            }
//move pc to next room
            game->pc->location = next_room;
            printf("You moved to room %d.\n", next_room);
//win condition
            if (pass_id == game->exit_door) {
                printf("Congratulations! You have escaped the maze!\n");
                exit(0);
            }
//check next room for guards and escapees encounters
            check_room(c);
            return;
        }
    }
    printf("Door %d is not in this room.\n", pass_id);
}


void lead(int lead_id, int c) {
    if (game->leadership <= 0) {//check leadership level
        printf("You cannot have followers.\n");
        return;
    }
    for (int i = 0; i < c; i++) {
//check if escapee is in same room and id
        if (creatures[i].creature_id == lead_id && creatures[i].type == 1 && creatures[i].location == game->pc->location) {
            creatures[i].is_following = 1;//follow
            game->leadership--;
            printf("Escapee %d is following you.\n", creatures[i].creature_id);
            return;
        }
    }
    printf("Escapee %d cannot follow PC.\n", lead_id);
}

void abandon(int abandon_id, int c) {
//check if escapee in same room and id
    for (int i = 0; i < c; i++) {
        if (creatures[i].creature_id == abandon_id && creatures[i].type == 1 && creatures[i].location == game->pc->location) {
            creatures[i].is_following = 0;//unfollow
            game->leadership++;
            printf("Escapee %d is no longer following you.\n", abandon_id);
            return;
        }
    }
    printf("Unable to abandon Escapee %d .\n", abandon_id);
}
