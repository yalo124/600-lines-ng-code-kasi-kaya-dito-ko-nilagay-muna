#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct employee_wage
{
    float Time_in;
    float Time_out;
    float EmSalary;
};

struct record
{
    char EmId[20];
    char EmName[40];
    char EmDepart[30];
    char EmAddress[200];
    char EmEmail[100];
    char DateOfEmployment[100];
    int DailyTime;

    float EmRatePerHour;

    int overtime;

    float DailyHours;
    float DailySalary;
    float MonthlySalary;

    struct employee_wage PAY;
};

//converting
float convertToFloat(int hour, int minute, char period[])
{
    if ((period[0] == 'P' || period[0] == 'p') && hour != 12)
        hour += 12;
    if ((period[0] == 'A' || period[0] == 'a') && hour == 12)
        hour = 0;

    return hour + (minute/60.0);
}

//add new employee
void INPUT(struct record *e)
{
    printf("Name: ");
    scanf(" %[^\n]", e->EmName);

    printf("ID: ");
    scanf("%s", e->EmId);

    printf("Department: ");
    scanf(" %[^\n]", e->EmDepart);

    printf("Email: ");
    scanf(" %[^\n]", e->EmEmail);

    printf("Date Of Employment: ");
    scanf(" %[^\n]", e->DateOfEmployment);

    printf("Rate Per Hour: ");
    scanf("%f", &e->EmRatePerHour);

    printf("Daily Time: ");
    scanf("%d", &e->DailyTime);
}

//existing employee
void INPUT1(struct record *e){
    printf("ID: ");
    scanf("%s", e->EmId);
}

//time in and time out for existing employee
void INPUT2(struct record *e)
{
    int h, m;
    char p[4], ans;

    printf("Time In: ");
    scanf("%d:%d %s", &h, &m, p);

    e->PAY.Time_in = convertToFloat(h, m, p);

    while (1)
    {
        printf("Time Out: ");
        scanf("%d:%d %s", &h, &m, p);

        e->PAY.Time_out = convertToFloat(h, m, p);

        if (e->PAY.Time_out <= e->PAY.Time_in)
        {
            printf("Next day? (Y/N): ");
            scanf(" %c", &ans);

            if (ans == 'Y' || ans == 'y')
            {
                e->overtime = 1;
                break;
            }

            printf("Invalid!\n");
            continue;
        }

        break;
    }
}

//compute of salary
void COMPUTE(struct record *e)
{
    float in = e->PAY.Time_in;
    float out = e->PAY.Time_out;

    if (out < in || e->overtime)
        out += 24;

    e->DailyHours = out - in;
    e->PAY.EmSalary = e->DailyHours * e->EmRatePerHour;
    e->DailySalary = e->DailyTime * e->EmRatePerHour;
    e->MonthlySalary = e->DailySalary * 20;
}

//file for employee salary
void SAVE_TO_FILE(struct record e)
{
    FILE *fp = fopen("EmployeeRecord.txt", "a");

    fprintf(fp,
        "\n=================================================\n"
        "EMPLOYEE RECORD\n"
        "=================================================\n"
        "Employee Name          : %s\n"
        "Employee ID Number     : %s\n"
        "Department             : %s\n"
        "Email                  : %s\n"
        "Date of Employment     : %s\n"
        "Rate Per Hour          : %.2f\n" 
        "Daily Time             : %d\n"
        "Monthly Salary         : Php %.2f\n"
        "=================================================\n",

        e.EmName,
        e.EmId,
        e.EmDepart,
        e.EmEmail,
        e.DateOfEmployment,
        e.EmRatePerHour,
        e.DailyTime,
        e.MonthlySalary);

    fclose(fp);
}

void SAVE_FILE_2(){
    struct record e;
    FILE *fk = fopen("EmployeeSalary.txt", "a");

     if (fk == NULL) {
        printf("Error opening file.\n");
        return;
    }

    //time in
    int hourIn = (int)e.PAY.Time_in;
    int minuteIn = (int)((e.PAY.Time_in - hourIn) * 60 + 0.5);
    char periodIn[3] = "AM";

    if (minuteIn == 60)
    {
        minuteIn = 0;
        hourIn++;
    }

    if (hourIn >= 12)
    {
        periodIn[0] = 'P';
        periodIn[1] = 'M';
    }

    if (hourIn == 0)
    {
        hourIn = 12;
    }
    else if (hourIn > 12)
    {
        hourIn -= 12;
    }

    //time out
    int hourOut = (int)e.PAY.Time_out;
    int minuteOut = (int)((e.PAY.Time_out - hourOut) * 60 + 0.5);
    char periodOut[3] = "AM";

    if (minuteOut == 60)
    {
        minuteOut = 0;
        hourOut++;
    }

    if (hourOut >= 12)
    {
        periodOut[0] = 'P';
        periodOut[1] = 'M';
    }

    if (hourOut == 0)
    {
        hourOut = 12;
    }
    else if (hourOut > 12)
    {
        hourOut -= 12;
    }


    fprintf(fk,
        "\n=================================================\n"
        "EMPLOYEE RECORD\n"
        "=================================================\n"
        "Employee Name          : %s\n"
        "Employee ID Number     : %s\n"
        "Department             : %s\n"
        "Time In                : %02d:%02d %s\n"
        "Time Out               : %02d:%02d %s\n"
        "Salary Gained Today    : %f\n"
        "=================================================\n",

        e.EmName,
        e.EmId,
        e.EmDepart,
        hourIn,  minuteIn,  periodIn,
        hourOut,  minuteOut,  periodOut,
        e.PAY.EmSalary
        );

    fclose(fk);
}
//view file
void CHECK_FILE()
{
    FILE *fp = fopen("EmployeeRecord.txt", "r");
    char ch;

    if (!fp)
    {
        printf("No records found.\n");
        return;
    } else{
        printf("File Exist!\n");
    }

    fclose(fp);
}

//==========searching========
//search by name
void SEARCH_BY_NAME(struct record e) {
    FILE *fp = fopen("EmployeeRecord.txt", "r");

    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    char search[100];
    int found = 0;

    printf("Name: ");
    scanf(" %[^\n]", search);

    while (fscanf(fp,
        "\n=================================================\n"
        "EMPLOYEE RECORD\n"
        "=================================================\n"
        "Employee Name          : %[^\n]\n"
        "Employee ID Number     : %[^\n]\n"
        "Department             : %[^\n]\n"
        "Email                  : %[^\n]\n"
        "Date of Employment     : %[^\n]\n"
        "Rate Per Hour          : %f\n"
        "Daily Time             : %d\n"
        "Monthly Salary         : Php %f\n"
        "=================================================\n",

        e.EmName,
        e.EmId,
        e.EmDepart,
        e.EmEmail,
        e.DateOfEmployment,
        &e.EmRatePerHour,
        &e.DailyTime,
        &e.MonthlySalary) == 8)
    {
        if (strcasecmp(search, e.EmName) == 0)
        {
            found = 1;

            printf(                       
            "\n=================================================\n"
            "EMPLOYEE RECORD\n"
            "=================================================\n"
            "Employee Name          : %s\n"
            "Employee ID Number     : %s\n"
            "Department             : %s\n"
            "Email                  : %s\n"
            "Date of Employment     : %s\n"
            "Rate Per Hour          : %.2f\n"
            "Daily Time             : %d\n"
            "Monthly Salary         : Php %.2f\n"
            "=================================================\n",

            e.EmName,
            e.EmId,
            e.EmDepart,
            e.EmEmail,
            e.DateOfEmployment,
            e.EmRatePerHour,
            e.DailyTime,
            e.MonthlySalary);

            break; 
        }
    }

    if (!found)
        printf("No employee with that name exists!\n");

    fclose(fp);
}

//search by id
void SEARCH_BY_ID(struct record e){
    FILE *fp = fopen("EmployeeRecord.txt", "r");

    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    char id[100];
    int found = 0;

    printf("id: ");
    scanf(" %[^\n]", id);

    while (fscanf(fp,
        "\n=================================================\n"
        "EMPLOYEE RECORD\n"
        "=================================================\n"
        "Employee Name          : %[^\n]\n"
        "Employee ID Number     : %[^\n]\n"
        "Department             : %[^\n]\n"
        "Email                  : %[^\n]\n"
        "Date of Employment     : %[^\n]\n"
        "Rate Per Hour          : %f\n"
        "Daily Time             : %d\n"
        "Monthly Salary         : Php %f\n"
        "=================================================\n",

        e.EmName,
        e.EmId,
        e.EmDepart,
        e.EmEmail,
        e.DateOfEmployment,
        &e.EmRatePerHour,
        &e.DailyTime,
        &e.MonthlySalary) == 8)
    {
        if (strcasecmp(id, e.EmId) == 0)
        {
            found = 1;

            printf(                      
            "\n=================================================\n"
            "EMPLOYEE RECORD\n"
            "=================================================\n"
            "Employee Name          : %s\n"
            "Employee ID Number     : %s\n"
            "Department             : %s\n"
            "Email                  : %s\n"
            "Date of Employment     : %s\n"
            "Rate Per Hour          : %.2f\n"
            "Daily Time             : %d\n"
            "Monthly Salary         : Php %.2f\n"
            "=================================================\n",

            e.EmName,
            e.EmId,
            e.EmDepart,
            e.EmEmail,
            e.DateOfEmployment,
            e.EmRatePerHour,
            e.DailyTime,
            e.MonthlySalary);

            break; 
        }
    }

    if (!found)
        printf("No employee with that name exists!\n");

    fclose(fp);
}

//search by salary
void SEARCH_BY_SALARY(struct record e)
{
    FILE *fp = fopen("EmployeeRecord.txt", "r");

    if(fp==NULL){
        printf("No records found\n.");
        return;
    }
    
    float targetsalary;
    int choice;
    int found=0;

    printf("Enter Salary: ");
    scanf("%f", &targetsalary);

    printf("\n[1] Below %.2f\n[2] Above %.2f\n[3] Same as %.2f\nChoice: ",
        targetsalary, targetsalary, targetsalary);
    scanf("%d", &choice);

    if(choice <1|| choice>3){
        printf("Invalid choice.\n");
        fclose(fp);
        return;
    }
    
    printf("\n=== SEARCH RESULTS ===\n");
     while (fscanf(fp,
        "\n=================================================\n"
        "EMPLOYEE RECORD\n"
        "=================================================\n"
        "Employee Name          : %[^\n]\n"
        "Employee ID Number     : %[^\n]\n"
        "Department             : %[^\n]\n"
        "Email                  : %[^\n]\n"
        "Date of Employment     : %[^\n]\n"
        "Rate Per Hour          : %f\n" 
        "Daily Time             : %d\n"
        "Monthly Salary         : Php %f\n"
        "=================================================\n",

        e.EmName,
        e.EmId,
        e.EmDepart,
        e.EmEmail,
        e.DateOfEmployment,
        &e.EmRatePerHour,
        &e.DailyTime,
        &e.MonthlySalary) == 8)
    { 
        int match=0;

        if (choice == 1 && e.MonthlySalary < targetsalary)
            match = 1;
        else if (choice == 2 && e.MonthlySalary > targetsalary)
            match = 1;
        else if (choice == 3 && e.MonthlySalary == targetsalary)
            match = 1;

        if (match)
        {
            found=1;
            printf("\n=================================================\n"
        "EMPLOYEE RECORD\n"
        "=================================================\n"
        "Employee Name          : %s\n"
        "Employee ID Number     : %s\n"
        "Department             : %s\n"
        "Email                  : %s\n"
        "Date of Employment     : %s\n"
        "Rate Per Hour          : %.2f\n" 
        "Daily Time             : %d\n"
        "Monthly Salary         : Php %.2f\n"
        "=================================================\n",

        e.EmName,
        e.EmId,
        e.EmDepart,
        e.EmEmail,
        e.DateOfEmployment,
        e.EmRatePerHour,
        e.DailyTime,
        e.MonthlySalary);
    }
}
if(!found){
    printf("No employees found.\n");

}
    fclose(fp);
}

//updating records
void UPDATE_RECORD()
{
    FILE *fp = fopen("EmployeeRecord.txt", "r");
    FILE *temp = fopen("TempFile.txt", "w");

    if (fp == NULL || temp == NULL) {
        printf("Error opening file.\n");
        if (fp)   fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    struct record e;
    char searchID[20];
    int found = 0;

    printf("Enter ID to update: ");
    scanf("%19s", searchID);

    while (fscanf(fp,
        "\n=================================================\n"
        "EMPLOYEE RECORD\n"
        "=================================================\n"
        "Employee Name          : %[^\n]\n"
        "Employee ID Number     : %[^\n]\n"
        "Department             : %[^\n]\n"
        "Email                  : %[^\n]\n"
        "Date of Employment     : %[^\n]\n"
        "Rate Per Hour          : %f\n" 
        "Daily Time             : %d\n"
        "Monthly Salary         : Php %f\n"
        "=================================================\n",

        e.EmName,
        e.EmId,
        e.EmDepart,
        e.EmEmail,
        e.DateOfEmployment,
        &e.EmRatePerHour,
        &e.DailyTime,
        &e.MonthlySalary) == 8)
    {
        if (strcmp(e.EmId, searchID) == 0 && !found)
        {
            found = 1;
            char ans;

            printf("Updating record...\n");

            printf("Change Name? (y/n): ");
            scanf(" %c", &ans);
                if (ans == 'y' || ans == 'Y'){
                    printf("Enter new Name: ");
                    getchar();
                    fgets(e.EmName, sizeof(e.EmName), stdin);
                    e.EmName[strcspn(e.EmName, "\n")] = '\0';
                }

   
            printf("Change ID? (y/n): ");
            scanf(" %c", &ans);
                if (ans == 'y' || ans == 'Y'){
                    printf("Enter new ID: ");
                    getchar();
                    fgets(e.EmId, sizeof(e.EmId), stdin);
                    e.EmId[strcspn(e.EmId, "\n")] = '\0';
                }

                printf("Change Department? (y/n): ");
                scanf(" %c", &ans);
                    if (ans == 'y' || ans == 'Y'){
                        printf("Enter new Department: ");
                        getchar();
                        fgets(e.EmDepart, sizeof(e.EmDepart), stdin);
                        e.EmDepart[strcspn(e.EmDepart, "\n")] = '\0';
                    }

                printf("Change Email? (y/n): ");
                scanf(" %c", &ans);
                    if (ans == 'y' || ans == 'Y'){
                        printf("Enter new Email: ");
                        getchar();
                        fgets(e.EmEmail, sizeof(e.EmEmail), stdin);
                        e.EmEmail[strcspn(e.EmEmail, "\n")] = '\0';
                    }

            COMPUTE(&e);
        }

        fprintf(temp,
            "\n=================================================\n"
        "EMPLOYEE RECORD\n"
        "=================================================\n"
        "Employee Name          : %s"
        "Employee ID Number     : %s"
        "Department             : %s"
        "Email                  : %s"
        "Date of Employment     : %s"
        "Rate Per Hour          : %.2f\n" 
        "Daily Time             : %d\n"
        "Monthly Salary         : Php %.2f\n"
        "=================================================\n",

        e.EmName,
        e.EmId,
        e.EmDepart,
        e.EmEmail,
        e.DateOfEmployment,
        e.EmRatePerHour,
        e.DailyTime,
        e.MonthlySalary);
    }

    fclose(fp);
    fclose(temp);

    remove("EmployeeRecord.txt");          
    rename("TempFile.txt", "EmployeeRecord.txt");

    if (!found)
        printf("Employee not found.\n");
}

int main()
{
    struct record e;
    int option, option2, option3;

    while (1)
    {
        printf("\n====================================\n");
        printf("PAYROLL SYSTEM\n");
        printf("====================================\n");

        printf("[1] Add New Employee\n");
        printf("[2] Time in & Time out\n");
        printf("[3] Check File\n");
        printf("[4] Search\n");
        printf("[5] Update\n");
        printf("[6] Exit\n");

        printf("Choice: ");
        scanf("%d", &option);

        switch (option)
        {
        case 1:
            INPUT(&e);
            COMPUTE(&e);
            SAVE_TO_FILE(e);
            break;
        
        case 2:
            INPUT1(&e);
            INPUT2(&e);
            COMPUTE(&e);
            SAVE_FILE_2();
            break;
        case 3:
            CHECK_FILE();
            break;

        case 4:
        do{
        printf("[1] Name\n");
        printf("[2] Id\n");
        printf("[3] Salary\n");
        scanf("%d", &option3);
                    if(option3==1){
                        SEARCH_BY_NAME(e);
                        break;
                    } else if(option3==2){
                        SEARCH_BY_ID(e);
                        break;
                    } else if(option3==3){
                        SEARCH_BY_SALARY(e);
                        break;
                    } else{
                        printf("Invalid! Try Again");
                    }
        }while(option3!=1 || option3!=2|| option3!=3);
            break;

        case 5:
            UPDATE_RECORD();
            break;

        case 6:
            return 0;

        default:
            printf("Invalid!\n");
        }
    }
}