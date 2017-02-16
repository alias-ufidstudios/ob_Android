package com.example.hiro.devicetest;

import android.os.Bundle;
import android.app.Activity;
import com.oralb.sdk.*;


public class AuthActivity extends Activity implements OBTSdkAuthorizationListener {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        try {
            OBTSDK.authorizeSdk(this);
        } catch (IllegalStateException e) {
            //App ID or App Key not set. Have you forgot to call initialize first or to add them to the manifest? Please see documentation for more details.
        }
    }

    @Override
    public void onSdkAuthorizationSuccess() {
        //We are now allowed to use bluetooth connectivity features
        System.out.println("onSdkAuthorizationSuccess()");
    }

    @Override
    public void onSdkAuthorizationFailed(int failureCodes) {
        System.out.println("onSdkAuthorizationFailed");
    }

}
