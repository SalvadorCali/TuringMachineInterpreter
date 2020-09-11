#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 10000
#define TRANSITIONS "tr\n"
#define ACCEPTATIONS "acc\n"
#define MAX_MOVE "max\n"
#define RUN "run\n"

typedef struct {
    int starting_state;
    char reading_symbol;
    char writing_symbol;
    char move;
    int next_state;
} transition;

typedef struct transitions{
    transition t;
    struct transitions * next;
} transition_t;

typedef struct{
    int transitions_length;
    int acceptations_length;
    int halt_length;
    int strings_length;
    int one_string_length;
} lengths;

typedef struct{
    int state;
    int index;
    int string_length;
    unsigned int current_moves;
    char * working_string;
} configuration;

typedef struct configurations{
    configuration c;
    struct configurations * next;
} configuration_t;

int main() {
    char * input = (char*) calloc(10000, sizeof(char));
    unsigned int max_moves = 0;
    int max_state = 0;
    int *acceptation_states = (int *) malloc(sizeof(int));
    char **strings = NULL;
    strings = malloc(sizeof(char *));
    transition_t *current_transition = NULL;
    char output = 'a';
    int *halt_states = (int *) malloc(sizeof(int));
    lengths length = {0, 0, 0, 0, 0};
    configuration_t * conf;
    configuration_t * conf_father;
    configuration_t * conf_daughter;
    configuration_t * head = NULL;
    configuration_t * tail = NULL;
    int states_number = 1000;
    int temp_number = 0;
    int starting_state = 0;
    char reading_symbol = 'a';
    char writing_symbol = 'a';
    char move = 'S';
    int next_state = 0;

    transition_t ***transitions_matrix = (transition_t ***) malloc((states_number) * sizeof(transition_t **));
    for(int g = 0; g < states_number; g++){
        transitions_matrix[g] = (transition_t**)malloc(75 * sizeof(transition_t*));
        for(int k = 0; k < 75; k++){
            transitions_matrix[g][k] = NULL;
        }
    }

    if(fgets(input, MAX_LENGTH, stdin));
    if (!strcmp(input, TRANSITIONS)) {
        if(fgets(input, MAX_LENGTH, stdin));
        do {
            sscanf(input, "%d %c %c %c %d", &starting_state, &reading_symbol, &writing_symbol, &move, &next_state);
            if(starting_state >= states_number){
                temp_number = states_number + 500;
                while(temp_number <= starting_state){
                    temp_number = temp_number + 500;
                }
                transitions_matrix = (transition_t ***) realloc(transitions_matrix, (temp_number) * sizeof(transition_t **));
                for(int g = states_number; g < temp_number; g++){
                    transitions_matrix[g] = (transition_t**)malloc(75 * sizeof(transition_t*));
                    for(int k = 0; k < 75; k++){
                        transitions_matrix[g][k] = NULL;
                    }
                }
                states_number = temp_number;
            }
            current_transition = transitions_matrix[starting_state][reading_symbol-48];
            if(current_transition == NULL){
                transitions_matrix[starting_state][reading_symbol-48] = (transition_t*) malloc(sizeof(transition_t));
                transitions_matrix[starting_state][reading_symbol-48]->t.starting_state = starting_state;
                transitions_matrix[starting_state][reading_symbol-48]->t.reading_symbol = reading_symbol;
                transitions_matrix[starting_state][reading_symbol-48]->t.writing_symbol = writing_symbol;
                transitions_matrix[starting_state][reading_symbol-48]->t.move = move;
                transitions_matrix[starting_state][reading_symbol-48]->t.next_state = next_state;
                transitions_matrix[starting_state][reading_symbol-48]->next = NULL;
            }else{
                while(current_transition->next != NULL){
                    current_transition = current_transition->next;
                }
                current_transition->next = (transition_t*)malloc(sizeof(transition_t));
                current_transition->next->t.starting_state = starting_state;
                current_transition->next->t.reading_symbol = reading_symbol;
                current_transition->next->t.writing_symbol = writing_symbol;
                current_transition->next->t.move = move;
                current_transition->next->t.next_state = next_state;
                current_transition->next->next = NULL;
            }
            if(next_state > max_state){
                max_state = next_state;
            }
            if(fgets(input, MAX_LENGTH, stdin));
        } while (strcmp(input, ACCEPTATIONS));
    }

    if (!strcmp(input, ACCEPTATIONS)) {
        if(fgets(input, MAX_LENGTH, stdin));
        do {
            acceptation_states[length.acceptations_length] = atoi(input);
            length.acceptations_length++;
            acceptation_states = realloc(acceptation_states, (length.acceptations_length + 1) * sizeof(int));
            if(fgets(input, MAX_LENGTH, stdin));
        } while (strcmp(input, MAX_MOVE));
    }
    acceptation_states = realloc(acceptation_states, (length.acceptations_length) * sizeof(int));

    if (!strcmp(input, MAX_MOVE)) {
        if(fgets(input, MAX_LENGTH, stdin));
        max_moves = (unsigned int) atoi(input);
    }

    if(fgets(input, MAX_LENGTH, stdin));
    if (!strcmp(input, RUN)) {
        while (fgets(input, MAX_LENGTH, stdin)) {
            strtok(input, "\n");
            length.one_string_length = strlen(input) + 1;
            strings[length.strings_length] = malloc((length.one_string_length) * sizeof(char));
            strcpy(strings[length.strings_length], input);
            length.strings_length++;
            strings = realloc(strings, (length.strings_length + 1) * sizeof(char *));
        }
    }
    strings = realloc(strings, (length.strings_length) * sizeof(char *));
    free(input);

    int temp = 0;
    for(int i = 0; i < (max_state + 2); i++){
        temp = 0;
        for(int j = 0; j < 75; j++){
            if(transitions_matrix[i][j]!=NULL){
                temp = 1;
                break;
            }
        }
        if(!temp){
            halt_states[length.halt_length] = i;
            length.halt_length++;
            halt_states = realloc(halt_states, (length.halt_length + 1) * sizeof(int));
        }
    }

    for (int i = 0; i < length.strings_length; i++) {
        output = '0';
        length.one_string_length = strlen(strings[i]) + 1;
        conf = (configuration_t*)malloc(sizeof(configuration_t));
        conf->c.state = 0;
        conf->c.index = 0;
        conf->c.current_moves = 0;
        conf->c.string_length = length.one_string_length;
        conf->c.working_string = (char*) malloc(conf->c.string_length*sizeof(char));
        strcpy(conf->c.working_string, strings[i]);
        conf->next = NULL;
        head = conf;
        tail = conf;
        while(1){
            if(output == '1'){
                break;
            }
            conf_father = head;
            if(tail && (head == tail)){
                tail = tail->next;
            }
            if(head){
                head = head->next;
            }
            if(conf_father == NULL){
                if(output == 'a'){
                    output = '0';
                }
                printf("%c\n", output);
                break;
            }
            else{
                if(conf_father->c.current_moves > max_moves){
                    output = 'U';
                    printf("%c\n", output);
                    while(conf_father != NULL){
                        free(conf_father->c.working_string);
                        free(conf_father);
                        conf_father = head;
                        if(tail && (head == tail)){
                            tail = tail->next;
                        }
                        if(head){
                            head = head->next;
                        }
                    }
                    break;
                }
            }
            if(output == '0'){
                output = 'a';
            }
            current_transition = transitions_matrix[conf_father->c.state][(conf_father->c.working_string[conf_father->c.index])-48];
            while (current_transition != NULL) {
                if(((current_transition->t.starting_state==current_transition->t.next_state) && (current_transition->t.reading_symbol==current_transition->t.writing_symbol)
                    && (current_transition->t.move == 'S')) || ((current_transition->t.starting_state==current_transition->t.next_state) && ((current_transition->t.reading_symbol)==('_')) && ((current_transition->t.writing_symbol)==('_'))
                                                                && ((current_transition->t.move == 'L') || (current_transition->t.move == 'R')))){
                    output = 'U';
                    current_transition = current_transition->next;
                    continue;
                }

                for (int q = 0; q < length.halt_length; q++) {
                    if(current_transition->t.next_state == halt_states[q]){
                        output = '0';
                        for(int h = 0; h < length.acceptations_length; h++){
                            if(current_transition->t.next_state == acceptation_states[h]){
                                output = '1';
                                printf("%c\n", output);
                                while(conf_father != NULL){
                                    free(conf_father->c.working_string);
                                    free(conf_father);
                                    conf_father = head;
                                    if(tail && (head == tail)){
                                        tail = tail->next;
                                    }
                                    if(head){
                                        head = head->next;
                                    }
                                }
                                break;
                            }
                        }
                        break;
                    }
                }
                if(output == '1' || output == '0'){
                    break;
                }

                if(current_transition->next == NULL){
                    conf_father->c.state = current_transition->t.next_state;
                    conf_father->c.current_moves++;
                    conf_father->next = NULL;
                    switch(current_transition->t.move){
                        case 'R':
                            if(conf_father->c.index >= (conf_father->c.string_length - 2)){
                                temp_number = (3*conf_father->c.string_length)/2;
                                conf_father->c.working_string = (char*) realloc(conf_father->c.working_string, (temp_number)* sizeof(char));
                                memset(&conf_father->c.working_string[conf_father->c.index + 1], '_', (size_t)(temp_number-conf_father->c.string_length));
                                memset(&conf_father->c.working_string[temp_number - 1], '\0', 1);
                                conf_father->c.string_length = temp_number;
                            }
                            if(conf_father->c.working_string[conf_father->c.index] != current_transition->t.writing_symbol){
                                memset(&conf_father->c.working_string[conf_father->c.index], current_transition->t.writing_symbol, 1);
                            }
                            conf_father->c.index++;
                            break;
                        case 'S':
                            if(conf_father->c.working_string[conf_father->c.index] != current_transition->t.writing_symbol){
                                memset(&conf_father->c.working_string[conf_father->c.index], current_transition->t.writing_symbol, 1);
                            }
                            break;
                        case 'L':
                            if(conf_father->c.index == 0){
                                temp_number = (3*conf_father->c.string_length)/2;
                                conf_father->c.working_string = (char*) realloc(conf_father->c.working_string, (temp_number)* sizeof(char));
                                memmove(&(conf_father->c.working_string[temp_number-conf_father->c.string_length]), conf_father->c.working_string, (conf_father->c.string_length) * sizeof(char));
                                memset(conf_father->c.working_string, '_', (size_t)(temp_number-conf_father->c.string_length));
                                conf_father->c.index = temp_number-conf_father->c.string_length;
                                conf_father->c.string_length = temp_number;
                            }
                            if(conf_father->c.working_string[conf_father->c.index] != current_transition->t.writing_symbol){
                                memset(&conf_father->c.working_string[conf_father->c.index], current_transition->t.writing_symbol, 1);
                            }
                            conf_father->c.index--;
                            break;
                    }
                    if(tail){
                        tail->next = conf_father;
                        tail = tail->next;
                    }else{
                        head = conf_father;
                        tail = conf_father;
                    }
                    conf_father = NULL;
                    current_transition = current_transition->next;
                }else {
                    conf_daughter = (configuration_t *) malloc(sizeof(configuration_t));
                    conf_daughter->c.state = current_transition->t.next_state;
                    conf_daughter->c.index = conf_father->c.index;
                    conf_daughter->c.string_length = conf_father->c.string_length;
                    conf_daughter->c.current_moves = conf_father->c.current_moves + 1;
                    conf_daughter->c.working_string = (char *) malloc(conf_daughter->c.string_length * sizeof(char));
                    strcpy(conf_daughter->c.working_string, conf_father->c.working_string);
                    conf_daughter->next = NULL;

                    switch(current_transition->t.move){
                        case 'R':
                            if(conf_daughter->c.index >= (conf_daughter->c.string_length - 2)){
                                temp_number = (3*conf_daughter->c.string_length)/2;
                                conf_daughter->c.working_string = (char*) realloc(conf_daughter->c.working_string, (temp_number)*
                                                                                                                   sizeof(char));
                                memset(&conf_daughter->c.working_string[conf_daughter->c.index + 1], '_', (size_t)(temp_number-conf_daughter->c.string_length));
                                memset(&conf_daughter->c.working_string[temp_number - 1], '\0', 1);
                                conf_daughter->c.string_length = temp_number;
                            }
                            if(conf_daughter->c.working_string[conf_daughter->c.index] != current_transition->t.writing_symbol){
                                memset(&conf_daughter->c.working_string[conf_daughter->c.index], current_transition->t.writing_symbol, 1);
                            }
                            conf_daughter->c.index++;
                            break;
                        case 'S':
                            if(conf_daughter->c.working_string[conf_daughter->c.index] != current_transition->t.writing_symbol){
                                memset(&conf_daughter->c.working_string[conf_daughter->c.index], current_transition->t.writing_symbol, 1);
                            }
                            break;
                        case 'L':
                            if(conf_daughter->c.index == 0){
                                temp_number = (3*conf_daughter->c.string_length)/2;
                                conf_daughter->c.working_string = (char*) realloc(conf_daughter->c.working_string, (temp_number)* sizeof(char));
                                memmove(&(conf_daughter->c.working_string[temp_number-conf_daughter->c.string_length]), conf_daughter->c.working_string, (conf_daughter->c.string_length) * sizeof(char));
                                memset(conf_daughter->c.working_string, '_', (size_t)(temp_number-conf_daughter->c.string_length));
                                conf_daughter->c.index = temp_number-conf_daughter->c.string_length;
                                conf_daughter->c.string_length = temp_number;
                            }
                            if(conf_daughter->c.working_string[conf_daughter->c.index] != current_transition->t.writing_symbol){
                                memset(&conf_daughter->c.working_string[conf_daughter->c.index], current_transition->t.writing_symbol, 1);
                            }
                            conf_daughter->c.index--;
                            break;
                    }

                    if(tail){
                        tail->next = conf_daughter;
                        tail = tail->next;
                    }else{
                        head = conf_daughter;
                        tail = conf_daughter;
                    }
                    current_transition = current_transition->next;
                }
            }
            if(conf_father){
                if(conf_father->c.working_string){
                    free(conf_father->c.working_string);
                }
                free(conf_father);
            }
        }
    }

    for(int i = 0; i < length.strings_length; i++){
        free(strings[i]);
    }
    free(strings);
    for(int g = 0; g < states_number; g++){
        for(int k = 0; k < 75; k++){

            free(transitions_matrix[g][k]);
        }
        free(transitions_matrix[g]);
    }
    free(acceptation_states);
    free(halt_states);
    free(transitions_matrix);
    return 0;
}