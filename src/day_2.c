#include <stdio.h>
#include <string.h>
#include <ctype.h>

const int c_red = 12;
const int c_green = 13;
const int c_blue = 14;


int check_valid_number(char *a_set){
    int len = strlen(a_set);
    int r = 0;
    int g = 0;
    int b = 0;
    char number[3];
    int c = 0;
    for(int i = 0; i < len; i++){
        if(isdigit(a_set[i])){
            number[c] = a_set[i];
            c++;
        }
        if(a_set[i] == 'r' && c != 0){
            sscanf(number, "%d", &r);
            c = 0;
        }
        if(a_set[i] == 'g' && c != 0){
            sscanf(number, "%d", &g);
            c = 0;
        }
        if(a_set[i] == 'b' && c != 0){
            sscanf(number, "%d", &b);
            c = 0;
        }
        if(c == 0){
            number[0] = '\0';
            number[1] = '\0';
            number[2] = '\0';
        }
    }
    if(r > c_red)
        return -1;
    if(g > c_green)
        return -1;
    if(b > c_blue)
        return -1;
    return 1;
}   

int process_line(char *a_line){
    char* token = strtok(a_line, ":");
    token = strtok(NULL, ";");
    int ret = 0;
    while (token != NULL) {
        ret = check_valid_number(token);
        if(ret == -1)
            return -1;
        token = strtok(NULL, ";");
    }
    return 1;
}

int main(){

    char *line;
    size_t size = 0;
    int i = 1;

    int sum = 0;

    while(getline(&line, &size, stdin) != -1){
        if(process_line(line) == 1){
            sum += i;
        }
        i++;
    }

    printf("sum: %d\n", sum);


    return 0;
}