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
ClassNode* headInput = NULL;

//clears the buffer whenever a user inputs an invalid input
void clearBuffer() {
    int c;
    while ((c=getchar()) != '\n' && c != EOF);
    return;
}

//in this program time is convered to minutes for easier calculations + comparisons
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

    //easier printing
    if(startMinutes == 0 )
    {
        printf("12:00am-");
    }
    else if(hoursStartDisplay >= HOURSINHALFDAY)
    {
        //converts the hours to the 12 hour clock version
        hoursStartDisplay %= 12;

        //time in the 12 hour clock doesn't use 0 
        if(hoursStartDisplay == 0)
        {
            hoursStartDisplay = 12;
        }
        printf("%02d:%02dpm-", hoursStartDisplay, minutesStartDisplay);
    }
    else{
        printf("%02d:%02dam-", hoursStartDisplay, minutesStartDisplay);
    }

    //easier printing
    if(endMinutes == MINUTESINDAY)
    {
        printf("12:00am\n");
    }
    else if(hoursEndDisplay >= HOURSINHALFDAY) {
        //same logic done in the start time display
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

//makes the code more neat
void printDays()
{
    printf("1. Monday\n");
    printf("2. Tuesday\n");
    printf("3. Wednesday\n");
    printf("4. Thursday\n");
    printf("5. Friday\n");
}

//day can be any letter with MTWYF. Ex M, TY, MWF, etc.
void findFreeTime()
{
    if(headMain == NULL)
    {
        printf("No Classes :(\n");
        printf("Free time from 0:00 to 24:00\n\n");
        return;
    }

    printDays();

    int choice = 0;
    char day;

    do {
         printf("\nWhich day would you like to see your free time for? (1-5): ");
    } while(scanf("%d", &choice) <= 0 && (choice < 1 || choice > 5));
    
    printf("\n == ");
    switch(choice)
    {
        case 1:
            day = 'M';
            printf("SCHEDULE FOR MONDAY!");
            break;
        case 2:
            day = 'T';
            printf("SCHEDULE FOR TUESDAY!");
            break;
        case 3:
            day = 'W';
            printf("SCHEDULE FOR WEDNESDAY!");
            break;
        case 4:
            //uses Y instead of TH since TH is not a single char
            day = 'Y';
            printf("SCHEDULE FOR THURSDAY!");
            break;
        case 5:
            day = 'F';
            printf("SCHEDULE FOR FRIDAY!");
            break;
    }
    printf(" ==\n");

    int previousMinutes = 0;
    ClassNode* temp = headMain;
    while(temp != NULL)
    {
            //checks if the class days has the specific day character (i.e F for Friday)
            if(strchr(temp->days, day) != NULL)
            {
                printf("Free time from ");
                //displays a formatted 12 hour time
                printTime(previousMinutes,timeToMinutes(temp,START)-5);
                printf("[%s] Class time from ", temp->name);
                printTime(timeToMinutes(temp,START), timeToMinutes(temp,END));
                previousMinutes = timeToMinutes(temp,END);
            }
            temp = temp->next;
    }
    //displays the final free time since it's at the end of all the classes assuming there is no class you take at midnight like 10pm to 12pm
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

    printf("== SCHEDULE FROM FILE == \n");
    ClassNode* temp = headMain;
    while(temp != NULL)
    {
        //prints the class information just like how it's formatted in the txt file but with 12 hour clock time
        printf("%s %s ", temp->name, temp->days);
        printTime(timeToMinutes(temp,START), timeToMinutes(temp,END));
        temp = temp->next;
    }
    printf("\n");
}

int timeConflict(ClassNode* class)
{
    if(headInput == NULL)
    {
        return 0;
    }
    else{
        ClassNode* temp = headInput;
        while(temp!=NULL)
        {
            if(strstr(temp->days, class->days) != NULL)
            {
                int classStartMinutes = timeToMinutes(class, START);
                int tempStartMinutes = timeToMinutes(temp, START);
                int tempEndMinutes = timeToMinutes(temp, END);
                if(classStartMinutes >= tempStartMinutes && classStartMinutes <= tempEndMinutes)
                {
                    printf("There is a time conflict between %s and %s!\n", class->name, temp->name);
                    printf("%s lasts from ", temp->name);
                    printTime(tempStartMinutes, tempEndMinutes);
                    printf("Please enter a start time that doesn't conflict!\n");
                    return 1;
                }
            }
            temp = temp->next;
        }
    }
    return 0;
}

void readClasses() {
    int classes = 0;
    char filename[50];
    printf("What file would you like to read your classes from?: [DON'T INCLUDE .TXT AT END] (Ex). student): ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename,"\n")] = '\0';

    //adds .txt to the end so the user doesn't have to input it
    strcat(filename, ".txt");

    FILE* fptr;
    fptr = fopen(filename, "r");
    //check if the file opened properly
    if (fptr == NULL)
    {
    printf("Error!");
    exit(1);
    }

    ClassNode* class = (ClassNode*)malloc(sizeof(ClassNode));

    //uses scanset to get information all the way until ~ sign
    while(fscanf(fptr, "%[^~]~%[^~]~%s\n", class->name, class->days, class->time) == 3)
    {
        //get's the individual integers from the time display
        //ie 10:25-10:40 would get 10, 25, 10, 40
        char* times = strtok(class->time, "-");

        //flag variable to swtich from start times to end times
        int start = 1;

        while(times != NULL)
        {
            //checks to see if the start or end time has minutes or not (ex 10:40)
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
            //if the time doesn't have minutes (ex 15)
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

        class->next = NULL;

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

void writeClasses(int num, const char* filename) {
	FILE* fptr;
    fptr = fopen(filename, "w");
    if (fptr == NULL)
    {
		printf("Error!");
		exit(1);
    }
	
	//hope this works
    ClassNode* temp = headInput;
    while(temp != NULL)
    {
        fprintf(fptr, "%s~%s~%02d:%02d-%02d:%02d\n", temp->name, temp->days, temp->start_hour, temp->start_mins, temp->end_hour, temp->end_mins);
    }
	
	
	fclose(fptr);
	
	printf("\nSchedule written to file.\n\n");
}

void querySchedule() {
	int num;
    char filename[100];

    printf(" == CUSTOMIZE SCHEDULE ==\n");
    printf("Enter the name of the file you wish to save the schedule to [DON'T INCLUDE .TXT AT END](Ex. schedule): ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename,"\n")] = '\0';
    strcat(filename, ".txt");

    //clearBuffer() is used to clear input buffer so user can input a new slate of input
	printf("Enter # of classes: ");
	while (scanf("%d", &num) <= 0 || num < 0) {
		printf("Invalid input.\nEnter number of classes: ");
		clearBuffer();
	}
	clearBuffer();
	
	if (num == 0)
		return;
	
	
	//time to get info for classes
	//name, days, start, end  
	for (int i = 0; i < num; i++) {
        ClassNode* classes = (ClassNode*) malloc(sizeof(ClassNode));
		printf("\nEnter name of class: ");
		char name[MAXLEN];
		fgets(name, MAXLEN, stdin);
		name[strcspn(name, "\n")] = '\0';

        strcpy(classes->name, name);
		
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
        strcpy(classes->days, days);
		
        int startHour, startMinute;

        do{
            printf("What time does it start? (0:00-23:59): ");
            while (scanf("%d:%d", &startHour, &startMinute) != 2 || startHour < 0 || startHour > 23 || startMinute < 0 || startMinute > 59) {
                printf("Invalid time! Must be 24 hour format\nWhat time does it start? (0:00-23:59): ");
                clearBuffer();
            }		
            clearBuffer();
            classes->start_hour = startHour;
		    classes->start_mins = startMinute;
        } while(timeConflict(classes));
		
		printf("What time does it end? (0:00-23:59): ");
		int endHour, endMinute;
		while (scanf("%d:%d", &endHour, &endMinute) != 2 || endHour < 0 || endHour > 23 || endMinute < 0 || endMinute > 59) {
			printf("Invalid time! Must be 24 hour format\nWhat time does it end? (0:00-23:59): ");
			clearBuffer();
		}		
		clearBuffer();
		
		classes->end_hour = endHour;
		classes->end_mins = endMinute;

        if(headInput == NULL)
        {
            headInput = classes;
        }
        else{
            ClassNode* temp = headInput;
            while(temp->next != NULL)
            {
                temp = temp->next;
            }
            temp->next = classes;
        }
	}
	
	writeClasses(num, filename);
}

int main()
{ 
    int intake;
    do {
        printf("== Class Management System ==\n1. Read Schedule\n2. Find Free Time\n3. Make New Schedule\n4. Exit\n\nChoose: ");
        while (scanf("%d", &intake) >= 0 && (intake < 1 || intake > 4)) {
            printf("Invalid input.\nChoose: ");
        }
        getchar();

        char choice;
        switch(intake)
        {
            case 1:
                readClasses();
                printClasses();
                break;
            case 2:
                findFreeTime();
                break;
            case 3:
				querySchedule();
                break;
        }

    } while (intake != 4);

    return 0;
}
