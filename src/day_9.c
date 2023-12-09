#include <stdio.h>
#include <string.h>
#include <ctype.h>

const int numbers_in_line = 6;
const int number_of_lines = 3;

int get_next_number(const char *a_line, int *a_index){

    char number[11];
    int i = 0;
    int j = *a_index;

    while(a_line[j] == ' ') j++;

    while(a_line[j] - '0' >= 0 || a_line[j] == '-'){
        number[i] = a_line[j];
        j++;
        i++;
    }
    number[i] = '\0';

    int n = 0;

    sscanf(number, "%d", &n);
    *a_index = j;
    return n;
}

int main(){
    int history[number_of_lines][numbers_in_line][numbers_in_line];
    int history_depth[number_of_lines];
    char *line;
    size_t size = 0;

    int i = 0;

    while(getline(&line, &size, stdin) != -1){
        int j = 0;
        int k = 0;
        while(k < numbers_in_line){
            history[i][0][k] = get_next_number(line, &j);
            k++;
        }

        int num_of_zeroes = 0;
        int nums_in_current_line = numbers_in_line - 1;
        int current_line = 1;
        history_depth[i] = 1;
        for(int l = 1; l < numbers_in_line; l++){
            history_depth[i]++;
            num_of_zeroes = 0;
            for(int n = 0; n < numbers_in_line - l; n++){
                history[i][l][n] = history[i][l - 1][n + 1] -
                                    history[i][l - 1][n];
                if(history[i][l][n] == 0)
                    num_of_zeroes++;
            }
            if(num_of_zeroes == numbers_in_line - l)
                break;
        }
        
        i++;
    }

    unsigned int sum = 0;
    for(i = 0; i < number_of_lines; i++){
        int temp = 0;
        for(int j = history_depth[i] - 1; j > 0; j--){
            temp += history[i][j-1][numbers_in_line - j];
        }
        sum += temp;
    }

    printf("sum: %d\n", sum);

    return 0;
}