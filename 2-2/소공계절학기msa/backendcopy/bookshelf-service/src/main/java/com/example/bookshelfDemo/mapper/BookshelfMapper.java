package com.example.bookshelfDemo.mapper;
import com.example.bookshelfDemo.model.Bookshelf;
import org.apache.ibatis.annotations.*;

@Mapper
public interface BookshelfMapper {
    @Select("SELECT * FROM Bookshelf WHERE UserID=#{UserID} AND DocumentID=#{DocumentID}")
    Bookshelf getBook(int UserID, int DocumentID);

    @Insert("Insert INTO Bookshelf(UserID, DocumentID, FavorTime) VALUES(#{UserID}, #{DocumentID}, NOW())")
    void addBookToBookshelf(Bookshelf bookshelf);

    @Delete("DELETE FROM Bookshelf WHERE UserID=#{UserID} AND DocumentID=#{DocumentID}")
    void deleteBookFromBookshelf(Bookshelf bookshelf);


}
