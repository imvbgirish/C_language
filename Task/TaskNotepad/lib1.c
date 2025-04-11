#include <stdio.h>
#include <string.h>

struct Library {
	int accession_number;
    	char title[100];
    	char author[100];
    	int price;
    	int issued;
};

int addBook(struct Library books[], int count);

void displayBooks(struct Library books[], int count);

void listBooksByAuthor(struct Library books[], int count);

void listTitleByAccession(struct Library books[], int count);

int main() {
    	struct Library books[100];	
    	int count = 0, choice;
    
    	do {
        	printf("\nLibrary Menu:\n");
        	printf("1. Add book\n");
		printf("2. Display books\n");
		printf("3. List books by author\n");
        	printf("4. Find title by accession number\n");
		printf("5. Count books\n");
		printf("6. Exit\n");
        	printf("Enter choice: ");
        	scanf("%d", &choice);
        
        	switch (choice) {
            		case 1: count = addBook(books, count); 
				break;
            		case 2: displayBooks(books, count); 
				break;
            		case 3: listBooksByAuthor(books, count); 
				break;
            		case 4: listTitleByAccession(books, count); 
				break;
            		case 5: printf("Total books: %d\n", count); 
				break;
            		case 6: printf("Exiting...\n"); 
				break;
            		default: printf("Invalid choice.\n");
        	}
    		}while (choice != 6);
    
return 0;
}

int addBook(struct Library books[], int count){
    	printf("Enter Accession Number: ");
    	scanf("%d", &books[count].accession_number);
    
    	getchar();
    	printf("Enter Title: ");
    	gets(books[count].title);

    	printf("Enter Author: ");
    	gets(books[count].author);

    	printf("Enter Price: ");
    	scanf("%d", &books[count].price);

    	printf("Is the book issued? (1 for Yes, 0 for No): ");
    	scanf("%d", &books[count].issued);
    	return count + 1;
}

void displayBooks(struct Library books[], int count){
    	for (int i = 0; i < count; i++){
        	printf("\nAccession Number: %d\nTitle: %s\nAuthor: %s\nPrice: %d\nIssued: %s\n",
               		books[i].accession_number, books[i].title, books[i].author,
               		books[i].price, books[i].issued ? "Yes" : "No");
    	}
}

void listBooksByAuthor(struct Library books[], int count){
    	char author[100];
    	printf("Enter Author Name: ");
    	getchar();
    	gets(author);
    
    	for (int i = 0; i < count; i++){
        	if (strcmp(books[i].author, author) == 0){
            		printf("Title: %s\n", books[i].title);
        	}
    	}
}

void listTitleByAccession(struct Library books[], int count){
    	int num;
    	printf("Enter Accession Number: ");
    	scanf("%d", &num);
    
    	for (int i = 0; i < count; i++) {
        	if (books[i].accession_number == num) {
            		printf("Title: %s\n", books[i].title);
            		return;
        	}
    	}
    printf("Book not found\n");
}