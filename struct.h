
typedef struct vhdd vhdd;
struct vhdd
{
    long int filesize;
    long int blocksize;
    FILE *fp;
    int smb;/*startting of metadata block data*/
    int sb;/*start of data block*/
    unsigned char buff[1024];
    int nbsize;//name block block size
    int nbnb;//name block number of block
    int mtds;/*block from meta data started*/
    int nbs;/* block from name block started*/
};
