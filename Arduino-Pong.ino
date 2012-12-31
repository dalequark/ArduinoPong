//Simple game of Arduino Pong, with Arduino Uno, Color LCD Shield, and
//a pot connected to arduino analog pin 3. Make sure you've downloaded
//Peter Davenport's ColorLCDShield library here:
//https://github.com/downloads/jimblom/ColorLCDShield/ColorLCDShield-July-16-2012.zip

//This line might have to be changed based on which version of
//the LCD library you're using. Any one should work.
#include <ColorLCDShield2.h>

#include <EEPROM.h>



#define BACKGROUND SALMON

LCDShield lcd;

//Screen is 132x132

//this is the potentiometer, should be connected to analog 3.

int dial=3;
int val = 0;

//pdy represents the speed of the paddle. locationx and locationy

int pdy;
int locationx;
int locationy;
int dx;
int dy;

//enemy y position. edy is the speed of the enemy paddle.
int enemyy;
int edy;


void setup(){
  //Optionally, attach an LED to digital Pin 2. It will flash 
  //when a game ends!
  
  pinMode(2,OUTPUT);
  digitalWrite(2,LOW);
  
  //change based on which model of the LCD screen you've got.
  lcd.init(EPSON);
  lcd.contrast(40); 
  lcd.clear(BACKGROUND);
  

//ball start position.
locationx = 50;
locationy = 50;

//paddle speed.
pdy = 5;

//x speed and y speed for ball. the y speed for the ball is random to
//some degree, to make the game more interesting. 

dx = 5;
dy = random(3,5);

//enemy start position;
enemyy=50;
//enemy speed.
edy = 5;
}

//Main loop that starts the game.

void loop(){
  startGame();
}


void startGame() 
{
 //erases previous position of the paddle.
 lcd.setRect(1,val+10,5,val-10,1,BACKGROUND);
 
 //val = value of potentiometer, mapped between 10 and 122 as per
 //the dimensions of our screen.
  val = analogRead(dial);
  val = map(val,0,1023,10,122);
  
  //erases previous position of the ball.
  lcd.setCircle(locationx,locationy,4,BACKGROUND);
  
  //move the ball
  locationx=locationx+dx;
  locationy=locationy+dy;
  
  //draw paddle in new position.
  lcd.setRect(1,val+10,5,val-10,1,WHITE);
  
  //draw circle in new position
  lcd.setCircle(locationx,locationy,4,BLUE);
    delay(150);
    
  //bounce back on hitting the paddle or opposite wall. If doesn't hit paddle,
  //game over.
  enemyPaddle();
  collision();
  gameOver();
  

}


void blinkLED(int led) {
  for(int i=0; i<10; i++) {
    digitalWrite(led, HIGH);
    delay(70);
    digitalWrite(led, LOW);
    delay(70);
  }
}


void gameOver() {

if((locationx<=0)||(locationx>=131)) {
    lcd.clear(BACKGROUND);
    if(locationx<=0){
      lcd.setStr("You're a loser!", 50, 10, PURPLE, BACKGROUND);
    }
    else{
      lcd.setStr("You're a winner!", 50, 10, PURPLE, BACKGROUND);
    }    
    blinkLED(2);
    lcd.clear(BACKGROUND);
    lcd.setStr("Start!",50, 70, PURPLE, BACKGROUND);
    delay(2000);
    lcd.clear(BACKGROUND);
    locationx = 50;
    locationy = 50;
  }
}

void collision() {
   //bounce back on hitting ball or ceiling
   if(locationy>=125||locationy<4)
  {
    //dx=dx*-1;
    dy=dy*-1;
  }
  //if hits paddle.
    if(locationx<=14&&locationy>=val-10&&locationy<=val+10)
  {
    dx=dx*-1;
  }
  //if hits enemy paddle.
    if(locationx>=120&&locationy>=enemyy-10&&locationy<=enemyy+10)
  {
    dx=dx*-1;
  }
}

void enemyPaddle() {
  lcd.setRect(125,enemyy+10,131,enemyy-10,1,BACKGROUND);
  //movement if ball is above enemyy.
  if(enemyy<locationy){
    enemyy = enemyy+edy;
  }
  //movement for below enemy.
  if(enemyy>locationy){
    enemyy = enemyy - edy;
  }  
  lcd.setRect(125,enemyy+10,131,enemyy-10,1,WHITE); 
  
}
