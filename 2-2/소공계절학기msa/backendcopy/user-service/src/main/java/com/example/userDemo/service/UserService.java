package com.example.userDemo.service;



import com.example.userDemo.mapper.UserMapper;
import com.example.userDemo.model.UserAccount;
import com.example.userDemo.model.UserInfo;

import org.springframework.stereotype.Service;
import org.springframework.beans.factory.annotation.Autowired;

@Service
public class UserService {
    private final UserMapper userMapper;

    @Autowired
    public UserService(UserMapper userMapper) {
        this.userMapper = userMapper;
    }

    public UserAccount getUserById(Long id) {
        return userMapper.getUserById(id);
    }

    public UserInfo getUserInfoById(int userId) {
        return userMapper.selectUserInfoById(userId);
    }

    // 其他用户相关方法...
}
