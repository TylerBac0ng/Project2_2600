#include <stdio.h>
//#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "address_book.h"
#include "address_book_menu.h"
#include "address_book_fops.h"

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

	int num;
	char character;

	printf("%s", msg);

	switch(type) {
		case NUM:
			scanf("%d", &num);
			printf("num=%d\n", num);
			while (getchar() != '\n'); //clears buffer
			if (num >= 0 && num <=6)  { //checks if input is between valid range
				return num;
			}
		case CHAR:
			scanf("%c", &character);
			while (getchar() != '\n');
			if(character == 'Y' || character == 'N') {
				return character;
			}
		case NONE:
			getchar();
			return 0;
		default:
			printf("Invalid input type\n");
			return -1;
	}
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
	if (str != NULL)
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
			get_option(NONE, "No meows found!!. Would you like to add? Use Add Contacts");

			continue;
		}

		switch (option)
		{
			case e_add_contact:
				add_contacts(address_book);
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

	//allocates memory if list isnt already allocated
	if (address_book->list == NULL) {
		address_book->list =malloc(10 * sizeof(ContactInfo)); //initially allocates memory for 10 contacts
		if (address_book->list == NULL) {
			printf("Memory allocation failed.\n");
			return e_fail;
		}
		address_book->count = 0; //initializes count at 0 contacts because there are none added yet
	}
	printf("count = %d\n", address_book->count);

	//if count exceeds the initial allocated memory (memory for 10 contacts), need to reallocate memory
	if (address_book->count >= 10) {
		address_book->list = realloc(address_book->list, 20 * sizeof(ContactInfo));
		if (address_book->list == NULL) {
			printf("Memory reallocation failed.\n");
			return e_fail;
		}
	}

	printf("Enter name: \n");
	//reads line inputted by user and stores it into memory locations
	fgets(address_book->list[address_book->count].name[0],NAME_LEN,stdin); //name plus the new line is stored so we have to remove the new line in the next line of code
	// fscanf(stdin, "%s", address_book->list[address_book->count].name[0]); //reads the name inputted by user
	//removes new line character (\n) from end of string stored in name[0], so the names will match 
	address_book->list[address_book->count].name[0][strcspn(address_book->list[address_book->count].name[0],"\n")] = 0;
	printf("name=%s\n", address_book->list[address_book->count].name[0]);

	//goes through a loop of phone number count (users can enter up to 5 phone numbers)
	for (int i = 0; i <= PHONE_NUMBER_COUNT; i++) {
		printf("Enter phone number %d (or press enter to skip): \n", i + 1); //prompts user to enter indexed phone number
		fgets(address_book->list[address_book->count].phone_numbers[i], NUMBER_LEN, stdin); //reads the phone number inputted by user
		// fscanf(stdin, "%s", address_book->list[address_book->count].phone_numbers[i]); //reads the phone number inputted by user
		address_book->list[address_book->count].phone_numbers[i][strcspn(address_book->list[address_book->count].phone_numbers[i],"\n")] = 0;
		//if the user presses enter without actually inputted numbers, stops asking for more and breaks out loop
		if(strlen(address_book->list[address_book->count].phone_numbers[i]) == 0) {
			break;
		}
	}

	//goes through a loop of emails (users can enter 5 emails)
	for (int i = 0; i <= EMAIL_ID_COUNT; i++) {
		printf("Enter email %d (or press enter to skip): ", i + 1);
		fgets(address_book->list[address_book->count].email_addresses[i], EMAIL_ID_LEN, stdin); //reads the email inputted by user
		
		printf("email=%s\n", address_book->list[address_book->count].email_addresses[i]);
		address_book->list[address_book->count].email_addresses[i][strcspn(address_book->list[address_book->count].email_addresses[i], "\n")] = 0;
		if(strlen(address_book->list[address_book->count].email_addresses[i]) == 0) {
			break;
		}
	}

	printf("Enter seriel number: ");
	scanf("%d", &address_book->list[address_book->count].si_no); //reads serial number inputted by user
	while (getchar() != '\n'); // Discards all characters until a newline is encountered

	address_book->count++;
	return e_success;
}

Status search(const char *str, AddressBook *address_book, int loop_count, int field, const char *msg, Modes mode)
 {
 	/* Add the functionality for adding contacts here */
 	
 	int found = 0;
 

 	for (int i = 0; i < address_book->count; i++) {
 
 		// Makes sure there's a match in the address book
 		int match = 0;
 
 		switch (field) {
 			case 0: // Search by name
 				for (int j = 0; j < NAME_COUNT; j++) {
 				if (strcasecmp(address_book->list[i].name[j], str) == 0) {
                 match = 1;
                 break;
 				}
 				break;
 			case 1: // Search by phone number
 				for (int j = 0; j < PHONE_NUMBER_COUNT; j++) {
 					if (strcasecmp(address_book->list[i].phone_numbers[j], str) == 0) {
 						match = 1;
 						break;
 					}
 				}
 				 break;
 			case 2: // Search by email
 				for (int j = 0; j < EMAIL_ID_COUNT; j++) {
 					if (strcasecmp(address_book->list[i].email_addresses[j], str) == 0) {
 						match = 1;
 						break;
 					}
 				}
 				break;
 			case 3: // Search by serial number
 				if (address_book->list[i].si_no == atoi(str)) {
 					match = 1;
 					break;
 				}
 				break;
 			default: 
 				break;
 			}
 		}
 
 		if (match) {
 			found = 1;
 			printf("Name: %s\n", address_book->list[i].name[0]);
 			for (int j = 0; j < PHONE_NUMBER_COUNT; j++) {
 				printf("Phone number %d: %s\n", j + 1, address_book->list[i].phone_numbers[j]);
 			}
 			for (int j = 0; j < EMAIL_ID_COUNT; j++) {
 				printf("Email %d: %s\n", j + 1, address_book->list[i].email_addresses[j]);
 			}
 			printf("Serial number: %d\n", address_book->list[i].si_no);
 		}
 	}
 
 	if (!found) {
         printf("No contact found with the given information\n");
         return e_fail;
     }
 	
 	return e_success;
 	}

Status search_contact(AddressBook *address_book)
{
	/* Add the functionality for search contacts here */
	//search string
	char str[NAME_LEN];
	//field to search by
	int field;
	
	printf("Search by:\n");
    printf("1. Name\n");
    printf("2. Phone Number\n");
    printf("3. Email Address\n");
    printf("Enter your choice: ");
    scanf("%d", &field);
    while (getchar() != '\n'); //clear input buffer
	if (field < 1 || field > 4) {
		printf("Invalid choice.\n");
		return e_fail;
	}

	field = field - 1; //adjust for 0-based indexing

    //prompt user to enter the search string
    printf("Enter the search term: ");
    fgets(str, NAME_LEN, stdin);
    str[strcspn(str, "\n")] = 0; //remove newline character

    //check if the search term is empty
    if (strlen(str) == 0) {
        printf("Search term cannot be empty. Please try again.\n");
        return e_fail;
    }
    printf("You selected field %d and entered: %s\n", field, str);

	Status result = search(str, address_book, address_book->count, field, "Searching...", e_search_contact);

    if (result == e_success) {
		printf("Contact found!\n");
	} else {
		printf("Contact not found.\n");
	}

	return result;	
}

Status edit_contact(AddressBook *address_book)
{
	/* Add the functionality for edit contacts here */
}

Status delete_contact(AddressBook *address_book)
{
	/* Add the functionality for delete contacts here */
}
