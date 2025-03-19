#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>

#include "address_book.h"

Status load_file(AddressBook *address_book)
{
	int ret; 

	/* 
	 * Check for file existance
	 */
	FILE *temp_file =fopen(DEFAULT_FILE,"r");
	int ret=-1;
	if(temp_file != NULL)
	{
		fclose(temp_file);
		ret = 0;
	}

	if (ret == 0)
	{
		/* 
		 * Do the neccessary step to open the file
		 * Do error handling
		 */ 
		address_book->fp=fopen(DEFAULT_FILE,"r+");
		if(address_book->fp==NULL)
		{
			return e_fail;
		}

		//read number of contacts
		fread(&address_book->count, sizeof(int),1,address_book->fp);

		//allocate memory for contacts if count>0
		if(address_book->count > 0)
		{
			address_book->list= (ContactInfo *)malloc(sizeof(ContactInfo) * address_book->count);
			if(address_book->list == NULL)
			{
				fclose(address_book->fp);
				return e_fail;
			}
			//read contacts from file
			fread(address_book->list, sizeof(ContactInfo), address_book->count,address_book->fp);
		}

	}
	else
	{
		/* Create a file for adding entries */
		address_book->fp = fopen(DEFAULT_FILE, "w+");
		if(address_book->fp == NULL)
		{
			return e_fail;
		}
		
		//init new address book
		address_book->count=0;

		//write init count to file
		fwrite(&address_book->count, sizeof(int),1,address_book->fp);
	}

	return e_success;
}

Status save_file(AddressBook *address_book)
{
	/*
	 * Write contacts back to file.
	 * Re write the complete file currently
	 */ 
	address_book->fp = fopen(DEFAULT_FILE, "w");

	if (address_book->fp == NULL)
	{
		return e_fail;
	}

	/* 
	 * Add the logic to save the file
	 * Make sure to do error handling
	 */ 
	//write the number of contacts first
    	if (fwrite(&address_book->count, sizeof(int), 1, address_book->fp) != 1)
    	{
        	fclose(address_book->fp);
        	return e_fail;
    	}

    	//write all contacts if there are any
    	if (address_book->count > 0 && address_book->list != NULL)
    	{
        	//write the entire contact array at once
        	if (fwrite(address_book->list, sizeof(ContactInfo), address_book->count, address_book->fp) != address_book->count)
        	{
            		fclose(address_book->fp);
            		return e_fail;
        	}
    	}
    
	fclose(address_book->fp);
	return e_success;
}
