#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the structure for a student
struct Student {
    char name[100];   // Name of the student
    int roll_no;      // Roll number of the student
    struct Student* next; // Pointer to the next student (linked list)
};

// Function to create a new student node
struct Student* createStudent(char name[], int roll_no) {
    struct Student* newStudent = (struct Student*)malloc(sizeof(struct Student));
    strcpy(newStudent->name, name);
    newStudent->roll_no = roll_no;
    newStudent->next = NULL;
    return newStudent;
}

// Function to add a student to the linked list
struct Student* addStudent(struct Student* head, char name[], int roll_no) {
    struct Student* newStudent = createStudent(name, roll_no);
    if (head == NULL) {
        return newStudent;  // If the list is empty, new student becomes the head
    } else {
        struct Student* temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newStudent;
    }
    return head;  // Return the head (unchanged if new student is added at the end)
}

// Function to display the list of students
void displayStudents(struct Student* head) {
    struct Student* temp = head;
    while (temp != NULL) {
        printf("Name: %s, Roll No: %d\n", temp->name, temp->roll_no);
        temp = temp->next;
    }
}

// Function to free the allocated memory
void freeList(struct Student* head) {
    struct Student* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    struct Student* head = NULL;

    // Add students to the linked list
    head = addStudent(head, "Alice", 101);
    head = addStudent(head, "Bob", 102);
    head = addStudent(head, "Charlie", 103);

    // Display the list of students
    printf("Student List:\n");
    displayStudents(head);

    // Free the allocated memory
    freeList(head);

    return 0;
}
