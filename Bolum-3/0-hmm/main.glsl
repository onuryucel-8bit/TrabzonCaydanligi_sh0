void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Normalized pixel coordinates (from 0 to 1)
    /*
    The fragCoord variable represents the XY coordinate of the canvas. 
    The bottom-left corner starts at (0, 0) and 
    the top-right corner is (iResolution.x, iResolution.y). 
    By dividing fragCoord by iResolution.xy,
    we are able to normalize the pixel coordinates between zero and one.

    y
    ^
    |
    |
    |
    .----------->x

    */
    
    //renk
    vec3 col = vec3(0);

    //ekrana yolla
    fragColor = vec4(col,1.0);
}