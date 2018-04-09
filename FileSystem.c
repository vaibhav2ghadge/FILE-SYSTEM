#include<stdio.h>
#include <math.h>
#include<stdlib.h>
#include<string.h>
#include "initial.h"
#include <dirent.h>
#include <fcntl.h>
int main(void)
{
    system("clear");
    FILE *fp;
    char fname[100];
    int ch1,no1,pp;
    vhdd Hdd; 
    int c=0,len,p=0;
    char **str;
    int no,nbs,i;
    while(1)
    {
        fprintf(stdout,"\n\t1) Create virtual Disk : ");     
        fprintf(stdout,"\n\t2) Insert Name As Long As Possible (: :  ");     
        fprintf(stdout,"\n\t4) DELETE Name");
        fprintf(stdout,"\n\t 6) PRINT NAME ");
        fscanf(stdin,"%d",&ch1);
        switch(ch1)
        {
            case 1:
            /*        fprintf(stdout,"\n\t Enter File Name: ");
                    fscanf(stdin,"%s",fname);
                    fprintf(stdout,"\n\t Enter File Size in Bytes: ");
                    fscanf(stdin,"%ld",&fsize);
                    fprintf(stdout,"\n\t Enter Block size In Kb: ");
                    fscanf(stdin,"%ld",&bsize);*/
                   /* Hdd = (vhdd*) malloc (sizeof(vhdd));
                    if(Hdd ==NULL){
                        fprintf(stdout,"\n\t Memory allocation Faild");
                            exit(0);
                    }*/
                    no1=createVHDD(1073741824,1024,&Hdd,"p");
                    if(no1==1)
                    {
                        fprintf(stdout,"\n\tVirtual Disk Created Succefully...");
                    }
                    else if(no1==2)
                        fprintf(stdout,"\n\t Virtual Disk Already Present");
                    
                    break;
            case 2:for(i=0;i<1000;i++){
            //  metadata(Hdd,"MYName is khan im ok not ok jadu haha");                   
                 metadata(Hdd,"Computer science is the study of the theory, experimentation, and engineering that form the basis for the design and use of computers. It is the scientific and practical approach to computation and its applications and the systematic study of the feasibility, structure, expression, and mechanization of the methodical procedures (or algorithms) that underlie the acquisition, representation, processing, storage, communication of, and access to, information. An alternate, more succinct definition of computer science is the study of automating algorithmic processes that scale. A computer scientist specializes in the theory of computation and the design of computational systems");}
                    /*
                    c=0;
                    nbs = Hdd.nbs;
                    len = stringsplit("ok hghghgghghgh ghggogle ok google ok google ",&str,26);
                    p=Hdd.nbs;
                     name1(Hdd,&str,&len,&(Hdd.nbs),&c,&p);
                    Hdd.nbs = nbs;
                    fprintf(stdout,"\nafter execution name block start %d\n",Hdd.nbs);
                    free(str);
                   // readblock(131,&Hdd);
                   // printbuff(Hdd);
                   //  metadata(Hdd,"ter limit is 280.Snapchat's caption character limit is 80.character limithghghjj");*/
                   break;
            case 3: 
            case 5: exit(0);
                    break;
            case ' ':
                    break;
            case 4:{
                       char *name = "MYName is khan im ok not ok jadu haha";
                    deletename(Hdd,name);
                    /*readblock(130,&Hdd);
                    printbuff(Hdd);*/
                   }
                    break;
            case 6:
                        printname(Hdd);
                        break;
            default : fprintf(stdout,"\n\tWrong choice: ");
        }
    }
 
        
}
