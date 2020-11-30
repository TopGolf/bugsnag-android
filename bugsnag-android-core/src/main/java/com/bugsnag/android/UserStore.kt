package com.bugsnag.android

import android.content.SharedPreferences
import java.io.File
import java.io.IOException

/**
 * This class is responsible for persisting and retrieving user information.
 */
internal class UserStore(
    private val prefMigrator: SharedPrefMigrator,
    private val prefs: SharedPreferences,
    private val persist: Boolean,
    private val deviceId: String?,
    private val file: File,
    logger: Logger
) {

    private val synchronizedStreamableStore: SynchronizedStreamableStore<User>

    init {
        try {
            if (!file.exists()) {
                file.createNewFile()
            }
        } catch (exc: IOException) {
            logger.w("Failed to created device ID file", exc)
        }
        this.synchronizedStreamableStore = SynchronizedStreamableStore(file)
    }

    fun load(): User {
        return when {
            persist -> loadPersistedUser()
            else -> User(deviceId, null, null)
        }
    }

    private fun loadPersistedUser(): User {

        // TODO handle IO errors
        return synchronizedStreamableStore.load(User.Companion::fromReader)

        // TODO migrate from sharedPrefs too
//        val legacyUser = prefMigrator.loadUser(deviceId) // TODO check if file exists first?
//        return legacyUser
    }

    fun save(user: User) {
        // TODO should check whether user has changed between last save before attempting IO


        if (persist) {
            // TODO handle IO exception
            synchronizedStreamableStore.persist(user)
        } else {
            val editor = prefs.edit()
            editor
                .remove(USER_ID_KEY)
                .remove(USER_NAME_KEY)
                .remove(USER_EMAIL_KEY)
            editor.apply()
        }
    }

    companion object {
        const val USER_ID_KEY = "user.id"
        const val USER_NAME_KEY = "user.name"
        const val USER_EMAIL_KEY = "user.email"
    }
}
