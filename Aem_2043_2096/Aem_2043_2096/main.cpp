#include "dbtproj.h"
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <sys/time.h>
#include <iostream>
#include <ctime>
using namespace std;

typedef unsigned long long timestamp_t;
static timestamp_t
get_timestamp ()
{
    struct timeval now;
    gettimeofday (&now, NULL);
    return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}



int main(int argc, char** argv)
{

    timestamp_t t0 = get_timestamp();



    int nblocks = 100;	// number of blocks in the file
    record_t record;
    block_t *buffer;
    buffer = (block_t*) malloc (sizeof(block_t));
    double secs ;
    char * buffer2;

    unsigned int nmem_blocks=5;

    //buffer = (block_t*)malloc(9*sizeof(block_t));

    unsigned int *nsorted_segs,*npasses,*nunique,*nres,*nios =0;
    unsigned int recid = 0;
    FILE *infile, *outfile;


    srand(time(NULL));
    // generate a file
    outfile = fopen("file17.bin", "w");

    for (int b=0; b<nblocks; ++b)   // for each block
    {

        buffer->blockid = b;

        for (int r=0; r<MAX_RECORDS_PER_BLOCK; ++r)   // for each record
        {

            // prepare a record
            record.recid = recid++;
            record.num = rand() % 1000000;
            buffer2 = (char*) malloc (10);


            for (int n=0; n<9; n++) {
                buffer2[n]=rand()%26+'a';
            buffer2[9]='\0';
            }

            strcpy(record.str,buffer2);   // put the same string to all records

            record.valid = true;

            memcpy(&buffer->entries[r], &record, sizeof(record_t)); // copy record to block
        }

        buffer->nreserved = MAX_RECORDS_PER_BLOCK;
        buffer->valid = true;

        buffer->misc='0';
        buffer->next_blockid=0;
        buffer->dummy=10;

        fwrite(buffer, 1, sizeof(block_t), outfile);	// write the block to the file
    }

    fclose(outfile);
    char *inn=new char[20];
    char *out=new char[20];
    char *out_2=new char[20];
    char *inn_1=new char[20];
    char *inn_2=new char[20];
    char *out_3=new char[20];
    strcpy(inn,"file17.bin");
    strcpy(out,"final_17.bin");
    strcpy(out_2,"final_new_17.bin");
    strcpy(inn_1,"final_new_17.bin");
    strcpy(inn_2,"final_new_16.bin");
    strcpy(out_3,"join_final6.bin");

    free(buffer);

//buffer=NULL;
    // open file and print contents
    MergeSort(inn,'1',buffer,nmem_blocks,out,nsorted_segs,npasses,nios);
  EliminateDuplicates (out, '1', buffer,  nmem_blocks, out_2, nunique, nios);
    MergeJoin (inn_1, inn_2, '1', buffer, nmem_blocks, out_3, nres, nios);
    timestamp_t t1 = get_timestamp();
    secs = (t1 - t0) / 1000000.0L;

    cout <<"Execution time :   "<<  secs<<" s"<<endl;
    return 0;
}
