package com.example.documentDemo.service;

import com.example.documentDemo.mapper.FavorDocMapper;
import com.example.documentDemo.model.FavorDoc;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class FavorDocService {

    @Autowired
    private FavorDocMapper favorDocMapper;

    public List<FavorDoc> getFavorDocs_UT(int userId) {
        return favorDocMapper.getFavorDocs_UT(userId);
    }

    public List<FavorDoc> getFavorDocs_FT(int userId) {
        return favorDocMapper.getFavorDocs_FT(userId);
    }

    public void deleteFavorDoc(int userId, int documentId) {
        favorDocMapper.deleteFavorDoc(userId, documentId);
    }
}
