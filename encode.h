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
#ifndef ENCODE_H
#define ENCODE_H
#include<string.h>
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

typedef struct _EncodeInfo
{
    /*Beautiful image */
    /* Source Image info */
    char *src_image_fname; /*STORE THE SOURCE FILE NAME */
    FILE *fptr_src_image; /*STORE THE SOURCE FILE POINTER */
    uint image_capacity; /* TO STORE THE SIZE OF THE IMAGE */
    uint bits_per_pixel;
    char image_data[MAX_IMAGE_BUF_SIZE]; /* To Store the Image data */

    /*SECRET FILE */
    /* Secret File Info */
    char *secret_fname; /* TO STORE THE SECRET FILE NAME */
    FILE *fptr_secret; /*TO STORE THE SECRET FILE POINTER */
    char extn_secret_file[MAX_FILE_SUFFIX];  /* TO STORE THE EXTENSION OF SECRET FILE */
    char secret_data[MAX_SECRET_BUF_SIZE]; /* TO STORE THE SECRET DATA */ 
    long int size_secret_file; /* TO STORE THE SIZE OF THE SECRET FILE */ 

    /* OUTPUT IMAGE FILE */
    /* Stego Image Info */
    char *stego_image_fname; /* TO STORE THE OUTPUT IMAGE NAME */
    FILE *fptr_stego_image; /* TO STORE THE OUTPUT FILE POINTER */

} EncodeInfo;


/* Encoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo);

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo);

/* Get File pointers for i/p and o/p files */
Status open_files(EncodeInfo *encInfo);

/* check capacity */
Status check_capacity(EncodeInfo *encInfo);

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image);

/* Get file size */
uint get_file_size(FILE *fptr,EncodeInfo *);

/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);

/* Store Magic String */
Status encode_magic_string(char *magic_string, EncodeInfo *encInfo);

/*Encode secret file extention size*/
Status encode_secret_file_exten_size(int size,EncodeInfo *encInfo);

/* Encode secret file extenstion */
Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo);


/* Encode secret file size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo);

/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo);

/* Encode function, which does the real encoding */
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image);

/*find the size ofthe img */
Status encode_size_to_image(int data, EncodeInfo *encInfo);
/* Encode a byte into LSB of image data array */
Status encode_byte_to_lsb(char data, char *image_buffer);

/* Encode a size into LSB of image data array */
Status encode_size_to_lsb(int size, char *image_buffer);

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);

#endif
