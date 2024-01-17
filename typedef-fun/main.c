#include <stdio.h>
#include <string.h>
 
typedef struct BookStruct {
   char title[50];
   char author[50];
   char subject[100];
    int id;
} Book;
 
int main( ) {

   Book book;
 
   strcpy( book.title, "C manual");
   strcpy( book.author, "DR"); 
   strcpy( book.subject, "C Programming");
   book.id = 6495407;
 
   printf( "Book title : %s\n", book.title);
   printf( "Book author : %s\n", book.author);
   printf( "Book subject : %s\n", book.subject);
   printf( "Book book_id : %d\n", book.id);

   return 0;
}
