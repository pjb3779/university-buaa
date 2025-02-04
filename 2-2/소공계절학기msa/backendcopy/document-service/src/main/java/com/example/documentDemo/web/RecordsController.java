package com.example.documentDemo.web;

import com.example.documentDemo.model.Records;
import com.example.documentDemo.service.RecordsService;
import org.springframework.http.HttpStatus;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;


@RestController
@RequestMapping("/records")
public class RecordsController {

    @Autowired
    private RecordsService recordsService;

    @GetMapping("/select/{userId}")
    public List<Records> getRecords(@PathVariable int userId) {
        return recordsService.getRecords(userId);
    }

    @DeleteMapping("/delete/{userId}/{documentId}")
    public void deleteRecords(@PathVariable int userId, @PathVariable int documentId) {
        recordsService.deleteRecords(userId, documentId);
    }

    @PostMapping("/post/")
    public ResponseEntity<String> postMethodName(@RequestBody Records records) {
        recordsService.InsertRecords(records.getUserID(), records.getDocumentID());
        return new ResponseEntity<>("success record insertion", HttpStatus.OK);
    }

}
