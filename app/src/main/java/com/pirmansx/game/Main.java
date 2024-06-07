package com.pirmansx.game;
import android.os.Bundle;
import android.os.PersistableBundle;

public class Main extends android.app.NativeActivity {
    static {
        
        System.loadLibrary("imgui");
        System.loadLibrary("game");
    }

    @Override
    public void onCreate(Bundle savedInstanceState, PersistableBundle persistentState) {
        super.onCreate(savedInstanceState, persistentState);
        
        
    }
}
