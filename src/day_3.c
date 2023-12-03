#include <stdio.h>
#include <string.h>
#include <ctype.h>

const int c_width = 140;
const int c_height = 140;

const int c_around_i[8] = {-1,  0,  1, -1, 1, -1, 0, 1};
const int c_around_j[8] = {-1, -1, -1,  0, 0,  1, 1, 1};

int get_digit(const char a_field[c_height][c_width], int a_i, int a_j){
    char number[10];
    int digits = 0;
    
    while( a_j >= 0 && isdigit(a_field[a_i][a_j]))
        a_j--;
    a_j++;
    
    while(a_j < c_width && isdigit(a_field[a_i][a_j])){
        number[digits] = a_field[a_i][a_j];
        digits++;
        a_j++;
    }
    number[digits] = '\0';
    
    int d = 0;
    sscanf(number, "%d", &d);
    return d;
}

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

int get_unique(int a_digits[], int a_amount){
    int nums[100];
    int a = 0;
    int i,j;
    for(i = 0; i < a_amount; i++){
        for(j = 0; j < a; j++){
            if(nums[j] == a_digits[i])
                break;
        }
        if(j == a){
            nums[a] = a_digits[i];
            a++;
        }
    }
    return a;
}

int calculate_sum_of_gears(const char a_field[c_height][c_width]){
    int digits[10];
    int amount = 0;
    int sum = 0;
    int w = 0;
    int h = 0;
    for(int i = 0; i < c_height; i++){
        for(int j = 0; j < c_width; j++){
            if(a_field[i][j] == '*'){
                
                for(int k = 0; k < 8; k++){
                    w = j + c_around_j[k];
                    h = i + c_around_i[k];
                    if(w < 0 || w >= c_width || h < 0 || h >= c_height)
                        continue;
                    
                    if(isdigit(a_field[h][w])){
                        digits[amount] = get_digit(a_field, h, w);
                        amount++;
                    }
                    
                }
                
                if(get_unique(digits, amount) == 2){
                    int a = digits[0];
                    int b = 0;
                    for(int k = 1; k < amount; k++){
                        if(digits[k] != a){
                            b = digits[k];
                            break;
                        }
                    }
                    sum += a * b;
                }
                amount = 0;
                for(int k = 0; k < 10; k++){
                    digits[k] = 0;
                }
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
    int sum2 = calculate_sum_of_gears(g_field);


    printf("parts: %d\n", sum1);
    printf("gears: %d\n", sum2);

    return 0;
}