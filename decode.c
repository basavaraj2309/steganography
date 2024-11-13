/*
NAME: BASAVARAJA BN
DATA: 8/10/2024
DESCIPTION: LSB Image Steganography 
-------------------------------------------
SAMPLE IP FOR ENCODE:
./a.out -e beautiful.bmp secret.txt
-----------------------------------------------
SAMPLE OP FOR ENCODE:
EODING PROCESS STARTED
STEP 1 :  OPEN FILES GETS SUCCESS
STEP 2 : CHECK_CAPACITY_ GETS SUCCESS
STEP 3 : COPY_BMP_HEADER GETS SUCCESS
STEP 4 : ENCODE_MAGIC_STRING SUCCESS
STEP 5 : SECRET FILE EXTENTION SIZE ENCODED SUCCESFULLY
STEP 6 : SECRET FILE EXTENTION SUCCESFULLY
STEP 7 : SECRET FILE SIZE ENCODEED SUCCESFULLY
STEP 8 : SECRET FILE DATA ENCODED SUCCESFULLY
STEP 9 : COPY IMG DATA SUCCESFULLY
ENCODING SUCCESS
--------------------------------------------
SAMPLE IP FOR DECODE:
./a.out -d stego.bmp
-------------------------------------------
SAMPLE OP FOR DECODE:
DECODING PROCESS STARTED
STEP 1 : OPEN FILES GETS SUCCESS
STEP 2 : DECODE_MAGIC_STRING SUCCESS
STEP 3 : SECRET FILE EXTENTION SIZE DECODED SUCCESFULLY
STEP 4 : SECRET FILE EXTENTION SUCCESFULLY
STEP 5 : SECRET FILE SIZE DECODEED SUCCESFULLY
STEP 6 : SECRET FILE DATA DECODED SUCCESFULLY
DECODING SUCCESS
 */
#include <stdio.h>
#include<string.h>
#include "decode.h"
#include "common.h"
#include "types.h"
#include<stdlib.h>
#include <unistd.h>



Status open_file(DecodeInfo *decInfo)
{
    // Open stego image file
    decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "r"); // open the file in read mode.
    if (decInfo->fptr_src_image == NULL) //checking that src have any data ot =r not.
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->src_image_fname); 
	return e_failure;
    }
    fseek(decInfo->fptr_src_image,54,SEEK_SET); // set the cur into 55 position.
    return e_success;
}
char decode_image_to_data(FILE * fptr_src_image)
{
    char image_buffer[8],sum=0;
    fread(image_buffer,8,1,fptr_src_image); // it read the 8 bits from the src and put it 1 time to buffer.
    for(int i=0;i<=7;i++)
    {
	sum=sum+((image_buffer[i]&1)<<7-i); // we get one by one 8 byte.
    }
    return sum;
}

int decode_image_to_size(FILE * fptr_src_image)
{
    char image_buffer[32],sum=0;
    fread(image_buffer,32,1,fptr_src_image);
    for(int i=0;i<=31;i++)
    {
	sum=sum+((image_buffer[i]&1)<<31-i);
    }

    return sum;
}


Status read_and_validate_decode_args(char *argv[],DecodeInfo *decinfo)
{
    if(strstr(argv[2],".bmp")!=NULL) // validate the argv have .bmp or not.
    {
	decinfo->src_image_fname=argv[2]; // store it in src.
	return e_success;
    }
    return e_failure;
}

Status decode_magic_string(DecodeInfo *decInfo)
{
    char magic_string[3];
    for(int i=0;i<2;i++)
    {
	magic_string[i]=decode_image_to_data(decInfo->fptr_src_image); // calling the image to data function.
    }
    if(!strcmp(magic_string,MAGIC_STRING)) // compare the we encoded magic string and original MAGIC STRIG is same or not.
    {
	return e_success; // return the value based on conditon .
    }
    else
    {
	return e_failure;
    }
}
Status decode_secret_file_exten_size(DecodeInfo *decInfo)
{
    decInfo->extn_size = decode_image_to_size(decInfo->fptr_src_image); // calling the function and store it in extn size.

    return e_success;
}

Status decode_secret_file_exten(DecodeInfo *decInfo)
{
    int i;
    for( i=0;i<decInfo->extn_size;i++) // run the for loop
    {
	decInfo->extn_secret_file[i]=decode_image_to_data(decInfo->fptr_src_image); //calliung the function and stored it in extn_secret_file.
    }
    decInfo->extn_secret_file[i]='\0';

    char fname[100]="output";
    strcat(fname,decInfo->extn_secret_file); // cancatination the name we stored in fname with decInfo.
    decInfo->output_fname=fname;
    decInfo->fptr_output=fopen(decInfo->output_fname,"w"); // open the file in write mode.
    return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    decInfo->size_secret_file=decode_image_to_size(decInfo->fptr_src_image); // calling the function 
    return e_success; // it return the e_success.
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char ch;
    for(int i=0;i<decInfo->size_secret_file;i++)
    {
	ch=decode_image_to_data(decInfo->fptr_src_image);
	putc(ch,decInfo->fptr_output); // we geting one by one char,
    }
    return e_success;
}
// here we checking condition and calling the fuction based on them only we printing the msg,.
Status do_decoding(DecodeInfo *decInfo)
{
    printf("\033[1;31mDECODING PROCESS STARTED\n");
    if(open_file(decInfo)==e_success)
    {
	sleep(1);
	printf("\033[0;33mSTEP 1 : OPEN FILES GETS SUCCESS\n");
	if(decode_magic_string(decInfo)==e_success)
	{
	    sleep(1);
	    printf("\033[0;33mSTEP 2 : DECODE_MAGIC_STRING SUCCESS\n");

	    if(decode_secret_file_exten_size(decInfo)==e_success)
	    {
		sleep(1);
		printf("\033[0;33mSTEP 3 : SECRET FILE EXTENTION SIZE DECODED SUCCESFULLY\n");

		if(decode_secret_file_exten(decInfo) == e_success )
		{
		    sleep(1);
		    printf("\033[0;33mSTEP 4 : SECRET FILE EXTENTION SUCCESFULLY\n");

		    if(decode_secret_file_size(decInfo)==e_success)
		    {
			sleep(1);
			printf("\033[0;33mSTEP 5 : SECRET FILE SIZE DECODEED SUCCESFULLY\n");

			if(decode_secret_file_data(decInfo)==e_success)
			{
			    sleep(1);
			    printf("\033[0;33mSTEP 6 : SECRET FILE DATA DECODED SUCCESFULLY\n");
			    return e_success;
			}
			else
			{
			    printf("\033[0;31mSTEP 6 : ERROR : secret file data decoding failed\n");
			}
		    }
		    else
		    {
			printf("\033[0;31mSTEP 5 : ERROR : secret file data size decoding failed\n");

		    }

		}
		else
		{
		    printf("\033[0;31mSTEP 4 : ERROR : sectet file extension decoding failed\n");

		}
	    }
	    else
	    {
		printf("\033[0;31mSTEP 3 : ERROR : secret file extension size decoding failed\n");
	    }
	}
	else
	{
	    printf("\033[0;31mSTEP 2 : ERROR : magic string decoding failed\n");
	}
    }
    else
    {
	printf("\033[0;31mSTEP 1 : ERROR : coping header failed\n");
    }
}
