package com.example.commentDemo.service;

import com.example.commentDemo.mapper.CommentMapper;
import com.example.commentDemo.model.Comment;
import org.springframework.beans.factory.annotation.Autowired;
import com.example.commentDemo.model.*;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class CommentService {
    private final CommentMapper commentMapper;

    @Autowired
    public CommentService(CommentMapper commentMapper) {
        this.commentMapper = commentMapper;
    }

    public void addComment(Comment comment) {
        commentMapper.insertComment(comment);
    }

    public List<Comment> getComments(int DocumentID, int chapterNum) {
        return commentMapper.getCommentsByUserIdAndDocumentId(DocumentID, chapterNum);
    }

    // 其他评论相关方法...
}

