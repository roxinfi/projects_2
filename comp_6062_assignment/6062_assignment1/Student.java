

package com.v_patel233507.assignment1;

public class Student {
    private String fullName;
    private String username;
    private String email;
    private long studentNumber;  // use long for numeric student IDs

    // Constructor
    public Student(String fullName, String username, String email, long studentNumber) {
        this.fullName = fullName;
        this.username = username;
        this.email = email;
        this.studentNumber = studentNumber;
    }

    // Getters
    public String getFullName() {
        return fullName;
    }

    public String getUsername() {
        return username;
    }

    public String getEmail() {
        return email;
    }

    public long getStudentNumber() {
        return studentNumber;
    }

    // Setter for fullName
    public void setFullName(String fullName) {
        this.fullName = fullName;
    }
}
