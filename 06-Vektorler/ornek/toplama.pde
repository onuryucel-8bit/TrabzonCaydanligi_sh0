boolean f_islem = true;

void setup()
{
  size(800, 600);
}

void mousePressed()
{
  
  if(mouseButton == LEFT)
  {
    f_islem = !f_islem;
  }
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
  stroke(0, 0, 0);
  line(0, 0, bx, by);
  
  
  stroke(0, 0, 255);
  
  if(f_islem == true)
  {
    //A + B
    line(0, 0, mx + bx, my + by);
  }
  else
  {
    //A - B
    line(0, 0, mx - bx, my - by); 
  }
  
  stroke(0, 0, 0);
  
  textSize(32);
  //vektor a
  text("(" + str(mx) + ", "+ str(my) + ")", mx, my);
  
  text("(" + str(bx) + ", "+ str(by) + ")", bx, by);
  
  //a + b
  if(f_islem == true)
  {
    text("Toplama", 10 - width / 2, 30 - height / 2);
    text("(" + str(mx + bx) + ", "+ str(my + by) + ")", mx + bx, my + by);
  }
  //a - b
  else
  {
    text("Cikarma", 10 - width / 2, 30 - height / 2);
    text("(" + str(mx - bx) + ", "+ str(my - by) + ")", mx - bx, my - by);
  }
  
  
}
