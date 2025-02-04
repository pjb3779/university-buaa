package com.example.commentDemo.web;
import com.example.commentDemo.model.*;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.*;
import org.springframework.web.bind.annotation.*;
import com.example.commentDemo.service.CommentService;

import java.util.List;

@RestController
@RequestMapping("/comments")
@CrossOrigin(origins = "http://localhost:5173")
public class CommentController {

    private final CommentService commentService;

    @Autowired
    public CommentController(CommentService commentService) {
        this.commentService = commentService;
    }

    @PostMapping("/")
    public ResponseEntity<String> addComment(@RequestBody Comment comment) {
        commentService.addComment(comment);
        return new ResponseEntity<>("Comment added successfully", HttpStatus.CREATED);
    }

    @GetMapping("/{documentId}/{chapterNum}")
    public ResponseEntity<List<Comment>> getComments(@PathVariable int documentId, @PathVariable int chapterNum) {
        List<Comment> comments = commentService.getComments(documentId, chapterNum);
        if (comments != null) {
            return new ResponseEntity<>(comments, HttpStatus.OK);
        } else {
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }
    }
}
