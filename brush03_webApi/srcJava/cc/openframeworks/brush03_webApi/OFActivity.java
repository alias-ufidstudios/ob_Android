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

    public String getExternalCacheDirJava(){
        return this.getExternalCacheDir().toString();
    }

    public void openShareIntent(String path){
        String p = "file://" + path;
        Uri uri = Uri.parse(p);
        Intent shareIntent = new Intent();
        shareIntent.setAction(Intent.ACTION_SEND);
        shareIntent.putExtra(Intent.EXTRA_STREAM, uri);
        shareIntent.setType("image/*");
        startActivity(Intent.createChooser(shareIntent, "Share Image"));
    }

    public static native void redirectFromWebAuth(String userToken);

}

