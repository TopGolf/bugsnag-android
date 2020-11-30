package com.bugsnag.android

internal class UserState(private val userStore: UserStore) : BaseObservable() {

    var user = userStore.load() // FIXME should be other way around
        private set

    fun setUser(id: String?, email: String?, name: String?) {
        user = User(id, email, name)
        userStore.save(user)
        emitObservableEvent()
    }

    fun emitObservableEvent() = notifyObservers(StateEvent.UpdateUser(user))

}
