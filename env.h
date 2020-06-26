#ifndef ENV_H
#define ENV_H

/* Change these if different on the target system */
typedef signed char         int8;
typedef char                uint8;
typedef unsigned short      uint16;
typedef short               int16;
typedef unsigned int        uint32;
typedef int                 int32;
typedef unsigned long long  uint64;
typedef long long           int64;
typedef float               float32;
typedef double              float64;

/* Select kind of real to be used by the program */
#define REAL32

/* Define real type depending on selected real kind */
#ifndef REAL64

#ifndef REAL32
#define REAL32
#endif /* REAL32 */

typedef float32             real;

#else /* REAL32 */

typedef float64             real;

#endif /* REAL64 */

/* Boolean in C89 way */
typedef uint32              bool;
enum {false = 0, true = 1};

/* In case standard library is not used, define NULL */
#ifndef NULL
#define NULL (void*)0
#endif /* NULL */



#endif /* ENV_H */
