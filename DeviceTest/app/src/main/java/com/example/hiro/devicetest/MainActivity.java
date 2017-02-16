package com.example.hiro.devicetest;

import java.util.List;

import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import com.oralb.sdk.*;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        System.out.println("MainActivity onCreate");;

        super.onCreate(savedInstanceState);

        try {
            //Call to initialize the OBTSDK
            OBTSDK.initialize(this);
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }

        setContentView(R.layout.activity_main);

        Intent intent = new Intent(this, AuthActivity.class);
        startActivity(intent);


        
    }
}
