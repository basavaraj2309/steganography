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
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc,char *argv[]) 
{
    int OT=check_operation_type(argv);
    if(OT== e_encode)
    { // checking and calling the function.
	if(argc >= 4 && argc <=5)  //checking the condition for argument count >4 and <5.
	{
	    EncodeInfo encInfo;
	    if(read_and_validate_encode_args(argv,&encInfo) == e_success)  // checking and calling the function.  
	    {
		if(do_encoding(&encInfo)==e_success)
		{
		    printf("\033[1;36mENCODING SUCCESS\n");  // printing the msg based on condition return success and failur.
		}
		else
		{
		    printf("\033[0;31mERROR : Encoding Failed\n");
		}
	    }
	    else
	    {
		printf("\033[0;31mERROR : read and validate has failed\n");
	    }
	}
	else
	{
	    printf("\033[0;31mERROR : Please pass the correct arguments.\nUSAGE :\n./a.out -e source_image_file secret_file output_file(optional)\n");
	}
    }
    else if(OT == e_decode)
    {	// ckecking the condition.
	if(argc>=3 && argc<=4) // chekingh the argv count more then >=3
	{
	    DecodeInfo decInfo;
	    if(read_and_validate_decode_args(argv,&decInfo) == e_success)
	    {
		if(do_decoding(&decInfo)==e_success)
		{
		    printf("\033[0;36mDECODING SUCCESS\n");
		}
		else
		{
		    printf("\033[0;31mDecoding Failed\n");
		}
	    }	
	    else
	    {
		printf("\033[0;31mUnsupported operation\n"); // printing the msg besed on return value.
	    }

	}
	else
	{
	    printf("\033[0;31mERROR : Please pass the correct arguments.\nUSAGE :\n ./a.out -d source_image_file secret_file output_file(optional)\n");
	}
    }
    else
    {
	printf("\033[0;31mERROR : Please pass the correct arguments.\nUSAGE :\n./a.out -d source_image_file secret_file output_file(optional)\n");

    }
}


OperationType check_operation_type(char *argv[])  // we writeing this function here bec we use this one for both encoding and decoding.
{
    if(argv[1]!=NULL)
    {
	if(strcmp(argv[1],"-e")==0) // if we pass -e it do encoding .
	{
	    return e_encode;
	}
	else if(strcmp(argv[1],"-d")==0) // it do the decodeing.
	{
	    return e_decode;
	}
	else
	{
	    return e_unsupported; 
	}
    }
    else
    {
	return e_unsupported; 
    }
}

