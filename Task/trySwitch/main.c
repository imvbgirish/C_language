#include <stdio.h>

// void run(){
//     printf("inside func\n");

//     switch(3){
//     case 1:printf("a\n");
//         return;

//     case 2:printf("b\n");
//         return;                              //prints c and does not returns.

//     case 3:printf("c\n");
//         break;;

//     default:printf("In default\n");
//     }
// }




// void run(){
//     printf("inside func\n");

//     switch(3){
//     case 1:printf("a\n");
//     case 2:printf("b\n");
//         exit(0);                         //prints c no matter what
//         break;

//     case 3:printf("c\n");
//         break;;;;;;

//     default:printf("In default\n");
//     }
// }

int main()
{
    printf("Hello World!\n");

    // int a;
    // switch(a){
    // case 1: printf("Hi\n");      //prints oooo, if we declare variable as float shows error, requires only int type
    //     break;
    // case 2: printf("Hello\n");
    //     break;
    // default:printf("oooo\n");
    // }


    // switch('a'){
    // case 'a':printf("a");
    // case 'b':printf("b");        //prints a,b,c
    // case 'c':printf("c");
    //     break;
    // default: printf("default");
    // }


    //char a = 'aa'; //multi character character constant

    //char b = "a";   //Incompatible pointer to integer conversion Error


    // char a = 'aa';
    // switch('aa'){
    // case 'aa': printf("Hi\n");       //prints Hi
    //     break;
    // default: printf("Hey");
    // }


    // int a = 3;
    // switch(a){               //Does not prints a
    //     printf("%d",a);
    // }


    // int a = 3;
    // switch(a){                  //Does not prints a
    // case 'a':
    //     printf("a");
    // }



    // switch(3){
    // case 1:printf("a");
    //     return;                     //main is int hence we cannot use return here
    // case 2:printf("b");
    //     break;
    // }

    // switch(printf("a"));             //prints a

    // switch(printf("%d",10));         //prints 10

    // we cannot put printf to case statement, only constant is allowed

    //run();

    return 0;
}
