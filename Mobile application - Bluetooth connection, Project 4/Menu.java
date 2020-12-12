package com.example.bluetoothconnection;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class Menu extends AppCompatActivity {

    //Variables for the menu
    TextView bluetooth_setting;

    //Buttons to switch the activity
    Button live_heart_rate_activity_button, heart_rate_graphs_activity_button, bluetooth_activity_button, strap_information_activity_button, database_activity_button;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_menu);

        live_heart_rate_activity_button= findViewById(R.id.live_heart_rate_button);
        heart_rate_graphs_activity_button= findViewById(R.id.heart_rate_graphs_button);
        bluetooth_activity_button= findViewById(R.id.bluetooth_settings_button);
        strap_information_activity_button= findViewById(R.id.strap_information_button);
        database_activity_button= findViewById(R.id.data_base_button);

        bluetooth_setting = findViewById(R.id.menu);

        bluetooth_setting.setText("Main Menu");

        live_heart_rate_activity_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(Menu.this, live_heart_rate_activity.class);
                startActivity(intent);
            }
        });

        heart_rate_graphs_activity_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(Menu.this, heart_rate_graphs_activity.class);
                startActivity(intent);
            }
        });

        bluetooth_activity_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(Menu.this, MainActivity.class);
                startActivity(intent);
            }
        });

        strap_information_activity_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(Menu.this, strap_information_activity.class);
                startActivity(intent);
            }
        });

        database_activity_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(Menu.this, database_activity.class);
                startActivity(intent);                
            }
        });



    }
}