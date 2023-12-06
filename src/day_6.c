#include <stdio.h>

int can_beat(int a_time, int a_distance, int a_hold){
    int time_left = a_time - a_hold;
    if(time_left * a_hold >= a_distance)
        return 1;
    return 0;
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

    

    printf("ways: %d\n", ways[0] * ways[1] * ways[2] * ways[3]);

    return 0;
}