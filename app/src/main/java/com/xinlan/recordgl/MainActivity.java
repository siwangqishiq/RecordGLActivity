package com.xinlan.recordgl;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.Button;
import android.widget.EditText;

import com.xinlan.shaderlab.ShaderView;

public class MainActivity extends AppCompatActivity {
    private ShaderView mShaderView;
    private Button mRunBtn;

    private String code = "vec4 mainImage( out vec4 fragColor, in vec2 fragCoord){\n" +
            "    vec2 uv = fragCoord/iResolution.xy;\n" +
            "\n" +
            "    // Time varying pixel color\n" +
            "    vec3 col = 0.5 + 0.5*cos(iTime+uv.xyx+vec3(0,2,4));\n" +
            "\n" +
            "    // Output to screen\n" +
            "    fragColor = vec4(0.0,1.0,1.0,1.0);\n" +
            "    return fragColor;\n" +
            "}";

    private EditText mCodeText;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mShaderView = findViewById(R.id.shader_view);
        mRunBtn = findViewById(R.id.run_btn);
        mCodeText = findViewById(R.id.code_text);

        mCodeText.setText(code);

        mRunBtn.setOnClickListener((v)->{
            compileAndRunShader();
        });
    }

    private void compileAndRunShader(){
        String code = mCodeText.getText().toString();
        mShaderView.compileAndRun(code);
    }
}//end class