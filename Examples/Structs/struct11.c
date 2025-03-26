//Bank-Problem

#include <stdio.h>

struct Account{
	int accountNum; int balance;
};

struct Customer{
	char name[20];
	struct Account a;
};


void printDetail(char *, int , int );
void updateBalance(int , int , int, int);

int main(){

int i, j, dh;
int accountNo, amount, code;

struct Customer c[3];


for(i=0; i<3; i++){
	printf("Enter Your Name, AccountNum, Balance: \n");
	scanf("%s %d %d",&c[i].name, &c[i].a.accountNum, &c[i].a.balance);
	while((dh = getchar()) != '\n');
}
printf("\nName and Account name of customers with balance less than 100:\n");

for(i=0; i<3; i++){
	printDetail(c[i].name, c[i].a.accountNum, c[i].a.balance);
}

printf("Enter AccountNum, Amount, Code(0=Deposit, 1=Withdrawal:\n");
printf("Account Number: ");
scanf("%d", &accountNo);

printf("Amount: ");
scanf("%d", &amount);

printf("Code: ");
scanf("%d", &code);

for(i=0; i<3; i++){

	if(c[i].a.accountNum == accountNo){
		updateBalance(accountNo, amount, code, c[i].a.balance);
		break;
	}
}
return 0;
}

void printDetail(char *name, int accNo, int bal){
	if(bal < 100){
		printf("Name: %s\n", name);
		printf("Account Number: %d\n", accNo);
		printf("\n");
	}
}

void updateBalance(int accNum, int amount, int code,int balance){
	if(code == 0){
		balance += amount;
		printf("Amount Credited, Available Balance is: %d", balance);
	}else if(code == 1 && balance >= amount){
		balance -= amount;
		printf("Amounted Debited, Available Balance is: %d", balance);
	}else{
		printf("Amount Entered is more than available balance");
	}

}

