// Define  all OUTPUT M = Motor and INVM = Invert Motor Rotation
// OUT1 and OUT2 it's IR Sensor signal for the auto mode

#define INVM1 2
#define M1    3
#define M2    6
#define INVM2 7
#define OUT1  9
#define OUT2  10

bool controller = false;
int velocityM1 = 0;
int velocityM2 = 0;
int convertVelocity(int character);

// Car motion functions

void moveForward();
void moveBackward();
void stopCar();
void moveLeft();
void moveRight();

void setup() {
  pinMode(M1,     OUTPUT);
  pinMode(M2,     OUTPUT);
  pinMode(INVM1,  OUTPUT);
  pinMode(INVM2,  OUTPUT);

// Security step

  digitalWrite(M1,    LOW);
  digitalWrite(M2,    LOW);
  digitalWrite(INVM1, LOW);
  digitalWrite(INVM2, LOW);
  Serial.begin(9600);
}


void loop() {

// Always verifing if the bluetooth module is availale

  if (Serial.available() > 0) {
    
    // Read all the chars that the app send to module
    
    int recievedData = Serial.read();
    getVelocity(recievedData);
    
    
    // Verifing if auto mode is on, else wait for the manual command
    
    if (recievedData == 88) { controller = true; } 
    else if (recievedData == 120) { controller = false; }

    if (controller) {
      int ROUT1 = digitalRead(OUT1);
      int ROUT2 = digitalRead(OUT2);

      velocityM1 = 95;
      velocityM2 = 95;
    
      if(!ROUT1 &&  !ROUT2){ moveForward(); }
      if(ROUT1 && !ROUT2){ moveLeft(); }
      if(!ROUT1 && ROUT2){ moveRight(); }
      
    } else{
     
      if(recievedData == 70){ moveForward(); }
      else if(recievedData == 66){ moveBackward(); }
      else if(recievedData == 76){ moveLeft(); }
      else if(recievedData == 82){ moveRight(); }
      else{ stopCar(); }
    }
    
  }

 
}




// movements

void moveLeft(){

  analogWrite(M1,     LOW);
  analogWrite(M2, velocityM2);
  digitalWrite(INVM1, LOW);
  digitalWrite(INVM2, LOW);
}

void moveRight(){
  analogWrite(M1, velocityM1);
  analogWrite(M2,     LOW);
  digitalWrite(INVM1, LOW);
  digitalWrite(INVM2, LOW);
}


void moveForward(){
  analogWrite(M1, velocityM1);
  analogWrite(M2, velocityM2);
  digitalWrite(INVM1, LOW);
  digitalWrite(INVM2, LOW);
}


void moveBackward(){
  analogWrite(M1, velocityM1);
  analogWrite(M2, velocityM2);
  digitalWrite(INVM1, HIGH);
  digitalWrite(INVM2, HIGH);
}

void stopCar(){
  digitalWrite(M1, LOW);
  digitalWrite(M2, LOW);
  digitalWrite(INVM1, LOW);
  digitalWrite(INVM2, LOW);
}




// velocity

void getVelocity(int character){
  if((character >= 48 && character <= 57) || character == 113){
    int tempVelocity = convertVelocity(character);
    
    velocityM1 = map(tempVelocity, 0, 10, 0, 255);
    velocityM2 = map(tempVelocity, 0, 10, 0, 255);
  }
}

int convertVelocity(int character){
  // convert recived character to a velocity between 0 and 10
  return character != 113 ? character - 48 : 10;
} 
  
