package cc.openframeworks.brush03_webApi;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import cc.openframeworks.OFAndroid;


public class OFActivity extends cc.openframeworks.OFActivity{

    @Override
    public void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);

        Intent appLinkIntent = getIntent();
        String appLinkAction = appLinkIntent.getAction();

        if (appLinkAction == "android.intent.action.MAIN") {

        } else if (appLinkAction == "android.intent.action.VIEW"){
            Uri appLinkData = appLinkIntent.getData();
            String host = appLinkData.getHost();
            String scheme = appLinkData.getScheme();
            redirectFromWebAuth(host + scheme);
        }
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

}

