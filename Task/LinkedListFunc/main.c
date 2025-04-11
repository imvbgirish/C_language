#include <stdio.h>
#include "list.h"

int main()
{
    Node *first = createNode(10);

    first->next = createNode(20);

    first->next->next = createNode(30);

    Node *temp = first;
    while(temp){
        printf("%d ", temp->data);
        temp = temp->next;
    }
    return 0;
}
