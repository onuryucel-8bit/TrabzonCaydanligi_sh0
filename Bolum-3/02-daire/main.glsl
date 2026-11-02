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
    vec2 uv = fragCoord/iResolution.xy * 2.0 - 1.0;

    //farkli ekran boyutlari icin uv.x = ires.x/ires.y
    //cikti 800x600, 1600x900 ...vb. boyutlardaki ekranlarda
    //bozulmaya ugramasin diye bolme yapiyoruz
    uv.x *= iResolution.x / iResolution.y;

    //kisa hali
    //vec2 uv = (fragCoord * 2.0 - iResolution.xy) / iResolution.y;

    float d = length(uv);

    d -= 0.5;
    
    //ekrana yolla
    fragColor = vec4(d,d,d,1.0);
}