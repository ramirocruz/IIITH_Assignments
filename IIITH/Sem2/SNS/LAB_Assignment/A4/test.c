#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<unistd.h>
int bof(char *str)
{
    char buffer[12];
    stpcpy(buffer,str);

    return 1;
}
int main(int argc,char **argv)
{
    
    char str[24];
    //FILE *badfile;
    //badfile = fopen("badfile","r");
    //fread(str,sizeof(char),517,badfile);
    //bof(str);
    gets(str);

    printf("Returned Properly\n");

    return 1;
}
