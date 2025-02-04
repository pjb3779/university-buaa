package com.example.chapterDemo.service;

import com.example.chapterDemo.mapper.ChapterMapper;
import com.example.chapterDemo.model.Chapter;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class ChapterService {

    private final ChapterMapper chapterMapper;

    @Autowired
    public ChapterService(ChapterMapper chapterMapper) {
        this.chapterMapper = chapterMapper;
    }

    public void publishChapter(Chapter chapter) {
        chapterMapper.publishChapter(chapter);
    }

    public List<Chapter> getChapters(int DocumentID) {
        return chapterMapper.getChapters(DocumentID);
    }

        public Chapter getChapterById(int DocumentID, int chapterNum) {
            return chapterMapper.getChapterById(DocumentID, chapterNum);
        }

        public void deleteChapter(int id) {
            chapterMapper.deleteChapter(id);
        }



}

