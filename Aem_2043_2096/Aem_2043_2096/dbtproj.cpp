#include "dbtproj.h"
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
using namespace std;


//Gia kapoio logo den mporousa na eisagw ellinikous xaraktires opote sxoliasa ton kwdika se greeklish !

void MergeSort (char *infile, unsigned char field, block_t *buffer, unsigned int nmem_blocks, char *outfile, unsigned int *nsorted_segs, unsigned int *npasses, unsigned int *nios)
{

    FILE *in,*out;
    unsigned long int n=0; //voithitiki metavliti gia na allazoume kathe fora to onoma tou taksinomimenou arxeiou
    char filename[25]; //voithitiki metavliti gia na allaksoume to onoma tou arxeiou
    in = fopen(infile, "r");

    int i=0;
    int size=0;
    long int counter_all=0; //metritis gia to plithos twn num stoixeiwn tou arxeiou
    record_t *buffer_rec;
    unsigned int length;
    length=nmem_blocks*MAX_RECORDS_PER_BLOCK; //gia tin xwritikotita tou pinaka buffer_rec[]


    buffer_rec = (record_t*)malloc(length*sizeof(record_t)); //pinakas ston opoio apothikeuoume kathe fora ena kainourgio block me dedomena

    buffer = (block_t*) malloc (sizeof(block_t)); //o buffer pou xrisimopoioume
    int blockid=0;  //to id tou kathe block
    double count_blocks=0; //metritis gia to plithos twn block tou arxikou arxeiou
    unsigned int segments,passes,ios=0;
    nsorted_segs= &segments; //plithos twn taksinomimenwn kommatiwn
    npasses=&passes; //plithos twn perasmatwn
    nios=&ios; //plithos twn IOs


    /*-----se auto to vrogxo oso den exoume ftasei sto telos tou arxeiou , tha ekteloume epanalipseis .Se kathe epanalipsi
    tha taksinomoume ton buffer_rec[] o opoios periexei enan plithos apo num stoixeia kai etsi tha dimiourgoume se kathe epanalipsi , ena taksinomimeno kainourgio arxeio-tmima.    ------*/
    while (!feof(in))
    {



        /*------ se auto ton vrogxo ekxwroume se kathe epanalipsi to pedio num apo ton pinaka entries[] ston pinaka buffer_rec[]
         auto to kanoume gia na taksinomisoume ola ta num tou arxeiou kai na dimiourgisoume taksinomimena tmimata-arxeia
         Etsi mexri na gemisei o buffer diladi mexri to i>nmem_blocks tha eisagoume blocks apo to arxeio.Sti sinexeia afou gemisei o buffer rec
         tha ton taksinomisoume . ------*/


        while (i<nmem_blocks)  //mexri na gemisei o buffer
        {
            count_blocks++; //


            // size=fread(&r, 1, sizeof(r), in);
            size=fread(buffer, 1, sizeof(block_t), in); // read the next block
            if (feof(in))
            {
                count_blocks--;  //auksanoume ton plithos twn blocks pou exoume diavasei apo to arxeio

                break;
            }


            //ekxwroume ston buffer_rec[] ta num apo to ekastote block tou arxeiou
            for (int j=0; j<MAX_RECORDS_PER_BLOCK ; j++)
            {

                buffer_rec[j+(i*MAX_RECORDS_PER_BLOCK)]=buffer->entries[j];

            }

            i++; //auksanoume ton metriti tou vrogxou


        }


        //kaloume tin quicksort gia na taksinomisoume ton buffer_rec[]

        Quicksort(buffer_rec, 0,(i*MAX_RECORDS_PER_BLOCK)-1,field );


        //metonomazoume to arxeio dinontas tou san onoma ton arithmo analoga me to pote taksinomithike , px to prwto arxeio tha exei onoma "1.bin"
        snprintf(filename,sizeof(filename),"%d",n);

        strcat(filename,".bin");
        out = fopen(filename, "w");

        //Sfalma se periptwsi pou den mporoume na anoiksoume to arxeio mas
        if(!out)
        {
            printf("Error! data file failed to open\n");
            exit(1);
        }


        /* Grafoume to ekastote taksinomimeno tmima se ena ksexwristo arxeio. */
        for (int k=0; k<i; k++)
        {
            buffer->blockid = ++blockid;

            for (int j=0; j<MAX_RECORDS_PER_BLOCK ; j++)
            {
                counter_all++; //auksanoume to plithos twn num stoixeiwn
                buffer->entries[j]=buffer_rec[j+(k*MAX_RECORDS_PER_BLOCK)]; //ekxwroumai ton taksinomimeno pinaka buffer_rec[] ston pinaka buffer gia na to grapsoume sto teliko arxeio
                // printf("this is block id: %d, record id: %d, num: %d, str: %s\n",
                //  buffer->blockid, buffer->entries[j].recid, buffer->entries[j].num, buffer->entries[j].str);
            }

            buffer->nreserved = MAX_RECORDS_PER_BLOCK;
            buffer->valid = true;
            fwrite(buffer, 1, sizeof(block_t), out);
        }
        fclose(out);
        ++n;

        i=0;


    }


    fclose(in);
    free(buffer); //apodesmeuoume tin mnimi mas

    *nsorted_segs= count_blocks/nmem_blocks; // o arithmos twn taksinomimenwn tmimatwn tou prwtou perasmatos



    Merging(buffer,  n,nmem_blocks,nsorted_segs, npasses, nios,counter_all,field);

    rename("0.bin",outfile); //metonomazoume to arxeio se "final.bin"

}

/* Methodos taksinomisis tou buffer_rec.Xrisimopoioume tin gnwsti methodo Quicksort i opoia stin sugkekrimeni periptwsi einai kai i veltisti methodos taksinomisis. */
void Quicksort(record_t arr[ ], int low, int high,unsigned char field)
{


    int pivot,j,i;
    record_t temp;


    if(low<high)
    {
        pivot = low;
        i = low;
        j = high;

        while(i<j)
        {
            if (field=='0')
            {


                while((arr[i].recid<=arr[pivot].recid)&&(i<high))
                {
                    i++;
                }


                while(arr[j].recid>arr[pivot].recid)
                {
                    j--;
                }



            }

            else if (field=='1')
            {


                while((arr[i].num<=arr[pivot].num)&&(i<high))
                {
                    i++;

                }


                while(arr[j].num>arr[pivot].num)
                {
                    j--;

                }


            }


            else
            {

                while ((strncmp(arr[i].str,arr[pivot].str,1)<=0)&&(i<high) )

                {

                    i++;

                }

                while ((strncmp(arr[j].str,arr[pivot].str,1)>0))

                {
                    j--;

                }

            }









            if(i<j)
            {
                temp=arr[i];
                arr[i]=arr[j];
                arr[j]=temp;
            }
        }



        temp=arr[pivot];
        arr[pivot]=arr[j];
        arr[j]=temp;
        Quicksort(arr,low,j-1,field);
        Quicksort(arr,j+1,high,field);
    }

}

/*-------    Afou exoume dimiourgisei ola ta taksinomimena tmimata-arxeia irthe i wra na ta sugxwneusoume ola se ena teliko arxeio.Auto tha to pragmatopoisioume
me tin parakatw sunartisi i opoia tha kanei [nmem_blocks-1] - way Merge.Gia paradeigma ean diathetoume 9 blocks stin kuria mnimi mas tote tha eisagoume ena block apo kathe ena apo ta  9 taksinomimena arxeia
kai tha ta ekxwrisoume sta 8 block pou exoume kai to teleutaio tha to xrisimopoiisoume san apothikeuto xwro gia ta apotelesmata tou 8 way-wmerge.Sti sunexeia tha   ektelesoume ton 8 way merge kai tha ekxwrisoume
to apotelesma sto teleutaio block .An gemisei to block auto tote grafoume ta apotelesmata sto output.Tin diadikasia auti tha tin ektelesoume mexris otou ekxwrisoume ola ta blocks apo ta taksinomimena tmimata , ston buffer mas.
Auto tha einai to deutero perasma.Tin diadikasia auti tha tin kanoume mexri na mas exei apomeinei mono ena arxeio to opoio tha einai kai to teliko arxeio to opoio tha exei ola ta blocks diladi ola ta  num taksinomimena   ------*/




void  Merging(block_t buffer[], int num_files,unsigned int nmem_blocks, unsigned int *nsorted_segs, unsigned int *npasses, unsigned int *nios,long int counter_all,unsigned char field)
{

    FILE *out;
    FILE *fp[num_files];
    buffer = (block_t*)malloc(nmem_blocks*sizeof(block_t));
    record_t records[nmem_blocks-1]; // o pinakas autos periexei kathe fora ena num apo kathe block pou exoume stin ram . Kanoume taksinomisi se auton kai to apotelesma to grafoume sto teleutaio block


    char filename[num_files][25]; // o pinakas me ta onomata twn taksinomimenwn arxeiwn pou dimiourgithikan sto prwto perasma
    char newfilename[25]="final20"; // to neo onoma tou arxeiou pou dimiourgeitai apo to sorted run

    int num_files_run_=num_files;  //o trexon arithmos arxeiwn pou ginetai to merging
    int index[nmem_blocks-1];  // deiktis pou deixnei se poia thesi vriskomaste ston pinaka entries [] pou exei to kathe block
    int min=0;  // o mikroteros  trexon arithmos
    char min_str[STR_LENGTH]=""; // i mikroteri trexon sumboloseira
    int thesi=0;  //i thesi tou mikroterou arithmou
    int newfiles=0;

    int counter=0;  //metraei to plithos twn arithmwn pou apothikeuontai sto output arxeio


    int empty_blocks=0;  //posa blocks  apo tin ram einai adeia
    int current_level_files= num_files; //arithmos arxeiwn gia to trexon epipedo
    int sorted_runs=0; //arithmos twn sorted runs gia to trexon epipedo
    int prev_number=0;  // to xrisimopoioume gia na ulopoiisoume to remove duplicates
    *npasses=0;
    *nios=0;

    int end=0;


    /*---  Kathe epanalipsi sto sugkekrimeno vrogxo antistoixei se ena perasma .Diladi edw mesa sugxneuoume kai taksinomoume kathe fora
    ola ta arxeia tou trexontos epipedou.Afou ta taksinomisoume ta apothikeuoume se  kainourgia arxeia.Auta ta kainourgia arxeia ta taksinomoume
    stin epomeni epanalipisi i opoia tha einai to epomeno perasma.Tin diadikasia autin tin akolouthoume mexris otou sto trexon epipedo pou tha vriskomaste
    na uparxei mono ena arxeio to opoio tha einai kai to teliko taksinomimeno arxeio.  ----*/


    while (current_level_files>1)    //oso ta arxeia sto trexon epipedo einai perissotera apo 1
    {
        int tmp=0;
        sorted_runs=0;
        newfiles=0;

        end =0; //metavliti pou leitourgei ws termatiki sunthiki se kapoious vrogxous

        /*Se kathe epanalipsi autou tou vrogxou kanoume mia 8-way Merge kai dimiourgoume ena kainourgio taksinomimeno arxeio.
        Mexris otou ta taskinomimena auta arxeia na einai ligotera apo ton arithmo twn trexon arxeiwn pou exw gia sugxwneusi / twn blocks pou exw stin Ram.
         */


        while ((double)sorted_runs<(double)current_level_files/(nmem_blocks-1))   //oso den exoume ftasei to orio twn sorted runs
        {

            //ean ta arxeia pou exw gia sugxwneusi einai perissotera apo twn arithmo twn diathesimwn blocks stin Ram
            if (num_files>nmem_blocks-1)
            {
                num_files=num_files - nmem_blocks +1;
                num_files_run_=nmem_blocks-1; //ta arxeia gia sugxwneusi tha einai osa kai ta blocks -1
                end =(sorted_runs+1) * (nmem_blocks-1) ;

            }
            else
            {
                end =end + num_files;
                num_files_run_=num_files;

            }

            //--Anoigma twn arxeiwn pou einai gia sugxwneusi sto trexon epipedo---
            for (int i =sorted_runs*(nmem_blocks-1); i<end; i++)
            {

                snprintf(filename[i],sizeof(filename[i]),"%d",i);

                strcat(filename[i],".bin");
                fp[i] = fopen(filename[i], "r");

                //Se periptwsi sfalmatos , exit .
                if(!fp[i])
                {
                    printf("Error! Unable to open fp1 files for merging\n");
                    printf("filename1:%s\n",filename[i]);

                    exit(1);
                }


            }
            //FIELD 0 !!!



            if ( field=='0')
            {
                //arxikopoiw tous pinakes records[] kai index[]
                for ( int m=0; m<num_files_run_; m++)
                {

                    records[m].recid=0;
                    index[m]=0;
                }

                out=fopen("temp.bin","w");



                if(!out)
                {
                    printf("Error! Unable to open temp.dat, to write merge data\n");
                    exit(1);
                }


                int j =sorted_runs*(nmem_blocks-1);

                //--ekxwrw apo kathe block stin ram , ton prwto arithmo ston pinaka record stin antistoixi thesi--
                for (int i=0; i<num_files_run_; i++)
                {
                    fread(&buffer[i], 1, sizeof(block_t), fp[j]);

                    records[i].recid=buffer[i].entries[0].recid;

                    j++;
                }

                min=records[0].recid; //ws min vazw to prwto stoixeio
                thesi=0;
                empty_blocks=0;


                //Ston sugkekrimeno vrogxo taksinomw olous ta num apo ta blocks pou exw stin mnimi. Kathe fora tha travaw enan arithmo apo kathe block tha ton vazw
                //se ena pinaka records[] kai tha ton taksinomw.Ton mikrotero tha ton vazw sto output.Stin sunexeia tha fernw ton epomeno arithmo apo to block to opoio vrika ton mikrotero sto proigoumeno vima.
                //Auto tha to kanw sunexeia mexri na adeiasoun ola ta blocks .An adeiasei ena block tote fernw ena allo apo ton antistoixo arxeio pou pira to sugkekrimeno.
                //An adeiasoun kai ola ta arxeia tote auto simainei oti ektelestike epitiximena 1 sorted run kai etsi teleiwnei o vrogxos
                do
                {
                    //Vriskw to min apo ton records[] kai tin antistoixi thesi tou
                    for (int i=0; i <num_files_run_; i++)
                    {
                        if (min>records[i].recid)
                        {
                            min=records[i].recid;
                            thesi=i;
                        }

                    }




                    //antigrafw to mikrotero record sto teleutaio block tis Ram.
                    memcpy(& buffer[nmem_blocks-1].entries[counter],&buffer[thesi].entries[index[thesi]], sizeof(record_t));
                    counter++;


                    //--------  An exei gemisei to output diladi to teleutaio block tis Ram tote ta antigrafw se ena arxeio --------
                    if (counter==MAX_RECORDS_PER_BLOCK)
                    {

                        buffer[nmem_blocks-1].blockid = tmp++;
                        buffer[nmem_blocks-1].nreserved = MAX_RECORDS_PER_BLOCK;
                        buffer[nmem_blocks-1].valid = true;
                        counter=0;
                        // Kanw print  ta apotelesmata
                        for (int j=0; j<MAX_RECORDS_PER_BLOCK; ++j)
                        {
                            //  printf("this is block id: %d, record id: %d, num: %d, str: %s\n",
                            //     buffer[nmem_blocks-1].blockid, buffer[nmem_blocks-1].entries[j].recid, buffer[nmem_blocks-1].entries[j].num, buffer[nmem_blocks-1].entries[j].str);
                        }
                        fwrite(&buffer[nmem_blocks-1], 1, sizeof(block_t), out);


                    }


                    //Efoson vrikame ton min arithmo tha prepei na feroume allo arithmo stin thesi tou gia tin taksinomisi.An omws den mporw na ferw allo arithmo epeidi exoume ftasei sto telos tou block tote :

                    if (index[thesi]==MAX_RECORDS_PER_BLOCK-1)  // an teleiwsei ena block apo tin ram tote tha ferw ena kainourgio apo to antistoixo arxeio pou pira kai to proigoumeno
                    {
                        int size=0;
                        size=sizeof(block_t);
                        if (size==fread(&buffer[thesi], 1, sizeof(block_t), fp[thesi]) ) //ean uparxei allo block sto arxeio tote to fernw
                        {

                            index[thesi]=0;
                            records[ thesi].recid=buffer[thesi].entries[index[thesi]].recid; //eisagw ton epomeno arithmo gia tin taksinomisi

                        }
                        else //ean dn uparxei allo block apo to antistoixo arxeio ekxwrw enan poli megalo arithmo stin antistoixi thesi tou records gia na eimai sigouros oti dn prokeitai na alloiwsei kanena apotelesma
                        {

                            records[thesi].recid=100000000;
                            empty_blocks++;
                        }
                        min=records[ thesi].recid;

                    }

                    else //ean  den exei ftasei sto telos tou to block tote
                    {
                        index[thesi]++;
                        records[thesi].recid=buffer[thesi].entries[index[thesi]].recid;    //eisagw ton epomeno arithmo gia tin taksinomisi
                        min=records[thesi].recid; //ton orizw ws ton mikrotero tuxaia
                    }

                }
                while(empty_blocks!=num_files_run_); //oso den exoun adeiasei ola ta block pou exw desmeusi stin ram
            }
            //FIELD 1 !!!!

            if ( field=='1')
            {
                //arxikopoiw tous pinakes records[] kai index[]
                for ( int m=0; m<num_files_run_; m++)
                {

                    records[m].num=0;
                    index[m]=0;
                }

                out=fopen("temp.bin","w");



                if(!out)
                {
                    printf("Error! Unable to open temp.dat, to write merge data\n");
                    exit(1);
                }


                int j =sorted_runs*(nmem_blocks-1);

                //--ekxwrw apo kathe block stin ram , ton prwto arithmo ston pinaka record stin antistoixi thesi--
                for (int i=0; i<num_files_run_; i++)
                {
                    fread(&buffer[i], 1, sizeof(block_t), fp[j]);

                    records[i].num=buffer[i].entries[0].num;

                    j++;
                }

                min=records[0].num; //ws min vazw to prwto stoixeio
                thesi=0;
                empty_blocks=0;


                //Ston sugkekrimeno vrogxo taksinomw olous ta num apo ta blocks pou exw stin mnimi. Kathe fora tha travaw enan arithmo apo kathe block tha ton vazw
                //se ena pinaka records[] kai tha ton taksinomw.Ton mikrotero tha ton vazw sto output.Stin sunexeia tha fernw ton epomeno arithmo apo to block to opoio vrika ton mikrotero sto proigoumeno vima.
                //Auto tha to kanw sunexeia mexri na adeiasoun ola ta blocks .An adeiasei ena block tote fernw ena allo apo ton antistoixo arxeio pou pira to sugkekrimeno.
                //An adeiasoun kai ola ta arxeia tote auto simainei oti ektelestike epitiximena 1 sorted run kai etsi teleiwnei o vrogxos
                do
                {
                    //Vriskw to min apo ton records[] kai tin antistoixi thesi tou
                    for (int i=0; i <num_files_run_; i++)
                    {
                        if (min>records[i].num)
                        {
                            min=records[i].num;
                            thesi=i;
                        }

                    }




                    //antigrafw to mikrotero record sto teleutaio block tis Ram.
                    memcpy(& buffer[nmem_blocks-1].entries[counter],&buffer[thesi].entries[index[thesi]], sizeof(record_t));
                    counter++;


                    //--------  An exei gemisei to output diladi to teleutaio block tis Ram tote ta antigrafw se ena arxeio --------
                    if (counter==MAX_RECORDS_PER_BLOCK)
                    {

                        buffer[nmem_blocks-1].blockid = tmp++;
                        buffer[nmem_blocks-1].nreserved = MAX_RECORDS_PER_BLOCK;
                        buffer[nmem_blocks-1].valid = true;
                        counter=0;
                        // Kanw print  ta apotelesmata
                        for (int j=0; j<MAX_RECORDS_PER_BLOCK; ++j)
                        {
                            //  printf("this is block id: %d, record id: %d, num: %d, str: %s\n",
                            //     buffer[nmem_blocks-1].blockid, buffer[nmem_blocks-1].entries[j].recid, buffer[nmem_blocks-1].entries[j].num, buffer[nmem_blocks-1].entries[j].str);
                        }
                        fwrite(&buffer[nmem_blocks-1], 1, sizeof(block_t), out);


                    }


                    //Efoson vrikame ton min arithmo tha prepei na feroume allo arithmo stin thesi tou gia tin taksinomisi.An omws den mporw na ferw allo arithmo epeidi exoume ftasei sto telos tou block tote :

                    if (index[thesi]==MAX_RECORDS_PER_BLOCK-1)  // an teleiwsei ena block apo tin ram tote tha ferw ena kainourgio apo to antistoixo arxeio pou pira kai to proigoumeno
                    {
                        int size=0;
                        size=sizeof(block_t);
                        if (size==fread(&buffer[thesi], 1, sizeof(block_t), fp[thesi]) ) //ean uparxei allo block sto arxeio tote to fernw
                        {

                            index[thesi]=0;
                            records[ thesi].num=buffer[thesi].entries[index[thesi]].num; //eisagw ton epomeno arithmo gia tin taksinomisi

                        }
                        else //ean dn uparxei allo block apo to antistoixo arxeio ekxwrw enan poli megalo arithmo stin antistoixi thesi tou records gia na eimai sigouros oti dn prokeitai na alloiwsei kanena apotelesma
                        {

                            records[thesi].num=10000000;
                            empty_blocks++;
                        }
                        min=records[ thesi].num;

                    }

                    else //ean  den exei ftasei sto telos tou to block tote
                    {
                        index[thesi]++;
                        records[thesi].num=buffer[thesi].entries[index[thesi]].num;    //eisagw ton epomeno arithmo gia tin taksinomisi
                        min=records[thesi].num; //ton orizw ws ton mikrotero tuxaia
                    }



                }
                while(empty_blocks!=num_files_run_); //oso den exoun adeiasei ola ta block pou exw desmeusi stin ram
            }
            //FIELD 2 !!!!


            if ( field=='2')
            {
                //arxikopoiw tous pinakes records[] kai index[]
                for ( int m=0; m<num_files_run_; m++)
                {

                    strcpy(records[m].str,"");
                    index[m]=0;
                }

                out=fopen("temp.bin","w");



                if(!out)
                {
                    printf("Error! Unable to open temp.dat, to write merge data\n");
                    exit(1);
                }


                int j =sorted_runs*(nmem_blocks-1);

                //--ekxwrw apo kathe block stin ram , ton prwto arithmo ston pinaka record stin antistoixi thesi--
                for (int i=0; i<num_files_run_; i++)
                {
                    fread(&buffer[i], 1, sizeof(block_t), fp[j]);

                    strcpy( records[i].str,buffer[i].entries[0].str);

                    j++;
                }

                strcpy(min_str,records[0].str); //ws min vazw to prwto stoixeio
                thesi=0;
                empty_blocks=0;


                //Ston sugkekrimeno vrogxo taksinomw olous ta num apo ta blocks pou exw stin mnimi. Kathe fora tha travaw enan arithmo apo kathe block tha ton vazw
                //se ena pinaka records[] kai tha ton taksinomw.Ton mikrotero tha ton vazw sto output.Stin sunexeia tha fernw ton epomeno arithmo apo to block to opoio vrika ton mikrotero sto proigoumeno vima.
                //Auto tha to kanw sunexeia mexri na adeiasoun ola ta blocks .An adeiasei ena block tote fernw ena allo apo ton antistoixo arxeio pou pira to sugkekrimeno.
                //An adeiasoun kai ola ta arxeia tote auto simainei oti ektelestike epitiximena 1 sorted run kai etsi teleiwnei o vrogxos
                do
                {
                    //Vriskw to min apo ton records[] kai tin antistoixi thesi tou
                    for (int i=0; i <num_files_run_; i++)
                    {
                        if ((strncmp(min_str,records[i].str,STR_LENGTH))>0)
                        {
                            strcpy(min_str,records[i].str);
                            thesi=i;
                        }

                    }




                    //antigrafw to mikrotero record sto teleutaio block tis Ram.
                    memcpy(& buffer[nmem_blocks-1].entries[counter],&buffer[thesi].entries[index[thesi]], sizeof(record_t));
                    counter++;


                    //--------  An exei gemisei to output diladi to teleutaio block tis Ram tote ta antigrafw se ena arxeio --------
                    if (counter==MAX_RECORDS_PER_BLOCK)
                    {

                        buffer[nmem_blocks-1].blockid = tmp++;
                        buffer[nmem_blocks-1].nreserved = MAX_RECORDS_PER_BLOCK;
                        buffer[nmem_blocks-1].valid = true;
                        counter=0;
                        // Kanw print  ta apotelesmata
                        for (int j=0; j<MAX_RECORDS_PER_BLOCK; ++j)
                        {
                            //  printf("this is block id: %d, record id: %d, num: %d, str: %s\n",
                            //     buffer[nmem_blocks-1].blockid, buffer[nmem_blocks-1].entries[j].recid, buffer[nmem_blocks-1].entries[j].num, buffer[nmem_blocks-1].entries[j].str);
                        }
                        fwrite(&buffer[nmem_blocks-1], 1, sizeof(block_t), out);


                    }


                    //Efoson vrikame ton min arithmo tha prepei na feroume allo arithmo stin thesi tou gia tin taksinomisi.An omws den mporw na ferw allo arithmo epeidi exoume ftasei sto telos tou block tote :

                    if (index[thesi]==MAX_RECORDS_PER_BLOCK-1)  // an teleiwsei ena block apo tin ram tote tha ferw ena kainourgio apo to antistoixo arxeio pou pira kai to proigoumeno
                    {
                        int size=0;
                        size=sizeof(block_t);
                        if (size==fread(&buffer[thesi], 1, sizeof(block_t), fp[thesi]) ) //ean uparxei allo block sto arxeio tote to fernw
                        {

                            index[thesi]=0;
                            strcpy (  records[ thesi].str,buffer[thesi].entries[index[thesi]].str); //eisagw ton epomeno arithmo gia tin taksinomisi

                        }
                        else //ean dn uparxei allo block apo to antistoixo arxeio ekxwrw enan poli megalo arithmo stin antistoixi thesi tou records gia na eimai sigouros oti dn prokeitai na alloiwsei kanena apotelesma
                        {

                            strcpy(records[thesi].str,"zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz");
                            empty_blocks++;
                        }
                        strcpy(min_str,records[ thesi].str);

                    }

                    else //ean  den exei ftasei sto telos tou to block tote
                    {
                        index[thesi]++;
                        strcpy (records[thesi].str,buffer[thesi].entries[index[thesi]].str);    //eisagw ton epomeno arithmo gia tin taksinomisi
                        strcpy(min_str,records[thesi].str); //ton orizw ws ton mikrotero tuxaia
                    }



                }
                while(empty_blocks!=num_files_run_); //oso den exoun adeiasei ola ta block pou exw desmeusi stin ram
            }
            //Kleisimo twn arxeio pou itan gia sugxwneusi kai diagrafi tous giati pleon einai peritta.

            for (int i =sorted_runs*(nmem_blocks-1); i<end; i++)
            {

                fclose(fp[i]);

                remove(filename[i]);

            }


            fclose(out);

            newfiles ++; //to auksanoume afou dimiourgithike akoma ena kainourgio arxeio



            *nsorted_segs=*nsorted_segs + 1; //metrame ta sorted runs stin ousia einai idia metavliti me to sorted run

            snprintf(newfilename,sizeof(newfilename),"%d",sorted_runs);   //metonomazoume to kainourgio taksinomimeno arxeio tin antistoixi xroniki seira pou to dimiourgisame
            sorted_runs ++; // to auksanoume afou teleiwse ena akoma sorted run

            strcat(newfilename,".bin");
            rename("temp.bin",newfilename);

        }
        current_level_files =newfiles; // ta current arxeia pleon einai o arithmos twn newn taksinomimenwn arxeiwn pou dimiourgisame
        num_files=newfiles;

//pleon allazoume epipedo stin sugxwneusi afou exoume oloklirwsi me epitixia to trexon perasma
        *npasses=*npasses +1;

    }

    *npasses=*npasses +1; // +1 apo tin taksinomisi stin arxi
    *nios=2*(*npasses)*counter_all; //o gnwstos tupos gia na  vroume ta IOs .(thimizoume counter_all einai o arithmos twn stoixeiwn pou prepei na diavasoume apo to arxiko arxeio kai na grapsoume sto teliko arxeio antistoixa.


    cout<<"Number of sorted segments produced  :  "<<*nsorted_segs<<endl;
    cout<<"-------------------------------------------"<<endl;
    cout<<"Number of passes required for sorting :  "<<*npasses<<endl;
    cout<<"-------------------------------------------"<<endl;
    cout<<"Number of IOs performed : "<<*nios<<endl;
    cout<<"-------------------------------------------"<<endl;

    free(buffer);
}

/*-----         Stoxos tis sunartisis autis einai na ginei apaloifi twn diplotupwn tou telikou mas arxeiou pou dimourgithike apo to external sorting.
Arxika eisagoume stin Ram mas ena block apo to arxeio.Me ton algorithmo apaloifis diplotipwn diagrafoume ta diplotupa kai molis ftasoume sto telos tou block
grafoume ta apotelesmata mas se ena allo block stin Ram mas(diladi se auti tin sunartisi desmeuoume 2 blocks stin Ram) .
Tin parapanw diadikasia tin ekteloume mexri na adeiasei olo to arxeio-input.       ------ */


void EliminateDuplicates (char *infile, unsigned char field, block_t *buffer, unsigned int , char *outfile, unsigned int *nunique, unsigned int *nios)
{


    FILE *in,*out;
    in = fopen(infile, "r");
    out = fopen(outfile, "w");
    int i =0;
    int k=0;
    int size= 0;
    int counter=0; //metritis gia to posa stoixeia grafoume  sto output

    unsigned int un,ios=0;
    nunique= &un;
    nios=&ios;
    *nunique=0; //arithmos monadikwn num
    *nios=0; //metritis gia ta IOs
    int tmp=0;

    long int counter_all=0; //metritis gia ta blocks pou diavazoume apo to arxeio
    int prev_num=-1; //apothikeuoume kathe fora to teleutaio stoixeio tou kathe block pou eksetasame gia na to eleksoume stin sunexeia me to prwto stoixeio apo to amesws epomeno block
    char prev_str[STR_LENGTH]="";
    buffer = (block_t*) malloc (2*sizeof(block_t));


    size=sizeof(block_t);


    //Oso den exoume ftasei sto telos tou arxeio diavazoume block kai diagrafoume ta diplotupa


    while  (size==fread(&buffer[0], 1, sizeof(block_t), in) )
    {
        counter_all++;


        //mexri na ftasoume sto telos tou current  block
        for (i=1; i<MAX_RECORDS_PER_BLOCK; i++)
        {

            //an eimaste sto prwto stoixeio tou block tote to sugkrinoume me to prev_num diladi me to teleutaio stoixeio tou proigoumenou block
            if ( i==1)
            {
                if (field=='0')
                {
                    if (prev_num!=buffer[0].entries[0].recid)
                    {
                        memcpy(& buffer[1].entries[counter],&buffer[0].entries[0], sizeof(record_t));
                        counter++;
                    }

                }
                if (field=='1')
                {
                    if (prev_num!=buffer[0].entries[0].num)
                    {
                        memcpy(& buffer[1].entries[counter],&buffer[0].entries[0], sizeof(record_t));
                        counter++;
                    }

                }
                if (field=='2')
                {
                    if (strncmp(prev_str,buffer[0].entries[0].str,STR_LENGTH)!=0)
                    {
                        memcpy(& buffer[1].entries[counter],&buffer[0].entries[0], sizeof(record_t));
                        counter++;
                    }

                }

            }

            //FIELD 0 !!

            if (field=='0')
            {

                //an vriskomaste sto teleutaio stoixeio tou current  block tote to apothikeuoume stin prev_num

                if ( i==MAX_RECORDS_PER_BLOCK-1)
                {
                    prev_num=buffer[0].entries[i].recid;
                }


                //an to stoixeio pou eksetazoume einai diaforetiko apo to amesws epomeno arxeio tote to apothikeuoume sto output
                if ( buffer[0].entries[i].recid!=buffer[0].entries[i+1].recid && i!=1 && i!=MAX_RECORDS_PER_BLOCK-1 )
                {


                    memcpy(& buffer[1].entries[counter],&buffer[0].entries[i], sizeof(record_t));
                    counter++;
                }


            }


            //FIELD 1 !!!

            if (field=='1')
            {

                //an vriskomaste sto teleutaio stoixeio tou current  block tote to apothikeuoume stin prev_num

                if ( i==MAX_RECORDS_PER_BLOCK-1)
                {
                    prev_num=buffer[0].entries[i].num;
                }


                //an to stoixeio pou eksetazoume einai diaforetiko apo to amesws epomeno arxeio tote to apothikeuoume sto output
                if ( buffer[0].entries[i].num!=buffer[0].entries[i+1].num && i!=1 && i!=MAX_RECORDS_PER_BLOCK-1 )
                {


                    memcpy(& buffer[1].entries[counter],&buffer[0].entries[i], sizeof(record_t));
                    counter++;
                }


            }





            //FIELD 2 !!!

            if (field=='2')
            {

                //an vriskomaste sto teleutaio stoixeio tou current  block tote to apothikeuoume stin prev_num

                if ( i==MAX_RECORDS_PER_BLOCK-1)
                {
                    strcpy(prev_str,buffer[0].entries[i].str);
                }


                //an to stoixeio pou eksetazoume einai diaforetiko apo to amesws epomeno arxeio tote to apothikeuoume sto output
                if ( strncmp(buffer[0].entries[i].str,buffer[0].entries[i+1].str,STR_LENGTH)!=0 && i!=1 && i!=MAX_RECORDS_PER_BLOCK-1 )
                {


                    memcpy(& buffer[1].entries[counter],&buffer[0].entries[i], sizeof(record_t));
                    counter++;
                }


            }


            //an exei gemisei to output tote prepei na grapsoume to block tou output sto teliko arxeio
            if (counter==MAX_RECORDS_PER_BLOCK)
            {


                buffer[1].blockid = tmp++;
                buffer[1].nreserved = MAX_RECORDS_PER_BLOCK;
                buffer[1].valid = true;


                counter=0;
                // print ta stoixeia tou block
                for (int j=0; j<MAX_RECORDS_PER_BLOCK; ++j)
                {

                    *nunique=*nunique +1; //auksanoume ta monadika stoixeia
                    //   printf("this is block id: %d, record id: %d, num: %d, str: %s\n",

                    //  buffer[1].blockid, buffer[1].entries[j].recid, buffer[1].entries[j].num, buffer[1].entries[j].str);
                }
                fwrite(&buffer[1], 1, sizeof(block_t), out);
            }

        }
    }

    //uparxei i periptwsi opou exei teleiwsei to arxeio kai kapoia stoixeia exoun meinei sto output xwris omws na ta exoume grapsei sto teliko arxeio
    //epeidi grafame mono otan to output gemize  me apotelesma na exoun meinei kapoia.Auta ta  grafoume twra sto teliko arxeio.

    buffer[1].blockid =tmp++;
    buffer[1].nreserved = counter;
    buffer[1].valid = true;

    fwrite(&buffer[1], 1, sizeof(block_t), out);

    for (int j=0; j<counter; ++j)
    {

        *nunique=*nunique +1; //auksanoume ta monadika stoixeia
        //  printf("this is block id: %d, record id: %d, num: %d, str: %s, char: %c,next: %d, dummy: %d , nreserverd: %d \n",
        //       buffer[1].blockid, buffer[1].entries[j].recid, buffer[1].entries[j].num, buffer[1].entries[j].str,buffer[1].misc,buffer[1].next_blockid,buffer[1].dummy,buffer[1].nreserved);
    }
    counter_all--;

    cout<<"Number of unique records in file :  "<<*nunique<<endl;
    cout<<"-------------------------------------------"<<endl;
    *nios=(counter_all*MAX_RECORDS_PER_BLOCK) + (*nunique) ;
    cout<<"Number  of  IOs performed  :  "<<*nios<<endl;
    cout<<"-------------------------------------------"<<endl;
    free(buffer); //apodesmeusi tis mnimis

}




/* ----Stin sunartisi auti ekteloume tin praksi tis sundesis anamesa se 2 arxeia .Arxika desmeuoume 2 blocks stin Ram.To prwto gia na eisagoume kathe fora ena block apo to prwto arxeio ,
to deutero gia na eisagoume apo to deutero arxeio .Afou kanoume tis sugkriseis se kathe stoixeo anamesa sta 2 arxeia , an teleiwsei ena block apo ena arxeio tote
tha feroume to amesws epomeno apo to antistoixo arxeio .An teleiwsoun ola ta block apo to ena arxeio tote teleiwnei kai i sunartisi       -----   */

void MergeJoin (char *infile1, char *infile2, unsigned char field, block_t *buffer, unsigned int nmem_blocks, char *outfile, unsigned int *nres, unsigned int *nios)
{

    int size=0;
    long int counter_1=0;  //metritis gia ton arithmo twn stoixeiwn pou exoume diavasei apo to arxeio 1
    long int counter_2=0;  //metritis gia ton arithmo twn stoixeiwn pou exoume diavasei apo to arxeio 2


    FILE *in,*in2;
    in = fopen(infile1, "r");
    in2 = fopen(infile2, "r");

    buffer = (block_t*) malloc (2*sizeof(block_t)); // o buffer me ta 2 blocks pou desmeuoume stin mnimi mas
    size=sizeof(block_t);
    unsigned int res,ios=0;
    nres= &res;
    nios=&ios;
    *nres=0; //pairs
    *nios=0; //ios

    ofstream out(outfile, ofstream::binary);



    //----arxika diavazoume ena block apo kathe arxeio kai to apothikeuoume stin mnimi ----

    fread(&buffer[0], 1, sizeof(block_t), in);
    fread(&buffer[1], 1, sizeof(block_t), in2);
    long int counter=0; // metritis gia ta sunolika stoixeia pou diavazoume kai apo ta 2 arxeia
    counter=counter+ buffer[1].nreserved;

    counter=counter+ buffer[0].nreserved;

//ston vrogxo auto mexri na ftasoume sto telos enos apo ta 2 arxeia tha sugkrinoume ta num stoixeia metaksu tous kai ama auta einai isa tote einai pair kai tha apothikeuoume to apotelesma sto output
//an den einai isa tote tha anevazoume ton deikti tou block sto opoio itan mikroteros o arithmos se sxesi me ton allo
    do
    {


        //ean exoume ftasei sto telos tou block pou eksetazoume tou prwtou arxeiou tote fernoume ena allo apo auto to arxeio
        //ean omws exoume ftasei kai sto telos tou arxeiou kai dn mporoume na feroume allo block tote kanoume break kai vgainoume apo to vrogxo
        if (counter_1 ==buffer[0].nreserved )
        {

            if (!feof(in))
            {
                if ( size==fread(&buffer[0], 1, sizeof(block_t), in))
                {

                    counter=counter+ buffer[0].nreserved;
                    counter_1=0;
                }
                else
                {

                    break;
                }
            }



        }

        //ean exoume ftasei sto telos tou block pou eksetazoume tou deuterou arxeiou tote fernoume ena allo apo auto to arxeio
        //ean omws exoume ftasei kai sto telos tou arxeiou kai dn mporoume na feroume allo block tote kanoume break kai vgainoume apo to vrogxo
        if (counter_2 ==buffer[1].nreserved )
        {

            if (!feof(in2))
            {
                if (size== fread(&buffer[1], 1, sizeof(block_t), in2))
                {

                    counter=counter+ buffer[1].nreserved;
                    counter_2=0;
                }
                else
                {
                    break;
                }
            }



        }

        //FIELD == 0 !!
        if (field=='0')
        {
            //ean ta stoixeia pou elegxoume einai idia tote grafoume sto output to pair kai auksanoume kai tous 2 deiktes gia na esksetasoume ta epomena stoixeia tous
            if (buffer[0].entries[counter_1].recid==buffer[1].entries[counter_2].recid )
            {


                out <<"ID_1 :  "<<buffer[0].blockid<<"str _1 :  "<<buffer[0].entries[counter_1].str<<"  ,   ""ID_2  :  "<<buffer[1].blockid<<"str  :  "<<buffer[1].entries[counter_2].str<<endl ;

                counter_1++;
                counter_2++;

                *nres =*nres + 1 ;



            }
            //ean to stoixeio apo to prwto block einai mikrotero tou deuterou tote auksanoume to deikti tou kai sunexizoume tin eksetasi me to epomeno stoixeio tou
            else if  (buffer[0].entries[counter_1].recid<buffer[1].entries[counter_2].recid)
            {

                counter_1++;
            }
            //ean to stoixeio apo to prwto block einai megalutero tou deuterou tote auksanoume to deikti tou kai sunexizoume tin eksetasi me to epomeno stoixeio tou
            else if  (buffer[0].entries[counter_1].recid>buffer[1].entries[counter_2].recid)
            {

                counter_2++;
            }

        }
        // FIELD == 1  !
        if  ( field =='1')
        {

            //ean ta stoixeia pou elegxoume einai idia tote grafoume sto output to pair kai auksanoume kai tous 2 deiktes gia na esksetasoume ta epomena stoixeia tous
            if (buffer[0].entries[counter_1].num==buffer[1].entries[counter_2].num )
            {


                out <<"ID _1 :  "<<buffer[0].blockid<<"str  :  "<<buffer[0].entries[counter_1].str<<"   ,  ""ID_2  :  "<<buffer[1].blockid<<"str _2  :  "<<buffer[1].entries[counter_2].str<<endl ;

                counter_1++;
                counter_2++;

                *nres =*nres + 1 ;



            }
            //ean to stoixeio apo to prwto block einai mikrotero tou deuterou tote auksanoume to deikti tou kai sunexizoume tin eksetasi me to epomeno stoixeio tou
            else if  (buffer[0].entries[counter_1].num<buffer[1].entries[counter_2].num)
            {

                counter_1++;
            }
            //ean to stoixeio apo to prwto block einai megalutero tou deuterou tote auksanoume to deikti tou kai sunexizoume tin eksetasi me to epomeno stoixeio tou
            else if  (buffer[0].entries[counter_1].num>buffer[1].entries[counter_2].num)
            {

                counter_2++;
            }
        }

        //       FIELD == 2 !!!
        if ( field=='2')
        {

            //ean ta stoixeia pou elegxoume einai idia tote grafoume sto output to pair kai auksanoume kai tous 2 deiktes gia na esksetasoume ta epomena stoixeia tous
            if (strncmp(buffer[0].entries[counter_1].str,buffer[1].entries[counter_2].str,STR_LENGTH)==0 )
            {


                    out <<"ID_1 :  "<<buffer[0].blockid<<"num_1  :  "<<buffer[0].entries[counter_1].num<<"   ,   ""ID_2  :  "<<buffer[1].blockid<<"num_2  :  "<<buffer[1].entries[counter_2].num<<endl ;

                counter_1++;
                counter_2++;

                *nres =*nres + 1 ;



            }
            //ean to stoixeio apo to prwto block einai mikrotero tou deuterou tote auksanoume to deikti tou kai sunexizoume tin eksetasi me to epomeno stoixeio tou
            else if  (strncmp(buffer[0].entries[counter_1].str,buffer[1].entries[counter_2].str,STR_LENGTH)<0)
        {

            counter_1++;
        }
        //ean to stoixeio apo to prwto block einai megalutero tou deuterou tote auksanoume to deikti tou kai sunexizoume tin eksetasi me to epomeno stoixeio tou
        else if  (strncmp(buffer[0].entries[counter_1].str,buffer[1].entries[counter_2].str,STR_LENGTH)>0)
        {

            counter_2++;
        }


    }


















}
while ( true);

    *nios=(counter + 2*( *nres)  ) ; //Ta IOs tha einai to athroisma olwn twn stoixeiwn pou diavasame  + twn arithmwn twn stoixeiwn pou grapsame sto output diladi twn arithmo twn pairs

    cout<<"Number of pairs in output  :  "<<*nres<<endl;
    cout<<"-------------------------------------------"<<endl;
    cout<<"Number of IOs performed :  "<<*nios<<endl;

    out.close();
    free(buffer); //apodesmeusi tis mnimis

}

