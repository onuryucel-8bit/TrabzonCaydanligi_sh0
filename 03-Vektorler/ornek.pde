//Temel Vektor

void setup()
{
  size(800, 600);
}

void drawPixel(int x, int y)
{
  loadPixels();
  
  int index = y * width + x;
  pixels[index] = color(255, 255, 255);
   
  updatePixels(); 
}

void drawCoordLines()
{
  //x line
  for(int i = 0; i < width; i += 20)
  {
   drawPixel(i, height /2);
  }
  //y line
  for(int i = 0; i < height; i += 20)
  {
   drawPixel(width / 2, i);
  }
}

void draw()
{
  background(140);
  
  translate(width / 2, height / 2);
  
  drawCoordLines();
  
  //mouseX - WindowWidth
  float mx = mouseX - (width/2);
  float my = mouseY - (height/2);

  line(0, 0, mx, my);
}
