package com.example.bluetoothconnection;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;

public class live_heart_rate_activity extends AppCompatActivity {

    Button previousButton;
    Thread workerThread;
    byte[] readBuffer;
    int readBufferPosition;
    volatile boolean stopWorker;
    InputStream mmInputStream;
    TextView myLabel;
    BluetoothSocket mmSocket;
    OutputStream mmOutputStream;
    BluetoothDevice mmDevice;
    BluetoothAdapter mBluetoothAdapter;
    int counter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.live_heart_rate_activity);

        myLabel = findViewById(R.id.label);

        //previous button actions
        Button openbutton = findViewById(R.id.open);
        previousButton = findViewById(R.id.previous_button);
        previousButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (counter == 1) {
                    counter = 0;
                    closeBT();
                }
                Intent intent = new Intent(live_heart_rate_activity.this, Menu.class);
                startActivity(intent);
            }
        });
        openbutton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    findBT();
                    openBT();
                } catch (IOException ex) {
                }
            }
        });
    }
    // open bluetooth
    void openBT() throws IOException{
        UUID uuid = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
        mmSocket = mmDevice.createRfcommSocketToServiceRecord(uuid);
        mmSocket.connect();
        mmOutputStream = mmSocket.getOutputStream();
        mmInputStream = mmSocket.getInputStream();
        counter = 1;

        waitForData();

        myLabel.setTextSize(25); //setting size of the information
        myLabel.setText("Bluetooth is opened \n- listening for data");
    }
    // find the bluetooth device
    void findBT(){
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (mBluetoothAdapter == null)
        {
            myLabel.setTextSize(32); //setting size of the information
            myLabel.setText("No Bluetooth adapter available");
        }
        if (!mBluetoothAdapter.isEnabled()){
            Intent enableBluetooth = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBluetooth, 0);
        }

        Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();
        if (pairedDevices.size()>0){
            for(BluetoothDevice device : pairedDevices){
                if(device.getName().equals("ESP32testing")){
                    mmDevice = device;
                    break;
                }
            }
        }
        myLabel.setTextSize(32); //setting size of the information
        myLabel.setText("Bluetooth Device Found");
    }


    //Waiting for the data sent by bluetooth
    void waitForData(){
        final Handler handler = new Handler();
        //ASCII code for a newline character
        final byte delimiter = 10;

        stopWorker = false;
        readBufferPosition = 0;
        readBuffer = new byte[1024];
        workerThread = new Thread(new Runnable() {
            @Override
            public void run() {
                while (!Thread.currentThread().isInterrupted() && !stopWorker){
                    try{
                        int bytesAvailable = mmInputStream.available();
                        if (bytesAvailable > 0){
                            byte[] packetBytes = new byte[bytesAvailable];
                            mmInputStream.read(packetBytes);
                            for (int i = 0; i<bytesAvailable; i++){
                                byte b = packetBytes[i];
                                if (b == delimiter){
                                    byte[] encodedBytes = new byte[readBufferPosition];
                                    System.arraycopy(readBuffer, 0, encodedBytes, 0, encodedBytes.length );
                                    final String data = new String(encodedBytes, "US-ASCII");
                                    readBufferPosition = 0;
                                    handler.post(new Runnable(){
                                        public void run() //Display received data in the app
                                        {
                                            myLabel.setTextSize(150); //setting the size of collected data from the optical sensor
                                            myLabel.setText(data);
                                            myLabel.setPadding(20,10,20,10);
                                        }
                                    });
                                }else{
                                    readBuffer[readBufferPosition++] = b;
                                }
                            }
                        }
                    }
                    catch (IOException ex){
                        stopWorker = true;
                    }
                }
            }
        });
        workerThread.start();
    }

    //Function that is used to close the bluetooth connection
    void closeBT(){
        try {
            mmSocket.close();
            mmOutputStream.close();
            mmInputStream.close();
            }
        catch (IOException e) {
            e.printStackTrace();
        }
    }


}