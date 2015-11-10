int LogiquePos = 0;

void setup() {
  pinMode(5, OUTPUT);
  pinMode(0, INPUT);
}
 
void loop() {

  if (digitalRead(0) == LOW)
  {
    if (LogiquePos == 0)
    {
      digitalWrite(5, HIGH);
      LogiquePos = 1;
    }
    else
    {
      digitalWrite(5, LOW);
      LogiquePos = 0;      
    }
    delay(1000);
  }
  
}
