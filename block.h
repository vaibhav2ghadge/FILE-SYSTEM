#include<string.h>
#include<stdlib.h>
union
{
unsigned int integer;
unsigned char byte[4];
}b;
void bytetoint(char *byte,int *integer)
{   
    strcpy(b.byte,byte);
    *integer=b.integer;
}
void inttobyte(int integer,char (*byte)[4])
{  
    b.integer=integer;
    strcpy(*byte,b.byte);
}

/*#include<stdio.h>
#include"block.h"
int main()
{
char byte[4];
int x=123456789;
inttobyte(x,&byte);
printf("%s",byte);
bytetoint(byte,&x);
printf("%d",x);
}
*/
