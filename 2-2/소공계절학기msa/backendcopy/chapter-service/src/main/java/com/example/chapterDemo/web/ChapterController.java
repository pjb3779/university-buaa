package com.example.chapterDemo.web;
import com.example.chapterDemo.model.*;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.*;
import org.springframework.web.bind.annotation.*;
import com.example.chapterDemo.service.ChapterService;
import java.util.List;

@RestController
@RequestMapping("/chapters")
public class ChapterController {

    private final ChapterService chapterService;

    @Autowired
    public ChapterController(ChapterService chapterService) {
        this.chapterService = chapterService;
    }

    @PostMapping("/")
    public ResponseEntity<String> publishChapter(@RequestBody Chapter chapter) {
        if (chapter == null) {
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }
        chapterService.publishChapter(chapter);
        return new ResponseEntity<>("Chapter published successfully", HttpStatus.OK);
    }

    @GetMapping("/{documentId}")
    public ResponseEntity<List<Chapter>> getChapters(@PathVariable int documentId) {
        if (documentId <= 0) {
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }
        List<Chapter> chapters = chapterService.getChapters(documentId);
        return chapters.isEmpty() ? new ResponseEntity<>(HttpStatus.NO_CONTENT) : new ResponseEntity<>(chapters, HttpStatus.OK);
    }

    @GetMapping("/{documentId}/{chapterNum}")
    public ResponseEntity<Chapter> getChapterById(@PathVariable int documentId, @PathVariable int chapterNum) {
        Chapter chapter = chapterService.getChapterById(documentId, chapterNum);
        if (chapter != null) {
            return new ResponseEntity<>(chapter, HttpStatus.OK);
        } else {
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }
    }

    @DeleteMapping("/deleteChapters/")
    public ResponseEntity<String> deleteChapter(@RequestParam int documentID){
        if(documentID==0){
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);}

        chapterService.deleteChapter(documentID);
        return new ResponseEntity<>("Delete chapter successfully", HttpStatus.OK);
    } 
}

