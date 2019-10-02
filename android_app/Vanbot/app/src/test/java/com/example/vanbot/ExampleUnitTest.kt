package com.example.vanbot

import org.junit.Test

import org.junit.Assert.*

/**
 * Example local unit test, which will execute on the development machine (host).
 *
 * See [testing documentation](http://d.android.com/tools/testing).
 */
class ExampleUnitTest {
    @Test
    fun addition_isCorrect() {
        assertEquals(4, 2 + 2)
    }

    @Test
    fun solardata_parser_isCorrect()
    {
        var sd = SolarData("SV12.356BV12.668SC-0.001BC0.759LC0.791DD0DE0MD0MV0.000MR0MP0.000ME0LE1FE0")
        for (p in sd.params){
            print(p.abbrev + ": "+ p.value)
        }

    }
}
