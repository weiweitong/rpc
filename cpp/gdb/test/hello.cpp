#include <stdio.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) 
{    
    if (argc <= 1)
    {
        printf("usage: hello name\n");
        return 0;
    }
    printf("Hello World %s!\n", argv[1]);
    return 0;
}

