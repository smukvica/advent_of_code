#include <stdio.h>
#include <string.h>
#include <ctype.h>

const int c_width = 140;
const int c_height = 140;

const int c_around_i[8] = {-1,  0,  1, -1, 1, -1, 0, 1};
const int c_around_j[8] = {-1, -1, -1,  0, 0,  1, 1, 1};

// return 0 if all surrounding are .
// return 1 if any surrounding has symbol
int check_surroundings(const char a_field[c_height][c_width], int a_i, int a_j){
    for(int i = 0; i < 8; i++){
        int h = a_i + c_around_i[i];
        int w = a_j + c_around_j[i];
        if(w < 0 || w >= c_width || h < 0 || h >= c_height)
            continue;

        if(a_field[h][w] != '.' && isdigit(a_field[h][w]) == 0)
           return 1;
    }
    return 0;
}

int calculate_sum_of_parts(const char a_field[c_height][c_width]){
    char number[10];
    int digits = 0;
    int is_part = 0;
    int sum = 0;
    int d = 0;
    for(int i = 0; i < c_height; i++){
        for(int j = 0; j < c_width; j++){
            if(isdigit(a_field[i][j]) > 0){
                number[digits] = a_field[i][j];
                digits++;
                if(check_surroundings(a_field, i, j))
                    is_part = 1;
            }

            if(isdigit(a_field[i][j]) == 0 && digits > 0){
                sscanf(number, "%d", &d);
                if(is_part == 1)
                    sum += d;

                for(int k = 0; k < 10; k++){
                    number[k] = '\0';
                }
                digits = 0;
                is_part = 0;
                d = 0;
            }
        }
    }
    return sum;
}

int main(){
    char g_field[c_height][c_width];

    char *line;
    size_t size = 0;
    int i = 0;
    int len = 0;

    while((len = getline(&line, &size, stdin)) && len != -1){
        line[len - 1] = '\0';
        strcpy(g_field[i], line);
        i++;
    }

    int sum1 = calculate_sum_of_parts(g_field);
    int sum2 = get_sum_of_gears(g_field);


    printf("parts: %d\n", sum1);
    printf("gears: %d\n", sum2);

    return 0;
}