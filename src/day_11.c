#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int c_width = 140;
const int c_height = 140;

typedef struct galaxy{
    int x;
    int y;
}galaxy;

int find_galaxies(const char *a_line, int a_start){
    while(a_line[a_start] != '#' && a_start < c_width){
        a_start++;
    }

    printf("%s %d\n", a_line, a_start);

    if(a_start == c_width)
        return -1;

    return a_start;
}

void calculate_cumulative(int *a_expand){
    for(int i = 1; i < c_width; i++){
        a_expand[i] += a_expand[i - 1];
    }
}

unsigned int calculate_distances(const galaxy *a_galaxies, 
                                 int a_num_of_galaxies, 
                                 int *a_expand_x,
                                 int *a_expand_y){
    unsigned int sum = 0;
    for(int i = 1; i < a_num_of_galaxies - 1; i++){
        for(int j = i + 1; j < a_num_of_galaxies; j++){
            sum += abs(a_galaxies[j].x - a_galaxies[i].x) + 
                   abs(a_expand_x[a_galaxies[j].x] - a_expand_x[a_galaxies[i].x]);
            sum += abs(a_galaxies[j].y - a_galaxies[i].y) + 
                   abs(a_expand_y[a_galaxies[j].y] - a_expand_y[a_galaxies[i].y]);
        }
    }

    return sum;
}

int main(){
    int expand_x[c_width];
    int expand_y[c_height];
    for(int i = 0; i < c_width; i++){
        expand_x[i] = 1;
    }
    for(int i = 0; i < c_height; i++){
        expand_y[i] = 0;
    }

    galaxy galaxies[c_width * c_height];

    char universe[c_width * c_height];

    unsigned int num_of_galaxies = 1;

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
                printf("%d %d %d\n", ret, i, num_of_galaxies);
                galaxies[num_of_galaxies].x = ret;
                galaxies[num_of_galaxies].y = i;
                printf("%d %d\n", galaxies[num_of_galaxies].x, galaxies[num_of_galaxies].y);
                num_of_galaxies++;
                expand_x[ret] %= 1;
                j = ret + 1;
            }else{
                break;
            }
            
        }
        if(t == num_of_galaxies){
            expand_y[i] = 1;
        }
        i++;
        u_index += c_width;
    }

    calculate_cumulative(&expand_x[0]);
    calculate_cumulative(&expand_y[0]);
    //galaxies[0].x = 6;
    for(i = 1; i < num_of_galaxies; i++){
        printf("%d %d\n", galaxies[i].x, galaxies[i].y);
    }
    
    unsigned int d = calculate_distances(&galaxies[0], num_of_galaxies, 
                                         &expand_x[0], &expand_y[0]);

    printf("distances: %u\n", d);

    return 0;
}