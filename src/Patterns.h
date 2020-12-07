#ifndef _PATTERNS_H_
#define _PATTERNS_H_

const char *twinkle       = "solid #001\ntwinkle #fff 1200";
const char *xmas1         = "dots 4 #00f #a0a #d40 #d00 #0d0";
const char *xmas2         = "dots 4 #d00 #0d0 #d61";
const char *incandescent  = "solid #d61";
const char *off           = "solid #000";

#ifndef PATTERN
#define PATTERN off
#endif

const char *office   = "office";
const char *holidays = "holidays";

#ifndef PROGRAM
#define PROGRAM holidays
#endif

#endif
