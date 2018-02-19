package cc.openframeworks.brush03_webApi;
import android.app.AlertDialog;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.net.Uri;
import android.opengl.GLES20;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.util.Log;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import cc.openframeworks.OFAndroid;
import cc.openframeworks.OFAndroidLifeCycle;

import com.facebook.CallbackManager;
import com.facebook.FacebookSdk;
import com.facebook.share.model.SharePhoto;
import com.facebook.share.model.SharePhotoContent;
import com.facebook.share.widget.ShareDialog;
import android.graphics.Bitmap;

import java.nio.IntBuffer;

public class OFActivity extends cc.openframeworks.OFActivity{

    private CallbackManager callbackManager;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    public void onDetachedFromWindow() {
    }

    // Menus
    // http://developer.android.com/guide/topics/ui/menus.html
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Create settings menu options from here, one by one or infalting an xml
        return super.onCreateOptionsMenu(menu);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // This passes the menu option string to OF
        // you can add additional behavior from java modifying this method
        // but keep the call to OFAndroid so OF is notified of menu events
        if(OFAndroid.menuItemSelected(item.getItemId())){

            return true;
        }
        return super.onOptionsItemSelected(item);
    }


    @Override
    public boolean onPrepareOptionsMenu (Menu menu){
        // This method is called every time the menu is opened
        //  you can add or remove menu options from here
        return  super.onPrepareOptionsMenu(menu);
    }


    @Override
    public void onResume() {
        super.onResume();
    }


    ///////////////////////////////////////////////////////////////
    // ofxAndroidWebView methods - START
    ///////////////////////////////////////////////////////////////

    WebView wb;
    private class HelloWebViewClient extends WebViewClient {
        @Override
        public boolean shouldOverrideUrlLoading(WebView view, String url) {

            if (url.startsWith("http://dominofactory.net/obTest")) {
                wb.stopLoading();
                //Intent intent = new Intent(Intent.ACTION_MAIN, Uri.parse(url));
                //view.getContext().startActivity(intent);
                hideWebView();
                redirectFromWebAuth(url);
                return true;

            }
            return super.shouldOverrideUrlLoading(view, url);
        }
    }

    public void showWebView(final String url) {

        Log.i("OF", "Loading url: " + url);

        this.runOnUiThread(new Runnable() {
            public void run() {
                wb=(WebView)findViewById(R.id.webView1);
                wb.getSettings().setJavaScriptEnabled(true);
                wb.getSettings().setLoadWithOverviewMode(true);
                wb.getSettings().setUseWideViewPort(true);
                wb.getSettings().setBuiltInZoomControls(false);
                wb.getSettings().setPluginState(WebSettings.PluginState.ON);
                wb.setWebViewClient(new HelloWebViewClient());
                wb.loadUrl(url);
                wb.setVisibility(View.VISIBLE);
            }
        });

    }

    public void hideWebView() {

        this.runOnUiThread(new Runnable() {
            public void run() {
                wb=(WebView)findViewById(R.id.webView1);
                wb.setVisibility(View.GONE);
            }
        });

    }

    @Override
    public void onBackPressed() {
        hideWebView();
    }

    ///////////////////////////////////////////////////////////////
    // ofxAndroidWebView methods - END
    ///////////////////////////////////////////////////////////////

    private boolean isAppInstalled(String targetPackage){
        PackageManager pm = this.getPackageManager();
        try {
            PackageInfo info = pm.getPackageInfo( targetPackage, PackageManager.GET_META_DATA);
        } catch (PackageManager.NameNotFoundException e) {
            return false;
        }
        return true;
    }

    public void initBuffer(int w, int h, int ch){

        imgWidth = w;
        imgHeight = h;
        imgCh = ch;
        int bufferSize = w * h * ch;
        buffer = new byte[bufferSize];
        for(int i=0; i<h; i++) {
            for(int j=0; j<w; j++) {
                int id = j+i*w;
                buffer[id*4 + 0] = (byte)255;  // A
                buffer[id*4 + 1] = (byte)123;  // R
                buffer[id*4 + 2] = (byte)56;   // G
                buffer[id*4 + 3] = (byte)78;   // B
            }
        }
    }

    public void startShare(){
        fillBuffer(buffer);
    }

    private Bitmap createTestBitmap(int w, int h){
        Bitmap bitmap = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);

        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                if (i % 2 == 0) bitmap.setPixel(j, i, 0xFFFF0000);
                else bitmap.setPixel(j, i, 0xFF0000FF);
            }
        }
        return bitmap;
    }

    private Bitmap createBitmapFromBuffer(int w, int h, byte[] buffer){
        Bitmap bitmap = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);

        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                int id = (j+i*w) * 4;
                int a = (buffer[id+0]+125) & 0xFF;
                int r = (buffer[id+1]+125) & 0xFF;
                int g = (buffer[id+2]+125) & 0xFF;
                int b = (buffer[id+3]+125) & 0xFF;
                int color = Color.argb(a,r,g,b);
                bitmap.setPixel(j, i, color);
            }
        }
        return bitmap;
    }

    public void shareImageFacebook(){

        boolean hasFacebookApp = isAppInstalled("com.facebook.katana");
        if(hasFacebookApp){

            Bitmap bitmap = createBitmapFromBuffer(imgWidth, imgHeight/3, buffer);

            SharePhoto photo = new SharePhoto.Builder().setBitmap(bitmap).setCaption("Test Photo from Android yay").build();
            SharePhotoContent content = new SharePhotoContent.Builder().addPhoto(photo).build();
            ShareDialog.show(this, content);
        }else{
            // TODO
            // uninstall fb app and test on device
            Log.w("shareImageFacebook()", "Can not find facebook app");

            // show dialog
            String message = "Please install Facebook Official App to share your image(or you can save an image)";
            OFAndroid.alertBox(message);
        }
    }

    private byte[] buffer = null;
    private int imgWidth = -123;
    private int imgHeight = -123;
    private int imgCh = -123;

    public static native void redirectFromWebAuth(String userToken);
    public static native void fillBuffer(byte[] data);

}

