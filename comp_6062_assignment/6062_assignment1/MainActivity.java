// --------------------->MainActivity.java<------------------------
// Vraj Patel
// COMP - 6057 Assignment 1
// Date: 2025-05-24

package com.v_patel233507.assignment1;

import android.os.Bundle;
import android.util.Log;
import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity 
{
    private static final String TAG = "Student";

    @Override
    protected void onCreate(Bundle savedInstanceState) 
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // 3.1.1 Create a new Student object with your info
        Student student1 = new Student
        (
            "Vraj Patel",             // fullName
            "vrajpatel233507",              // username
            "v_patel233507@fanshaweonline.ca", // email
            1243437                 // studentNumber
        );

        // 3.1.2 Log each private variable
        Log.i(TAG, "Full Name: " + student1.getFullName());
        Log.i(TAG, "Username: " + student1.getUsername());
        Log.i(TAG, "Email: " + student1.getEmail());
        Log.i(TAG, "Student Number: " + student1.getStudentNumber());

        // 3.1.3 Change the name
        student1.setFullName("Sahil A. Patel");

        // 3.1.4 Log the updated name
        Log.i(TAG, "Updated Full Name: " + student1.getFullName());
    }
}
