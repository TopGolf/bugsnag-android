package com.bugsnag.android.mazerunner.scenarios;

import com.bugsnag.android.Configuration;

import android.content.Context;

import androidx.annotation.NonNull;

public class AutoDetectNdkEnabledScenario extends Scenario {

    static {
        System.loadLibrary("cxx-scenarios");
    }

    public native void crash();

    /**
     */
    public AutoDetectNdkEnabledScenario(@NonNull Configuration config, @NonNull Context context) {
        super(config, context);
        config.setAutoTrackSessions(false);
        config.getEnabledErrorTypes().setNdkCrashes(true);
    }

    @Override
    public void run() {
        super.run();
        String metadata = getEventMetaData();
        if (metadata != null && metadata.equals("non-crashy")) {
            return;
        }
        crash();
    }
}
