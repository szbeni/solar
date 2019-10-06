package com.example.vanbot

import android.graphics.Color
import com.github.mikephil.charting.charts.LineChart
import com.github.mikephil.charting.components.YAxis.AxisDependency
import com.github.mikephil.charting.data.Entry
import com.github.mikephil.charting.data.LineData
import com.github.mikephil.charting.data.LineDataSet
import com.github.mikephil.charting.utils.ColorTemplate


class ChartHandler() {
    private var chart: LineChart? = null
    private var data = LineData()

    init {
        chart = MainActivity.getInstance()?.findViewById(R.id.chart1)

        data.setValueTextColor(Color.BLACK)
        data.setValueTextSize(9f)

        // set data
        chart?.data = data

    }

    private fun createSet(name: String): LineDataSet {
        val set = LineDataSet(null, name)
        set.axisDependency = AxisDependency.LEFT
        set.color = ColorTemplate.getHoloBlue()
        set.setDrawCircles(false)
        set.setCircleColor(set.color)
        set.lineWidth = 2f
        set.circleRadius = 0.0f
        set.fillAlpha = 65
        set.fillColor = ColorTemplate.getHoloBlue()
        set.highLightColor = Color.rgb(244, 117, 117)
        set.valueTextColor = set.color
        set.valueTextSize = 9f
        set.setDrawValues(false)

        return set
    }

    fun newData(x: Double, y: Double) {
        var dataset = data.getDataSetByIndex(0)

        if (dataset == null){
            dataset = createSet("Default")
            data.addDataSet(dataset)
        }


        dataset.addEntry(Entry(x.toFloat(), y.toFloat()))
        if (dataset.entryCount > 300)
            dataset.removeEntry(0)

        data.notifyDataChanged()

        chart?.notifyDataSetChanged()
        chart?.moveViewToX(x.toFloat())

    }

    public fun onPlottedParamChanged(selectedIndex: Int)
    {
        var sd = SolarData(null)
        var name = sd.getParamList()[selectedIndex]

        data.removeDataSet(0)
        data.addDataSet(createSet(name))
        var min = sd.params[selectedIndex].min?.toFloat()
        var max = sd.params[selectedIndex].max?.toFloat()

        chart?.axisLeft?.axisMinimum  = min!!
        chart?.axisLeft?.axisMaximum = max!!



    }

}