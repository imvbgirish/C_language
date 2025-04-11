#include <stdio.h>

struct student{
	
	int rollNo; char name[20]; char depart[20]; char course[20]; int yearOfJoin;

};

void display(struct student *, int join);

void displayData(struct student *, int rollNo);



int main(){


struct student s[10];

int i, dh, join, rollNo;


for(i=0; i<10; i++){
	printf("Enter rollNo, name, department, course, yearOfJoin\n");
	scanf("%d %s %s %s %d",&s[i].rollNo, &s[i].name, &s[i].depart, &s[i].course, &s[i].yearOfJoin);
	while((dh = getchar()) != '\n');
}

printf("Enter year to get students name who joined in that particular year: ");
scanf("%d",&join);  

printf("Enter rollNo: ");
scanf("%d", &rollNo);

for(i=0; i<10; i++){
	display(&s[i], join);
	displayData(&s[i], rollNo);
}

return 0;
}

void display(struct student *std, int join){
	
	if((std->yearOfJoin) == join)
		printf("%s\n", std->name);
		
}

void displayData(struct student *std, int rollNo){
	
	if((std->rollNo) == rollNo)
		printf("%s %s %s %d", std->name,std->depart,std->course,std->yearOfJoin);
}







