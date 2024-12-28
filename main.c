#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define FILENAME "betyg.txt"
#define BUFFER_SIZE 256
#define COURSE_CHARS 6

typedef struct Node{
    char course[COURSE_CHARS + 1]; // '\0'
    float hp;
    float grade;
    struct Node *tail;
} Node;

void freeAll(Node *head){
    Node *prev;
    while(head != NULL){
        prev = head;
        head = head->tail;
        free(prev);
    }
}

int main(){
    FILE *betyg = fopen(FILENAME, "r");
    if (betyg == NULL) {
        printf("File with name %s not found\n", FILENAME);
        exit(1);
    }

    Node *head = malloc(sizeof(Node));
    head->tail = NULL;
    Node *tmpHead = head;
    uint8_t first = 1;

    char buffer[BUFFER_SIZE];
    uint8_t line_number = 0;

    printf("===Printar buffer===\n");
    while(fgets(buffer, BUFFER_SIZE, betyg)){
        line_number++;
        printf("[%2i]: ", line_number);
        if(!first){
            tmpHead->tail = malloc(sizeof(Node));
            tmpHead = tmpHead->tail;
            tmpHead->tail = NULL;
        } else {
            first = 0;
        }
        
        printf("%s", buffer);

        uint8_t bufferChar = 0;
        for(uint8_t i = 0; i < COURSE_CHARS; i++){
            tmpHead->course[i] = buffer[bufferChar];
            bufferChar++;
        }
        bufferChar++; //skip ','
        char *endChar = ",";
        tmpHead->hp = strtof(&buffer[bufferChar], &endChar);

        float golv = floor(tmpHead->hp);
        if(tmpHead->hp != golv){
            tmpHead->hp = floor(tmpHead->hp*10)/10;
        }else{
            tmpHead->hp = golv;
        }

        while(buffer[bufferChar] != ','){
            bufferChar++;
        }
        bufferChar++; //skip ','
        char tmpGradeChar = buffer[bufferChar];
        float gradeVal = 0;

        switch(tmpGradeChar){
            case 'A': gradeVal = 5; break;
            case 'B': gradeVal = 4.5; break;
            case 'C': gradeVal = 4; break;
            case 'D': gradeVal = 3.5; break;
            case 'E': gradeVal = 3; break;
            case 'F': gradeVal = 0; break;
            default: gradeVal = 0;
        }
        if(gradeVal == 0){
            printf("gradeVal = 0 för [%i]\n", line_number);
        }
        tmpHead->grade = gradeVal;
    }
    printf("====================\n\n");

    printf("=====Printar LL=====\n");
    printf("%-8s%-4s%-8s\n", "Kurs", "HP", "Betyg");
    uint8_t i = 0;
    tmpHead = head;
    while(tmpHead != NULL){
        i++;
        if(tmpHead->hp != floor(tmpHead->hp)){
            printf("%-8s%-4.1f%-8.0f\n", 
                tmpHead->course, tmpHead->hp, tmpHead->grade);
        }else{
            printf("%-8s%-4.0f%-8.0f\n", 
                tmpHead->course, tmpHead->hp, tmpHead->grade);
        }
        
        tmpHead = tmpHead->tail;
    }
    printf("====================\n\n");

    //Calculate GPA
    tmpHead = head;
    float totGrade = 0;
    float sumHP = 0;

    while(tmpHead != NULL){
        totGrade += tmpHead->hp * tmpHead->grade;
        sumHP += tmpHead->hp;
        tmpHead = tmpHead->tail;
    }

    printf("Ditt gpa är %f\n", (totGrade/sumHP));

    freeAll(head);
    
    return 1;
}