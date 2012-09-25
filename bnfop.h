#ifndef _BNFOP_H
#define _BNFOP_H
#include "cpp.h"
#include <stdbool.h>

/******************************************************************************
 * Private stuff
 ******************************************************************************/
static const char * UNUSED_VARIABLE __BNF_NOARGS;

/*
 * Assumes bnf() is being called within main(), and that argc and argv
 * are present.
 */
#define __bnf(usage, ...) \
        bnfop(argc, argv, usage, __VA_ARGS__)

/* 
 * Appends the dummy value NOARGS to the variable argument list, to
 * handle cases where no argument is otherwise supplied. Because the
 * number of arguments taken from the list is restricted first by the
 * value of argc, NOARGS will never be assigned when there truly are
 * "no args."
 */
#define __bnf_append_noargs(usage, ...) \
        __bnf(usage, __VA_ARGS__ VA_COMMA(__VA_ARGS__) __BNF_NOARGS)


/******************************************************************************
 * Public stuff
 ******************************************************************************/
/*
 * The macro to be used.
 */
#define bnf(usage, ...) \
        __bnf_append_noargs(usage, __VA_ARGS__) 


bool bnfop(int argc, char *argv[], const char *usage, ...);


#endif
