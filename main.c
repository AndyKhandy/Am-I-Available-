#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXLEN 100
#define MINUTESINDAY 1440
#define HOURSINHALFDAY 12
#define START 1
#define END 0

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

void clearBuffer() {
    int c;
    while ((c=getchar()) != '\n' && c != EOF);
    return;
}

int timeToMinutes(ClassNode* node, int choice) {
    if(choice)
    {
        return (node->start_hour*60 + node->start_mins);
    }
    return (node->end_hour*60 + node->end_mins);
}

void printTime(int startMinutes, int endMinutes)
{
    //use width and padding to make the display look good
    int hoursEndDisplay = endMinutes / 60;
    int hoursStartDisplay = startMinutes / 60;

    int minutesEndDisplay = endMinutes % 60;
    int minutesStartDisplay = startMinutes % 60;

    if(hoursStartDisplay >= HOURSINHALFDAY)
    {
        hoursStartDisplay %= 12;
        if(hoursStartDisplay == 0)
        {
            hoursStartDisplay = 12;
        }
        printf("%02d:%02dpm-", hoursStartDisplay, minutesStartDisplay);
    }
    else{
        printf("%02d:%02dam-", hoursStartDisplay, minutesStartDisplay);
    }

    if(hoursEndDisplay >= HOURSINHALFDAY)
    {
        hoursEndDisplay %= 12;
        if(hoursEndDisplay == 0)
        {
            hoursEndDisplay = 12;
        }
        
        printf("%02d:%02dpm\n", hoursEndDisplay, minutesEndDisplay);
    }
    else{
        printf("%02d:%02dam\n", hoursEndDisplay, minutesEndDisplay);
    }
}

//day can be any letter with MTWYF. Ex M, TY, MWF, etc.
void findFreeTime()
{

    if(headMain == NULL)
    {
        printf("No Classes :(\n");
        printf("Free time from 0:00 to 24:00\n");
        return;
    }

    int choice = 0;
    char day;

    do {
         printf("\nWhich day would you like to see your free time for? (1-5): ");
    } while(scanf("%d", &choice) <= 0 && (choice < 1 || choice > 5));
    
    switch(choice)
    {
        case 1:
            day = 'M';
            printf("Scheduele for Monday!\n\n");
            break;
        case 2:
            day = 'T';
            printf("Schedule for Tuesday\n\n");
            break;
        case 3:
            day = 'W';
            printf("Schedule for Wednesday\n\n");
            break;
        case 4:
            day = 'Y';
            printf("Schedule for Thursday\n\n");
            break;
        case 5:
            day = 'F';
            printf("Schedule for Friday\n\n");
            break;
    }

    int previousMinutes = 0;
    ClassNode* temp = headMain;
    while(temp != NULL)
    {
            if(strchr(temp->days, day) != NULL)
            {
                printf("Free time from ");
                printTime(previousMinutes,timeToMinutes(temp,START)-5);
                printf("[%s] Class time from ", temp->name);
                printTime(timeToMinutes(temp,START), timeToMinutes(temp,END));
                previousMinutes = timeToMinutes(temp,END);
            }
            temp = temp->next;
    }
    printf("Free time from ");
    printTime(previousMinutes, MINUTESINDAY);
    printf("\n");
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
        printf("%s %s ", temp->name, temp->days);
        printTime(timeToMinutes(temp,START), timeToMinutes(temp,END));
        temp = temp->next;
    }
    printf("\n");
}

void printDays()
{
    printf("1. Monday\n");
    printf("2. Tuesday\n");
    printf("3. Wednesday\n");
    printf("4. Thursday\n");
    printf("5. Friday\n");
}



void readClasses() {
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
        } else if (timeToMinutes(headMain,1) > timeToMinutes(class,1)) {
            class->next = headMain;
            headMain = class;
        } else {
            ClassNode* temp = headMain;
            //looking for last node that is either null or bigger
            while(temp->next != NULL && timeToMinutes(temp->next,1) < timeToMinutes(class,1))
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

void writeClasses(ClassNode* list, int num, const char* filename) {
	FILE* fptr;
    fptr = fopen(filename, "w");
    if (fptr == NULL)
    {
		printf("Error!");
		exit(1);
    }
	
	//hope this works
	for (int i = 0; i < num; i++) {
		fprintf(fptr, "%s~%s~%d:%d-%d:%d\n", (list+i)->name, (list+i)->days, (list+i)->start_hour, (list+i)->start_mins, (list+i)->end_hour, (list+i)->end_mins);
	}
	
	fclose(fptr);
	
	printf("\nSchedule written to file.\n\n");
}

void querySchedule() {
	int num;
	printf("\nEnter # of classes: ");
	while (scanf("%d", &num) <= 0 || num < 0) {
		printf("Invalid input.\nEnter number of classes: ");
		clearBuffer();
	}
	clearBuffer();
	
	if (num == 0)
		return;
	
	ClassNode* classes = (ClassNode*) malloc(sizeof(ClassNode)*num);
	
	//time to get info for classes
	//name, days, start, end  
	for (int i = 0; i < num; i++) {
		printf("\nEnter name of class: ");
		char name[MAXLEN];
		fgets(name, MAXLEN, stdin);
		name[strcspn(name, "\n")] = '\0';
		
		char days[6];
		int n = -1;
		
		printf("Is it on Monday? (Y/N): ");
		if (tolower(getchar()) == 'y' && ++n>=0)
			days[n] = 'M';
		getchar(); //eat the newline
		printf("Is it on Tuesday? (Y/N): ");
		if (tolower(getchar()) == 'y' && ++n>=0)
			days[n] = 'T';
		getchar(); //eat the newline
		printf("Is it on Wednesday? (Y/N): ");
		if (tolower(getchar()) == 'y' && ++n>=0)
			days[n] = 'W';
		getchar(); //eat the newline
		printf("Is it on Thursday? (Y/N): ");
		if (tolower(getchar()) == 'y' && ++n>=0)
			days[n] = 'Y';
		getchar(); //eat the newline
		printf("Is it on Friday? (Y/N): ");
		if (tolower(getchar()) == 'y' && ++n>=0)
			days[n] = 'F';
		getchar(); //eat the newline
		days[n+1] = '\0';
		
		printf("What time does it start? (0:00-23:59): ");
		int startHour, startMinute;
		while (scanf("%d:%d", &startHour, &startMinute) != 2 || startHour < 0 || startHour > 23 || startMinute < 0 || startMinute > 59) {
			printf("Invalid time! Must be 24 hour format\nWhat time does it start? (0:00-23:59): ");
			clearBuffer();
		}		
		clearBuffer();
		
		printf("What time does it end? (0:00-23:59): ");
		int endHour, endMinute;
		while (scanf("%d:%d", &endHour, &endMinute) != 2 || endHour < 0 || endHour > 23 || endMinute < 0 || endMinute > 59) {
			printf("Invalid time! Must be 24 hour format\nWhat time does it end? (0:00-23:59): ");
			clearBuffer();
		}		
		clearBuffer();
		
		(classes+i)->start_hour = startHour;
		(classes+i)->start_mins = startMinute;
		(classes+i)->end_hour = endHour;
		(classes+i)->end_mins = endMinute;
		strcpy((classes+i)->name, name);
		strcpy((classes+i)->days, days);
	}
	
	writeClasses(classes, num, "student.txt");
}

int main()
{ 
    int intake;
    do {
        printf("== Class Management System ==\n1. Read Schedule\n2. Find Free Time\n3. Make New Schedule\n4. Exit\n\nChoose: ");
        while (scanf("%d", &intake) >= 0 && (intake < 1 || intake > 4)) {
            printf("Invalid input.\nChoose: ");
        }

        char choice;
        switch(intake)
        {
            case 1:
                readClasses();
                printClasses();
                break;
            case 2:
                printDays();
                findFreeTime();
                break;
            case 3:
				querySchedule();
                break;
        }

    } while (intake != 4);

    return 0;
}
