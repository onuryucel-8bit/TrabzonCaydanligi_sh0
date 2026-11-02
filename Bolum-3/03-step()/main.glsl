void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
   
    /*
    The fragCoord variable represents the XY coordinate of the canvas. 
    The bottom-left corner starts at (0, 0) and 
    the top-right corner is (iResolution.x, iResolution.y). 
    By dividing fragCoord by iResolution.xy,
    we are able to normalize the pixel coordinates between zero and one.

    fragcoord koordinatlari
    y 1
    ^
    |
    |
    |
    .----------->x
    -1          1
    */
    
    /*

    uv = fragCoord/iResolution.xy;

    iResolution.x => pencereGenisligi
    iResolution.y => pencereYuksekligi
    iResolution.z => derinlik

             y -1
              ^
              |
              |
              |
              |
x -1 <--------.----------> x 1
              |
              |
              |
              | 
              v
             y 1


             y 1
              ^
              |
              |
              |
              |
x -1 <--------.----------> x 1
              |
              |
              |
              | 
              v
             y -1
    
    */

    //koordinatlari birim haline getir
    //vec2 uv = fragCoord/iResolution.xy * 2.0 - 1.0;

    //farkli ekran boyutlari icin uv.x = ires.x/ires.y
    //cikti 800x600, 1600x900 ...vb. boyutlardaki ekranlarda
    //bozulmaya ugramasin diye bolme yapiyoruz
    //uv.x *= iResolution.x / iResolution.y;

    //kisa hali
    vec2 uv = (fragCoord * 2.0 - iResolution.xy) / iResolution.y;

    vec3 col = vec3(0);

    //if(uv.x > 0.4)
    //{
        //col = vec3(1,0,0);
    //}

    //yukardaki if blogu ile ayni seyi yapiyor
    col = vec3(step(0.5, uv.x),0,0);
    
    //ekrana yolla
    fragColor = vec4(col,1.0);
}