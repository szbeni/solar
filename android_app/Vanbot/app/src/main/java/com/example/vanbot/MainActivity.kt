package com.example.vanbot

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothSocket
import android.content.Intent
import android.os.Handler
import android.util.Log
import android.view.View
import android.widget.AdapterView
import android.widget.ArrayAdapter
import android.widget.Toast
import kotlinx.android.synthetic.main.activity_main.*
import com.jjoe64.graphview.series.*
import java.io.IOException
import java.io.InputStream
import java.io.OutputStream
import java.lang.Exception
import java.lang.Thread.currentThread

import java.util.*
import kotlin.concurrent.thread
import java.util.Calendar


class MainActivity : AppCompatActivity() {

    val startTime = Calendar.getInstance().time
    private lateinit var mmInputStream: InputStream
    private lateinit var mmOutputStream: OutputStream
    private lateinit var mmSocket: BluetoothSocket
    private lateinit var mBluetoothAdapter: BluetoothAdapter
    private var mmDevice: BluetoothDevice? = null
    private var stopWorker = true
    private var selectedIndex = 0

    private lateinit var series1: LineGraphSeries<DataPoint>

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)



        spinner.onItemSelectedListener = object : AdapterView.OnItemSelectedListener{
            override fun onNothingSelected(parent: AdapterView<*>?) {

            }

            override fun onItemSelected(parent: AdapterView<*>?, view: View?, position: Int, id: Long) {
                selectedIndex = position
            }
        }

        var sd = SolarData(null)
        var listOfParams = sd.getParamList()

        val aa = ArrayAdapter(this, android.R.layout.simple_spinner_item, listOfParams)
        aa.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item)
        spinner!!.adapter = aa

        series1 = LineGraphSeries<DataPoint>()
        plot.addSeries(series1)
        plot.viewport.isScrollable = true
        plot.viewport.isScalable = true
        plot.viewport.isYAxisBoundsManual = true
        plot.viewport.setMinY(0.0)
        plot.viewport.setMaxY(100.0)
        plot.viewport.isXAxisBoundsManual = true
        plot.viewport.setMinX(0.0)
        plot.viewport.setMaxX(100.0)
        var labelRenderer = plot.gridLabelRenderer
        labelRenderer?.setHumanRounding(true, true)



        //Open Button
        openButton.setOnClickListener {
            try{
                closeBT()
                findBT()
                openBT()
            }
            catch (e: Exception)
            {
                myLabel.text = "Exception during opening bluetooth: " + e.toString()
            }
        }

        //Send Button
        sendButton.setOnClickListener {
            sendData()
        }

        //Close button
        closeButton.setOnClickListener {
            closeBT()

        }
    }

    private fun findBT() {
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter()
        if(mBluetoothAdapter == null)
        {
            myLabel.text = "No bluetooth adapter available"
        }

        if(!mBluetoothAdapter.isEnabled)
        {
            val enableBluetooth = Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE)
            startActivityForResult(enableBluetooth, 0)
        }

        val pairedDevices = mBluetoothAdapter.bondedDevices
        if(pairedDevices.size > 0)
        {
            for(device in pairedDevices)
            {
                if(device.name == "OBDII")
                {
                    mmDevice = device
                    break
                }
            }
        }
        myLabel.text = "Bluetooth Device Found"
    }

    private fun openBT(){

        val uuid = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB") //Standard SerialPortService ID
        if (mmDevice != null)
        {
            mmSocket = mmDevice!!.createRfcommSocketToServiceRecord(uuid)
            mmSocket.connect()
            mmOutputStream = mmSocket.outputStream
            mmInputStream = mmSocket.inputStream

            beginListenForData()
            myLabel.text = "Bluetooth Opened"
        }
        else
        {
            myLabel.text = "Cannot find bluetooth device"
        }
    }

    private fun sendData()
    {
        Toast.makeText(this, "Send", Toast.LENGTH_SHORT).show()
    }

    private fun closeBT()
    {
        if (mmDevice != null)
        {
            mmDevice = null
            stopWorker = true
            mmOutputStream.close()
            mmInputStream.close()
            mmSocket.close()
            myLabel.setText("Bluetooth Closed")
        }
    }

    private fun beginListenForData()
    {
        var handler = Handler()
        var delimiter: Byte?
        delimiter = 10 //"\n".toByte()  //This is the ASCII code for a newline character
        stopWorker = false
        var readBufferPosition = 0
        var readBuffer = ByteArray(1024)

        var receiveThread = thread( start = false) {

            while(!currentThread().isInterrupted && !stopWorker)
            {
                try
                {
                    var bytesAvailable = mmInputStream.available()
                    if(bytesAvailable > 0)
                    {
                        var packetBytes = ByteArray(bytesAvailable)
                        mmInputStream.read(packetBytes)

                        for(i in 0 until bytesAvailable)
                        {
                            var b = packetBytes[i]
                            if(b == delimiter)
                            {

                                var encodedBytes = ByteArray(readBufferPosition);
                                System.arraycopy(readBuffer, 0, encodedBytes, 0, encodedBytes.size)
                                var data = String(encodedBytes)
                                readBufferPosition = 0

                                handler.post(Runnable{
                                    received.text = data
                                    val sd = SolarData(received.text.toString())
                                    if (sd.initialized)
                                    {
                                        values.text = sd.toString()
                                        val diff = sd.time.time - startTime.time
                                        val x:Double = diff.toDouble()
                                        val y:Double = sd.params[selectedIndex].value!!.toDouble()

                                        sd.params[selectedIndex].max?.let { plot.viewport.setMaxY(it) }
                                        sd.params[selectedIndex].min?.let { plot.viewport.setMinY(it) }

                                        plot.viewport.setMaxX(x)
                                        series1.appendData(DataPoint(x,y),false,300)
                                    }
                                })
                            }
                            else
                            {
                                readBuffer[readBufferPosition++] = b
                            }
                        }
                    }
                }
                catch (e: IOException)
                {
                    handler.post(Runnable {
                        received.text = "Error during receive: " + e.toString()
                    })
                    stopWorker = true
                }
            }
        }

        receiveThread.start()
    }
    /*
    void findBT()
    {
    }

    fun OpenBT
    {
    }

    void beginListenForData()
    {
    }

    void sendData() throws IOException
    {
        String msg = myTextbox.getText().toString();
        msg += "\n";
        mmOutputStream.write(msg.getBytes());
        myLabel.setText("Data Sent");
    }


     */

}

//private fun getBluetoothDevices(): String {
//
//    val mBTAdapter = BluetoothAdapter.getDefaultAdapter()
//    val mPairedDevices = mBTAdapter.bondedDevices
//
//    val devicesStr = StringBuilder()
//
//    for (dev in mPairedDevices)
//    {
//        devicesStr.append(dev.name).append("\n")
//    }
//
//    return devicesStr.toString()
//
//}

//        val helloText = findViewById<TextView>(R.id.hello)
//        helloText.text = getBluetoothDevices()
//
//        var buttonConnect = findViewById<Button>(R.id.button_connect)
//
//        buttonConnect.setOnClickListener {
//            // make a toast on button click event
//            Toast.makeText(this, "Hi there! This is a Toast.", Toast.LENGTH_LONG).show()
//        }



