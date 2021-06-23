package com.xinlan.recordgl;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.Button;

import com.xinlan.shaderlab.ShaderView;

public class MainActivity extends AppCompatActivity {
    private ShaderView mShaderView;
    private Button mRunBtn;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mShaderView = findViewById(R.id.shader_view);
        mRunBtn = findViewById(R.id.run_btn);

        mRunBtn.setOnClickListener((v)->{
            compileAndRunShader();
        });
    }

    private void compileAndRunShader(){

    }
}//end class