package com.example.draftDemo.service;

import com.example.draftDemo.mapper.DraftMapper;
import com.example.draftDemo.model.Draft;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class DraftService {
    private final DraftMapper draftMapper;

    @Autowired
    public DraftService(DraftMapper draftMapper) {
        this.draftMapper = draftMapper;
    }

    public void saveDraft(Draft draft) {
        draftMapper.saveDraft(draft);
    }

    public List<Draft> getAllDraft(int id) {
        return draftMapper.getAllDrafts(id);
    }

    public void deleteDraft(int id) {
        draftMapper.deleteDraft(id);
    }

    public void updateDraft(String content, int id) {
        draftMapper.updateDraft(content, id);
    }

    // 其他草稿相关方法...
}
