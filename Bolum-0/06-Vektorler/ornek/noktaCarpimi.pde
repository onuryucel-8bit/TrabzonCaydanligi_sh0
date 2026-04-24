void setup()
{
  size(800, 600);
}

void draw()
{
  background(100);
  strokeWeight(2);
  
  translate(width/2, height/2);
  
  textSize(20);
  
  //---------------------------------------------//
  PVector a = new PVector(200, 100);
  
  strokeWeight(4);
  line(0, 0, a.x, a.y);
  
  text("(" + str(a.x) + "," + str(a.y) + ")", a.x, a.y);
  //---------------------------------------------//
  
  //---------------------------------------------//
  PVector b = new PVector(mouseX - width/2, mouseY - height/2);
  
  strokeWeight(4);
  line(0, 0, b.x, b.y);
  
  text("(" + str(b.x) + "," + str(b.y) + ")", b.x, b.y);
  //---------------------------------------------//
  float noktaCarpimi = a.x * b.x + a.y * b.y; 
  
  
  
  //a.x * b.x + a.y * b.y  
  translate(-width/2, -height/2);
  
  textSize(32);
  text("nokta carpimi(ax * bx + ay * by): " + str(noktaCarpimi), 10, 50);
  text("a.x * b.x: " + str(a.x * b.x), 10, 90);
  text("a.y * b.y: " + str(a.y * b.y), 10, 120);
  
}