#include "unity.h"
#include "address_book_menu.h"
#include "address_book.h"
#include "address_book_fops.h"

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

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_add_contacts);
    return UNITY_END();
}
