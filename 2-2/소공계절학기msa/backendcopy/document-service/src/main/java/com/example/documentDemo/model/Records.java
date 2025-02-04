package com.example.documentDemo.model;

public class Records {
    private int documentID;
    private String title;
    private String tags;
    private byte[] cover;
    private int userID;

    public int getUserID() {
        return userID;
    }
    public void setUserID(int userID) {
        this.userID = userID;
    }

    public int getDocumentID() {
        return documentID;
    }

    public void setDocumentID(int documentId) {
        this.documentID = documentId;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getTags() {
        return tags;
    }

    public void setTags(String tags) {
        this.tags = tags;
    }

    public byte[] getCover() {
        return cover;
    }

    public void setCover(byte[] cover) {
        this.cover = cover;
    }
}
