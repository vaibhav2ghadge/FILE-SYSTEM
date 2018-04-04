#include "struct.h"
#include <dirent.h>
#include <math.h>
#include "header.h"

void allocationTable(vhdd v)
{
    float noblock =(float) (v.filesize/v.blocksize);
    float tblesize = (float)(ceil((noblock/(v.blocksize*8))));
   fprintf(stdout,"no block %f initialize tble block size %f o %ld",noblock,tblesize,v.blocksize);
    int i;
unsigned char buff[v.blocksize];
    fseek(v.fp,v.blocksize,SEEK_SET);
    for(i=0;i<v.blocksize;i++)
        buff[i] = 0x00;
    for(i=0;i<10000/*tblesize-1*/;i++)
    fwrite(buff, sizeof(char), sizeof(buff),v.fp);
    fflush(v.fp);
}
void print(vhdd v)
{
    //14 blno 4 subbno
    readblock(v.mtds,&v);
    unsigned char ch[4];
    unsigned char ch1[4];
    int jj,jjj;
    int i;
   for(i=(v.blocksize/8)/8;i<v.blocksize/8;i++){
        ch[0] = v.buff[i];
        ch[1] = v.buff[i+1];
        ch[2] = v.buff[i+2];
        ch[3] = v.buff[i+3];
        ch1[0] = v.buff[i+4];
        ch1[1] = v.buff[i+5];
        ch1[2] = v.buff[i+6];
        ch1[3] = v.buff[i+7];
    jj = hextoint(ch);
    jjj = hextoint(ch1);
    if(jj&&jjj)
    fprintf(stdout,"block %d sub block %d",jj,jjj);
   }
}
int metadatablockinitialize(vhdd v)
{
    int i,k,p;
    long int freeblock = getfirstemptyblockpos(v); 
    setusedblock(v,freeblock);
    readblock(freeblock,&v);
    fprintf(stdout,"\n\nmeta data l%ld\n\n",freeblock);
    i = (v.blocksize/8)/8;
    for(k=0;k<i;k++)
        v.buff[k]=0x00;
        v.buff[0]=0x03;//hardcoded value
        v.buff[k-1]=0x80;//hardcoded value last block for address
    writeblock(freeblock,v);
    return freeblock;
}
int findmetafreeblock(vhdd *v)
{
    int i,j,free=0;
    for(i=0;i<((*v).blocksize/8)/8;i++)
    {
        if((int)(*v).buff[i]<255)
        {
            for(j=0;j<8;j++)
               if( ((*v).buff[i] & (1<<j))==0)
               {
                    (*v).buff[i]=(*v).buff[i]| 1<<j;//unused
                    return free+j+1;
               }
        }
        free+=8;
    }
    return 0;
}
int metadata(vhdd v,char *str)
{
    char **result;
    int i = stringsplit(str,&result,26);
    int c=0;
    int pos;
    int isd=v.nbs;
    int nv=v.nbs;
   pos= name1(v,&result,&i,&(nv),&c,&isd);
   free(result);
   fprintf(stdout,"return from name block subblock %d block %d",pos,isd);
    int block = isd;
   metadata1(v,block,pos,v.mtds);
    
}
/* print the meta data*/
void printname(vhdd v)
{
    readblock(130,&v);
    int i = (v.blocksize/8)/8;
    unsigned char block[4];
    unsigned char subblock[4];
    int bno;
    int subbloc;
    //    fprintf(stdout,"\n\t before before printing\n");
  // printbuff(v);
    for(i;i<v.blocksize;)
    {
        block[0] = v.buff[i-1];
        block[1] = v.buff[i];
        block[2] = v.buff[i+1];
        block[3] = v.buff[i+2];
        subblock[0]=v.buff[i+3];
        subblock[1]=v.buff[i+4];
        subblock[2]=v.buff[i+5];
        subblock[3]=v.buff[i+6];
     bno = hextoint(block);
     subbloc = hextoint(subblock);
    if((int)bno && (int)subbloc && subbloc<32)
    {
   fprintf(stdout,"\nbno %d sub %d\n",bno,subbloc);
     printnamesub(bno,subbloc,v);
    }
    // fprintf(stdout,"\n %d \n",i);
  // printbuff(v);
     i =i+8;
    }
}
void printnamesub(int bno,int subno,vhdd v)
{
    int i,j;
    unsigned char ch;
    unsigned char ch1[4];
    int bno1;
    readblock(bno,&v);
    while(1)
    {
        
    for(i = (subno-1)*v.nbsize;i<(subno-1)*v.nbsize+v.nbsize-5;i++)
        fprintf(stdout,"%c",v.buff[i]);
    ch = v.buff[i];
    ch1[0] = v.buff[i+1];
    ch1[1]=v.buff[i+2];
    ch1[2]=v.buff[i+3];
    ch1[3]=v.buff[i+4];
    j = hextoint(ch1);
  // if((int) v.buff[v.buff[1020]]==132)
  //fprintf(stdout,"\n*******finally founf********* %d %x %d j %d\n",ch1[0],ch1[1],ch1[2],j);    
/*   ch1[1] = v.buff[v.buff[i+2]];
   ch1[2] = v.buff[v.buff[i+3]];
   ch1[3] = v.buff[v.buff[i+4]];
  */  bno1 = hextoint(ch1);
  if((int)ch && j)
  {
    printnamesub(j,(int)ch,v);
    return;
  }
  else if((int)ch)
      subno = (int)ch;
    else
    return;
  // printnamesub(bno,(int)ch,v);
   //     fprintf(stdout,"\n");
    }
}
int metadata1(vhdd v,int block,int sub,int bno)
{
    unsigned char ch[4];
    unsigned char *pp;
    readblock(bno,&v);
    int p = findmetafreeblock(&v);
    fprintf(stdout,"\n p %d block %d sub block %d\n",p,block,sub);
    if(p)
    {
        unsigned char *a = (unsigned char*) &block;
        unsigned char *b = (unsigned char*) &sub;
        //fprintf(stdout,"\n\n postion start %d \n",(p-1)*8);
        v.buff[(p-1)*8-1]=a[0];
        v.buff[(p-1)*8]=a[1];
        v.buff[(p-1)*8+1]=a[2];
        v.buff[(p-1)*8+2]=a[3];
        v.buff[(p-1)*8+3]=b[0];
        v.buff[(p-1)*8+4]=b[1];
        v.buff[(p-1)*8+5]=b[2];
        v.buff[(p-1)*8+6]=b[3];
        writeblock(bno,v);
    }
    else
    {
       ch[0] = v.buff[v.blocksize-4];  
       ch[1] = v.buff[v.blocksize-3];  
       ch[2] = v.buff[v.blocksize-2];  
       ch[3] = v.buff[v.blocksize-1];  
       p = hextoint(ch);
       if(p)
        metadata1(v,block,sub,p);
       else
       {
            p = metadatablockinitialize(v);
            pp = (unsigned char*) &p;
            v.buff[v.blocksize-4] = pp[0];  
            v.buff[v.blocksize-3] = pp[1];  
            v.buff[v.blocksize-2] = pp[2];  
            v.buff[v.blocksize-1] = pp[3];
            writeblock(bno,v);
            metadata1(v,block,sub,p);
       
       }
    }

}

int freeblocknm(vhdd v,int b)
{
    int i,no=0,j=0;
    while(j<b)
    {
        if((int)v.buff[i]<255)
            for(i=0;i<7;i++)
                if(((v.buff[j]) & (1<<i))==0)
                    return no+i+1;
                else
                    no=no+8;
        j++;
    }
    return 0;/*if block not free return */
}
int stringsplit(char *str,char ***result,int no)
{
    int x = strlen(str)/no +1,i,k=0,j=0;
    *result = (char**)malloc(sizeof(char*)*x);
    if(*result==NULL)
        return 0;
    while(str[k]!='\0')
    {
        (*result)[j] = (char*)malloc(sizeof(char)*no);
        for(i=0;i<no&&str[k]!='\0';k++,i++)
        {
           (*result)[j][i]=str[k]; 
        }
    
     //   fprintf(stdout,"%s\n",(*result)[j]);
           (*result)[j][i]='\0'; 
        
        j++;
    }
    return j-1;
}
int findfreenameblock(vhdd *v)
{
    int i,j,free=0;
    for(i=0;i<(*v).nbsize/8;i++)
    {
        if((int)(*v).buff[i]<255)
        {
            for(j=0;j<8;j++)
               if( ((*v).buff[i] & (1<<j))==0)
               {
                    (*v).buff[i]=(*v).buff[i]| 1<<j;//unused
                    return free+j+1;
               }
        }
        free+=8;
    }
    return 0;
}
void insernameblockblock(vhdd *v,char *str,int bno)
{
    int i,j=0;
    for(i=(bno-1)*(*v).nbsize;i<bno*(*v).nbsize+(*v).nbsize && str[j]!='\0';i++)
        (*v).buff[i]=str[j++];
}
int hextoint(unsigned char *str)
{
    int ans=0;
    ans = (int)str[0]<<0;
    ans+=(int)str[1]<<8;
    ans+=(int)str[2]<<16;
   // ans+=(int)str[3]<<24;
    return ans;
}
void insertnameadd(vhdd *v,int sublock,int wblock,int bno1,int bno)
{
    int i,j=0,k;
    unsigned char *c = (unsigned char*) &sublock; 
  //fprintf(stdout,"\n in name %x  name %d size %d\n ",c[0],c[0],wblock*(*v).nbsize-5);
  // fprintf(stdout,"\n address pos %d add %d add %x \n",wblock*(*v).nbsize-5,c[0],c[0]);
    (*v).buff[wblock*(*v).nbsize-5]=c[0];
    c = (unsigned char*) &bno; 
    if(bno==bno1)
    {
       // fprintf(stdout,"\nputing zero to name\n");
        (*v).buff[wblock*(*v).nbsize-4]=0x00;
         (*v).buff[wblock*(*v).nbsize-3]=0x00;
        (*v).buff[wblock*(*v).nbsize-2]=0x00;
        (*v).buff[wblock*(*v).nbsize-1]=0x00;
    }
    else
    {
       // fprintf(stdout,"\nputing address %x %x %x %x\n",c[0],c[1],c[2],c[3]);
        (*v).buff[wblock*(*v).nbsize-4]=c[0];
        (*v).buff[wblock*(*v).nbsize-3]=c[1];
        (*v).buff[wblock*(*v).nbsize-2]=c[2];
        (*v).buff[wblock*(*v).nbsize-1]=c[3];
    }


}
int writeblock(int bno,vhdd v)
{
    fseek(v.fp,bno*(v.blocksize),SEEK_SET);
    fwrite(v.buff,1,(v.blocksize),v.fp);
    fflush(v.fp);
}
int name1(vhdd v,char ***nmstr,int *len,int *bno,int *c,int *lbno)
{
      readblock(*bno,&(v));
    int p =  name(&(v),&(*nmstr),&(*len),&(*bno),&(*c),&(*lbno));
   writeblock(*bno,v);
 //  printbuff(v);
  fprintf(stdout,"\nwriting %d\n",*bno);
    return p;//need to allocate new structure
}
int name(vhdd *v,char ***nmstr,int *len,int *bno,int *c,int *lbno) 
{
    int wp,i,p,j,free=0,ans=0,bno1,nm,lk,mbk;
    unsigned char *pp;
    unsigned char hex[4];
    unsigned long gh;
    unsigned char buff[(*v).blocksize];
    if((*c)>(*len))
    {
        return 0;
    }
    else
    {
        p=findfreenameblock(&(*v));
      fprintf(stdout,"\np %d\n",p);
        if(p)
        {
            j=(*c);
            *c = (*c)+1;
            bno1=*bno;
            i=name(&(*v),&(*nmstr),&(*len),&(*bno),&(*c),&(*lbno));
            insernameblockblock(&(*v),(*nmstr)[j],p);
            insertnameadd(&(*v),i,p,bno1,*lbno);
            *lbno=bno1;
            *bno = bno1;
            return p; 
        }
        else
        {
                lk = *bno;
            hex[0] = (*v).buff[(*v).nbsize-4] ;
            hex[1] = (*v).buff[(*v).nbsize-3] ;
            hex[2] = (*v).buff[(*v).nbsize-2] ;
            hex[3] = (*v).buff[(*v).nbsize-1] ;
            *bno= hextoint(hex);
         //    fprintf(stdout,"\nnot free %d\n",*bno);
          //   fprintf(stdout,"\nhex[0] %d\n",hex[0]);
           //  fprintf(stdout,"\nhex[1] %d\n",hex[1]);
            // fprintf(stdout,"\nhex[2] %d\n",hex[2]);
            // fprintf(stdout,"\nhex[3] %d\n",hex[3]);
            if(*bno)
            {
              //  fprintf(stdout,"\n Under if %d\n",*bno);
        
                 p=  name1(*v,&(*nmstr),&(*len),&(*bno),&(*c),&(*lbno));
                 *bno = lk;
                 return p;
            }
            else{
                gh=nameblockinitialize(*v);
                (*lbno)=gh;
                pp=((unsigned char*) &gh);
            (*v).buff[(*v).nbsize-4]= pp[0];
            (*v).buff[(*v).nbsize-3]=pp[1];
            (*v).buff[(*v).nbsize-2]=pp[2];
            (*v).buff[(*v).nbsize-1]=pp[3];
              *bno=gh;
            nm=name1(*v,&(*nmstr),&(*len),&(*bno),&(*c),&(*lbno));
           *bno = lk;
            return nm;
            }
        }
    }
    
       //printbuff(*v);
}
/*int nameblockinitialize(vhdd v)
{
   int fb = getfirstemptyblockpos(v),i;
   for(i=0;i<(v.blocksize/v.nbsize)/8;i++)
       v.buff[i]=0x00;
   v.buff[0]=0xfe;
writeblock(fb,v);
setusedblock(v,fb);
return fb;
}
*/
void puthardinfo(vhdd v)
{
    fseek(v.fp,0,SEEK_SET);
    fprintf(v.fp,"%ld",v.filesize);
    fseek(v.fp,10,SEEK_SET);
    fprintf(v.fp,"%ld",v.blocksize);
}
void setblockfree(vhdd v,long int block)
{
    float noblock =(float) (v.filesize/v.blocksize);
    int start = ceil((noblock/(v.blocksize*8)))+1,mn=start;
    unsigned char buff[v.blocksize];
    int i;
    fseek(v.fp,v.blocksize,SEEK_SET);
    while(mn--)
    {
        for(i=0;i<v.blocksize;i++)
            buff[i]=(unsigned char)fgetc(v.fp);
        if(block<=start+(v.blocksize*8))
        {
            int jj;
            for(jj=0;jj<v.blocksize;jj++)
            {
                if(block<=start+((jj+1)*8))
                {
                    long int x = block -start;
                    buff[jj] = (unsigned char)(buff[jj]^(buff[jj] & 1<<(x-1)));
                    fseek(v.fp,-(v.blocksize),SEEK_CUR);
                    fwrite(buff, sizeof(char), sizeof(buff),v.fp);
                    fflush(v.fp);
                    return;
                }
                start +=8;
            }
        }
        else
            start = (int)(start + (v.blocksize*8));
    }
}
void setusedblock(vhdd v,long int block)
{
    
    float noblock = (float)(v.filesize/v.blocksize);
    int start = (int)(ceil((noblock/(v.blocksize*8)))+1),mn=(int)start;
    unsigned char buff[v.blocksize];
    int i;
    fseek(v.fp,v.blocksize,SEEK_SET);
    while(mn--)
    {
        for(i=0;i<v.blocksize;i++)
            buff[i]=(unsigned char)fgetc(v.fp);
        if(block<=start+(v.blocksize*8))
        {
            int j;
            for(j=0;j<v.blocksize;j++)
            {
                if(block<=start+((j+1)*8))
                {
                    int x = (int)(block-start);
                    buff[j] =(unsigned char)(buff[j]| 1<<(x-1));
                    fseek(v.fp,-(v.blocksize),SEEK_CUR);
                    fwrite(buff, sizeof(char), sizeof(buff),v.fp);
                    fflush(v.fp);
                    return;
                }
                start+=8;
            }
        }
        else
            start =(int)( start + (v.blocksize*8));
    }

}
unsigned long getfirstemptyblockpos(vhdd v)
{
    int i=0;

       int noblock = (v.filesize/v.blocksize);
    float tblesize = ceil((noblock/(v.blocksize*8)));
    unsigned long fbn=tblesize+1;
    unsigned char buff[v.blocksize];
    fseek(v.fp,1024,SEEK_SET);
    while(tblesize--)
    {
        for(i=0;i<v.blocksize;i++)
            buff[i]=(unsigned char)fgetc(v.fp);

        for(i=0;i<v.blocksize;i++)
        {
            if((int)buff[i]<255)
            {
                int j,p=1;
                for(j=0;j<8;j++)
                {
                    p = buff[i] & 1<<j;
                    if(p==0)
                    {
                        fbn+=(j+1);
                        return fbn;
                    }
                }
            }
            else
                fbn+=8;
        }
    }
    return 0;
}

int createVHDD(unsigned long int fsize,unsigned long int bsize,vhdd *Hdd,char *fname)
{
    
   int s; 
    DIR *d;
    char fal[25];
    char f1[10];
    struct dirent *dir;
    d = opendir(".");
    if(d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if(!(strcmp(dir->d_name,fname)))
                    return 2;
        }
        closedir(d);
    }
    strcpy(fal,"fallocate -l ");
    s = (int )fsize/(1024*1024*1024);
    fprintf(stdout,"%d\n",s);
    sprintf(f1,"%d",s);
    fprintf(stdout,"%s\n",f1);
    strcat(fal,f1);
    strcat(fal,"G ");
    strcat(fal,fname);
    fprintf(stdout,"%s",fal);
    (*Hdd).filesize = (long int)fsize;
    (*Hdd).blocksize =(long int) bsize;
    (*Hdd).smb = (bsize/8)/8;
    (*Hdd).sb = (int)(ceil(((fsize/bsize)/(bsize*8))))+1;
 //(*Hdd).buff = (unsigned char*)malloc(sizeof(unsigned char)*bsize);
 (*Hdd).nbsize=32;
(*Hdd).nbnb = bsize/(*Hdd).nbsize;
 //if((*Hdd).buff==NULL)
  //   return 2;
 //system(fal);
(*Hdd).fp = fopen("t","r+");
 
allocationTable(*Hdd);
 (*Hdd).mtds = metadatablockinitialize(*Hdd);
(*Hdd).nbs = nameblockinitialize(*Hdd);/* from where name block start */
puthardinfo(*Hdd);
   return 1;
}
unsigned long nameblockinitialize(vhdd v)
{
   unsigned long fb = getfirstemptyblockpos(v),i;
  fprintf(stdout,"name block %lu",fb);

   for(i=0;i<v.blocksize;i++)
       v.buff[i]=0x00;

   for(i=0;i<(v.blocksize/v.nbsize)/8;i++)
       v.buff[i]=0x00;
  v.buff[0]=0x01;/*first block already allocated to initialize table*/
writeblock(fb,v);
setusedblock(v,fb);
return fb;
}/*
int writeblock(int bno,vhdd v)
{

    fseek(v.fp,bno v.blocksize,SEEK_SET);
    fwrite(v.buff,(v.blocksize)-1,1,v.fp);
    fflush(v.fp);
}*/
int readblock(int bno,vhdd *v)
{

    fseek((*v).fp,bno * (*v).blocksize,SEEK_SET);
    fread((*v).buff,1,(*v).blocksize,(*v).fp);
}
void printbuff(vhdd v)
{
    int i;
       fprintf( stdout,"\n*****Hex*********\n");
    for(i=0;i<v.blocksize;i++)
       fprintf( stdout,"%x",v.buff[i]);
       fprintf( stdout,"\n*****Integer*********\n");
    for(i=0;i<v.blocksize;i++)
       fprintf( stdout,"%d",v.buff[i]);
       fprintf( stdout,"\n*****Character*********\n");
    for(i=0;i<v.blocksize;i++)
       fprintf( stdout,"%c",v.buff[i]);
}

