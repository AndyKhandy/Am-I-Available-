#include <stdio.h>
#include<stdlib.h>
#include <string.h>

#define MAXLEN 100

typedef struct ClassNode {
    char name[MAXLEN];
    char days[6]; //mtwyf
    int start_mins;
    int start_hour;
    int end_hour;
    int end_mins;
    char time[50];
    struct ClassNode* next;

} ClassNode;

void printNodes(ClassNode* head){
    if(head == NULL)
    {
        printf("no classes :(");
        return;
    }
    ClassNode* temp = head;
    while(temp != NULL)
    {
        printf("%s %s %d %d %d %d\n", temp->name, temp->days, temp->start_hour,temp->start_mins, temp->end_hour, temp->end_mins);
        temp = temp->next;
    }
}

int main()
{
    char buffer[100];
    int classes = 0;

    FILE* fptr;
    fptr = fopen("student.txt", "r");
    if (fptr == NULL)
    {
    printf("Error!");
    exit(1);
    }

    ClassNode* headMain = NULL;
    ClassNode* class = (ClassNode*)malloc(sizeof(ClassNode));

    while(fgets(buffer, sizeof(buffer), fptr) != NULL)
    {
        buffer[strcspn(buffer,"\n")] = '\0';
        sscanf(buffer, "%[^~]~%[^~]~%s", class->name, class->days, class->time);
        class->next = NULL;

        char* times = strtok(class->time, "-");
        int start = 1;

        while(times != NULL)
        {
            if(strlen(times) > 2)
            {
                if(start){
                    sscanf(times, "%d:%d", &class->start_hour, &class->start_mins);
                    start = 0;
                }
                else{
                    sscanf(times, "%d:%d", &class->end_hour, &class->end_mins);
                    start = 1;
                }
            }
            else{
               if(start){
                    sscanf(times, "%d", &class->start_hour);
                    class->start_mins = 0;
                    start = 0;
                }
                else{
                    sscanf(times, "%d", &class->end_hour);
                    class->end_mins = 0;
                    start = 1;
                } 
            }
            times = strtok(NULL, "-");
        }


        if (headMain == NULL) {
            headMain = class;
        } else {
            ClassNode* temp = headMain;
            while(temp->next != NULL)
            {
                temp = temp->next;
            }
            temp->next = class;
        }
        class = (ClassNode*)malloc(sizeof(ClassNode));

    }
    printNodes(headMain);


    fclose(fptr);
    return 0;
}
