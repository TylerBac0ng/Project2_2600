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
			get_option(NONE, "No contacts found!!. Would you like to add? Use Add Contacts");

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
    // Allocate memory if the list isn't already allocated
    if (address_book->list == NULL) {
        address_book->list = malloc(10 * sizeof(ContactInfo)); // Initially allocate memory for 10 contacts
        if (address_book->list == NULL) {
            printf("Memory allocation failed.\n");
            return e_fail;
        }
        address_book->count = 0; // Initialize count at 0 contacts because there are none added yet
    }

    // If count exceeds the initial allocated memory (memory for 10 contacts), reallocate memory
    if (address_book->count >= 10) {
        address_book->list = realloc(address_book->list, 20 * sizeof(ContactInfo));
        if (address_book->list == NULL) {
            printf("Memory reallocation failed.\n");
            return e_fail;
        }
    }

	//initializes the new contact's fields to default values
    ContactInfo *new_contact = &address_book->list[address_book->count];
    memset(new_contact, 0, sizeof(ContactInfo)); //sets all bytes to 0
    for (int i = 0; i < NAME_COUNT; i++) {
        new_contact->name[i][0] = '\0'; //initializes names to empty strings
    }
    for (int i = 0; i < PHONE_NUMBER_COUNT; i++) {
        new_contact->phone_numbers[i][0] = '\0'; 
    }
    for (int i = 0; i < EMAIL_ID_COUNT; i++) {
        new_contact->email_addresses[i][0] = '\0';
    }
    new_contact->si_no = 0; //initializes serial number to 0

    int field;
    do {
        //display the menu with current values
		printf("#######  Address Book #######\n");
		printf("#######  Add Contact\n");
        printf("\n0. Back\n");
        printf("1. Name: [%s]\n", 
        strlen(address_book->list[address_book->count].name[0]) > 0 ? 
        address_book->list[address_book->count].name[0] : "");
        printf("2. Phone Number(s):\n");
		for (int i = 0; i < PHONE_NUMBER_COUNT; i++) {
			printf("   %d. [%s]\n", i + 1,
            strlen(address_book->list[address_book->count].phone_numbers[i]) > 0 ? 
            address_book->list[address_book->count].phone_numbers[i] : "");
		}
        printf("3. Email(s):\n");
		for (int i = 0; i < EMAIL_ID_COUNT; i++) {
			printf("   %d. [%s]\n", i + 1,
            strlen(address_book->list[address_book->count].email_addresses[i]) > 0 ? 
            address_book->list[address_book->count].email_addresses[i] : "");
		}
        printf("4. Serial Number: [%d]\n", 
               address_book->list[address_book->count].si_no > 0 ? 
               address_book->list[address_book->count].si_no : 0);

        printf("Select an option: (0-4) ");
        scanf("%d", &field);
        while (getchar() != '\n'); //clears input buffer

        //handles invalid input
        if (field < 0 || field > 4) {
            printf("Invalid choice. Please try again.\n");
            continue;
        }

        //handles each option
        switch (field) {
            case 0: // Exit
                printf("Exiting add contacts menu.\n");
                break;

            case 1: // adds name
                printf("Enter name: ");
                fgets(address_book->list[address_book->count].name[0], NAME_LEN, stdin);
                address_book->list[address_book->count].name[0][strcspn(address_book->list[address_book->count].name[0], "\n")] = 0; // Remove newline
                break;

            case 2: // adds phone number
                for (int i = 0; i < PHONE_NUMBER_COUNT; i++) {
                    printf("Enter phone number %d (or press enter to skip): ", i + 1);
                    fgets(address_book->list[address_book->count].phone_numbers[i], NUMBER_LEN, stdin);
                    address_book->list[address_book->count].phone_numbers[i][strcspn(address_book->list[address_book->count].phone_numbers[i], "\n")] = 0; // Remove newline
                    if (strlen(address_book->list[address_book->count].phone_numbers[i]) == 0) {
                        break;
                    }
                }
                break;

            case 3: // adds email
                for (int i = 0; i < EMAIL_ID_COUNT; i++) {
                    printf("Enter email %d (or press enter to skip): ", i + 1);
                    fgets(address_book->list[address_book->count].email_addresses[i], EMAIL_ID_LEN, stdin);
                    address_book->list[address_book->count].email_addresses[i][strcspn(address_book->list[address_book->count].email_addresses[i], "\n")] = 0; // Remove newline
                    if (strlen(address_book->list[address_book->count].email_addresses[i]) == 0) {
                        break;
                    }
                }
                break;

            case 4: // adds serial number
                printf("Enter serial number: ");
                scanf("%d", &address_book->list[address_book->count].si_no);
                while (getchar() != '\n'); 
                break;

            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }

    } while (field != 0); // repeat until the user selects "Back" (0)

    // increments the contact count after adding all fields
    address_book->count++;
    printf("Contact added successfully! Total contacts: %d\n", address_book->count);

    return e_success;
}

Status search(const char *str, AddressBook *address_book, int loop_count, int field, const char *msg, Modes mode)
{
    int found = 0;

    for (int i = 0; i < address_book->count; i++) {
        // Check if there's a match in the address book
        int match = 0;

        switch (field) {
            case 0: // Search by name
                for (int j = 0; j < NAME_COUNT; j++) {
                    if (strcasecmp(address_book->list[i].name[j], str) == 0) {
                        match = 1;
                        break;
                    }
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
                }
                break;

            default:
                break;
        }

        if (match) {
            if (!found) {
				printf("#######  Address Book  #######\n");
				printf("#######  Search Results:\n\n");
                //prints the table header only once
                printf("====================================================================================================================\n");
                printf(": S.No : Name                            : Phone Number(s)                       : Email(s)                        :\n");
                printf("====================================================================================================================\n");
            }

            found = 1;

        	// prints the contact details with consistent column widths
			printf(": %-5d: %-30s: %-40s: %-30s:\n",
			address_book->list[i].si_no,
			address_book->list[i].name[0],
			address_book->list[i].phone_numbers[0],
			address_book->list[i].email_addresses[0]);

			// determines the maximum number of rows needed for phone numbers and emails
			int max_rows = PHONE_NUMBER_COUNT > EMAIL_ID_COUNT ? PHONE_NUMBER_COUNT : EMAIL_ID_COUNT;

			// prints additional phone numbers and emails in the same row
			for (int j = 1; j < max_rows; j++) {
 				printf(": %-5s: %-30s: %-40s: %-30s:\n",
				"", // leaves S.No blank
				"", // leaves Name blank
				j < PHONE_NUMBER_COUNT && strlen(address_book->list[i].phone_numbers[j]) > 0 ? address_book->list[i].phone_numbers[j] : "",
				j < EMAIL_ID_COUNT && strlen(address_book->list[i].email_addresses[j]) > 0 ? address_book->list[i].email_addresses[j] : "");
			}

			// prints a separator line after each contact
			printf("====================================================================================================================\n");
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
	
	printf("#######  Address Book  #######\n");
	printf("#######  Search Contact by:\n\n");
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
 TylerB_branch
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
 main
}

Status delete_contact(AddressBook *address_book)
{
	/* Add the functionality for delete contacts here */
}
