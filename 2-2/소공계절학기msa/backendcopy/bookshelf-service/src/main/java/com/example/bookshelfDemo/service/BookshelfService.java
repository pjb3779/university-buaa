package com.example.bookshelfDemo.service;

import com.example.bookshelfDemo.mapper.BookshelfMapper;
import org.springframework.beans.factory.annotation.Autowired;
import com.example.bookshelfDemo.model.Bookshelf;
import org.springframework.stereotype.Service;


@Service
public class BookshelfService {
    private final BookshelfMapper bookshelfMapper;

    @Autowired
    public BookshelfService(BookshelfMapper bookshelfMapper) {
        this.bookshelfMapper = bookshelfMapper;
    }

    public Bookshelf getBook(int UserID, int DocumentID) {
        return bookshelfMapper.getBook(UserID, DocumentID);
    }
    public void addBookToBookshelf(Bookshelf book) {
        bookshelfMapper.addBookToBookshelf(book);
    }
    public void deleteBookFromBookshelf(Bookshelf bookshelf) {
        bookshelfMapper.deleteBookFromBookshelf(bookshelf);
    }
    // 其他书架相关方法...


}
