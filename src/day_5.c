#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <ctype.h>

int get_map(const char *a_line){
    if(strcmp(a_line, "seed-to-soil map:\n") == 0){
        return 1;
    }
    if(strcmp(a_line, "soil-to-fertilizer map:\n") == 0){
        return 2;
    }
    if(strcmp(a_line, "fertilizer-to-water map:\n") == 0){
        return 3;
    }
    if(strcmp(a_line, "water-to-light map:\n") == 0){
        return 4;
    }
    if(strcmp(a_line, "light-to-temperature map:\n") == 0){
        return 5;
    }
    if(strcmp(a_line, "temperature-to-humidity map:\n") == 0){
        return 6;
    }
    if(strcmp(a_line, "humidity-to-location map:\n") == 0){
        return 7;
    }
    return 0;
}

int get_number(const char *a_line, int a_start_index, uint64_t *a_location){

    char number[64] = { 0 };
    int current = 0;

    while(a_line[a_start_index] - '0' >= 0){
        number[current] = a_line[a_start_index];
        current++;
        a_start_index++;
    }

    int amount = 0;
    uint64_t num = 0;
    int pow = 1;
    current--;
    while(current != -1){
        num += (uint64_t)((uint64_t)(number[current] - '0') * pow);
        current--;
        pow *= 10;
        amount++;
    }

    *a_location = num;

    return a_start_index;
}

int get_seeds(const char *a_line, uint64_t *a_seeds){
    int i = 0;
    while(isdigit(a_line[i]) == 0){
        i++;
    }
    int len = strlen(a_line);
    int s = 0;
    while(i < len){
        i = get_number(a_line, i, &a_seeds[s]) + 1;
        s++;
    }
    return s;
}

int main(){
    char *line;
    char lines[256][256];
    size_t size = 0;
    int i = 0;

    while(getline(&line, &size, stdin) != -1){
        strcpy(&lines[i][0], line);
        i++;
    }

    uint64_t seeds[100];
    int seed_amount = 0;
    uint64_t maps[7][100][3];
    uint64_t temp_values[3];
    int map_sizes[7];

    seed_amount = get_seeds(lines[0], &seeds[0]);
    
    int k = 1;
    int ret = 0;
    int m = -1;
    uint64_t temp = 69;
    while(k < i){
        ret = get_map(lines[k]);
        if(ret != 0){
            m = ret;
        }
        if(ret == 0 && lines[k][0] - '0' >= 0){
            int j = 0;
            j = get_number(lines[k], j, &maps[m-1][map_sizes[m-1]][0]) + 1;
            j = get_number(lines[k], j, &maps[m-1][map_sizes[m-1]][1]) + 1;
            j = get_number(lines[k], j, &maps[m-1][map_sizes[m-1]][2]) + 1;
            map_sizes[m-1]++;
        }

        k++;
    }

    uint64_t solo = 0xFFFFFFFFFFFFFFFF;
    uint64_t current = 0;
    for(int s = 0; s < seed_amount; s++){
        current = seeds[s];
        for(i = 0; i < 7; i++){
            for(m = 0; m < map_sizes[i]; m++){
                if(maps[i][m][1] <= current && maps[i][m][1] + maps[i][m][2] > current){
                    current = maps[i][m][0] + (current - maps[i][m][1]);
                    break;
                }
            }
        }
        if(current < solo){
            solo = current;
        }
    }

    printf("lowest solo:  %lu\n", solo);

    return 0;

}