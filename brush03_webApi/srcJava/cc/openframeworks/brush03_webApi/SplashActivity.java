package cc.openframeworks.brush03_webApi;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;

public class SplashActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        // Start home activity
        startActivity(new Intent(SplashActivity.this, OFActivity.class));

        // close splash activity
        finish();
    }
}
