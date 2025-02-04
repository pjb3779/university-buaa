package com.example.documentDemo.web;

import com.example.documentDemo.model.FavorDoc;
import com.example.documentDemo.service.FavorDocService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/documents")
@CrossOrigin(origins = "http://localhost:5173")
public class FavorDocController {


    @Autowired
    private FavorDocService favordocService;

    @GetMapping("/favorDoc/select/FT/{userId}")
    public ResponseEntity<List<FavorDoc>> getFavorDocs_FT(@PathVariable int userId) {
        List<FavorDoc> favorDocs = favordocService.getFavorDocs_FT(userId);
        if(favorDocs != null){
            return new ResponseEntity<>(favorDocs, HttpStatus.OK);
        }else{
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }
    }
    @GetMapping("/favorDoc/select/UT/{userId}")
    public ResponseEntity<List<FavorDoc>> getFavorDocs_UT(@PathVariable int userId) {
        List<FavorDoc> favorDocs = favordocService.getFavorDocs_UT(userId);
        if(favorDocs != null){
            return new ResponseEntity<>(favorDocs, HttpStatus.OK);
        }else{
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }
        // return favordocService.getFavorDocs_UT(userId);
    }

    @DeleteMapping("/favorDoc/delete/{userId}/{documentId}")
    public void deleteFavorDoc(@PathVariable int userId, @PathVariable int documentId) {
        favordocService.deleteFavorDoc(userId, documentId);
    }
}
