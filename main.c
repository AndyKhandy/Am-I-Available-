#include <stdio.h>
#include <stdlib.h>
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

ClassNode* headMain = NULL;

int startToMinutes(ClassNode* node) {
    return (node->start_hour*60 + node->start_mins);
}

void printClasses(){
    if(headMain == NULL)
    {
        printf("no classes :(");
        return;
    }
    ClassNode* temp = headMain;
    while(temp != NULL)
    {
        printf("%s %s %d %d %d %d\n", temp->name, temp->days, temp->start_hour,temp->start_mins, temp->end_hour, temp->end_mins);
        temp = temp->next;
    }
}

void readClasses() {
    char buffer[100];
    int classes = 0;

    FILE* fptr;
    fptr = fopen("student.txt", "r");
    if (fptr == NULL)
    {
    printf("Error!");
    exit(1);
    }

    ClassNode* class = (ClassNode*)malloc(sizeof(ClassNode));

    while(fscanf(fptr, "%[^~]~%[^~]~%s\n", class->name, class->days, class->time) == 3)
    {
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

        //insert in least-to-greatest
        if (headMain == NULL) {
            headMain = class;
        } else if (startToMinutes(headMain) > startToMinutes(class)) {
            class->next = headMain;
            headMain = class;
        } else {
            ClassNode* temp = headMain;
            //looking for last node that is either null or bigger
            while(temp->next != NULL && startToMinutes(temp->next) < startToMinutes(class))
            {
                temp = temp->next;
            }

            //first bigger node placed on right of our node, our node inserted to the left
            class->next = temp->next;
            temp->next = class;
        }
        class = (ClassNode*)malloc(sizeof(ClassNode));

    }

    fclose(fptr);
}

int main()
{ 
    int intake;
    do {
        printf("== Class Management System ==\n1. Read Schedule\n2.Find Free Time\n3. Make New Schedule\n4. Exit\n\nChoose: ");
        while (scanf("%d", &intake) >= 0 && (intake < 1 || intake > 4)) {
            printf("Invalid input.\nChoose: ");
        }
        switch(intake)
        {
            case 1:
                readClasses();
                printClasses();
                break;
            case 2:
                break;
            case 3:
                break;
        }

    } while (intake != 4);

    return 0;
}
