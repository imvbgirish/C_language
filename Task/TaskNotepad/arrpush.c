#include <stdio.h>

void moveDuplicatesToEnd(int arr[], int size) {
    int i, j = 0;
    int duplicates[size];
    int duplicateCount = 0;

    // Step 1: Traverse the array to collect non-duplicates in the front
    for (i = 0; i < size; i++) {
        int isDuplicate = 0;
        // Check if the element is a duplicate
        for (int k = 0; k < j; k++) {
            if (arr[i] == arr[k]) {
                isDuplicate = 1;
                break;
            }
        }
        // If not duplicate, add it to the front part of the array
        if (!isDuplicate) {
            arr[j++] = arr[i];
        } else {
            // If it's a duplicate, store it in the duplicates array
            duplicates[duplicateCount++] = arr[i];
        }
    }

    // Step 2: Append the duplicates to the end of the array
    for (i = 0; i < duplicateCount; i++) {
        arr[j++] = duplicates[i];
    }
}

int main() {
    int arr[] = {1, 2, 3, 3, 4, 5, 4,4,6};
    int size = sizeof(arr) / sizeof(arr[0]);

    moveDuplicatesToEnd(arr, size);

    // Print the modified array
    printf("Array after moving duplicates to the end: \n");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}