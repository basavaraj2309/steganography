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
#include "encode.h"
#include "common.h"
#include "types.h"
#include<stdlib.h>
#include <unistd.h>
/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */

uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    //   printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    // printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

uint get_file_size(FILE *fptr,EncodeInfo *encInfo)
{
    fseek(fptr,0,SEEK_END); // set the curser position to 0.
    encInfo->size_secret_file=ftell(fptr); // here we using the ftell function to get curcer position.
    return ftell(fptr);
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status read_and_validate_encode_args(char *argv[],EncodeInfo *encinfo)
{
    if(strstr(argv[2],".bmp")!=NULL) // checking our 2 argument is have a .bmp or not.
    {
	encinfo->src_image_fname=argv[2]; //store it in src_img_fname.
    }
    if((strstr(argv[3],".txt")!=NULL) || (strstr(argv[3],".c")!=NULL) || (strstr(argv[3],".sh")!=NULL)) // here checking the 3 argumnet is have .txt or .c or .sh.
    {
	encinfo->secret_fname=argv[3];
    }
    if(argv[4]==NULL) // it is for optional.
    {
	encinfo->stego_image_fname="stego.bmp";
	return e_success;
    }
    else
    {
	encinfo->stego_image_fname=argv[4];
	return e_success;
    }
    printf("\033[0;31mERROR : Please pass the correct arguments.\nUSAGE :\n./a.out -e source_image_file secret_file output_file(optional)\n");
    return e_failure;
}

Status check_capacity(EncodeInfo* encInfo)
{
    // strcpyencIfo->extn_secret_file,strstr(argv[3],",").
    int val=(54+strlen(MAGIC_STRING)+4+strlen(strstr(encInfo->secret_fname,".")+4+get_file_size(encInfo->fptr_secret,encInfo))*8);// store the extn array here ,that is in 
																 // u get 36                       
    if(val<get_image_size_for_bmp(encInfo->fptr_src_image)) // we checking the value
	return e_success;
}

Status copy_bmp_header(FILE *fptr_src_image,FILE *fptr_dest_image)
{
    char buffer[54]; // taking one char arr
    rewind(fptr_src_image); //set the curcer position to the src
    fread(buffer,54,1,fptr_src_image); // here fatching the 54bytes 1time from src and put it in buffer.
    rewind(fptr_dest_image);
    fwrite(buffer,54,1,fptr_dest_image); // here fetching the data from  buffer to dest. 
    return e_success;
}

Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
    encode_data_to_image(magic_string,strlen(magic_string),encInfo->fptr_src_image,encInfo->fptr_stego_image); // calling the function passing the argument.
    return e_success;
}

Status encode_secret_file_exten_size(int size,EncodeInfo *encInfo)
{
    encode_size_to_image(size,encInfo); // calling the size_to_imgae
    return e_success;
}

Status encode_secret_file_exten(char *file_extn, EncodeInfo *encInfo)

{
    encode_data_to_image(file_extn,strlen(file_extn),encInfo->fptr_src_image,encInfo->fptr_stego_image);
    return e_success; // it will return the success.
}

Status encode_secret_file_size(long file_size,EncodeInfo *encInfo)
{
    encode_size_to_image(file_size,encInfo); // calling the size_to_image.
    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    rewind(encInfo->fptr_secret); // set the cur.
    char str[100],ch=fgetc(encInfo->fptr_secret); //get the char from secrest.
    int i=0;
    while(ch > 0) // run the loop based on ch.
    {
	str[i++]=ch;  // store that char into str[i] index position.
	ch=fgetc(encInfo->fptr_secret);
    }
    str[i]='\0';
    encode_data_to_image(str,encInfo->size_secret_file,encInfo->fptr_src_image,encInfo->fptr_stego_image);

    return e_success; // it return e_success.
}

Status encode_data_to_image(char *data,int size,FILE *fptr_src_image,FILE *fptr_stego_image)
{
    char image_buffer[8]; 
    for(int i=0;i<size;i++ ) // it rune the loop based on size times.
    {
	fread(image_buffer,8,1,fptr_src_image);  
	encode_byte_to_lsb(data[i],image_buffer); 
	fwrite(image_buffer,8,1,fptr_stego_image); // # will enoced successfully stored in output file.
    }

}

Status encode_size_to_image(int data, EncodeInfo *encInfo)
{
    char image_buffer[32]; // taking the one buffer.

    fread(image_buffer,1,32,encInfo->fptr_src_image); // reaf the 32bytes from the src and put it in buffer.
    encode_size_to_lsb(data,image_buffer); // calling the size_to_lab.
    fwrite(image_buffer,1,32,encInfo->fptr_stego_image);

}

Status encode_size_to_lsb(int size, char *image_buffer)
{
    for(int i=0;i<32;i++)
    {
	image_buffer[i]=image_buffer[i] & ~ 1 | (unsigned)((size & (1<<(31-i)))>>(31-i)); // it stroe the 32 bit by bit.
    }

}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i=0;i<8;i++)
    {
	image_buffer[i]=image_buffer[i] & ~ 1 |(unsigned) (data & (1<<7-i))>>7-i; //image size is updated value in the LSB. 

    }
}

Status copy_remaining_img_data(FILE *fptr_src,FILE *fptr_dest)
{
    char ch;
    while(fread(&ch,1,1,fptr_src) == 1) // it run untill the no char found.
    {
	fwrite(&ch,1,1, fptr_dest); // write in file that one by one char.
    }
    return e_success;
    // strcpy(fptr_dest,fptr_src);

}

Status do_encoding(EncodeInfo *encInfo)
{
    printf("\033[1;31mENCODING PROCESS STARTED\n");
    if(open_files(encInfo)==e_success)
    {
	sleep(1);
	printf("\033[0;33mSTEP 1 :  OPEN FILES GETS SUCCESS\n");
	if(check_capacity(encInfo) == e_success)
	{
	    sleep(1);
	    printf("\033[0;33mSTEP 2 : CHECK_CAPACITY_ GETS SUCCESS\n");
	    if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
	    {
		sleep(1);
		printf("\033[0;33mSTEP 3 : COPY_BMP_HEADER GETS SUCCESS\n");

		if(encode_magic_string(MAGIC_STRING,encInfo)==e_success)
		{
		    sleep(1);
		    printf("\033[0;33mSTEP 4 : ENCODE_MAGIC_STRING SUCCESS\n");

		    if(encode_secret_file_exten_size(strlen(strstr(encInfo->secret_fname,".")),encInfo)==e_success)
		    {
			sleep(1);
			printf("\033[0;33mSTEP 5 : SECRET FILE EXTENTION SIZE ENCODED SUCCESFULLY\n");

			if(encode_secret_file_exten(strchr(encInfo->secret_fname,'.'),encInfo) == e_success )
			{
			    sleep(1);
			    printf("\033[0;33mSTEP 6 : SECRET FILE EXTENTION SUCCESFULLY\n");

			    if(encode_secret_file_size(encInfo->size_secret_file,encInfo)==e_success)
			    {
				sleep(1);
				printf("\033[0;33mSTEP 7 : SECRET FILE SIZE ENCODEED SUCCESFULLY\n");

				if(encode_secret_file_data(encInfo)==e_success)
				{
				    sleep(1);
				    printf("\033[0;33mSTEP 8 : SECRET FILE DATA ENCODED SUCCESFULLY\n");
				    if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
				    {
					sleep(1);
					printf("\033[0;33mSTEP 9 : COPY IMG DATA SUCCESFULLY\n");
					return e_success;
				    }
				    else
				    {
					printf("\033[0;31mSTEP 8 : ERROR : Remaining data encoding failed\n");
				    }
				}
				else
				{
				    printf("\033[0;31mSTEP 7 : ERROR : secret file data encoding failed\n");
				}
			    }
			    else
			    {
				printf("\033[0;31mSTEP 6 : ERROR : secret file data size encoding failed\n");

			    }

			}
			else
			{
			    printf("\033[0;31mSTEP 5 : ERROR : sectet file extension encoding failed\n");


			}
		    }
		    else
		    {
			printf("\033[0;31mSTEP 4 : ERROR : secret file extension size encoding failed\n");
		    }
		}
		else
		{
		    printf("\033[0;31mSTEP 3 : ERROR : magic string encoding failed\n");
		}
	    }
	    else
	    {
		printf("\033[0;31mSTEP 2 : ERROR : coping header failed\n");
	    }
	}
	else
	{
	    printf("\033[0;31mSTEP 1 : ERROR : capacity is failed\n");
	}
    }
    else
    {
	printf("\033[0;31mERROR : open file failed\n");
    }
    return e_failure;
}

