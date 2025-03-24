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
	menu_header("Search Contact to Edit by:");
	printf("\n0. Back\n");
	printf("1. Name\n");
	printf("2. Phone No\n");
	printf("3. Email ID\n");
	printf("4. Serial No\n");
	printf("Please select an option: ");
	scanf("%d", &field_choice);
	while (getchar() != '\n'); //clear input buffer
	
	// Check for back option
	if (field_choice == 0)
	{
		return e_back;
	}
	
	//adjust field_choice to match internal numbering (1-based to 0-based)
	field_choice--;
	
	//get search term
	char search_term[50];
	printf("Enter the %s: ", field_choice == 0 ? "Name" : 
						field_choice == 1 ? "Phone No" :
						field_choice == 2 ? "Email ID" : "Serial No");
	fgets(search_term, 50, stdin);
	search_term[strcspn(search_term, "\n")] = 0; //remove newline
	
	//search for the contact
	Status search_result;
	search_result = search(search_term, address_book, address_book->count, field_choice, "Search Result:", e_edit);
	
	if (search_result == e_fail)
	{
		get_option(NONE, "No contact found with the given information\n\nPress enter to continue...");
		return e_fail;
	}
	
	//prompt for select/cancel as shown in Fig 5.17
	printf("\nPress: [s] | Select, [q] | Cancel: ");
	char choice;
	scanf("%c", &choice);
	while (getchar() != '\n'); //clear input buffer
	
	if (choice == 'q' || choice == 'Q')
	{
		return e_back;  //cancel and go back
	}
	
	if (choice != 's' && choice != 'S')
	{
		get_option(NONE, "Invalid option!\n\nPress enter to continue...");
		return e_fail;
	}
	
	//ask which contact to edit (by serial number)
	int contact_index = -1;
	printf("Select a Serial Number (S.No) to Edit: ");
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
	
	//display contact details and edit options
	while (1)
	{
		menu_header("Edit Contact:");
		printf("\n0. Back\n");
		printf("1. Name     : %s\n", address_book->list[actual_index].name[0]);
		printf("2. Phone No : ");
		//display all phone numbers
		for (int i = 0; i < PHONE_NUMBER_COUNT; i++)
		{
			if (strlen(address_book->list[actual_index].phone_numbers[i]) > 0)
			{
				printf("%s\n", address_book->list[actual_index].phone_numbers[i]);
				if (i < PHONE_NUMBER_COUNT - 1 && strlen(address_book->list[actual_index].phone_numbers[i+1]) > 0)
				{
					printf("             ");
				}
			}
		}
		printf("3. Email ID : ");
		//display all email addresses
		for (int i = 0; i < EMAIL_ID_COUNT; i++)
		{
			if (strlen(address_book->list[actual_index].email_addresses[i]) > 0)
			{
				printf("%s\n", address_book->list[actual_index].email_addresses[i]);
				if (i < EMAIL_ID_COUNT - 1 && strlen(address_book->list[actual_index].email_addresses[i+1]) > 0)
				{
					printf("             ");
				}
			}
		}
		
		printf("\nPlease select an option: ");
		int edit_choice;
		scanf("%d", &edit_choice);
		while (getchar() != '\n'); //clear input buffer
		
		if (edit_choice == 0)
		{
			//return to main menu
			return e_success;
		}
		
		switch (edit_choice)
		{
			case 1: //edit name
				printf("Enter the new Name: ");
				fgets(address_book->list[actual_index].name[0], NAME_LEN, stdin);
				address_book->list[actual_index].name[0][strcspn(address_book->list[actual_index].name[0], "\n")] = 0;
				break;
				
			case 2: //edit phone numbers
				{
					//show existing phone numbers with indices
					printf("Enter Phone Number index to be changed (Max %d): ", PHONE_NUMBER_COUNT);
					int phone_idx;
					scanf("%d", &phone_idx);
					while (getchar() != '\n'); //clear input buffer
					
					if (phone_idx < 1 || phone_idx > PHONE_NUMBER_COUNT)
					{
					printf("Invalid phone number index!\n");
					continue;
					}
					
					phone_idx--; //convert to 0-based index
					
					//get new phone number - exact prompt as shown in Fig 5.19
					printf("Enter Phone Number %d [just enter removes the entry]: ", phone_idx + 1);
					fgets(address_book->list[actual_index].phone_numbers[phone_idx], NUMBER_LEN, stdin);
					address_book->list[actual_index].phone_numbers[phone_idx][strcspn(address_book->list[actual_index].phone_numbers[phone_idx], "\n")] = 0;
				}
				break;
				
			case 3: //edit email addresses
				{
					//show existing email addresses with indices
					printf("Enter Email ID index to be changed (Max %d): ", EMAIL_ID_COUNT);
					int email_idx;
					scanf("%d", &email_idx);
					while (getchar() != '\n'); //clear input buffer
					
					if (email_idx < 1 || email_idx > EMAIL_ID_COUNT)
					{
					printf("Invalid email index!\n");
					continue;
					}
					
					email_idx--; //convert to 0-based index
					
					//get new email
					printf("Enter Email ID %d [just enter removes the entry]: ", email_idx + 1);
					fgets(address_book->list[actual_index].email_addresses[email_idx], EMAIL_ID_LEN, stdin);
					address_book->list[actual_index].email_addresses[email_idx][strcspn(address_book->list[actual_index].email_addresses[email_idx], "\n")] = 0;
				}
				break;
				
			default:
				printf("Invalid choice!\n");
				continue;
		}
	}
	
	return e_success;
}

Status delete_contact(AddressBook *address_book)
{
	/* Add the functionality for delete contacts here */
}
