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

	int ret;
	char line[256];
	char *token;
	char *phone_num_token;

	/* Check for file existence */
	FILE *temp_file = fopen(DEFAULT_FILE, "r");
	ret = -1;
	if (temp_file != NULL)
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

		printf("%-d", count);
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

		fgets(line, sizeof(line), address_book->fp);
		printf("%s\n", line);

				int i = 1;
		while (fgets(line, sizeof(line), address_book->fp) != NULL && i < count)
		{
			/* Remove trailing newline if present */
			size_t len = strlen(line);
			if (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
			{
				line[len - 1] = '\0';
			}

			/* Parse CSV line */
			// Inside your CSV reading function
			/* Set serial number */
			token = strtok(line, ",");
			/* Set serial number */
			if (token != NULL)
			{
				address_book->list[i].si_no = atoi(token);
				printf("%-5d", address_book->list[i].si_no);
				printf("\n");

				/* Set name */
				token = strtok(NULL, ",");
				strncpy((char *)address_book->list[i].name, token, NAME_LEN - 1);
				printf("%-5s", (char *)address_book->list[i].name);

				printf("\n");

				/* Set phone number */
				token = strtok(NULL, ",");
				// strncpy((char *)address_book->list[i].phone_numbers, token, NUMBER_LEN - 1);
				if (token[0] == '0')
				{
					for (int j = 0; token != NULL && j < PHONE_NUMBER_COUNT; j++)
					{
						// phone_num_token = strtok(NULL, ",");
						if (j == 0)
						{
							strncpy((char *)address_book->list[i].phone_numbers[j], token, NUMBER_LEN - 1);
							printf("%-5s", (char *)address_book->list[i].phone_numbers[j]);
							printf("\n");
						}
						else if ((token[0] == '0' && token != NULL))
						{

							strncpy((char *)address_book->list[i].phone_numbers[j], token, NUMBER_LEN - 1);
							address_book->list[i].phone_numbers[j][NUMBER_LEN - 1] = '\0'; // Ensure null termination

							printf("%-5s", (char *)address_book->list[i].phone_numbers[j]);
							printf("\n");
						}
						else
						{
							break;
						}
						token = strtok(NULL, ",");
					}
				}
				else
				{
					token = strtok(NULL, ",");
				}

				/* Set email */
				for (int k = 0; token != NULL && k < EMAIL_ID_COUNT; k++)
				{
					if (k == 0)
					{
						strncpy((char *)address_book->list[i].email_addresses[k], token, EMAIL_ID_LEN - 1);
						printf("%-5s", (char *)address_book->list[i].email_addresses[k]);
						printf("\n");
					}
					else if ((token != NULL))
					{
						strncpy((char *)address_book->list[i].email_addresses[k], token, EMAIL_ID_LEN - 1);
						address_book->list[i].email_addresses[k][EMAIL_ID_LEN - 1] = '\0'; // Ensure null termination

						printf("%-5s", (char *)address_book->list[i].email_addresses[k]);
						printf("\n");
					}
					else
					{
						break;
					}
					token = strtok(NULL, ",");
				}
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
