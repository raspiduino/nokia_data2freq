#define SEPARATOR 10
#define STEP 1
#define DEBUG

const int analogInPin = A1;  // Analog input pin that the potentiometer is attached to
int sensorValue = 0;  // Value read from the port

int pcount = 0;
char p[3];
bool waitendframe = false;

// Value range for each freq
//                  1     2     3     4     5    6    7    8    9   10    Hz
const int min[] = {4200, 2050, 1350, 1000, 780, 650, 550, 500, 430, 350};
const int max[] = {4400, 2200, 1500, 1100, 900, 700, 600, 520, 470, 420};

// It's bad, but it works
int freq_lookup(int value) {
  for (int i = 0; i < 10; i++) {
      if (min[i] <= value && value <= max[i]) return i + 1;
  }

  return -1;
}

char combineParts(char x, char y, char z) {
  char c = 0; // Initialize c to 0

  // Combining x, y, and z parts
  c |= ((x - STEP) << 5); // Left shift x by 5 bits and OR with c
  c |= ((y - STEP) << 2); // Left shift y by 2 bits and OR with c
  c |= (z - STEP); // OR z with c

  return c;
}

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
}

void loop() {
  int count = 0;

  // read the analog in value:
  sensorValue = analogRead(analogInPin);

  while (sensorValue <= 2) {
    count++;
    // read the analog in value:
    sensorValue = analogRead(analogInPin);
  }

  if (count > 10) {
    int f = freq_lookup(count);
    if (f < 1) return;
#ifdef DEBUG
    Serial.print(f);
    Serial.print(" Hz (counted ");
    Serial.print(count);
    Serial.println(")");
#endif
    if (f == SEPARATOR) {
      if (waitendframe == true) {
#ifdef DEBUG
        Serial.println("End frame");
#endif
        waitendframe = false;
        digitalWrite(13, HIGH);
        return;
      }
    } else if (waitendframe == false){
      p[pcount] = f;
      waitendframe = true;
      digitalWrite(13, LOW);
#ifdef DEBUG
      Serial.println("Wait end frame");
#endif
      if (pcount >= 2) {
        Serial.print(combineParts(p[0], p[1], p[2]));
        pcount = 0;
      } else {
        pcount++;
      }
    }
  }

  // wait 2 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  delay(2);
}
