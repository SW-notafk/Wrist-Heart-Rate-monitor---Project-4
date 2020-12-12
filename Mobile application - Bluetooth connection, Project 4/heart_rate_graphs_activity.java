package com.example.bluetoothconnection;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class heart_rate_graphs_activity extends AppCompatActivity {

    Button previousButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.heart_rate_graphs_activity);

        previousButton = findViewById(R.id.previous_button);

        previousButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(heart_rate_graphs_activity.this, Menu.class);
                startActivity(intent);
            }
        });
    }
}