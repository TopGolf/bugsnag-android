package com.bugsnag.android

import android.content.Context
import android.content.SharedPreferences
import androidx.test.core.app.ApplicationProvider
import org.junit.After
import org.junit.Assert.assertEquals
import org.junit.Assert.assertNull
import org.junit.Before
import org.junit.Test
import java.io.File

internal class UserStoreTest {

    lateinit var file: File
    lateinit var storageDir: File
    lateinit var prefMigrator: SharedPrefMigrator
    lateinit var prefs: SharedPreferences

    @Before
    fun setUp() {
        val ctx = ApplicationProvider.getApplicationContext<Context>()
        storageDir = ctx.cacheDir
        file = File(storageDir, "user.json")
        file.delete()
        prefMigrator = SharedPrefMigrator(ctx)
        prefs = ctx.getSharedPreferences("com.bugsnag.android", Context.MODE_PRIVATE)
    }

    @After
    fun tearDown() {
        storageDir.delete()
        prefs.edit().clear().commit()
    }

    @Test
    fun sharedPrefMigration() {
        prefs.edit()
            .putString("user.id", "jf123")
            .putString("user.email", "test@example.com")
            .putString("user.name", "Jane Fonda")
            .commit()

        val store = UserStore(prefMigrator, prefs, true, "0asdf", file, NoopLogger)
        val user = store.load()
        assertEquals("jf123", user.id)
        assertEquals("Jane Fonda", user.name)
        assertEquals("test@example.com", user.email)
    }

    /**
     * A file should be created if it does not already exist
     */
    @Test
    fun nonExistentFile() {
        val nonExistentFile = File(storageDir, "foo")
        nonExistentFile.delete()
        val store = UserStore(prefMigrator, prefs, true, "device-id", file, NoopLogger)
        val user = store.load()
        assertEquals("device-id", user.id)
        assertNull(user.email)
        assertNull(user.name)
    }

    /**
     * An empty file should return the default user
     */
    @Test
    fun emptyFile() {
        val store = UserStore(prefMigrator, prefs, true, "device-id", file, NoopLogger)
        val user = store.load()
        assertEquals("device-id", user.id)
        assertNull(user.email)
        assertNull(user.name)
    }

    /**
     * A file of the correct length with invalid contents should return the default user
     */
    @Test
    fun invalidFileContents() {
        file.writeText("{\"hamster\": 2}")
        val store = UserStore(prefMigrator, prefs, true, "device-id", file, NoopLogger)
        val user = store.load()
        assertEquals("device-id", user.id)
        assertNull(user.email)
        assertNull(user.name)
    }

    /**
     * A non-writable file does not crash the app
     */
    @Test
    fun nonWritableFile() {
        val nonReadableFile = File(storageDir, "foo").apply {
            delete()
            createNewFile()
            setWritable(false)
        }
        val store = UserStore(prefMigrator, prefs, true, "device-id", nonReadableFile, NoopLogger)
        val user = store.load()
        assertEquals("device-id", user.id)
        assertNull(user.email)
        assertNull(user.name)
    }

    /**
     * A valid file with persisted user information should be readable
     */
    @Test
    fun validFileContents() {
        file.writeText("{\"id\":\"jf123\",\"email\":\"test@example.com\",\"name\":\"Jane Fonda\"}")

        val store = UserStore(prefMigrator, prefs, true, "0asdf", file, NoopLogger)
        val user = store.load()
        assertEquals("jf123", user.id)
        assertEquals("Jane Fonda", user.name)
        assertEquals("test@example.com", user.email)
    }

    /**
     * If persistUser is false a user is still returned
     */
    @Test
    fun loadWithoutPersistUser() {
        val store = UserStore(prefMigrator, prefs, false, "device-id-123", file, NoopLogger)
        val user = store.load()
        assertEquals("device-id-123", user.id)
        assertNull(user.email)
        assertNull(user.name)
        assertEquals("", file.readText())
    }

    /**
     * If persistUser is false a user is not saved
     */
    @Test
    fun saveWithoutPersistUser() {
        val store = UserStore(prefMigrator, prefs, false, "", file, NoopLogger)
        store.save(User("123", "joe@yahoo.com", "Joe Bloggs"))
        assertEquals("", file.readText())
    }

    /**
     * Saving user when persistUser is true
     */
    @Test
    fun saveWithPersistUser() {
        val store = UserStore(prefMigrator, prefs, true, "0asdf", file, NoopLogger)
        val user = User("jf123", "test@example.com", "Jane Fonda")
        store.save(user)
        val expected = "{\"id\":\"jf123\",\"email\":\"test@example.com\",\"name\":\"Jane Fonda\"}"
        assertEquals(expected, file.readText())
    }

    /**
     * The user must have been changed in order for disk IO to occur when persisting.
     */
    @Test
    fun userRequiresChangeForDiskIO() {
        TODO()
    }
}
