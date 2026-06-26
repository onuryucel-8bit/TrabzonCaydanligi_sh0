class Rect
{
  PVector pos;
  PVector size;
  
  Rect()
  {
  }
  
  Rect(PVector pos_, PVector size_)
  {
    pos = pos_;
    size = size_;
  }
}

float scale = 2.5;
Rect[] rectList = new Rect[9];
Rect[] rect2dList = new Rect[9];

void setup()
{
  size(800, 600);
  
  PVector size = new PVector(32 * scale, 32 * scale);
  
  for(int i = 0; i < 9; i++)
  {
    PVector pos = new PVector(50 + 32 * scale * i, 10);
    
    rectList[i] = new Rect(pos, size);    
  }
  
  for(int i = 0; i < 3; i++)
  {
    for(int j = 0; j < 3; j++)
    {
        PVector pos = new PVector(50 + 32 * scale * j, 200 + 32 * scale * i);
        
        rect2dList[i * 3 + j] = new Rect(pos, size);
    }    
  }
}

boolean collisionAABB
(PVector mpos, PVector msize,
 PVector rpos, float bw, float bh)
{
    return (mpos.x           < rpos.x + bw &&
            mpos.x + msize.x > rpos.x      &&
            mpos.y           < rpos.y + bh &&
            mpos.y + msize.y > rpos.y);
}

void draw()
{
  background(100);
      
  for(int i = 0; i < 9; i++)
  {
                       
    PVector mouseVec = new PVector(mouseX, mouseY);
    if(collisionAABB(rect2dList[i].pos, rect2dList[i].size, mouseVec, 5, 5) ||
      collisionAABB(rectList[i].pos, rectList[i].size, mouseVec, 5, 5)
    )
    {
       fill(210,0,0);
    }
    else
    {
       fill(0,210,0);
    }
    
    
    
    rect(
    rectList[i].pos.x, 
    rectList[i].pos.y,
    rectList[i].size.x,
    rectList[i].size.y);
    
    rect(
    rect2dList[i].pos.x, 
    rect2dList[i].pos.y,
    rect2dList[i].size.x,
    rect2dList[i].size.y);
  }
  
  
  fill(0,255,0);
  strokeWeight(7);
  line(0, 32 * scale + 70, width, 32 * scale + 70);
  
  drawTexts();
  
  rect(mouseX, mouseY, 5,5);  
  
  int posx = (int) ((mouseX - 50)  / (32 * scale) );
  int posy = (int) ((mouseY - 200) / (32 * scale) );
   
  fill(200,200,200);
  text("[y * width + x]", 350, 250);
  text("[" + str(posy) +"* 3 + " + str(posx) + "]", 350, 300);
  text("[" + str(posy * 3 + posx) + "]", 350, 350);  
  
}

void drawTexts()
{
  textSize(32);
  for(int i = 0; i < 9; i++)
  {
    text("["+ str(i) +"]", 80 + 32 * scale * i, 32 * scale + 36);
  }
  
  for(int i = 0; i < 3; i++)
  {
    text("["+ str(i) +"]", 50 + 32 * scale * i + 30 , 190);
  }
  
  for(int i = 0; i < 3; i++)
  {
    text("["+ str(i) +"]", 10 , 250 + 32 * scale * i);
  }
}