#include <stdlib.h>
#include <string.h>
#include "unity.h"
#include "address_book_menu.h"
#include "address_book.h"
#include "address_book_fops.h"

Status search(const char *str, AddressBook *address_book, int loop_count, int field, const char *msg, Modes mode);

void setUp(void) {
    // Set up code here (if needed)
}

void tearDown(void) {
    // Tear down code here (if needed)
}

void test_add_contacts(void) {
    // Test the address_book_menu function
    AddressBook address_book;
    address_book.count = 0;
    address_book.list = NULL; // Initialize contacts to NULL
    address_book.fp = NULL; // Initialize file pointer to NULL

    add_contacts(&address_book);
    // Add assertions to verify the expected behavior
    TEST_ASSERT_TRUE(address_book.count == 1); // Replace with actual assertions
}

void test_search(void) {
    // Test the search_contact function
    AddressBook address_book;
    address_book.count = 1;
    address_book.list = malloc(sizeof(ContactInfo)); // Allocate memory for one contact
    strcpy(address_book.list[0].name[0],"Ryder"); // Set name for the contact
    strcpy(address_book.list[0].phone_numbers[0], "12345"); // Set phone number for the contact
    strcpy(address_book.list[0].email_addresses[0], "ryder@gmail.com");
    strcpy(address_book.list[0].email_addresses[1], "ryder@cpp.edu"); 
    address_book.list[0].si_no = 1; // Set serial number for the contact
    address_book.fp = NULL; // Initialize file pointer to NULL

    Status result = search("Ryder", &address_book, 0, 0, "Searching", e_search_contact);   
    // Add assertions to verify the expected behavior
    TEST_ASSERT_TRUE(result == e_success);

    result = search("Bob", &address_book, 0, 0, "Searching", e_search_contact);
    TEST_ASSERT_TRUE(result == e_fail);


}

int main(void) {
    UNITY_BEGIN();
    // RUN_TEST(test_add_contacts);
    RUN_TEST(test_search);
    return UNITY_END();
}
