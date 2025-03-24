#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>
#include "address_book.h"
#include "address_book_fops.h"

Status load_file(AddressBook *address_book)
{
TylerB_branch
	int ret;
	char line[256];
	char *token;
	
	/* Check for file existence */
	FILE *temp_file = fopen(DEFAULT_FILE, "r");
	ret = -1;
	if (temp_file != NULL)
    main
	{
		fclose(temp_file);
		ret = 0;
	}
	
	if (ret == 0)
	{
		/* Open the file */
		address_book->fp = fopen(DEFAULT_FILE, "r");
		if (address_book->fp == NULL)
		{
			return e_fail;
		}
		
		/* First count the number of lines in CSV to allocate memory */
		int count = 0;
		while (fgets(line, sizeof(line), address_book->fp) != NULL)
		{
			count++;
		}
		
		/* Reset file pointer to beginning */
		rewind(address_book->fp);
		
		/* Allocate memory for contacts */
		address_book->count = count;
		address_book->list = (ContactInfo *)malloc(sizeof(ContactInfo) * address_book->count);
		if (address_book->list == NULL)
		{
			fclose(address_book->fp);
			return e_fail;
		}
		
		/* Initialize all memory to zero */
		memset(address_book->list, 0, sizeof(ContactInfo) * address_book->count);
		
		/* Read CSV data line by line */
		int i = 0;
		while (fgets(line, sizeof(line), address_book->fp) != NULL && i < count)
		{
			/* Remove trailing newline if present */
			size_t len = strlen(line);
			if (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
				line[len-1] = '\0';
			
			/* Parse CSV line */
			token = strtok(line, ",");
			if (token != NULL)
			{
				/* Set serial number */
				address_book->list[i].si_no = atoi(token);
				
				/* Set name */
				token = strtok(NULL, ",");
				if (token != NULL)
					strncpy(address_book->list[i].name[0], token, NAME_LEN - 1);
				
				/* Set phone number */
				token = strtok(NULL, ",");
				if (token != NULL)
					strncpy(address_book->list[i].phone_numbers[0], token, NUMBER_LEN - 1);
				
				/* Set email */
				token = strtok(NULL, ",");
				if (token != NULL)
					strncpy(address_book->list[i].email_addresses[0], token, EMAIL_ID_LEN - 1);
			}
			i++;
		}
	}
	else
	{
		/* Create a new file if it doesn't exist */
		address_book->fp = fopen(DEFAULT_FILE, "w+");
		if (address_book->fp == NULL)
		{
			return e_fail;
		}
		
		/* Initialize new address book */
		address_book->count = 0;
		address_book->list = NULL;
	}
	
	/* Close the file as we've loaded everything into memory */
	fclose(address_book->fp);
	address_book->fp = NULL;
	
	return e_success;
}

Status save_file(AddressBook *address_book)
{
	/* Open file for writing */
	address_book->fp = fopen(DEFAULT_FILE, "w");
	if (address_book->fp == NULL)
	{
		return e_fail;
	}
	
	/* Write all contacts as CSV */
	for (int i = 0; i < address_book->count; i++)
	{
		fprintf(address_book->fp, "%d,%s,%s,%s\n", 
				address_book->list[i].si_no,
				address_book->list[i].name[0],
				address_book->list[i].phone_numbers[0],
				address_book->list[i].email_addresses[0]);
	}
	
	fclose(address_book->fp);
	address_book->fp = NULL;
	
	return e_success;
}
