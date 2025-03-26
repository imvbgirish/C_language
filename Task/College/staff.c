#include "staff.h"
#include <stdio.h>
#include <stdlib.h>

int staff_num;
struct Staff *staff;

void addStaffData(){

    printf("\nEnter Number of Staff: ");
    scanf("%d", &staff_num);

    staff = (struct Staff *)malloc(staff_num * sizeof(struct Staff));

    for(int i=0; i<staff_num; i++){
        printf("Enter Name: ");
        scanf("%s",staff[i].name);
        printf("\n");
    }
}

void displayStaff(){
    if(staff_num == 0){
        puts("No data to display");
    }else{
        for(int i=0; i<staff_num; i++){
            printf("Name: %s\n", staff[i].name);
        }
    }

    free(staff);
}
