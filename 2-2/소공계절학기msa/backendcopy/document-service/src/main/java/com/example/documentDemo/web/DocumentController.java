package com.example.documentDemo.web;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.*;
import org.springframework.web.bind.annotation.*;
import com.example.documentDemo.service.DocumentService;
import com.example.documentDemo.model.Document;

import java.util.List;

@RestController
@RequestMapping("/documents")
@CrossOrigin(origins = "http://localhost:5173")
public class DocumentController {

    private final DocumentService documentService;

    @Autowired
    public DocumentController(DocumentService documentService) {
        this.documentService = documentService;
    }

    @PostMapping("/")
    public ResponseEntity<Integer> publishDocument(@RequestBody Document doc) {
        if (doc == null) {
            return new ResponseEntity<>(HttpStatus.BAD_REQUEST);
        }
        documentService.publishDoc(doc);
        return new ResponseEntity<>(doc.getDocumentID(), HttpStatus.CREATED);
    }

    @GetMapping("/user/{userId}")
    public ResponseEntity<List<Document>> getAllDocuments(@PathVariable int userId) {
        if (userId <= 0) {
            return new ResponseEntity<>(HttpStatus.BAD_REQUEST);
        }
        List<Document> documents = documentService.getAllDocuments(userId);
        return documents.isEmpty() ? new ResponseEntity<>(HttpStatus.NO_CONTENT) : new ResponseEntity<>(documents, HttpStatus.OK);
    }

    @GetMapping("/document/{documentId}")
    public ResponseEntity<Document> getDocumentById(@PathVariable Long documentId) {
        Document document = documentService.getDocumentById(documentId);
        if (document != null) {
            return new ResponseEntity<>(document, HttpStatus.OK);
        } else {
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }
    }

    @DeleteMapping("/{documentId}")
    public ResponseEntity<String> deleteDocument(@PathVariable int documentId) {
        if (documentId <= 0) {
            return new ResponseEntity<>(HttpStatus.BAD_REQUEST);
        }
        documentService.deleteDoc(documentId);
        return new ResponseEntity<>("Document deleted successfully", HttpStatus.NO_CONTENT);
    }

    @GetMapping("/{userId}/docCount")
    public ResponseEntity<Integer> getDocumentCount(@PathVariable int userId) {
        int count = documentService.docCount(userId);
        return new ResponseEntity<>(count, HttpStatus.OK);
    }

    @GetMapping("/{userId}/popularity")
    public ResponseEntity<Integer> getDocumentPopularity(@PathVariable int userId) {
        int popularity = documentService.docPopularity(userId);
        return new ResponseEntity<>(popularity, HttpStatus.OK);
    }

    @PutMapping("/updateChapterNum/")
    public ResponseEntity<String> updateChapterNum(@RequestBody Document doc){
        if(doc==null){
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);}
        documentService.updateChapterNum(doc.getChapterNum(),doc.getDocumentID());
        return new ResponseEntity<>("Delete chapter successfully", HttpStatus.OK);
    } 


    
}
