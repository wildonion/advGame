#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>// gcc filename.c -lncurses++conio.h in windows
#define SIZE 200
char *name[SIZE],NAme[SIZE];
int phonenumber[SIZE];
 int i;
int ans;
int n, numberofpeople, Num;
/*------------------------------------------------------*/
int info(char *name[SIZE],int phonenumber[SIZE])
{
     printf("\nENTER THE NUMBER OF CONTACT HERE :: ");
     scanf("%d", &Num);
     system("clear");//in ubuntu.
   for (i=0;i<Num;i++)
   name[i]=(char*)malloc(sizeof(char));
     for (i=0;i<Num;i++)
   {
     printf("ENTER NAME & PHONENUMBER OF %d-TH CONTACT ::\n ", i+1);
     scanf("%s %d", name[i], &phonenumber[i]);
   }
  system("clear");
 return Num;
}
/*------------------------------------------------------*/
int searching(char *name[SIZE], char NAme[SIZE] ,int n)
{

   for (i=0;i<n;i++)
  {
     if (strcmp(name[i],NAme)==0)
     return 1;
  }
   if (i==n)
    return 0;
}
/*-------------------------------------------------------*/
void main()
{
    numberofpeople=info(name,phonenumber);
 for(;;)
  {
      printf("\n\nENTER A NAME TO SEARCHING HIS/HER PHONENUMBER :::: ");
      scanf("%s", NAme);
      if (searching(name,NAme,numberofpeople)==1)
      printf("\n%s HAS %d PHONENUMBER IN YOUR LIST.\n", name[i],phonenumber[i]);
      if (searching(name,NAme,numberofpeople)==0)
      printf("%s IS NOT IN YOUR CONTACT LIST.\n", NAme);
      printf("\n\nSEARCHING :::: [0] :::: ");
     scanf("%d", &ans);
     system("clear");
    if (ans!=0)
     break;
  }
}
