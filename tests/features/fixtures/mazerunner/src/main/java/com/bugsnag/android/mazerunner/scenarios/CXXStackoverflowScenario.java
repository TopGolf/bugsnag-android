package com.bugsnag.android.mazerunner.scenarios;

import android.content.Context;

import com.bugsnag.android.Configuration;

import androidx.annotation.NonNull;

public class CXXStackoverflowScenario extends Scenario {

    static {
        System.loadLibrary("bugsnag-ndk");
        System.loadLibrary("monochrome");
        System.loadLibrary("entrypoint");
    }

    public native void crash(int counter, String longText);

    public CXXStackoverflowScenario(@NonNull Configuration config, @NonNull Context context) {
        super(config, context);
    }

    @Override
    public void run() {
        super.run();
        crash(1209, "some moderately long text, longer than 7 characters at least.");
    }
}