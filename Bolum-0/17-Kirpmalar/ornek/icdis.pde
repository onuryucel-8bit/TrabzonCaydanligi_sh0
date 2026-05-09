float FOV = 60;

void setup()
{
  size(1000, 800);
}

void draw()
{
  background(100);
  strokeWeight(4);

  stroke(0, 190, 0);
  line(0, height/2, width, height/2);
  line(width/2, 0, width/2, height);

  fill(0, 0, 255);
  translate(width/2, height/2);
  
  float alfa = radians(FOV);

  //sol sag kesim duzlemi
  line(0, 0, 300* cos(alfa), 300*sin(alfa));
  line(0, 0, -300* cos(alfa), 300*sin(alfa));



  noStroke();
  PVector currentVertex = new PVector(mouseX - width/2, mouseY-height/2); 
  ellipse(currentVertex.x, currentVertex.y, 20, 20);

  stroke(0, 0, 250);  
  PVector planeNormalLeft = new PVector(200*cos(alfa/2), 200*sin(alfa/2));
  line(0, 0, planeNormalLeft.x, planeNormalLeft.y);
  PVector planeNormalRight = new PVector(-200*cos(alfa/2), 200*sin(alfa/2));
  line(0, 0, planeNormalRight.x, planeNormalRight.y);
  
  
  
  line(0,0,currentVertex.x, currentVertex.y);
  
  float currentDotLeft = currentVertex.dot(planeNormalLeft);
  float currentDotRight = currentVertex.dot(planeNormalRight);
  
  textSize(30);
  if(currentDotLeft > 0 && currentDotRight > 0)
  {
    text("Icerde", -100,0);
  }
  else
  {
    text("Disarda",-100,0);
  }
  
  //ellipse(-10,50,10,10);
}
