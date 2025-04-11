#include <stdio.h>
#include <stdlib.h>

struct MyData {
  int id;
  char name[50];
  float value;
};

int main() {
    int num_entries;

    printf("Enter the number of data entries: ");
    scanf("%d", &num_entries);

    struct MyData *data_array = (struct MyData *)malloc(num_entries * sizeof(struct MyData));

    if (data_array == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    for (int i = 0; i < num_entries; i++) {
        printf("Enter details for entry %d:\n", i + 1);

        printf("Enter ID: ");
        scanf("%d", &(data_array[i].id));

        printf("Enter Name: ");
        scanf(" %49[^\n]", data_array[i].name); // Read until newline, max 49 characters

        printf("Enter Value: ");
        scanf("%f", &(data_array[i].value));
    }

    printf("\nDisplaying Data:\n");
    for (int i = 0; i < num_entries; i++) {
        printf("Entry %d:\n", i+1);
        printf("  ID: %d\n", data_array[i].id);
        printf("  Name: %s\n", data_array[i].name);
        printf("  Value: %f\n", data_array[i].value);
    }
    free(data_array);
    return 0;
}