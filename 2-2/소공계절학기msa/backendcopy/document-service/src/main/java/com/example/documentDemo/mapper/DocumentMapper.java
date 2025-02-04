package com.example.documentDemo.mapper;
import com.example.documentDemo.model.Document;
import org.apache.ibatis.annotations.*;

import java.util.List;

@Mapper
public interface DocumentMapper {
    @Insert("INSERT INTO Document(Title,Tags,AuthorID,Summary,Popularity,chapterNum,CurrentModifyTime,publishTime,CategoryID,cover) VALUES(#{title},#{tags},#{authorID},#{summary},0,#{chapterNum},NOW(),NOW(),#{categoryID},#{cover})")
    @Options(useGeneratedKeys = true, keyProperty = "DocumentID")
    int publishDoc(Document document);

    @Select("SELECT * FROM Document WHERE AuthorID=#{userID}")
    List<Document> getAllDocuments(int userID);

    @Delete("DELETE FROM Document WHERE DocumentID=#{id}")
    void deleteDoc(int id);

    @Select("SELECT COUNT(*) FROM Document WHERE AuthorID = #{userID}")
    int docCount(int userID);

    @Select("SELECT SUM(Popularity) FROM Document WHERE AuthorID = #{userID}")
    int docPopularity(int userID);

    @Select("SELECT * FROM Document WHERE DocumentID=#{id}")
    Document getDocumentById(Long id);

    @Update("UPDATE Document SET chapterNum = #{chapterNum} WHERE DocumentID = #{DocumentID}")
    void updateChapterNum(int chapterNum, int DocumentID);
}
