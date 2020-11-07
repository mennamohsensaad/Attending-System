
String in, out;
int total = 0;
void setup() {
  pinMode(0, INPUT);
  pinMode(5, INPUT);
  Serial.begin(9600);
}
//void Show(){
//  Serial.print(total);
//    Serial.println(" people in room.");
//}
void loop() {
  if (digitalRead(0) == LOW) {
    while (digitalRead(5) == HIGH) {
    }
    analogWrite(A1, 255);
    Serial.print("Person entered there are ");
    total++;
    //show();
    Serial.print(total);
    Serial.println(" people in room.");
    delay(1000);
  } else if (digitalRead(5) == LOW) {
    while (digitalRead(0) == HIGH) {
    }
   
    Serial.print("Person exited there are ");
    total--;
    //show();
    Serial.print(total);
    Serial.println(" people in room.");
    delay(1000);
  }
}
