package com.example.userDemo.mapper;

import com.example.userDemo.model.*;
import org.apache.ibatis.annotations.*;

@Mapper
public interface UserMapper {
    @Select("SELECT * FROM UserAccount WHERE userID=#{id}")
    UserAccount getUserById(Long id);

    @Select("SELECT userName, Email, creatDate FROM UserAccount WHERE userID = #{userId}")
    UserInfo selectUserInfoById(int userId);
}