#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

const int c_width = 140;
const int c_height = 140;

const uint64_t c_expansion_value = 1000000;

typedef struct galaxy{
    int x;
    int y;
}galaxy;

uint64_t abs_m(uint64_t a, uint64_t b){
    if(a > b)
        return a - b;
    if(a < b)
        return b - a;
    return 0;
}

int find_galaxies(const char *a_line, int a_start){
    while(a_line[a_start] != '#' && a_start < c_width){
        a_start++;
    }

    if(a_start == c_width)
        return -1;

    return a_start;
}

void calculate_cumulative(uint64_t *a_expand){
    for(int i = 1; i < c_width; i++){
        a_expand[i] += a_expand[i - 1];
    }
}

uint64_t calculate_distances(const galaxy *a_galaxies, 
                             int a_num_of_galaxies, 
                             uint64_t *a_expand_x,
                             uint64_t *a_expand_y){
    uint64_t sum = 0;
    for(int i = 0; i < a_num_of_galaxies - 1; i++){
        for(int j = i + 1; j < a_num_of_galaxies; j++){
            sum += (uint64_t)abs(a_galaxies[j].x - a_galaxies[i].x) + 
                   abs_m(a_expand_x[a_galaxies[j].x], a_expand_x[a_galaxies[i].x]);
            sum += (uint64_t)abs(a_galaxies[j].y - a_galaxies[i].y) + 
                   abs_m(a_expand_y[a_galaxies[j].y], a_expand_y[a_galaxies[i].y]);
        }
    }

    return sum;
}

int main(){
    uint64_t expand_x[c_width];
    uint64_t expand_y[c_height];
    for(int i = 0; i < c_width; i++){
        expand_x[i] = c_expansion_value - 1;
    }
    for(int i = 0; i < c_height; i++){
        expand_y[i] = 0;
    }

    galaxy galaxies[c_width * c_height];

    char universe[c_width * c_height];

    unsigned int num_of_galaxies = 0;

    char *line;
    size_t size;
    int u_index = 0;
    int i = 0;
    int j = 0;
    int ret = -1;

    while(getline(&line, &size, stdin) != -1){
        line[c_width] = '\0';
        strcpy(&universe[u_index], line);
        j = 0;
        int t = num_of_galaxies;
        while(j < c_width){
            ret = find_galaxies(line, j);
            if(ret >= 0){
                galaxies[num_of_galaxies].x = ret;
                galaxies[num_of_galaxies].y = i;
                num_of_galaxies++;
                expand_x[ret] %= c_expansion_value - 1;
                j = ret + 1;
            }else{
                break;
            }
            
        }
        if(t == num_of_galaxies){
            expand_y[i] = c_expansion_value - 1;
        }
        i++;
        u_index += c_width;
    }

    calculate_cumulative(&expand_x[0]);
    calculate_cumulative(&expand_y[0]);

    galaxies[0].x = 6;
    
    uint64_t d = calculate_distances(&galaxies[0], num_of_galaxies, 
                                     &expand_x[0], &expand_y[0]);

    printf("distances: %lu\n", d);

    return 0;
}