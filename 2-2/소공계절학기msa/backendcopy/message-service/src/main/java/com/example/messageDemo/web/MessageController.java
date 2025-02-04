package com.example.messageDemo.web;

import com.example.messageDemo.model.Messages;
import com.example.messageDemo.service.MessageService;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/messages")
public class MessageController {

    @Autowired
    private MessageService messagesService;

    @GetMapping("/select/{userId}")
    public List<Messages> getMessages(@PathVariable int userId) {
        return messagesService.getMessages(userId);
    }

    @DeleteMapping("/delete/{userId}/{messageId}")
    public void deleteMessages(@PathVariable int userId, @PathVariable int messageId) {
        messagesService.deleteMessages(userId, messageId);
    }

    @PostMapping("/send")
    public void sendMessage(@RequestBody Messages message) {
        messagesService.sendMessage(message);
    }
}
