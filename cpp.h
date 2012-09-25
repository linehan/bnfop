#ifndef _CPP_H
#define _CPP_H


#ifdef __GNUC__
#define UNUSED_VARIABLE __attribute__ ((unused))
#else
#define UNUSED_VARIABLE 
#endif



/* 
 * VA_COMMA surrounds its arguments (__VA_ARGS__) with 8 additional 
 * arguments: one empty argument before (doesn't have to be empty --
 * it's thrown away) and 6 commas and an empty argument after.
 * These 8 or more arguments are passed to GET_8TH_ARG, which, as 
 * its name implies, expands to the eighth argument. All other 
 * arguments are discarded.
 */
#define GET_8TH_ARG(_0, _1, _2, _3, _4, _5, _6, _7, ...) _7
#define COMMA ,

#define VA_COMMA(...) \
        GET_8TH_ARG(/*empty*/, ##__VA_ARGS__, COMMA, COMMA, COMMA, COMMA, COMMA, COMMA, /*empty*/)


#endif
