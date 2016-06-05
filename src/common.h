#ifndef _common_h
#define _common_h

void die(char *msg);
int randomint();
int randomport();
void int_to_str(int int_value, char* str_buf, int buf_len);
void random_octet(char* oct_buf);
void randomip(char* buf);

#endif
