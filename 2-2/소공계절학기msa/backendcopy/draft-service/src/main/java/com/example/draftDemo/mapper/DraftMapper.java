package com.example.draftDemo.mapper;

import org.apache.ibatis.annotations.*;
import com.example.draftDemo.model.*;
import java.util.List;

@Mapper
public interface DraftMapper {
    @Insert("INSERT INTO Draft(Title,Content,UserID,currentModityTime) VALUES (#{title},#{content},#{userID},NOW())")
    void saveDraft(Draft draft);

    @Select("SELECT * FROM Draft WHERE UserID=${userID}")
    List<Draft> getAllDrafts(int userID);

    @Delete("DELETE FROM Draft WHERE id=${id}")
    void deleteDraft(int id);

    @Update("UPDATE Draft SET content = #{content} WHERE id = #{id}")
    void updateDraft(String content, int id);

}
