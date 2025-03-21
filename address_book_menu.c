#include <stdio.h>
//#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "address_book.h"
#include "address_book_menu.h"

//#include "abk_fileops.h"
//#include "abk_log.h"
//#include "abk_menus.h"
//#include "abk.h"

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

		if ((address_book-> count == 0) && (option != e_add_contact))
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
	//check if there are any contacts to edit
	if (address_book->count == 0)
	{
		get_option(NONE, "No contacts in address book to edit!\n\nPress enter to continue...");
		return e_fail;
	}
	
	//menu for search criteria
	int field_choice;
	menu_header("Edit Contact");
	printf("\nSearch contact by:\n");
	printf("0. Name\n");
	printf("1. Phone Number\n");
	printf("2. Email\n");
	printf("3. Serial Number\n");
	printf("Enter choice (0-3): ");
	scanf("%d", &field_choice);
	while (getchar() != '\n'); //clear input buffer
	
	// Get search term
	char search_term[50];
	printf("Enter search term: ");
	fgets(search_term, 50, stdin);
	search_term[strcspn(search_term, "\n")] = 0; //remove newline
	
	//search for the contact
	Status search_result;
	search_result = search(search_term, address_book, address_book->count, field_choice, "Edit Contact", e_edit);
	
	if (search_result == e_fail)
	{
		get_option(NONE, "No contact found with the given information\n\nPress enter to continue...");
		return e_fail;
	}
	
	//ask which contact to edit (by serial number)
	int contact_index = -1;
	printf("Enter the serial number of the contact to edit: ");
	scanf("%d", &contact_index);
	while (getchar() != '\n'); //clear input buffer
	
	//find the index in our list that matches this serial number
	int actual_index = -1;
	for (int i = 0; i < address_book->count; i++)
	{
		if (address_book->list[i].si_no == contact_index)
		{
			actual_index = i;
			break;
		}
	}
	
	if (actual_index == -1)
	{
		get_option(NONE, "Invalid serial number!\n\nPress enter to continue...");
		return e_fail;
	}
	
	//show what can be edited
	int edit_choice;
	menu_header("Edit Contact");
	printf("\nWhat would you like to edit?\n");
	printf("1. Name\n");
	printf("2. Phone Numbers\n");
	printf("3. Email Addresses\n");
	printf("Enter choice (1-3): ");
	scanf("%d", &edit_choice);
	while (getchar() != '\n'); //clear input buffer
	
	//edit based on user choice
	switch (edit_choice)
	{
		case 1: // Edit name
			printf("Current name: %s\n", address_book->list[actual_index].name[0]);
			printf("Enter new name: ");
			fgets(address_book->list[actual_index].name[0], NAME_LEN, stdin);
			address_book->list[actual_index].name[0][strcspn(address_book->list[actual_index].name[0], "\n")] = 0;
			break;
			
		case 2: //edit phone numbers
			//first display current phone numbers
			printf("Current phone numbers:\n");
			for (int i = 0; i < PHONE_NUMBER_COUNT; i++)
			{
				if (strlen(address_book->list[actual_index].phone_numbers[i]) > 0)
				{
					printf("%d. %s\n", i+1, address_book->list[actual_index].phone_numbers[i]);
				}
			}
			
			//then get new phone numbers
			printf("\nEnter new phone numbers (press enter to skip):\n");
			for (int i = 0; i < PHONE_NUMBER_COUNT; i++)
			{
				printf("Phone %d: ", i+1);
				fgets(address_book->list[actual_index].phone_numbers[i], NUMBER_LEN, stdin);
				address_book->list[actual_index].phone_numbers[i][strcspn(address_book->list[actual_index].phone_numbers[i], "\n")] = 0;
				
				if (strlen(address_book->list[actual_index].phone_numbers[i]) == 0)
				{
					break;
				}
			}
			break;
			
		case 3: //edit email addresses
			//first display current emails
			printf("Current email addresses:\n");
			for (int i = 0; i < EMAIL_ID_COUNT; i++)
			{
				if (strlen(address_book->list[actual_index].email_addresses[i]) > 0)
				{
					printf("%d. %s\n", i+1, address_book->list[actual_index].email_addresses[i]);
				}
			}
			
			//then get new emails
			printf("\nEnter new email addresses (press enter to skip):\n");
			for (int i = 0; i < EMAIL_ID_COUNT; i++)
			{
				printf("Email %d: ", i+1);
				fgets(address_book->list[actual_index].email_addresses[i], EMAIL_ID_LEN, stdin);
				address_book->list[actual_index].email_addresses[i][strcspn(address_book->list[actual_index].email_addresses[i], "\n")] = 0;
				
				if (strlen(address_book->list[actual_index].email_addresses[i]) == 0)
				{
					break;
				}
			}
			break;
			
		default:
			get_option(NONE, "Invalid choice!\n\nPress enter to continue...");
			return e_fail;
	}
	
	get_option(NONE, "Contact updated successfully!\n\nPress enter to continue...");
	return e_success;
}

Status delete_contact(AddressBook *address_book)
{
	/* Add the functionality for delete contacts here */
}
