package com.bugsnag.android.mazerunner.scenarios

import android.content.Context
import com.bugsnag.android.Configuration

/**
 * Attempts to send an ignored handled exception to Bugsnag, which should not result
 * in any operation.
 */
internal class IgnoredExceptionScenario(
    config: Configuration,
    context: Context,
    eventMetadata: String
) : Scenario(config, context, eventMetadata) {

    init {
        config.autoTrackSessions = false
        config.discardClasses = setOf("java.lang.RuntimeException")
    }

    override fun startScenario() {
        super.startScenario()
        throw RuntimeException("Should never appear")
    }
}
