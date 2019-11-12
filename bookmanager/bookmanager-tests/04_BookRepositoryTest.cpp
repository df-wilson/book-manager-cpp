#include "catch.hpp"

#include "../src/Book.h"
#include "../src/BookRepository.h"
#include <iostream>
#include <string>
#include "SQLiteCpp/SQLiteCpp.h"

using namespace dw;
using namespace std;

const string NAME = "Dean";
const string EMAIL = "test@tester.com";
const string NEW_PASSWORD = "New Password";
const string PASSWORD = "password";
const string JSON = R"({"id": 1, "title": "Test Book", "author": "Test Author", "year": "2018", "read": true, "rating": 4})";
const string TITLE = "Test Book";
const string AUTHOR = "Test Author";
const string YEAR = "2018";
const int    RATING = 4;
const bool   IS_READ = true;
const int    BOOK_1_USER_ID = 1;
const string BOOK_1_TITLE="Sorcerer's Daughter";
const string BOOK_1_AUTHOR="Terry Brooks";
const string BOOK_1_YEAR = "2009";
const int    BOOK_1_RATING = 4;
const bool   BOOK_1_IS_READ = true;
const long   NEW_BOOK_ID = 14;
const int    NUM_BOOKS_INIT = 13;
const long   NEW_USER_ID = 2;

TEST_CASE("Test Book class.") 
{

   Book book(NEW_BOOK_ID, NEW_USER_ID, TITLE, AUTHOR, YEAR, true, RATING);
   REQUIRE (book.id() == NEW_BOOK_ID);
   REQUIRE (book.userId() == NEW_USER_ID);
   REQUIRE (book.title() == TITLE);
   REQUIRE (book.author() == AUTHOR);
   REQUIRE (book.year() == YEAR);
   REQUIRE (book.read() == IS_READ);
   REQUIRE (book.rating() == RATING);
   
   // Test the toJson functionality. Note the order of the elements is not preserved.
   string json = book.toJson();
   Book bookCopy(json);

   REQUIRE (bookCopy.id() == NEW_BOOK_ID);
   REQUIRE (bookCopy.userId() == NEW_USER_ID);
   REQUIRE (bookCopy.title() == TITLE);
   REQUIRE (bookCopy.author() == AUTHOR);
   REQUIRE (bookCopy.year() == YEAR);
   REQUIRE (bookCopy.read() == IS_READ);
   REQUIRE (bookCopy.rating() == RATING);
   
   // Test creating a book with a JSON string.
   Book bookJSON(JSON); 
   REQUIRE (book.id() == NEW_BOOK_ID);
   REQUIRE (book.userId() == NEW_USER_ID);
   REQUIRE (book.title() == TITLE);
   REQUIRE (book.author() == AUTHOR);
   REQUIRE (book.year() == YEAR);
   REQUIRE (book.read() == IS_READ);
   REQUIRE (book.rating() == RATING);
 
   REQUIRE (book.toString() == "14 - 2 - Test Book - Test Author - 2018 - 1 - 4");
}

TEST_CASE( "Test BookRepository class. Get all books for user" ) 
{
   long userId = 1;
   
   BookRepository repository;
   std::vector<Book> books = repository.getAll(userId);
   REQUIRE (books.size() == NUM_BOOKS_INIT); 
   REQUIRE (books[0].id() == 1);
   REQUIRE (books[0].title() == BOOK_1_TITLE);
   REQUIRE (books[0].author() ==  BOOK_1_AUTHOR);
   REQUIRE (books[0].year() == BOOK_1_YEAR);
   REQUIRE (books[0].read() == BOOK_1_IS_READ);
   REQUIRE (books[0].rating() == BOOK_1_RATING);
}

TEST_CASE( "Test BookRepository class. Create and get by ID." ) 
{
   Book book(NEW_BOOK_ID, NEW_USER_ID, TITLE, AUTHOR, YEAR, true, RATING);
   BookRepository repository;

   int bookId = repository.store(book);
   
   Book book2 = repository.getById(NEW_USER_ID, bookId);
   REQUIRE (book2.id() == bookId);
   REQUIRE (book2.userId() == NEW_USER_ID);
   REQUIRE (book2.title() == TITLE);
   REQUIRE (book2.author() == AUTHOR);
   REQUIRE (book2.year() == YEAR);
   REQUIRE (book2.read() == IS_READ);
   REQUIRE (book2.rating() == RATING);

   // Test getting a book that doesn't belong to a user.   
   REQUIRE_THROWS_WITH(repository.getById(BOOK_1_USER_ID, bookId), "Book with that id does not exist for user.");

}

TEST_CASE("Test BookRepository class. Test search for book by author and title.")
{
   BookRepository repository;
   
   std::vector<Book> books = repository.search(BOOK_1_USER_ID, BookRepository::SEARCH_TYPE::AUTHOR, "Jack");
   REQUIRE(books.size() == 2);
   
   std::vector<Book> books2 = repository.search(BOOK_1_USER_ID, BookRepository::SEARCH_TYPE::TITLE, "Jack");
   REQUIRE(books2.size() == 0);
   
   std::vector<Book> books3 = repository.search(BOOK_1_USER_ID, BookRepository::SEARCH_TYPE::AUTHOR, "Sword");
   REQUIRE(books3.size() == 0);
   
   std::vector<Book> books4 = repository.search(BOOK_1_USER_ID, BookRepository::SEARCH_TYPE::TITLE, "Sword");
   REQUIRE(books4.size() == 1);
   
}


TEST_CASE("Test BookRepository class. Test updating book information.")
{
   BookRepository repository;
   Book book(NEW_BOOK_ID, NEW_USER_ID, "Updated Title", "Updated Author", "2000", false, 1);
   repository.update(book);
   
   Book updatedBook = repository.getById(NEW_USER_ID, NEW_BOOK_ID);
   REQUIRE (updatedBook.id() == NEW_BOOK_ID);
      REQUIRE (updatedBook.userId() == NEW_USER_ID);
   REQUIRE (updatedBook.title() == "Updated Title");
   REQUIRE (updatedBook.author() == "Updated Author");
   REQUIRE (updatedBook.year() == "2000");
   REQUIRE (updatedBook.read() == false);
   REQUIRE (updatedBook.rating() == 1);
}


TEST_CASE("BookRepository class. Test removing a user from the database.")
{
   BookRepository repository;
   
   // Try to remove a book with the wrong user ID
   REQUIRE_THROWS_WITH(repository.getById(NEW_USER_ID, BOOK_1_USER_ID), "Book with that id does not exist for user.");
   
   // Try to remove a book that belongs to a user.
   repository.remove(NEW_USER_ID, NEW_BOOK_ID);
   REQUIRE_THROWS_WITH(repository.getById(NEW_USER_ID, NEW_BOOK_ID), "Book with that id does not exist for user.");
}

