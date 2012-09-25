#include <stdlib.h>
#include <stdio.h>
#include "bnfop.h"

const char *word;


int main(int argc, char *argv[])
{
        if (bnfop(argc, argv, "./test -x <word>", &word)) {
                printf("Got %s.\n", word);
        } else {
                printf("no\n");
        }

        return 1;
}
