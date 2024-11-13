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
#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /* Stego Image info */
    char *src_image_fname; /* To store the input stego image file name */
    FILE *fptr_src_image; /* Stego image file pointer */

    /* Decoded output file info */
    char *output_fname; /* To store the output decoded file name */
    FILE *fptr_output; /* Output file pointer */

    /* Secret File Info */
    char extn_secret_file[MAX_FILE_SUFFIX]; /* To store the secret file extension */
    int extn_size; /* Size of the secret file extension */
    long size_secret_file; /* Size of the secret file */
} DecodeInfo;


char fun(FILE * fptr_src_image);

/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the encoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_file(DecodeInfo *decInfo);

/* Store Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

/*Encode secret file extention size*/
Status decode_secret_file_exten_size(DecodeInfo *decInfo);

/* Encode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decInfo);


/* Encode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Encode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Encode function, which does the real encoding */
Status decode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image);

/*find the size ofthe img */
Status decode_size_to_image(int data, DecodeInfo *decInfo);
/* Encode a byte into LSB of image data array */
Status decode_byte_to_lsb(char data, char *image_buffer);

/* Encode a size into LSB of image data array */
Status decode_size_to_lsb(int size, char *image_buffer);


#endif
