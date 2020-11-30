package com.bugsnag.android

import android.content.Context

/**
 * Reads legacy information left in SharedPreferences and migrates it to the new location.
 */
internal class SharedPrefMigrator(context: Context) {

    private val prefs = context
        .getSharedPreferences("com.bugsnag.android", Context.MODE_PRIVATE)

    fun loadDeviceId() = prefs.getString(INSTALL_ID_KEY, null)

    fun loadUser(deviceId: String?) = User(
        prefs.getString(UserStore.USER_ID_KEY, deviceId),
        prefs.getString(UserStore.USER_EMAIL_KEY, null),
        prefs.getString(UserStore.USER_NAME_KEY, null)
    )

    companion object {
        private const val INSTALL_ID_KEY = "install.iud"
    }
}
