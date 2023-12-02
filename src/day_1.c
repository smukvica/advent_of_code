#include <stdio.h>
#include <string.h>
#include <ctype.h>

const int max_number_of_words = 1000;
const int max_word_length = 256;

const char words[9][10] = { "one", "two", "three", "four", "five", "six", 
                            "seven", "eight", "nine"};

// sum an array of numbers
int sum(int a_array[]){
    int c = 0;
    for(int i = 0; i < max_number_of_words; i++){
        c += a_array[i];
    }

    return c;
}

int get_number(const char *a_word){
    int len = strlen(a_word);
    int a = 0;
    int b = 0;
    // search for first digit appearing in a_word from start
    for(int i = 0; i < len; i++){
        if(isdigit(a_word[i])){
            a = a_word[i] - '0';
            break;
        }
    }

    // search for first digit appearing in a_word from end
    for(int i = len - 1; i >= 0; i--){
        if(isdigit(a_word[i])){
            b = a_word[i] - '0';
            break;
        }
    }
    return a * 10 + b;
}

// gets numbers written with text and with digits
int get_number_from_word(const char *a_word){
    int len = strlen(a_word);
    char numbers[len];
    int amount = 0;
    int i, j, k;
    // loop through whole string
    for(i = 0; i < len; i++){
        // if already a digit add it to numbers string
        if(isdigit(a_word[i])){
            numbers[amount] = a_word[i];
            amount++;
            continue;
        }
        // regular character
        // check all 9 possible writen words
        for(j = 0; j < 9; j++){
            int len_of_number = strlen(words[j]);
            // set found word to true
            int found = 1;
            // loop through writen word and compare it to string if it contains
            // a number writen with text
            for(k = 0; k < len_of_number; k++){
                // if the characters aren't the same we set found to false
                if(a_word[i + k] != words[j][k]){
                    found = 0;
                }
            }
            // if word was found we put the word as a digit to numbers string
            if(found){
                numbers[amount] = (char)(j + 1 + '0');
                amount++;
            }
        }
    }
    // null terminate the string
    numbers[amount] = '\0';
    // use regular number search
    // numbers array contains all digits in order of appearance
    return get_number(numbers);
}

int main(){
    char word[max_word_length];

    int numbers[max_number_of_words];
    int numbers_with_words[max_number_of_words];

    for(int i = 0; i < max_number_of_words; i++){
        scanf("%s", word);
        numbers[i] = get_number(word);
        numbers_with_words[i] = get_number_from_word(word);
    }
    
    int s1 = sum(numbers);
    int s2 = sum(numbers_with_words);

    printf("just digits:   %d\n", s1);
    printf("digits + text: %d\n", s2);

    return 0;
}