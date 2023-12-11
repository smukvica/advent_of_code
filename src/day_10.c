#include <stdio.h>
#include <string.h>

const int c_width = 140;
const int c_height = 140;

const char c_west_south = '7';
const char c_west_east = '-';
const char c_west_north = 'J';
const char c_north_south = '|';
const char c_east_south = 'F';
const char c_east_north = 'L';
const char c_ground = '.';

int surroundings_x[4] = {-1, 1, 0, 0};
int surroundings_y[4] = {0, 0, -1, 1};

int get_index(const char *a_map, const char a_element){
    for(int i = 0; i < c_width * c_height; i++){
        if(a_map[i] == a_element)
            return i;
    }
    return -1;
}

int is_connected(const char *a_map, int a_start_x, int a_start_y, 
                 int a_dir_x, int a_dir_y){
    // check out of bounds
    if(a_start_x + a_dir_x < 0 || a_start_x + a_dir_x >= c_width) return 0;
    if(a_start_y + a_dir_y < 0 || a_start_y + a_dir_y >= c_height) return 0;

    char d = a_map[(a_start_y + a_dir_y) * c_width + (a_start_x + a_dir_x)];
    if(a_dir_x == 0){   // up down
        if(a_dir_y == -1)   // up
            if(d == c_north_south || d == c_east_south || d == c_west_south)
                return 1;
        if(a_dir_y == 1)    // down
            if(d == c_north_south || d == c_east_north || d == c_west_north)
                return 1;
    }
    if(a_dir_y == 0){
        if(a_dir_x == -1)    // right
            if(d == c_west_east || d == c_east_south || d == c_east_north)
                return 1;
        if(a_dir_x == 1)   // left
            if(d == c_west_east || d == c_west_south || d == c_west_north)
                return 1;
    }

    return 0;
}

void calculate_distances(const char *a_map, int *a_distances, 
                         int a_start_x, int a_start_y, int a_d){
    int distance = a_distances[a_start_y * c_width + a_start_x];
    if(distance != -1 && distance <= a_d)
        return;
    a_distances[(a_start_y) * c_width + (a_start_x)] = a_d;
    for(int i = 0; i < 4; i++){
        if(a_map[(a_start_y) * c_width + (a_start_x)] == 'S'){
            if(is_connected(a_map, 
                        a_start_x, 
                        a_start_y, 
                        surroundings_x[i], 
                        surroundings_y[i]))
                calculate_distances(a_map, 
                                a_distances, 
                                a_start_x + surroundings_x[i],
                                a_start_y + surroundings_y[i],
                                a_d + 1);
        }else{ 
            if(is_connected(a_map, 
                        a_start_x, 
                        a_start_y, 
                        surroundings_x[i], 
                        surroundings_y[i]) &&
               is_connected(a_map, 
                        a_start_x + surroundings_x[i], 
                        a_start_y + surroundings_y[i],
                        -surroundings_x[i], 
                        -surroundings_y[i])){
                calculate_distances(a_map, 
                                    a_distances, 
                                    a_start_x + surroundings_x[i],
                                    a_start_y + surroundings_y[i],
                                    a_d + 1);
            }
        }
    }
}

int main(){

    int distances[c_width * c_height];
    char map[c_width * c_height];
    for(int i = 0; i < c_width * c_height; i++){
        distances[i] = -1;
    }

    char *line;
    size_t size;
    int i = 0;

    while(getline(&line, &size, stdin) != -1){
        line[c_width] = '\0';
        strcpy(&map[i], line);
        i += c_width;
    }

    int start = get_index(map, 'S');

    calculate_distances(map, &distances[0], start % c_width, start / c_height, 0);

    int farthest = 0;
    for(int i = 0; i < c_width * c_height; i++){
        if(distances[i] > farthest)
            farthest = distances[i];
        printf("%5d ", distances[i]);
        if((i+1) % c_width == 0)
            printf("\n");
    }

    printf("farthest: %d\n", farthest);


    return 0;
}