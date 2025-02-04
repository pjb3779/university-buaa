package com.example.documentDemo.service;

import com.example.documentDemo.mapper.DocumentMapper;
import org.springframework.beans.factory.annotation.Autowired;
import com.example.documentDemo.model.Document;
import org.springframework.stereotype.Service;

import java.util.*;

@Service
public class DocumentService {
    private final DocumentMapper documentMapper;

    @Autowired
    public DocumentService(DocumentMapper documentMapper) {
        this.documentMapper = documentMapper;
    }

    public int publishDoc(Document doc) {
        return documentMapper.publishDoc(doc);
    }

    public List<Document> getAllDocuments(int userID) {
        return documentMapper.getAllDocuments(userID);
    }

    public void deleteDoc(int id) {
        documentMapper.deleteDoc(id);
    }

    public int docCount(int id) {
        return documentMapper.docCount(id);
    }

    public int docPopularity(int id) {
        return documentMapper.docPopularity(id);
    }

    public Document getDocumentById(Long id) {
        return documentMapper.getDocumentById(id);
    }

    public void updateChapterNum(int chapterNum, int documentID) {
        documentMapper.updateChapterNum(chapterNum, documentID);
    }
    // 其他文档相关方法...
}
