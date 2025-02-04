package com.example.messageDemo.service;

import com.example.messageDemo.mapper.MessageMapper;
import com.example.messageDemo.model.Messages;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class MessageService {

    @Autowired
    private MessageMapper messagesMapper;

    public List<Messages> getMessages(int userId) {
        return messagesMapper.getMessages(userId);
    }

    public void deleteMessages(int userId, int documentId) {
        messagesMapper.deleteMessages(userId, documentId);
    }

    public void sendMessage(Messages message) {
        messagesMapper.sendMessage(message);
    }
}
