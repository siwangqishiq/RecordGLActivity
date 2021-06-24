vec4 mainImage( out vec4 fragColor, in vec2 fragCoord){
    vec4 fragColor;
    vec2 uv = fragCoord/iResolution.xy;

    // Time varying pixel color
    vec3 col = 0.5 + 0.5*cos(iTime+uv.xyx+vec3(0,2,4));

    // Output to screen
    fragColor = vec4(col,1.0);
    return fragColor;
}
