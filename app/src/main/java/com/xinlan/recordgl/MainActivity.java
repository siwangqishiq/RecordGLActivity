package com.xinlan.recordgl;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import com.xinlan.shaderlab.ShaderView;

public class MainActivity extends AppCompatActivity {
    private ShaderView mShaderView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mShaderView = findViewById(R.id.shader_view);
    }
}//end class