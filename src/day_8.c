#include <stdio.h>
#include <string.h>
#include <inttypes.h>

typedef struct node{
    char name[4];
    int l;
    int r;
}node;

uint64_t gcd(uint64_t a, uint64_t b){ 
    if (b == 0)
        return a;
    else 
        return gcd(b, a % b);
}

uint64_t lcm(uint64_t a_a, uint64_t a_b){
    return a_a * ((a_b) / gcd(a_a, a_b));
}

int get_index(const node a_nodes[], const char *a_name, int a_num){
    for(int i = 0; i < a_num; i++){
        if(strcmp(a_nodes[i].name, a_name) == 0)
            return i;
    }
    return -1;
}

int get_indices(const node a_nodes[], const char *a_name, int a_num, int *a_indices){
    int amount = 0;
    for(int i = 0; i < a_num; i++){
        if(strcmp(a_nodes[i].name, a_name) == 0){
            a_indices[amount] = i;
            amount++;
        }
    }
    return amount;
}

void get_names_from_line(const char *a_line, char *a_name, char *a_l, char *a_r){
    int len = strlen(a_line);

    int i = 0;
    int l = 0;
    int r = 0;
    while(a_line[i] != ' '){
        a_name[i] = a_line[i];
        i++;
    }
    a_name[i] = '\0';
    while(a_line[i] - 'A' < 0) i++;

    while(a_line[i] != ','){
        a_l[l] = a_line[i];
        l++;
        i++;
    }
    a_l[l] = '\0';

    while(a_line[i] - 'A' < 0) i++;

    while(a_line[i] != '\n'){
        a_r[r] = a_line[i];
        r++;
        i++;
    }
    a_r[r-1] = '\0';
}

node construct_node_index(const char a_lines[1000][20], int a_index, int a_num){
    node n = {"   \0", -1, -1};
    char line[20];
    strcpy(line, a_lines[a_index]);

    char t[4];
    char l[4];
    char r[4];

    get_names_from_line(line, &n.name[0], &l[0], &r[0]);

    for(int i = 0; i < a_num; i++){
        strcpy(line, a_lines[i + 2]);
        line[3] = '\0';
        if(strcmp(line, l) == 0)
            n.l = i;
        if(strcmp(line, r) == 0)
            n.r = i;

        if(n.l != -1 && n.r != -1)
            break;
    }

    return n;
}

int main(){
    char lines[1000][20];
    char instructions[512];
    char *line;
    size_t size = 0;
    int num_of_nodes = 0;
    int i = 0;

    while(getline(&line, &size, stdin) != -1){
        if(i == 0)
            strcpy(instructions, line);
        else
            strcpy(lines[i], line);
        i++;
    }

    num_of_nodes = i - 2;

    node nodes[num_of_nodes];

    

    for(int j = 2; j < num_of_nodes + 2; j++){
        nodes[j-2] = construct_node_index(lines, j, i);
    }

    int index = get_index(nodes, "AAA", num_of_nodes);
    i = 0;
    unsigned int steps1 = 0;
    char d = instructions[0];
    int len = strlen(instructions)-1;
    while(1){
        if(strcmp(nodes[index].name, "ZZZ") == 0)
            break;
        if(d == 'R')
            index = nodes[index].r;
        if(d == 'L')
            index = nodes[index].l;
        
        i++;
        i %= len;
        d = instructions[i];
        steps1++;
    }

    for(i = 0; i < num_of_nodes; i++){
        nodes[i].name[0] = nodes[i].name[2];
        nodes[i].name[1] = '\0';
    }

    int indices[1000];

    int amount = get_indices(nodes, "A", num_of_nodes, &indices[0]);

    i = 0;
    uint64_t steps2[amount];
    d = instructions[0];
    for(int j = 0; j < amount; j++){
        d = instructions[0];
        i = 0;
        steps2[j] = 0;
        while(1){
            if(strcmp(nodes[indices[j]].name, "Z") == 0)
                break;
            if(d == 'R')
                indices[j] = nodes[indices[j]].r;
            if(d == 'L')
                indices[j] = nodes[indices[j]].l;
            i++;
            i %= len;
            d = instructions[i];
            steps2[j]++;
        }
    }

    uint64_t steps2_final = lcm(steps2[0], steps2[1]);
    for (int j = 2; j < amount; j++){
        steps2_final = lcm(steps2_final, steps2[j]);
    }

    printf("steps1: %u\n", steps1);
    printf("steps2: %lu\n", steps2_final);


    return 0;
}