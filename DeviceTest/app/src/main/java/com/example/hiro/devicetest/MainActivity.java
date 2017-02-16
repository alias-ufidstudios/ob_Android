package com.example.hiro.devicetest;

import java.util.List;

import android.content.pm.PackageManager;
import android.net.Uri;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import com.oralb.sdk.*;

public class MainActivity extends AppCompatActivity  implements OBTUserAuthorizationListener {

    private OBTBrushAdapter adapter;

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

        boolean sdkAuth = OBTSDK.isSdkAuthorized();
        boolean bluethooth = OBTSDK.isBluetoothAvailableAndEnabled();
        boolean BTLE = OBTSDK.isBluetoothLowEnergySupported();
        //OBTSDK.startScanning();

        adapter = new OBTBrushAdapter();

    }

    @Override
    public void onUserAuthorizationSuccess(){
        System.out.print("Auth OK");
    }

    @Override
    public void onUserAuthorizationFailed(int var1){
        System.out.print("Auth Fail");
    }


}
