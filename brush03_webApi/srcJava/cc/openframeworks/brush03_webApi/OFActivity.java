package cc.openframeworks.brush03_webApi;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.util.Log;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import cc.openframeworks.OFAndroid;


public class OFActivity extends cc.openframeworks.OFActivity{

    @Override
    public void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);

//        setContentView(R.layout.main_layout);
//        //レイアウトで指定したWebViewのIDを指定する。
//        WebView  myWebView = (WebView)findViewById(R.id.webView1);
//
//        //リンクをタップしたときに標準ブラウザを起動させない
//        myWebView.setWebViewClient(new WebViewClient());
//
//        //最初にgoogleのページを表示する。
//        myWebView.loadUrl("http://www.google.co.jp");
//
//        //jacascriptを許可する
//        myWebView.getSettings().setJavaScriptEnabled(true);

//        Intent appLinkIntent = getIntent();
//        String appLinkAction = appLinkIntent.getAction();
//
//        if (appLinkAction == "android.intent.action.MAIN") {
//
//        } else if (appLinkAction == "android.intent.action.VIEW"){
//            Uri appLinkData = appLinkIntent.getData();
//            String host = appLinkData.getHost();
//            String scheme = appLinkData.getScheme();
//            redirectFromWebAuth(host + scheme);
//        }
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

    public native static void redirectFromWebAuth(String userToken);


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

}

