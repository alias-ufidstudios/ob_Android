package com.example.hiro.devicetest;

import com.oralb.sdk.OBTBrush;
import com.oralb.sdk.OBTBrushListener;

import java.util.List;

public class OBTBrushAdapter implements OBTBrushListener {

    public OBTBrushAdapter(){

    }

    public void onNearbyBrushesFoundOrUpdated(List<OBTBrush> var1) {
        System.out.println("onNearbyBrushesFoundOrUpdated");
    }

    public void onBluetoothError() {
        System.out.println("onBluetoothError");
    }

    public void onBrushDisconnected() {
        System.out.println("onBrushDisconnected");
    }

    public void onBrushConnected() {
        System.out.println("onBrushConnected");
    }

    public void onBrushConnecting() {
        System.out.println("onBrushConnecting");
    }

    public void onBrushingTimeChanged(long var1) {
        System.out.println("onBrushingTimeChanged");
    }

    public void onBrushingModeChanged(int var1) {
        System.out.println("onBrushingModeChanged");
    }

    public void onBrushStateChanged(int var1) {
        System.out.println("onBrushStateChanged");
    }

    public void onRSSIChanged(int var1) {
        System.out.println("onRSSIChanged");
    }

    public void onBatteryLevelChanged(float var1) {
        System.out.println("onBatteryLevelChanged");
    }

    public void onSectorChanged(int var1) {
        System.out.println("onSectorChanged");
    }

    public void onHighPressureChanged(boolean var1) {
        System.out.println("onHighPressureChanged");
    }
}
