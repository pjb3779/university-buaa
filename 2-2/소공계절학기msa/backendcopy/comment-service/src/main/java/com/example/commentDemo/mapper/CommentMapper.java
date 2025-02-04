package com.example.commentDemo.mapper;
import com.example.commentDemo.model.Comment;
import com.example.commentDemo.model.*;
import org.apache.ibatis.annotations.*;

import java.util.List;

@Mapper
public interface CommentMapper {
    @Insert("INSERT INTO Comment(CommentID, UserID, Content, PublishTime, DocumentID, likes, chapterNum, userName) VALUES(#{CommentID}, #{UserID}, #{Content}, NOW(), #{DocumentID}, #{likes}, #{chapterNum}, #{userName})")
    void insertComment(Comment comment);

    @Select("SELECT * FROM Comment WHERE DocumentID = #{DocumentID} AND chapterNum=#{chapterNum}")
    List<Comment> getCommentsByUserIdAndDocumentId(int DocumentID, int chapterNum);

}
