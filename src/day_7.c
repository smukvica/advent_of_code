#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>

#define CARD_COUNT 13

const int c_num_of_cards = CARD_COUNT;
const int c_cards_in_hand = 5;
const int c_number_of_hands = 1000;

const int c_card_values1[CARD_COUNT] = { 1,   2,   3,   4,   5,   
                                         6,   7,   8,   9, 
                                        10,  11,  12,  13};
const int c_card_values2[CARD_COUNT] = { 1,   2,   3,   4,   5,   
                                         6,   7,   8,   9, 
                                         0,  11,  12,  13};
const int c_card_symbols[CARD_COUNT] = {'2', '3', '4', '5', '6', 
                                       '7', '8', '9', 'T',
                                       'J', 'Q', 'K', 'A'};

struct hand_power{
    uint64_t power_of_hand;
    char cards[6];
    int bid;
};

int partition(struct hand_power *a_hp, int a_l, int a_r){
    struct hand_power pivot = a_hp[a_r];
    struct hand_power temp;

    int i = a_l - 1;
   
    for(int j = a_l; j <= a_r; j++){
        if(a_hp[j].power_of_hand < pivot.power_of_hand){
            i++;
            temp = a_hp[i];
            a_hp[i] = a_hp[j];
            a_hp[j] = temp;
        }
    }

    temp = a_hp[i + 1];
    a_hp[i + 1] = a_hp[a_r];
    a_hp[a_r] = temp;

    return i + 1;
}

void q_sort(struct hand_power *a_hp, int a_l, int a_r){
    if(a_r <= a_l || a_l < 0)
        return;

    int p = partition(a_hp, a_l, a_r);

    q_sort(a_hp, a_l, p-1);
    q_sort(a_hp, p+1, a_r);
}

void replace_char(char *a_source, char a_target, char a_wish){
    for(int i = 0; i < strlen(a_source); i++){
        if(a_source[i] == a_target)
            a_source[i] = a_wish;
    }
}

int get_most_beneficial_change(int *a_counts){
    int change = 0;
    for(int i = 0; i < c_num_of_cards; i++){
        if(a_counts[i] >= a_counts[change] && i != 9)
            change = i;
    }
    return change;
}

uint64_t get_card_counts(const char *a_card, int *a_counts, const int *a_card_values){
    uint64_t p = 1;
    uint64_t value = 0;
    for(int i = c_cards_in_hand - 1; i >= 0; i--){
        if(isdigit(a_card[i]) > 0){ // 2-9
            int c = a_card[i] - '0' - 2;
            a_counts[c]++;
            value += p * a_card_values[c];
        }
        if(a_card[i] == 'A'){    // ace
            a_counts[12]++;
            value += p * a_card_values[12];
        }
        if(a_card[i] == 'K'){    // king
            a_counts[11]++;
            value += p * a_card_values[11];
        }
        if(a_card[i] == 'Q'){    // queen
            a_counts[10]++;
            value += p * a_card_values[10];
        }
        if(a_card[i] == 'J'){    // jack
            a_counts[9]++;
            value += p * a_card_values[9];
        }
        if(a_card[i] == 'T'){    // 10
            a_counts[8]++;
            value += p * a_card_values[8];
        }
        p *= 100;
    }
    return value;
}

void get_card_power(const char *a_card, struct hand_power *a_hp){
    int cards[c_num_of_cards];
    for(int i = 0; i < c_num_of_cards; i++){
        cards[i] = 0;
    }

    a_hp->power_of_hand = get_card_counts(a_card, &cards[0], &c_card_values1[0]);

    uint64_t score = 0;

    for(int i = 0; i < c_num_of_cards; i++){
        if(cards[i] == 5){  // five of a kind
            score = 9;
        }
        if(cards[i] == 4){  // four of a kind
            score = 8;
        }
        if(cards[i] == 3){  // three of a kind
            score += 5;
        }
        if(cards[i] == 2){  // two of a kind
            score += 2;
        }
    }
    for(int i = 0; i < 5; i++)
        score *= 100;

    a_hp->power_of_hand += score;
}

void get_card_power_2(char *a_card, struct hand_power *a_hp){
    int cards[c_num_of_cards];
    for(int i = 0; i < c_num_of_cards; i++){
        cards[i] = 0;
    }

    a_hp->power_of_hand = get_card_counts(a_card, &cards[0], &c_card_values2[0]);

    int change = get_most_beneficial_change(&cards[0]);

    replace_char(a_card, 'J', c_card_symbols[change]);

    for(int i = 0; i < c_num_of_cards; i++){
        cards[i] = 0;
    }

    get_card_counts(a_card, &cards[0], &c_card_values2[0]);

    uint64_t score = 0;

    for(int i = 0; i < c_num_of_cards; i++){
        if(cards[i] == 5){  // five of a kind
            score = 9;
        }
        if(cards[i] == 4){  // four of a kind
            score = 8;
        }
        if(cards[i] == 3){  // three of a kind
            score += 5;
        }
        if(cards[i] == 2){  // two of a kind
            score += 2;
        }
    }
    for(int i = 0; i < 5; i++)
        score *= 100;

    a_hp->power_of_hand += score;
}

int main(){

    char cards[c_number_of_hands][c_cards_in_hand+1];
    struct hand_power hand_powers[c_number_of_hands];
    int bids[c_number_of_hands];

    for(int i = 0; i < c_number_of_hands; i++){
        scanf("%s %d\n", &cards[i], &bids[i]);
        get_card_power(cards[i], &hand_powers[i]);
        hand_powers[i].bid = bids[i];
        strcpy(hand_powers[i].cards, cards[i]);
    }

    q_sort(&hand_powers[0], 0, c_number_of_hands);
    
    unsigned int sum1 = 0;
    for(int i = 0; i < c_number_of_hands; i++){
        sum1 += hand_powers[i].bid * (i+1);
    }

    for(int i = 0; i < c_number_of_hands; i++){
        get_card_power_2(cards[i], &hand_powers[i]);
        hand_powers[i].bid = bids[i];
        strcpy(hand_powers[i].cards, cards[i]);
    }

    q_sort(&hand_powers[0], 0, c_number_of_hands);

    unsigned int sum2 = 0;
    for(int i = 0; i < c_number_of_hands; i++){
        sum2 += hand_powers[i].bid * (i+1);
    }

    printf("sum1: %d\n", sum1);
    printf("sum2: %d\n", sum2);

    return 0;
}