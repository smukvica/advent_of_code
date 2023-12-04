#include <stdio.h>
#include <ctype.h>

const int c_cards_amount = 10;
const int c_you_amount = 25;


int matching(const int *a_cards, const int *a_you){
    int c = 0;
    for(int i = 0; i < c_cards_amount; i++){
        for(int j = 0; j < c_you_amount; j++){
            if(a_cards[i] == a_you[j]){
                c++;
                break;
            }
        }
    }

    return c;
}

int get_digits(const char *a_line, int *a_cards, int *a_you){
    int i = 0;
    int c = 0;
    int y = 0;
    char num[5];
    int amount = 0;
    while(a_line[i] != ':') i++;
    while(a_line[i] != '\0'){
        if(isdigit(a_line[i]) > 0){
            num[amount] = a_line[i];
            amount++;
        }
        if(amount > 0 && isdigit(a_line[i]) == 0){
            amount = 0;
            if(c < c_cards_amount){
                sscanf(num, "%d", &a_cards[c]);
                c++;
            }else if(y < c_you_amount){
                sscanf(num, "%d", &a_you[y]);
                y++;
            }
            for(int j = 0; j < 5; j++){
                num[j] = 0;
            }
        }
        i++;
    }
    return c+y;
}

int main(){
    int cards[c_cards_amount];
    int you[c_you_amount];

    char *line;
    size_t size = 0;
    int i = 0;
    int len = 0;

    int sum1 = 0;
    int sum2 = 0;
    int ret = 0;

    int instances[220];

    for(int j = 0; j < 220; j++){
        instances[j] = 1;
    }

    while(getline(&line, &size, stdin) != -1){
        get_digits(line, &cards[0], &you[0]);
        ret = matching(&cards[0], &you[0]);
        if(ret)
            sum1 += 1 << (ret - 1);

        for(int j = 0; j < ret && j < 219; j++){
            instances[i + j + 1] += instances[i];
        }

        sum2 += instances[i];
        i++;
    }

    printf("sum1: %d\n", sum1);
    printf("sum2: %d\n", sum2);

    return 0;
}