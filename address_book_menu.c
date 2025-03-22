#include <stdio.h>
// #include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "address_book.h"
#include "address_book_menu.h"

// #include "abk_fileops.h"
// #include "abk_log.h"
// #include "abk_menus.h"
// #include "abk.h"

int get_option(int type, const char *msg)
{
	/*
	 * Mutilfuction user intractions like
	 * Just an enter key detection
	 * Read an number
	 * Read a charcter
	 */

	/* Fill the code to add above functionality */
}

Status save_prompt(AddressBook *address_book)
{
	char option;

	do
	{
		main_menu();

		option = get_option(CHAR, "\rEnter 'N' to Ignore and 'Y' to Save: ");

		if (option == 'Y')
		{
			save_file(address_book);
			printf("Exiting. Data saved in %s\n", DEFAULT_FILE);

			break;
		}
	} while (option != 'N');

	free(address_book->list);

	return e_success;
}

Status list_contacts(AddressBook *address_book, const char *title, int *index, const char *msg, Modes mode)
{
	/*
	 * Add code to list all the contacts availabe in address_book.csv file
	 * Should be menu based
	 * The menu provide navigation option if the entries increase the page size
	 */

	char input;

	// check to see if there are any contacts in address book
	if (!address_book || address_book->count == 0)
	{
		printf("There are no contacts available. \n");
		return e_fail;
	}

	int page = 0;
	int total_num_of_pages = (address_book->count + PAGE_SIZE - 1) / PAGE_SIZE;

	do
	{
		menu_header(title);
		printf("%s\n", msg);

		int start_page = page * PAGE_SIZE;
		int end = (page + 1) * PAGE_SIZE < address_book->count ? (page + 1) * PAGE_SIZE : address_book->count;

		printf("=======================================================================================================\n");

		// print the contact info categories
		printf(": %-10s  : %-30s : %-30s  : %-18s :\n", "S.No", "Name", "Phone No", "Email ID");

		printf("=======================================================================================================\n");

		int total_num_entries = EMAIL_ID_COUNT > PHONE_NUMBER_COUNT ? EMAIL_ID_COUNT : PHONE_NUMBER_COUNT;

		// prints all entries with corresponding number + email
		for (int i = 0; i < total_num_entries; i++)
		{
			if (i == 0)
			{
				printf(": %-10d  : %-30s : %-30s  : %-30s :\n", address_book->list[*index].si_no, address_book->list[*index].name[i], address_book->list[*index].phone_numbers[i], address_book->list[*index].email_addresses[i]);
			}
			else
			{
				printf(": %-10s  : %-15s  : %-20s :\n", "", address_book->list[*index].phone_numbers[0] ? address_book->list[*index].phone_numbers[i] : "", address_book->list[*index].email_addresses[0] ? address_book->list[*index].email_addresses[i] : "");
			}
		}

		printf("=======================================================================================================");

		printf("Press:\n ");

		// goes back to previous page
		if (page > 0)
		{
			printf("(b) Back");
		}
		// if end < total number of contact --> there are more contacts left
		if (end < address_book->count)
		{
			printf("(N) Next");
		}
		printf("(Q) Cancel:\n");

		// read in user input for navigation option
		printf("Choose an option");
		scanf(" %c", &input);

		switch (input)
		{
		case 'B':
		case 'b':
			if (page > 0)
			{
				page--;
			}
			break;
		case 'N':
		case 'n':
			if ((page + 1) * PAGE_SIZE < total_num_of_pages)
			{
				page++;
			}
			break;
		case 'Q':
		case 'q':
			printf("Exit program \n");
		default:
			printf("Entered Invalid Input, Exiting Program. \n");
		}
	} while (1);

	return e_success;
}

void menu_header(const char *str)
{
	fflush(stdout);

	system("clear");

	printf("#######  Address Book  #######\n");
	if (str != '\0')
	{
		printf("#######  %s\n", str);
	}
}

void main_menu(void)
{
	menu_header("Features:\n");

	printf("0. Exit\n");
	printf("1. Add Contact\n");
	printf("2. Search Contact\n");
	printf("3. Edit Contact\n");
	printf("4. Delete Contact\n");
	printf("5. List Contacts\n");
	printf("6. Save\n");
	printf("\n");
	printf("Please select an option: ");
}

Status menu(AddressBook *address_book)
{
	ContactInfo backup;
	Status ret;
	int option;

	do
	{
		main_menu();

		option = get_option(NUM, "");

		if ((address_book->count == 0) && (option != e_add_contact))
		{
			get_option(NONE, "No entries found!!. Would you like to add? Use Add Contacts");

			continue;
		}

		switch (option)
		{
		case e_add_contact:
			/* Add your implementation to call add_contacts function here */
			break;
		case e_search_contact:
			search_contact(address_book);
			break;
		case e_edit_contact:
			edit_contact(address_book);
			break;
		case e_delete_contact:
			delete_contact(address_book);
			break;
		case e_list_contacts:
			break;
			/* Add your implementation to call list_contacts function here */
		case e_save:
			save_file(address_book);
			break;
		case e_exit:
			break;
		}
	} while (option != e_exit);

	return e_success;
}

Status add_contacts(AddressBook *address_book)
{
	/* Add the functionality for adding contacts here */
}

Status search(const char *str, AddressBook *address_book, int loop_count, int field, const char *msg, Modes mode)
{
	/* Add the functionality for adding contacts here */
}

Status search_contact(AddressBook *address_book)
{
	/* Add the functionality for search contacts here */
}

Status edit_contact(AddressBook *address_book)
{
	/* Add the functionality for edit contacts here */
}

Status delete_contact(AddressBook *address_book)
{
	/* Add the functionality for delete contacts here */
}
