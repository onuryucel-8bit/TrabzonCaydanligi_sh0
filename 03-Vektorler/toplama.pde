void setup()
{
  size(800, 600);
}

void drawCoordLines()
{
  for(int i = 0; i < width; i += 10)
  {
    circle(i ,height / 2, 1);
  }
  
  for(int i = 0; i < height; i += 10)
  {
    circle(width / 2 , i, 1);
  }
 
}

void draw()
{
  background(140);
  
  drawCoordLines();
  
  translate(width / 2, height / 2);
  
  
  
  //mouseX - WindowWidth
  float mx = mouseX - (width/2);
  float my = mouseY - (height/2);

  strokeWeight(5);

  //vektor A
  line(0, 0, mx, my);
  
  float bx = 100;
  float by = 100;
  //vektor B
  line(0, 0, bx, by);
  
  //A + B
  line(0, 0, mx + bx, my + by);
  
  
}
