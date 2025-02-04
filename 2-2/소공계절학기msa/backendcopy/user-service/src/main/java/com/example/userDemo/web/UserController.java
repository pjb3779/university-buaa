package com.example.userDemo.web;

import com.example.userDemo.model.UserAccount;
import com.example.userDemo.model.UserInfo;
import com.example.userDemo.service.UserService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.*;
import org.springframework.web.bind.annotation.*;


@RestController
@RequestMapping("/users")
@CrossOrigin(origins = "http://localhost:5173")
public class UserController {

    private final UserService userService;

    @Autowired
    public UserController(UserService userService) {
        this.userService = userService;
    }

    @GetMapping("/{userId}")
    public ResponseEntity<UserAccount> getUserById(@PathVariable Long userId) {
        UserAccount user = userService.getUserById(userId);
        if (user != null) {
            return new ResponseEntity<>(user, HttpStatus.OK);
        } else {
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }
    }

    @GetMapping("/info/{userId}")
    public UserInfo getUserInfo(@PathVariable int userId) {
        return userService.getUserInfoById(userId);
    }
}
