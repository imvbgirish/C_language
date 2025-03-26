#include <stdio.h>

int main(){

int cost_price;
int selling_price;

printf("enter cost price: ");
scanf("%d",&cost_price);

printf("enter selling price: ");
scanf("%d",&selling_price);

int profit = 0;
int loss = 0;

if(selling_price > cost_price){
	profit = selling_price - cost_price;
	printf("The seller had made profit and profit amount is: %d",profit);
}
else{
	loss = cost_price - selling_price;
	printf("The seller had made loss and loss amount is: %d",loss);
}

return 0;
}
