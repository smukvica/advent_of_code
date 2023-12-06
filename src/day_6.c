#include <stdio.h>
#include <stdint.h>

uint64_t can_beat(uint64_t a_time, uint64_t a_distance, uint64_t a_hold){
    uint64_t time_left = a_time - a_hold;
    if(time_left * a_hold >= a_distance)
        return 1;
    return 0;
}

uint64_t append_numbers_together(int a_array[4]){

    uint64_t temps[4];
    for(int i = 0; i < 4; i++){
        temps[i] = (uint64_t)a_array[i];
    }


    for(int i = 3; i > 0; i--){
        int p = 1;
        int t = a_array[i];
        while(t > 0){
            t /= 10;
            p *= 10;
        }
        for(int j = 0; j < i; j++){
            temps[j] *= p;
        }
    }
    uint64_t n = 0;
    for(int i = 0; i < 4; i++){
        n += temps[i];
    }
    return n;
}

int main(){

    int times[4];
    int distances[4];
    int ways[4];

    scanf("Time: %d %d %d %d\n", &times[0], &times[1], &times[2], &times[3]);

    scanf("Distance: %d %d %d %d\n", &distances[0], &distances[1], &distances[2], &distances[3]);

    

    for(int i = 0; i < 4; i++){
        int a = 0;
        int b = 0;
        for(int j = 1; j < times[i]; j++){
            if(can_beat(times[i], distances[i], j) == 1){
                a = j;
                break;
            }
        }

        for(int j = times[i]; j > 0; j--){
            if(can_beat(times[i], distances[i], j) == 1){
                b = j;
                break;
            }
        }

        ways[i] = b - a + 1;
    }

    printf("ways1: %d\n", ways[0] * ways[1] * ways[2] * ways[3]);

    uint64_t time_k = append_numbers_together(times);
    uint64_t distance_k = append_numbers_together(distances);

    int a = 0;
    int b = 0;
    for(int j = 1; j < time_k; j++){
        if(can_beat(time_k, distance_k, j) == 1){
            a = j;
            break;
        }
    }

    for(int j = time_k; j > 0; j--){
        if(can_beat(time_k, distance_k, j) == 1){
            b = j;
            break;
        }
    }

    printf("ways2: %lu\n", b - a + 1);

    return 0;
}