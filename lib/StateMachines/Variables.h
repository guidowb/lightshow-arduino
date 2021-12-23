#ifndef _VARIABLES_H_
#define _VARIABLES_H_

#define ST(A) #A
#define STR(A) ST(A)

extern const char *twinkle;
extern const char *xmas1;
extern const char *xmas2;
extern const char *incandescent;
extern const char *off;

#ifndef PATTERN
#define PATTERN off
#endif

extern const char *office;
extern const char *holidays;

#ifndef PROGRAM
#define PROGRAM holidays
#endif

extern const char *ota_hostname;
extern const char *ota_password;

#endif
