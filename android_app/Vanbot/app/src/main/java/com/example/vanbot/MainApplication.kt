package com.example.vanbot

import android.app.Application
import android.content.Context

// Not object class. AndroidManifest.xml error happen.
class MainApplication : Application() {

    init {
        instance = this
    }

    companion object {
        private var instance: MainApplication? = null

        fun applicationContext() : Context {
            return instance!!.applicationContext
        }
    }

    override fun onCreate() {
        super.onCreate()
        // initialize for any

        // Use ApplicationContext.
        // example: SharedPreferences etc...
        //val context: Context = MainApplication.applicationContext()
    }
}
