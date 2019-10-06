package com.example.vanbot

import java.lang.StringBuilder
import java.util.*
import kotlin.collections.ArrayList


public class SolarDataParam (val name: String, val abbrev: String, val min: Double?, val max: Double?, var value: Number?) {}

public class SolarData(inputStr: String?)
{
    var initialized = false

    public val params = listOf<SolarDataParam>(
        SolarDataParam("MPPT Power", "MP", 0.0, 200.0, 0),
        SolarDataParam("Battery Voltage", "BV", 10.0, 15.0, 0) ,
        SolarDataParam("DCDC Duty", "DD", 0.0, 100.0,0),
        SolarDataParam("Battery Current", "BC", -10.0, 10.0, 0),
        SolarDataParam("Solar Voltage", "SV", 0.0, 40.0, 0),
        SolarDataParam("Solar Current", "SC", -10.0, 10.0,0 ),
        SolarDataParam("Load Current", "LC", -10.0, 10.0, 0),
        SolarDataParam("DCDC Enable", "DE", 0.0, 1.0,0 ),
        SolarDataParam("MPPT Enable", "ME", 0.0, 1.0, 0),
        SolarDataParam("MPPT Deadtime", "MD", 0.0, 1000.0, 0),
        SolarDataParam("MPPT Voltage", "MV", 0.0, 40.0,0),
        SolarDataParam("MPPT Direction", "MR", 0.0, 1.0, 0),
        SolarDataParam("MPPT State", "MS", 0.0, 2.0, 0),
        SolarDataParam("Load Enable", "LE", 0.0, 1.0,0 ),
        SolarDataParam("Fan Speed", "FS", 0.0, 255.0, 0),
        SolarDataParam("Error Counter", "EC", 0.0, 255.0, 0)
    )

    val time = Calendar.getInstance().time
    init{
        if (inputStr != null)
        {
            parseString(inputStr)
        }
    }

    public fun getParamList(): ArrayList<String>{
        var a = ArrayList<String>()
        for (p in params)
        {
            a.add(p.name)
        }
        return a
    }

    private fun parseString(inputStr: String) {
        val regex = "([a-zA-Z]+)([0-9\\-\\.]+)".toRegex()
        val matchResult = regex.findAll(inputStr)

        for (m in matchResult) {
            for (p in params) {
                if (p.abbrev == m.groups[1]?.value) {
                    p.value = m.groups[2]?.value?.toFloat()
                }
            }
        }

        initialized = true
    }

    public fun isInitialized(): Boolean {
        return initialized
    }

    override fun toString(): String
    {
        var sb = StringBuilder()
        for (p in params)
        {
            sb.append(p.name)
            sb.append(": ")
            sb.append(p.value)
            sb.append("\n")
        }
        return sb.toString()
    }

}

