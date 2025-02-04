package com.example.bookshelfDemo.web;
import com.example.bookshelfDemo.model.Bookshelf;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.*;
import org.springframework.web.bind.annotation.*;
import com.example.bookshelfDemo.service.BookshelfService;

@RestController
@RequestMapping("/bookshelf")
@CrossOrigin(origins = "http://localhost:5173")
public class BookshelfController {

    private final BookshelfService bookshelfService;

    @Autowired
    public BookshelfController(BookshelfService bookshelfService) {
        this.bookshelfService = bookshelfService;
    }

    @GetMapping("/{userId}/{documentId}")
    public ResponseEntity<Bookshelf> isBookInBookshelf(@PathVariable int userId, @PathVariable int documentId) {
        Bookshelf bookshelf = bookshelfService.getBook(userId, documentId);
        if (bookshelf != null) {
            return new ResponseEntity<>(bookshelf, HttpStatus.OK);
        } else {
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }
    }

    @PostMapping("/")
    public ResponseEntity<String> addBookToBookshelf(@RequestBody Bookshelf bookshelf) {
        bookshelfService.addBookToBookshelf(bookshelf);
        return new ResponseEntity<>("Book added to bookshelf successfully", HttpStatus.CREATED);
    }

    @DeleteMapping("/")
    public ResponseEntity<String> removeBookFromBookshelf(@RequestBody Bookshelf bookshelf) {
        bookshelfService.deleteBookFromBookshelf(bookshelf);
        return new ResponseEntity<>("Book removed from bookshelf successfully", HttpStatus.NO_CONTENT);
    }
}
