package com.example.draftDemo.web;

import com.example.draftDemo.service.DraftService;
import com.example.draftDemo.model.Draft;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;
import org.springframework.http.*;

import java.util.List;
@RestController
@RequestMapping("/drafts")
@CrossOrigin(origins = "http://localhost:6869")
public class DraftController {

    private final DraftService draftService;

    @Autowired
    public DraftController(DraftService draftService) {
        this.draftService = draftService;
    }

    @PostMapping("/")
    public ResponseEntity<String> saveDraft(@RequestBody Draft draft) {
        if (draft == null) {
            return new ResponseEntity<>(HttpStatus.BAD_REQUEST);
        }
        draftService.saveDraft(draft);
        return new ResponseEntity<>("Draft saved successfully", HttpStatus.CREATED);
    }

    @GetMapping("/getDrafts/{userID}")
    public ResponseEntity<List<Draft>> getAllDrafts(@PathVariable int userID) {
        if (userID <= 0) {
            return new ResponseEntity<>(HttpStatus.BAD_REQUEST);
        }
        List<Draft> drafts = draftService.getAllDraft(userID);
        return drafts.isEmpty() ? new ResponseEntity<>(HttpStatus.NO_CONTENT) : new ResponseEntity<>(drafts, HttpStatus.OK);
    }

    @PutMapping("/")
    public ResponseEntity<String> updateDraft(@RequestBody Draft draft) {
        if (draft == null) {
            return new ResponseEntity<>(HttpStatus.BAD_REQUEST);
        }
        draftService.updateDraft(draft.getContent(), draft.getId());
        return new ResponseEntity<>("Draft updated successfully", HttpStatus.OK);
    }

    @DeleteMapping("/{draftId}")
    public ResponseEntity<String> deleteDraft(@PathVariable int draftId) {
        if (draftId <= 0) {
            return new ResponseEntity<>(HttpStatus.BAD_REQUEST);
        }
        draftService.deleteDraft(draftId);
        return new ResponseEntity<>("Draft deleted successfully", HttpStatus.NO_CONTENT);
    }
}
