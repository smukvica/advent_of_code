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

int possible_directions(const char a_pipe, int a_dir_x, int a_dir_y){
    if(a_pipe == c_west_south)
        if(a_dir_x == -1 || a_dir_y == 1)
            return 1;
    if(a_pipe == c_west_east)// = '-';
        if(a_dir_x == 1 || a_dir_x == -1)
            return 1;
    if(a_pipe == c_west_north)
        if(a_dir_x == -1 || a_dir_y == -1)
            return 1;
    if(a_pipe == c_north_south)
        if(a_dir_y == 1 || a_dir_y == -1)
            return 1;
    if(a_pipe == c_east_south)
        if(a_dir_x == 1 || a_dir_y == 1)
            return 1;
    if(a_pipe == c_east_north)
        if(a_dir_x == 1 || a_dir_y == -1)
            return 1;
    return 0;
}

void flood_fill(char *a_expanded_map, int a_start_x, int a_start_y){
    
    if(a_expanded_map[a_start_y * c_width * 3 + a_start_x] != '.')
        return;
    a_expanded_map[a_start_y * c_width * 3 + a_start_x] = 'O';
    for(int i = 0; i < 4; i++){
        int dir_x = surroundings_x[i];
        int dir_y = surroundings_y[i];
        if(a_start_x + dir_x < 0 || a_start_x + dir_x >= c_height * 3)
            continue;
        if(a_start_y + dir_y < 0 || a_start_y + dir_y >= c_width * 3)
            continue;
        if(a_expanded_map[(a_start_y + dir_y) * c_width * 3 + a_start_x + dir_x] != '.')
            continue;
        flood_fill(a_expanded_map, a_start_x + dir_x, a_start_y + dir_y);
    }
}

int expand_map(char *a_map, int *a_distances, char *a_expanded_map){
    // remove pipes not in loop
    for(int i = 0; i < c_width * c_height; i++){
        if(a_distances[i] < 0)
            a_map[i] = '.';
    }

    // expand the map
    for(int i = 0; i < c_height * 3; i++){
        for(int j = 0; j < c_width * 3; j++){
            if(i % 3 == 0 && j % 3 == 0)
                a_expanded_map[i * c_height * 3 + j] = a_map[i / 3 * c_height + j / 3];
            else
                a_expanded_map[i * c_height * 3 + j] = '.';
        }
    }

    // fill expanded map with pipes - #
    for(int i = 0; i < c_height * 3; i++){
        for(int j = 0; j < c_width * 3; j++){
            if(a_expanded_map[i * c_width * 3 + j] == '.')
                continue;
            if(a_expanded_map[i * c_width * 3 + j] == 'S')
                for(int k = 0; k < 4; k++){
                    if(a_expanded_map[(i+surroundings_y[k]*3) * c_width * 3 + j + surroundings_x[k]*3] != '.'){
                        a_expanded_map[(i + surroundings_y[k]) * (c_width * 3) + (surroundings_x[k] + j)] = '#';
                    }
                }
            for(int k = 0; k < 4; k++){
                if(possible_directions(a_expanded_map[i * c_width * 3 + j], 
                                surroundings_x[k], 
                                surroundings_y[k])){
                    a_expanded_map[(i + surroundings_y[k]) * (c_width * 3) + (surroundings_x[k] + j)] = '#';
                }
            }
        }
    }

    flood_fill(a_expanded_map, 0, 0);
    int inside = 0;
    for(int i = 1; i < c_height * 3; i += 3){
        for(int j = 1; j < c_width * 3; j += 3){
            if(a_expanded_map[i * c_width * 3 + j] == '.'){
                int c = 0;
                for(int x = -1; x <= 1; x++){
                    for(int y = -1; y <= 1; y++){
                        if(a_expanded_map[(i+y) * c_width * 3 + (j+x)] == '.')
                            c++;
                    }
                }
                if(c == 9)
                    inside++;
            }
        }
    }
    return inside;
}

int main(){

    int distances[c_width * c_height];
    char map[c_width * c_height];
    char expanded_map[c_width * 3 * c_height * 3];
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
        //printf("%5d ", distances[i]);
        //if((i+1) % c_width == 0)
        //    printf("\n");
    }

    int inside = expand_map(map, distances, expanded_map);
    
    for(int i = 0; i < c_width * c_height * 3 * 3; i++){
        printf("%c", expanded_map[i]);
        if((i+1) % (c_width * 3) == 0)
            printf("\n");
    }

    
    
    printf("farthest: %d\n", farthest);
    printf("inside: %d\n", inside);

    return 0;
}