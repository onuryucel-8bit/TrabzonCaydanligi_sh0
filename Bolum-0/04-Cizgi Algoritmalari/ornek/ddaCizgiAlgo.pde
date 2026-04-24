boolean f_soltik = false;
boolean f_sagtik = false;

float x0 = 0;
float y0 = 0;

float x1 = 0;
float y1 = 0;

void mousePressed()
{         
   if (mouseButton == LEFT)
   {
     f_soltik = !f_soltik;
     
     if(f_soltik == true)
     {             
        x0 = 32 * (mouseX / 32);
        y0 = 32 * (mouseY / 32);
     }
   }
   
   if(mouseButton == RIGHT)
   {
     f_sagtik = !f_sagtik;
     
     if(f_sagtik == true)
     {
        x1 = 32 * (mouseX / 32);
        y1 = 32 * (mouseY / 32);
     }
   }
}

void setup()
{
  size(800, 600);
}

void draw()
{
  background(100);
  
  for(int x = 0; x < width; x+= 32)
  {
     line(x, 0, x, height);
  }
  
  for(int y = 0; y < height; y+= 32)
  {
     line(0, y, width, y);
  }
        
  float mx = mouseX;
  float my = mouseY;
   
  
  fill(0,0,0);
  if(f_soltik == false)
  {    
    rect(mx, my, 32, 32);
  }
  else
  { 
    fill(0,0,255);    
    rect(x0, y0, 32, 32);
    fill(0,255,0);
    text("(" + str((int)x0) + ", " + str((int)y0) + ")", x0, y0 - 10);
    
  }
  
  if(f_sagtik == true)
  {
    fill(0,255,0);
    text("(" + str((int)x1) + ", " + str((int)y1) + ")", x1, y1 - 10);
    rect(x1, y1, 32, 32);
  }
  
  float deltax = x1 - x0;
  float deltay = y1 - y0;
  float sideLength = 0;
  
  if(abs(deltax) >= abs(deltay))
  {
    sideLength = abs(deltax);
  }
  else
  {
    sideLength = abs(deltay);
  }
  
  float incx = deltax / sideLength;
  float incy = deltay / sideLength;
  
  float currentx = x0;
  float currenty = y0;
  
  for(int i = 0; i <= sideLength; i+= 32)
  {
     float gx = 32 * floor(currentx / 32);
     float gy = 32 * floor(currenty / 32);
    
     rect(gx, gy, 32, 32);
     currentx += incx * 32;
     currenty += incy * 32;
  }
  
  
  text("dx:" + str(deltax), 10, 80);
  text("dy:" + str(deltay), 10, 120);
  
  text("incx:" + str(incx), 10, 150);
  text("incy:" + str(incy), 10, 170);
  
  text("sideLength:" + str(sideLength), 10, 190);
  
  strokeWeight(3);
  line(x0 + 16, y0 + 16, x1 + 16, y1 + 16);
        
  fill(0,255,0);
  textSize(20);
  text("mx: " + str(mx), 10, 20);
  text("my: " + str(my), 10, 40);
  
}