#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include "bnfop.h"

/******************************************************************************
 * HELPER UTILITIES
 ******************************************************************************/

/**
 * is_ws
 * `````
 * Test whether a character is a whitespace character or not.
 *
 * @c    : Character to be tested.
 * Return: TRUE if character is whitespace, else FALSE.
 */
static inline bool is_ws(char c) {
        switch (c) {
        case ' ':
        case '\n':
        case '\t':
        case '\f':
        case '\r':
                return true;
        default:
                return false;
        }
}


/**
 * next_word
 * `````````
 * Advance a pointer to the next whitespace-delimited word, or EOL.
 *
 * @ptr  : Pointer to a string.
 * Return: Pointer to the beginning of the next word, or to EOL.
 */
static inline const char *next_word(const char *ptr)
{
        /* End of string */
        if (*ptr == 0) {
               return ptr; 
        }

        /*
         * If the ptr is inside a word already,
         * advance to the end of it.
         */
        while (!is_ws(*ptr)) {
                ptr++;
                if (*ptr == 0) {
                        return ptr;
                }
        }

        /* 
         * If the ptr is whitespace, advance to 
         * the next non-whitespace character.
         */
        while (is_ws(*ptr)) {
                ptr++;
                if (*ptr == 0) {
                        return ptr;
                }
        }

        /* We are at the beginning of the next word. */
        return ptr;
}


/******************************************************************************
 * BNFOP 
 ******************************************************************************/
        
/**
 * bnfop
 * `````
 * Use a BNF format string to validate arguments and print usage.
 *
 * @argc : Argument count.
 * @argv : Argument vector.
 * @usage: Format string written in mini-BNF.
 * @...  : Variable number of arguments (one for each <foo> in @usage).
 * Return: TRUE if match, else FALSE.
 * 
 * NOTE
 * May abort program execution if a malformed message is detected.
 *
 * USAGE
 *
 *      With a binary infix operator (@):
 *      --------------------------------
 *      const char *name;
 *      const char *host;
 *
 *      if (bnfop("prog <name> @ <host>", &name, &host)) {
 *              my_func(name, host);
 *      }
 *
 *
 *      With a traditional short option flag:
 *      ------------------------------------
 *      const char *name;
 *
 *      if (bnfop("prog -o <name>", &name)) {
 *              my_func(name);
 *      }
 *
 *
 *      With only a program invocation:
 *      ------------------------------
 *      const char *name;
 *
 *      if (bnfop("prog <name>$", &name)) {
 *              my_func(name);
 *      }
 *
 *
 *      With no arguments:
 *      -----------------
 *      if (bnfop("prog$", &name)) {
 *              my_func(name);
 *      }
 *      
 * 
 */
bool bnfop(int argc, char *argv[], const char *usage, ...)
{
        const char *ptr;        // Tracks across usage string
        char **param;           // Points to variable argument list.
        va_list args;           // The variable argument list
        size_t len;             // Length of the option word
        bool onematch = false;  // If the first (program name) option fits
        bool twomatch = false;  // If the second (option word) option fits
        int n = 0;              // Number of words processed.

        ptr = usage;

        va_start(args, usage);

        assert(!is_ws(*ptr) && "Leading whitespace in option string.\n");

        do {
                /* Required argument. */
                if (*ptr == '<') {
                        while (*ptr != '>') {
                                ptr++;
                                assert((*ptr != 0) && "Unterminated '<'.\n");
                        }

                        /* 
                         * Assign the corresponding argv to the 
                         * argument in the variadic argument list 
                         * of the caller. 
                         */ 
                        param  = va_arg(args, char **);
                        *param = argv[n];

                        goto next;
                }


                /* Option word */
                len = strlen(argv[n]);

                if (memcmp(ptr, argv[n], len) == 0) {
                        if (n == 0) {
                                onematch = true; /* Program name */
                        } else {
                                twomatch = true; /* Option */
                        }
                        ptr += len; /* advance past the word */
                        goto next;
                } else {
                        break;
                }

next:
                ptr = next_word(ptr);
                n  += 1;

        } while (n != argc);

        va_end(args); /* Clean up after the variable arguments */


        /*
         * Judgement!
         */

        if (twomatch == true) {
                if (*ptr == 0) {
                        return true;
                } else {
                        fprintf(stderr, "(Usage) %s\n", usage);
                        exit(1); /* malformed */
                }
        } else if (onematch == true) {
                if (*ptr == '$') {
                        return true;
                } else {
                        return false;
                }
        }

        return false;
}

