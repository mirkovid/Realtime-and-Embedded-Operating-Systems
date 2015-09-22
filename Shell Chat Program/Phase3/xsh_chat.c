/** Chat program for CSE 321 Project 2 Phase 3 by Tejas Vyas*/

#include <kernel.h>
#include <stdlib.h>
#include <shell.h>
#include <stdio.h>
#include <string.h>

#define message_length 32

command xsh_chat(ushort stdin, ushort stdout, ushort stderr,
ushort nargs, char *args[])
{
char * messages[message_length];

while (1) {
read(TTY0, messages, message_length);
write(TTY1, messages, message_length);
read(TTY1, messages, message_length);
write(TTY0, messages, message_length);
*messages = NULL;
}

return OK;
}