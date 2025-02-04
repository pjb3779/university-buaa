package com.example.chapterDemo.mapper;
import com.example.chapterDemo.model.*;
import org.apache.ibatis.annotations.*;

import java.util.List;

@Mapper
public interface ChapterMapper {
    @Insert("INSERT INTO chapter(Num,creatTime,DocumentID,content,title) VALUES (#{Num},NOW(),#{DocumentID},#{content},#{title})")
    void publishChapter(Chapter chapter);

    @Delete("DELETE FROM chapter WHERE DocumentID=#{documentID}")
    void deleteChapter(int documentID);

    @Select("SELECT * FROM chapter WHERE DocumentID=#{DocumentID} AND Num=#{chapterNum}")
    Chapter getChapterById(int DocumentID, int chapterNum);

    @Select("SELECT * FROM chapter WHERE DocumentID = #{DocumentID}")
    List<Chapter> getChapters(int DocumentID);

}
