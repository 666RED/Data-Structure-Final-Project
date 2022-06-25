#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

//Struct Declaration
typedef struct employeeData{
  char name[51];
  char ID[5];
  int age;
  float grossSalary;
}employeeData;

typedef struct employee{
  employeeData data;
  struct employee *next;
  struct employee *prev;
}employee;


//Global Head Pointer
employee* head = NULL;


//Main
void readFile();
void menu();
void displayOption();
void createFile();


//Get Data
void inputData();
void modifyData();
void updateData();
void insertNewEmployee();
employee* getNewNode(char name[], char ID[], int, float);


//Sort
void sortList();
void insertSort(int);
void insert(employee*, employee*, int);
void changeData(employee**, employee**);
void reverseList();


//Delete
void deleteData();
void deleteSingleData();
void deleteAllData();


//Display
void displayData();
void displayAllData();
void displaySingleDetail();
void displaySingleData(employee*);


//Validity
char* validName();
int repeatedName(char name[]);
char* validID();
int repeatedID(char id[]);
int validAge();
float validGrossSalary();


//Calculate
int calcInsurance(int);
float calcIncomeTax(float);


//Number & Integer
int isNumber(char option[]);
int isInteger(char input[]);


//Other Function
employee* searchID();
int exitAction();
void lower(char name[]);
void capitalized(char name[]);


/////////////////////////////////////////////Main Program/////////////////////////////////////////////

int main(){
	readFile();
    menu();
	createFile();
}


////////////////////////////////////////////Function Definition////////////////////////////////////////////

//Main
void readFile(){
    int numOfEmployee;
    employee* temp, *temp2;
    FILE *myFile;
    char prevName[50];
    char prevID[5];
    int prevAge;
    float prevGrossSalary;

    myFile = fopen("C:\\Users\\User\\Desktop\\Data.txt", "r");
    if(myFile != NULL){
        fscanf(myFile, "%d", &numOfEmployee);
        for(int i = 0; i < numOfEmployee; i++){
            fscanf(myFile, "\n%[^\n]s", prevName);
            fscanf(myFile, "%s %d %f", prevID, &prevAge, &prevGrossSalary);
            temp = getNewNode(prevName, prevID, prevAge, prevGrossSalary);
            if(head == NULL){
                head = temp;
            }else{
                temp2 = head;
                while(temp2->next != NULL){
                    temp2 = temp2->next;
                }
                temp2->next = temp;
                temp->prev = temp2;
            }
        }
    }
    fclose(myFile);
}

void menu(){
    int result, option;
    char input[1];

    do{
        do{
            displayOption();
            scanf("\n%[^\n]s", input);
            result = isNumber(input);
            if(result != 1){
                printf("\nInvalid input.\n\n");
                system("pause");
                system("cls");
            }else{
                option = atoi(input);
            }
        }while(result != 1);

        switch(option){
            case 1:
                if(head == NULL){
                    system("cls");
                    inputData();
                }else{
                    system("cls");
                    modifyData();
                }
            break;
            case 2:
                system("cls");
                updateData();
                break;
            case 3:
                system("cls");
                insertNewEmployee();
                break;
            case 4:
                system("cls");
                sortList();
                break;
            case 5:
                system("cls");
                deleteData();
                break;
            case 6:
                system("cls");
                displayData();
                break;
            case 7:
                printf("\nThank you for using this system.\n\n");
                system("pause");
                break;
            default:
                printf("\nInvalid input.\n\n");
                system("pause");
        }
        system("cls");
    }while(option != 7);
}

void displayOption(){
    printf("\n ---------------------------------");
    printf("\n| Press 1 - Input Employee Data\t |\n| Press 2 - Update Employee Data |\n| Press 3 - Insert New Employee\t |\n| Press 4 - Sort List\t\t |\n| Press 5 - Delete Employee Data |\n| Press 6 - Display Data\t |\n| Press 7 - Exit\t\t |\n");
    printf(" ---------------------------------\n");
    printf("\nInput:");
}

void createFile(){
    FILE *myFile, *myFile2;
    int num = 0;
    employee* temp = head;
    while(temp != NULL){
        num++;
        temp = temp->next;
    }

    myFile = fopen("C:\\Users\\User\\Desktop\\Data.txt", "w");
    fprintf(myFile, "%d", num);
    temp = head;
    while(temp != NULL){
        fprintf(myFile, "\n%s\n%s %d %.2f ", temp->data.name, temp->data.ID, temp->data.age, temp->data.grossSalary);
        temp = temp->next;
    }
    fclose(myFile);

    myFile2 = fopen("C:\\Users\\User\\Desktop\\Table.txt", "w");
    temp = head;
    int i=1;
    float totalGrossSalary = 0.0;
    int totalInsurance = 0;
    float totalIncomeTax = 0.0;
    float totalNetSalary = 0.0;

    fprintf(myFile2, "\n-------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    fprintf(myFile2, "| No |\t\t\t\tName\t\t\t\t|   ID   |  Age  |  Gross Salary (RM)  |  Insurance (RM)  |  Income Tax (RM)  |  Net Salary (RM)  |\n");
    fprintf(myFile2, "|----|----------------------------------------------------------|--------|-------|---------------------|------------------|-------------------|-------------------|\n");
    while(temp != NULL){
        totalGrossSalary += temp->data.grossSalary;
        int insurance = calcInsurance(temp->data.age);
        totalInsurance += insurance;
        float incomeTax = calcIncomeTax(temp->data.grossSalary);
        totalIncomeTax += incomeTax;
        float netSalary = temp->data.grossSalary - insurance - incomeTax;
        totalNetSalary += netSalary;
        fprintf(myFile2, "|%3d |%-57s |%6s  |%5d  |%14.2f       |%11d       |%12.2f\t      |%12.2f\t  |\n",i, temp->data.name, temp->data.ID, temp->data.age, temp->data.grossSalary, insurance, incomeTax, netSalary);
        if(temp->next == NULL){
            fprintf(myFile2, "|----|---------------------------------------------------------------------------|---------------------|------------------|-------------------|-------------------|\n");
        }else{
            fprintf(myFile2, "|----|----------------------------------------------------------|--------|-------|---------------------|------------------|-------------------|-------------------|\n");
        }
        temp = temp->next;
        i++;
    }

    fprintf(myFile2, "|    |\t\t\t\t\t\t\t\t\t   Total |%14.2f       |%11d       |%12.2f\t      |%12.2f\t  |\n", totalGrossSalary, totalInsurance, totalIncomeTax, totalNetSalary);
    fprintf(myFile2, "-------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    fclose(myFile2);
}


//Get Data
void inputData(){
    int result,age, number, repeat, quantity = 1;
    float grossSalary;
    char numberOfEmployee[5], name[50], id[5];

    printf("\n-------------------------------------------------------------\n\t\t\tInput Data\n-------------------------------------------------------------\n");
    do{
        printf("\nPlease input the number of employee you want to enter:");
        scanf("\n%[^\n]s",numberOfEmployee);
        result = isInteger(numberOfEmployee);
        if(result!=1){
            printf("\nInvalid Input.\n");
        }else{
            number = atoi(numberOfEmployee);
        }
    }while(result!=1);


    for(int i = 0; i < number; i++){
        printf("\nEmployee %d\n", quantity);
        printf("------------");
        quantity++;

        strcpy(name, validName());
        if(strcmp(name, "stop")== 0){
            return;
        }
        do{
            strcpy(id, validID());
            repeat = repeatedID(id);
            if(repeat == 1){
                printf("\nThe ID is repeated.\n");
            }
        }while(repeat == 1);

        age = validAge();
        grossSalary = validGrossSalary();
        employee* temp = getNewNode(name,id,age,grossSalary);
        employee* current = head;

        if(temp == NULL){
            printf("\nNo Enough Memory.\n");
            return;
        }else{
            if(head==NULL){
                head = temp;
            }else{
                while(current->next != NULL){
                    current = current->next;
                }
                current->next = temp;
                temp->prev = current;
            }
        }
        system("cls");
    }
    printf("\nYour have done the input process.\n\n");
    system("pause");
}

void modifyData(){
    char input[1];
    int result, option;

    do{
        do{
            printf("\nPress 1 - Create New Data (All data will be deleted)\nPress 2 - Input New Data\nPress 3 - Insert new data\nPress 4 - Back to menu\n\nInput:");
            scanf("\n%[^\n]s", input);
            result = isNumber(input);
            if(result != 1){
                printf("\nInvalid input.\n");
            }else{
                option = atoi(input);
            }
        }while(result != 1);

        switch(option){
            case 1:
                deleteAllData();
                inputData();
                return;
            case 2:
                system("cls");
                inputData();
                return;
            case 3:
                system("cls");
                insertNewEmployee();
            return;
                case 4:
                return;
            default:
                printf("\nInvalid input.\n");
        }
    }while(option != 4);
}

void updateData(){
    if(head == NULL){
        printf("\nThe list is empty.\n\n");
        system("pause");
        return;
    }
    printf("\n-------------------------------------------------------------\n\t\tUpdate Data\n-------------------------------------------------------------\n");
    char input[1], name[50], ID[5];
    int result, option, exit, age;
    float grossSalary;

    employee* temp = searchID();
    if(temp == NULL){
        return;
    }
    displaySingleData(temp);
    do{
        do{
            printf("\n1. Name\n2. ID\n3. Age\n4. Gross Salary\n5. Back to menu\n\nEnter the number to update the data or exit.\nInput:");
            scanf("\n%[^\n]s", input);
            result = isNumber(input);
            if(result != 1){
                printf("\nInvalid input.\n");
            }else{
                option = atoi(input);
            }
        }while(result != 1);

        switch(option){
            case 1:
                system("cls");
                strcpy(name, validName());
                if(strcmp(name, "stop") == 0){
                    return;
                }
                if(repeatedName(name) == 1){
                    printf("\nThis name is repeated.\n");
                }else{
                    strcpy(temp->data.name, name);
                    system("cls");
                    displaySingleData(temp);
                }
                exit = exitAction();
                if(exit == 1){
                    return;
                }else{
                    system("cls");
                    printf("\nCurrent ID: %s\n", temp->data.ID);
                    break;
                }
            case 2:
                system("cls");
                strcpy(ID, validID());
                if(repeatedID(ID) == 1){
                    printf("\nThis ID is repeated.\n");
                }else{
                    strcpy(temp->data.ID, ID);
                    system("cls");
                    displaySingleData(temp);
                }
                exit = exitAction();
                if(exit == 1){
                    return;
                }else{
                    system("cls");
                    printf("\nCurrent ID: %s\n", temp->data.ID);
                    break;
                }
            case 3:
                system("cls");
                age = validAge();
                temp->data.age = age;
                system("cls");
                displaySingleData(temp);
                exit  = exitAction();
                if(exit == 1){
                    return;
                }else{
                    system("cls");
                    printf("\nCurrent ID: %s\n", temp->data.ID);
                    break;
                }
            case 4:
                system("cls");
                grossSalary = validGrossSalary();
                temp->data.grossSalary = grossSalary;
                system("cls");
                displaySingleData(temp);
                exit = exitAction();
                if(exit == 1){
                    return;
                }else{
                    system("cls");
                    printf("\nCurrent ID: %s\n", temp->data.ID);
                    break;
                }
            case 5:
                return;
            default:
                printf("\nInvalid input.\n");
        }
    }while(option != 5 || exit != 1);
}

void insertNewEmployee(){
    printf("\n-------------------------------------------------------------\n\t\tInsert New Employee\n-------------------------------------------------------------\n");
    char name[50], ID[5];
    int age, repeat;
    float grossSalary;

    strcpy(name, validName());
    if(strcmp(name, "stop") == 0){
        return;
    }
    do{
        strcpy(ID, validID());
        repeat = repeatedID(ID);
        if(repeat == 1){
            printf("\nThis ID is repeated.\n");
        }
    }while(repeat == 1);

    age = validAge();
    grossSalary = validGrossSalary();
    employee* temp = getNewNode(name, ID, age, grossSalary);

    if(temp == NULL){
        printf("\nNo enough memory.\n");
        return;
    }
    if(head == NULL){
        head = temp;
        return;
    }
    employee* current = head;
    while(current->next != NULL){
        current = current->next;
    }
    current->next = temp;
    temp->prev = current;

    system("cls");
    printf("\nYour have done the insertion process.\n\n");
    system("pause");
}

employee* getNewNode(char name[], char ID[], int age, float grossSalary){
    employee* temp = (employee*)malloc(sizeof(employee));
    if(temp == NULL){
        return NULL;
    }else{
        strcpy(temp->data.name, name);
        strcpy(temp->data.ID, ID);
        temp->data.age = age;
        temp->data.grossSalary = grossSalary;
        temp->next = NULL;
        temp->prev = NULL;
        return temp;
    }
}


//Sort
void sortList(){
    if(head == NULL){
        printf("\nThe list is empty.\n\n");
        system("pause");
        return;
    }
    if(head->next == NULL){
        printf("\nThere is only one employee.\n");
        displayAllData();
        system("pause");
        return;
    }

    char input[1], order[1];
    int result, option, answer;
    do{
        do{
            printf("\n-------------------------------------------------------------\n\t\t\tSort List\n-------------------------------------------------------------\n");
            printf("\nSorting:\nPress 0 - Back to menu\nPress 1 - Name\nPress 2 - Age\nPress 3 - Salary\nPress 4 - ID\nPress 5 - Reverse the list\n\nInput:");
            scanf("\n%[^\n]s", input);
            result = isNumber(input);
            if(result != 1){
                printf("\nInvalid input.\n");
                system("pause");
                system("cls");

            }else{
                option = atoi(input);
            }
        }while(result != 1);

        if(option == 0){
            return;
        }else if(option == 1 || option == 2 || option == 3 || option == 4){
            insertSort(option);
            do{
                do{
                    printf("\nPress 1 - Ascending Order\nPress 2 - Descending Order\n\nInput:");
                    scanf("\n%[^\n]s", order);
                    result = isNumber(order);
                    if(result != 1){
                        printf("\nInvalid input.\n");
                    }else{
                        answer = atoi(order);
                        system("cls");
                    }
                }while(result != 1);

                if(answer == 1){
                    displayAllData();
                    system("pause");
                    return;
                }else if(answer == 2){
                    reverseList();
                    system("pause");
                    return;
                }else{
                    printf("\nInvalid input.\n");
                }
            }while(true);
        }else if(option == 5){
            system("cls");
            reverseList();
            system("pause");
            return;
        }else{
            printf("\nInvalid input.\n");
        }
    }while(option != 0);
}

void insertSort(int input){
    employee* curr = head;
    employee* temp = head->next;
    while(temp != NULL){
        insert(curr, temp, input);
        curr = curr->next;
        temp = temp->next;
    }
}

void insert(employee* curr, employee* temp, int input){
    employeeData final = temp->data;
    if(input == 1){
        while(curr != NULL && strcmp(curr->data.name, final.name) > 0){
        changeData(&curr, &temp);
        }
    }else if(input == 2){
        while(curr != NULL && curr->data.age > final.age){
            changeData(&curr, &temp);
        }
    }else if(input == 3){
        while(curr != NULL && curr->data.grossSalary > final.grossSalary){
            changeData(&curr, &temp);
        }
    }else if(input == 4){
        while(curr != NULL && strcmp(curr->data.ID, final.ID) > 0){
            changeData(&curr, &temp);
        }
    }
    temp->data = final;
}

void changeData(employee** curr, employee** temp){
    (*temp)->data = (*curr)->data;
    *temp = (*temp)->prev;
    *curr = (*curr)->prev;
}

void reverseList(){
    employee* temp2 = head;
    do{
        employee* temp = temp2->prev;
        temp2->prev = temp2->next;
        temp2->next = temp;
        if(temp2->prev != NULL){
            temp2 = temp2->prev;
        }
    }while(temp2->prev != NULL);

    head = temp2;
    displayAllData();
}


//Delete
void deleteData(){

    if(head == NULL){
        printf("\nThe list is empty.\n\n");
        system("pause");
        return;
    }
    if(head->next == NULL){
        char input[1];
        int option, result;
        printf("\nThere is only one employee.\n");
        displayAllData();
        do{
            printf("\nDo you want to delete this employee?\n\nPress 0 - Exit\nPress 1 - Delete\n\nInput:");
            scanf("\n%[^\n]s", input);
            result = isNumber(input);
            if(result != 1){
                printf("\nInvalid input.\n");
            }else{
                option = atoi(input);
            }
        }while(result != 1);
        if(option == 1){
            deleteAllData();
            return;
        }else if(option == 0){
            return;
        }else{
            printf("\nInvalid input.\n");
        }
    }
    printf("\n-------------------------------------------------------------\n\t\t\tDelete Data\n-------------------------------------------------------------\n");

    char input[1];
    int result, option;
    do{
        do{
            printf("\nPress 1 - Delete Single Employee Data\nPress 2 - Delete All Employee Data\nPress 3 - Back to menu\n");
            printf("\nInput:");
            scanf("\n%[^\n]s", input);
            result = isNumber(input);
            if(result != 1){
                printf("\nInvalid input.\n");
            }else{
                option = atoi(input);
            }
        }while(result != 1);

        if(option == 1){
            if(head->next == NULL){
                system("cls");
                char input[1];
                int option, result;
                printf("\nThere is only one employee.\n");
                displayAllData();
                do{
                    printf("\nDo you want to delete this employee?\n\nPress 0 - Exit\nPress 1 - Delete\n\nInput:");
                    scanf("\n%[^\n]s", input);
                    result = isNumber(input);
                    if(result != 1){
                        printf("\nInvalid input.\n");
                    }else{
                        option = atoi(input);
                    }
                }while(result != 1);
                if(option == 1){
                    deleteAllData();
                    return;
                }else if(option == 0){
                    return;
                }else{
                    printf("\nInvalid input.\n");
                }
            }else{
                deleteSingleData();
                if(head == NULL){
                    return;
                }
            }
        }else if(option == 2){
            deleteAllData();
            return;
        }else if(option == 3){
            return;
        }else{
            printf("\nInvalid input.\n");
        }
    }while(option != 3);
}

void deleteSingleData(){
    employee* temp = searchID();
    if(temp == NULL){
        system("cls");
        return;
    }
    if(temp == head){
        if(temp->next != NULL){
            head->next->prev = NULL;
            head = head->next;
            free(temp);
        }else{
            head = NULL;
            free(temp);
        }
    }else{
        employee* previous = temp->prev;
        if(temp->next != NULL){
            employee* previous = temp->prev;
            previous->next = temp->next;
            temp->next->prev = previous;
            free(temp);
        }else{
            previous->next = NULL;
            free(temp);
        }

    }
    printf("\nThe data is deleted.\n\n");
    system("pause");
    system("cls");
}

void deleteAllData(){
    while(head != NULL){
        employee* temp = head;
        head = head->next;
        free(temp);
    }
    printf("\nAll data have been deleted.\n\n");
    system("pause");
    system("cls");
}


//Display
void displayData(){
    if(head == NULL){
        printf("\nThe list is empty.\n\n");
        system("pause");
        return;
    }
    if(head->next == NULL){
        displayAllData();
        system("pause");
        return;
    }
    printf("\n-------------------------------------------------------------\n\t\tDisplay Data\n-------------------------------------------------------------\n");

    char input[1];
    int result, option;
    do{
        do{
            printf("\nPress 1 - Display All Data\nPress 2 - Display Single Data\nPress 3 - Back to menu\n\nInput:");
            scanf("\n%[^\n]s", input);
            result = isNumber(input);
            if(result != 1){
                printf("\nInvalid input.\n");
                system("cls");
            }else{
                option = atoi(input);
            }
        }while(result != 1);

        if(option == 1){
            system("cls");
            displayAllData();
            system("pause");
            return;
        }else if(option == 2){
            system("cls");
            displaySingleDetail();
        }else if(option == 3){
            return;
        }else{
            printf("\nInvalid input.\n");
        }
    }while(option != 3);
}

void displayAllData(){
    int i=1;
    employee* temp = head;
    float totalGrossSalary = 0.0;
    int totalInsurance = 0;
    float totalIncomeTax = 0.0;
    float totalNetSalary = 0.0;

    printf("\n-------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("| No |\t\t\t\tName\t\t\t\t|   ID   |  Age  |  Gross Salary (RM)  |  Insurance (RM)  |  Income Tax (RM)  |  Net Salary (RM)  |\n");
    printf("|----|----------------------------------------------------------|--------|-------|---------------------|------------------|-------------------|-------------------|\n");
    while(temp != NULL){
        totalGrossSalary += temp->data.grossSalary;
        int insurance = calcInsurance(temp->data.age);
        totalInsurance += insurance;
        float incomeTax = calcIncomeTax(temp->data.grossSalary);
        totalIncomeTax += incomeTax;
        float netSalary = temp->data.grossSalary - insurance - incomeTax;
        totalNetSalary += netSalary;
        printf("|%3d |%-57s |%6s  |%5d  |%14.2f       |%11d       |%12.2f\t      |%12.2f\t  |\n",i, temp->data.name, temp->data.ID, temp->data.age, temp->data.grossSalary, insurance, incomeTax, netSalary);
        if(temp->next == NULL){
            printf("|----|---------------------------------------------------------------------------|---------------------|------------------|-------------------|-------------------|\n");
        }else{
            printf("|----|----------------------------------------------------------|--------|-------|---------------------|------------------|-------------------|-------------------|\n");
        }
        temp = temp->next;
        i++;
    }
    printf("|    |\t\t\t\t\t\t\t\t\t   Total |%14.2f       |%11d       |%12.2f\t      |%12.2f\t  |\n", totalGrossSalary, totalInsurance, totalIncomeTax, totalNetSalary);
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
}

void displaySingleDetail(){
    employee* temp = searchID();
    if(temp == NULL){
        system("cls");
        return;
    }
    int i = 1;
    int insurance = calcInsurance(temp->data.age);
    float incomeTax = calcIncomeTax(temp->data.grossSalary);
    float netSalary = temp->data.grossSalary - insurance - incomeTax;

    printf("\n-------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("| No |\t\t\t\tName\t\t\t\t|  ID  | Age | Gross Salary (RM) | Insurance (RM) | Income Tax (RM) | Net Salary (RM) |\n");
    printf("|----|----------------------------------------------------------|------|-----|-------------------|----------------|-----------------|-----------------|\n");
    printf("|%3d | %-57s| %s |  %d |%13.2f\t |%10d\t  |%11.2f\t    |%11.2f      |\n",i, temp->data.name, temp->data.ID, temp->data.age, temp->data.grossSalary, insurance, incomeTax, netSalary);
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    system("pause");
    system("cls");
}

void displaySingleData(employee* temp){
    printf("\nName: %s\nID: %s\nAge: %d\nGross Salary: %.2f\n", temp->data.name, temp->data.ID, temp->data.age, temp->data.grossSalary);
}


//Validity
char* validName(){
    int result;
    char* name = (char*)malloc(sizeof(head->data.name));

    do{
        do{
            result = 1;
            printf("\nName (Type 'stop' to stop entering data):");
            scanf("\n%51[^\n]s",name);
            fflush(stdin);
            lower(name);
            if(strcmp(name,"stop")==0){
                return name;
            }else if(strlen(name)<3){
                printf("\nThe name is too short.(At least 3 characters)\n");
                result = 0;
            }else if(strlen(name)>50){
                printf("\nThe name is too long(At most 50 characters)\n");
                result = 0;
            }else if(name[strlen(name)-1]==32){
                printf("\nYou should not put space at the end of the name.\n");
                result = 0;
            }
            else{
                for(int i = 0; i < strlen(name); i++){
                    if(name[i+1] == 32 && name[i] == 32){
                        printf("\nYour should not place double space in name.\n");
                        result = 0;
                        break;
                    }else if((name[i] < 97 || name[i] > 122) && name[i] != 32){
                        printf("\nInvalid name.\n");
                        result = 0;
                        break;
                    }
                }
            }
        }while(result == 0);
        capitalized(name);
        result =repeatedName(name);
        if(result == 1){
            printf("\nThe name is repeated.\n");
        }else{
            return name;
        }
    }while(result==1);
}

int repeatedName(char name[]){
    employee* temp = head;
    while(temp != NULL){
        if(strcmp(temp->data.name,name)==0){
            return 1;
        }else{
            temp = temp->next;
        }
    }
    return 0;
}

char* validID(){
    int result = 1;
    char* ID = (char*)(malloc(sizeof(5)));

    do{
        printf("\nEnter the employee ID(0000-9999):");
        scanf("\n%5[^\n]s", ID);
        fflush(stdin);
        for(int i = 0; i < strlen(ID); i++){
            if(strlen(ID) != 4){
                printf("\nInvalid input.\n");
                result = 0;
                break;
            }else if(ID[i] == 32){
                printf("\nShould not put space in the ID.\n");
                result = 0;
                break;
            }else if(ID[i] < 48 || ID[i] > 57){
                printf("\nID should be a digit. (0000-9999)\n");
                result = 0;
                break;
            }else{
                result =1;
            }
        }
    }while(result == 0);
    return ID;
}

int repeatedID(char id[]){
    employee* temp = head;
    while(temp != NULL){
        if(strcmp(temp->data.ID,id)==0){
            return 1;
        }else{
            temp = temp->next;
        }
    }
    return 0;
}

int validAge(){
    char age[5];
    int intAge,result;

    do{
        printf("\nAge(18-99):");
        scanf("\n%[^\n]s",age);
        if(age[0] == 45){
            printf("\nAge should be a positive value.\n");
            result = 0;
        }else{
            for(int i=0; i<strlen(age);i++){
                if(age[i]==32){
                    printf("\nYou should not space in age.\n");
                    result = 0;
                    break;
                }else if(age[i]<48 || age[i]>57){
                    printf("\nThe age is not a digit.\n");
                    result = 0;
                    break;
                }else{
                    result = 1;
                }
            }
        }
        if(result==1){
            intAge = atoi(age);
            if(intAge<18 || intAge>99){
                printf("\nInvalid Age.(18-99)\n");
                result = 0;
            }else{
                result = 1;
            }
        }
    }while(result == 0);
    return intAge;
}

float validGrossSalary(){
    char grossSalary[11];
    int result, h;
    float floatGrossSalary;

    do{
        printf("\nGross Salary(RM0-RM1000000):");
        scanf("\n%[^\n]s", grossSalary);
        if(grossSalary[0] == 45){
            printf("\nGross Salary should be a positive value.\n");
            result = 0;
        }else{
            for(h=0; h<strlen(grossSalary); h++){
                if(grossSalary[h] == 46){
                    if(h == strlen(grossSalary) - 3){
                        break;
                    }else{
                        printf("\nYou should only input two decimal places in gross salary.\n");
                        h=-1;
                        result = 0;
                        break;
                    }
                }
            }
            for(int i=0; i<strlen(grossSalary); i++){
                if(grossSalary[i] == 32){
                    printf("\nYou should not input space in gross salary.\n");
                    result = 0;
                    break;
                }
                if(i != h){
                    if(grossSalary[i]<48 || grossSalary[i]>57){
                        printf("\nThe gross salary should be a digit.\n");
                        result = 0;
                        break;
                    }
                    else
                        result = 1;
                }
            }
        }
        if(result == 1){
            floatGrossSalary = atof(grossSalary);
            if(floatGrossSalary < 0 || floatGrossSalary > 1000000 ){
                printf("\nInvalid gross salary(RM0 - RM100000\n");
                result = 0;
            }
            else{
                return floatGrossSalary;
            }
        }
    }while(result == 0);
}


//Calculate
int calcInsurance(int age){
    if(age <= 35){
        return 110;
    }else if(age >= 36 && age <=65){
        return 160;
    }else{
        return 250;
    }
}

float calcIncomeTax(float grossSalary){
    if(grossSalary < 1000.00){
        return 0;
    }else if(grossSalary >= 1000.00 && grossSalary <= 2999.99){
        return grossSalary * 0.025;
    }else{
        return grossSalary * 0.05;
    }
}

//Number & Integer
int isNumber(char input[]){
    if(strlen(input) > 1){
        return 0;
    }else if(input[0] > 57 || input[0] < 48){
        return 0;
    }else{
        return 1;
    }
}

int isInteger(char input[]){
    if(strlen(input) > 3){
        return 0;
    }else{
        for(int i = 0; i < strlen(input); i++){
            if(input[i] > 57 || input[i] < 48){
                return 0;
            }else{
              return 1;
            }
        }
    }
}


//Other Functions
employee* searchID(){
    char ID[5];
    int exit;

    do{
        employee* temp = head;
        strcpy(ID, validID());
        while(temp != NULL){
            if(strcmp(temp->data.ID, ID) == 0){
                return temp;
            }else{
                temp = temp->next;
            }
        }
        printf("\nID not found.\n");
        exit = exitAction();
        if(exit == 1){
            return NULL;
        }else{
            system("cls");
        }
    }while(exit != 1);
}

int exitAction(){
    char input[1];
    int result, option;

    do{
        do{
            printf("\nPress 1 - Exit\nPress 0 - Continue\n\nInput:");
            scanf("\n%[^\n]s", input);
            result = isNumber(input);
            if(result != 1){
                printf("\nInvalid input.\n");
            }else{
                option = atoi(input);
            }
        }while(result != 1);

        switch(option){
            case 1:
                return 1;
            case 0:
                return 0;
            default:
                printf("Invalid value.\n");
        }
    }while(true);
    system("pause");
    system("cls");
}

void lower(char name[]){
    for(int i = 0; i < strlen(name); i++){
        name[i] = tolower(name[i]);
    }
}

void capitalized(char name[]){
    name[0]= toupper(name[0]);
    for(int i=0;i<strlen(name);i++){
        if(name[i]==32){
            name[i+1]= toupper(name[i+1]);
        }
    }
}


