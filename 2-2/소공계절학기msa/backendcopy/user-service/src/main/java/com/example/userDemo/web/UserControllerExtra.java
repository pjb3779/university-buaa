package com.example.userDemo.web;


import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.autoconfigure.EnableAutoConfiguration;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.mail.SimpleMailMessage;
import org.springframework.mail.javamail.JavaMailSenderImpl;
import org.springframework.web.bind.annotation.*;
import com.example.userDemo.model.*;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;


@RestController
@RequestMapping("/users")
@EnableAutoConfiguration
public class UserControllerExtra {
    @Autowired
    private JdbcTemplate jdbcTemplate;
    @Autowired
    private JavaMailSenderImpl sender;

    @RequestMapping("/checkLogin/{username}/{password}")
    public String checkLogin(@PathVariable("username") String username,@PathVariable("password") String password){
        String sql= """
                SELECT COUNT(*) as num
                FROM UserAccount
                WHERE userName=? AND passWords=?
                """;
        List<Map<String, Object>> list =  jdbcTemplate.queryForList(sql,username+"",password+"");
        Long num = (Long) getNum(list);
        if(num == 0)return "No such user account !";
        return "well";
    }

    @RequestMapping("/getLoginMsg/{username}/{password}")
    public List<Map<String, Object>> getUser_Login(@PathVariable("username") String username,@PathVariable("password") String password){
        String sql = """
                SELECT * FROM UserAccount
                WHERE userName=? and passWords=?
                """;
        //加上“”是为了让ide的参数列表提示显示出来
        return jdbcTemplate.queryForList(sql,username+"",password+"");
    }

    @RequestMapping("/getUserByEmail/{email}")
    public List<Map<String, Object>> getUserByEmail(@PathVariable("email") String email){
        String sql = """
                SELECT * FROM UserAccount
                WHERE Email=?
                """;
        //加上“”是为了让ide的参数列表提示显示出来
        List<Map<String, Object>> list =  jdbcTemplate.queryForList(sql,email+"");
        return list;
    }

    @RequestMapping("/updatePassword/{email}/{password}")
    public String updatePassword(@PathVariable("email") String email,@PathVariable("password") String password){
        String sql = """
                UPDATE  UserAccount 
                SET passWords= ?
                WHERE Email=?
                """;
        //加上“”是为了让ide的参数列表提示显示出来
        jdbcTemplate.update(sql,password,email+"");
        return "well";
    }

    private Object getNum(List<Map<String, Object>> list) {
        if (list.isEmpty()) {
            return 0; // 如果列表为空，返回0
        }
        Set<Map.Entry<String, Object>> entryies = list.get(0).entrySet();

        Object num = null;
        for (Map.Entry<String, Object> entry : entryies) {
            Object value = entry.getValue();
            if(value instanceof Long){
                num = (Long) value;
            }else {
                num = (Integer) value;
            }
        }
        return num;
    }


    @RequestMapping("/checkEmail/{email}")
    public String checkEmailValid(@PathVariable("email") String email){
        // 生成令牌并发送验证邮件
        Random random = new Random();
        int number = random.nextInt(10000); // 生成 0 到 9999 之间的随机数
        String key = String.format("%04d", number); // 格式化为四位数
        SimpleMailMessage message = new SimpleMailMessage();
        message.setFrom("xuexia.xzp@qq.com");
        message.setTo(email);
        message.setText("Below is your key:\n\n"
                +  key);
        sender.send(message);
        return key;
    }


    @RequestMapping("/checkUsername/{username}")
    public String checkRepeatName(@PathVariable("username") String name){
        String sql= """
                SELECT COUNT(*) as num
                FROM UserAccount
                WHERE userName = ?
                """;
        List<Map<String, Object>> list =  jdbcTemplate.queryForList(sql, name+"");
        Long num = (Long) getNum(list);
        System.out.println(num+" "+name);
        if(num != 0)return "repeat!";
        return "well";
    }

    @RequestMapping("/checkEmailUsed/{email}")
    public String checkEmailUsed(@PathVariable("email") String email){
        String sql= """
                SELECT COUNT(*) as num
                FROM UserAccount
                WHERE Email = ?
                """;
        List<Map<String, Object>> list =  jdbcTemplate.queryForList(sql, email+"");
        Long num = ((Number) getNum(list)).longValue(); // 修改这里
        if(num != 0)return "repeat!";
        return "well";
    }

    @RequestMapping("/userInsert")
    public String createUser(@RequestBody User formRequest){
        String sql1= """
                SELECT MAX(userID) as maxid
                FROM UserAccount
                """;
        List<Map<String, Object>> list =  jdbcTemplate.queryForList(sql1);
        Integer maxId = (Integer) getNum(list);
        String sql = """
                INSERT INTO 
                UserAccount(userID, userName, passWords, Email, creatDate, salt) 
                VALUES (?,?,?,?,date(NOW()),?)
                """;
        jdbcTemplate.update(sql,maxId+1,formRequest.getUsername(),formRequest.getPassword(),
                formRequest.getEmail(),formRequest.getPassword());
        return "yes";
    }
    @RequestMapping("/getUsrByID/{id}")
    public List<Map<String, Object>> getUserById(@PathVariable("id") int id){
        String sql = """
                SELECT * FROM UserAccount
                WHERE userID=?
                """;
        //加上“”是为了让ide的参数列表提示显示出来
        List<Map<String, Object>> list =  jdbcTemplate.queryForList(sql,id);
        return list;
    }

    @RequestMapping("/testInsert/{id}")
    public String testInsert(@PathVariable Long id){
        String sql1= """
                SELECT MAX(DocumentID) as maxid
                FROM Document
                """;
        List<Map<String, Object>> list =  jdbcTemplate.queryForList(sql1);
        Integer maxId = (Integer) getNum(list);
        String sql = "INSERT INTO Document(DocumentID,Title,Tags,AuthorID,Summary,Popularity," +
                "chapterNum,Content,CurrentModifyTime,publishTime,CategoryID) VALUES (?, ?, ?, ?, ?, " +
                "0,1,?,now(),now(),1 )";

        String fileContent = "";
        try {
            byte[] fileBytes = Files.readAllBytes(Paths.get("src/content"+id+".txt"));
            fileContent = new String(fileBytes, StandardCharsets.UTF_8);
        } catch (IOException e) {
            e.printStackTrace();
            return "insert error!";
        }
        jdbcTemplate.update(sql, maxId+1+"", "testDoc"+(maxId+1),"test","1001","这是文件"+(maxId+1)+"的简介",fileContent);
        return "man!";
    }

}
