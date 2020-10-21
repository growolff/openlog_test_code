 /*
  por Gonzalo Olave Wolff
  Programa de ejemplo para configurar un OpenLog y logear mensajes
  Basado en el ejemplo "Recording to OpenLog example" de Nathan Seidle y en un ejemplo que me envio Juan Carlos Pina de Timer
  
  This is an example of basic recording to OpenLog using software serial. This example is best for users who
  want to use OpenLog with an Uno or other platform capable of softwareSerial.

  Connect the following OpenLog to Arduino:
  RXI of OpenLog to pin 7 on Arduino
  VCC to 5V 
  GND to GND
  GRN to pin 4

  This example records whatever the user OpenLog.prints(). This uses software serial on pin 7 instead
  of hardware serial (TX/RX pins). Nearly any pin can be used for software serial.
*/

#include <SoftwareSerial.h>

#define TIMEOUT_SD 2200000
#define TIMEOUT_LOG 10000

SoftwareSerial OpenLog(6, 7); // 6 -> Soft RX pin, 7 -> Soft TX pin

// pin del led integrado en el arduino para debugueo
int statLED = 13;

// pin para reiniciar el OpenLog conectado al pin GRN 
int resetOpenLog = 4;

// contador de ciclos
int c = 0;

// caracter de salida, configurado en el archivo config.txt almacenado en la tarjeta micro SD
int exitChar = 36;

// variables para configurar nombre del archivo a escribir y para los mensajes que se enviaran
char fileName[18];
char ol_buffer[18];

void setup() {
  pinMode(statLED, OUTPUT);
  pinMode(resetOpenLog, OUTPUT);
  
  Serial.begin(9600); //Open serial port at 9600bps
  Serial.println("Open software serial port");
  OpenLog.begin(9600); //Open software serial port at 9600bps

    //Reset OpenLog
  digitalWrite(resetOpenLog, LOW);
  delay(100);
  digitalWrite(resetOpenLog, HIGH);   
  Serial.println("Reset openlog");

  // espera que el OpenLog responda un caracter '<' para continuar
  long t = millis();
  while(millis() - t < TIMEOUT_LOG) { 
    Serial.print(".");
    if(OpenLog.available())
      if(OpenLog.read() == '<'){
        Serial.println("Ready to receive info");
        break;
      }
      delay(500);
  }

  // envia comandos al openlog para entrar al modo lecutra/escritura
  openlog_command_mode();
  Serial.println("Ready to receive commands");
  blinkLed();

  // crea un nuevo archivo llamado 'LOGTIMER.csv'
  sprintf(fileName, "new LOGTIMER.csv\r");  
  OpenLog.print(fileName); //\r in string + regular print works with older v2.5 Openlogs

  // 
  sprintf(fileName, "append LOGTIMER.csv\r");
  OpenLog.print(fileName);
  Serial.println("Ready to get the real data");
}

void loop() {  
  // escribe los datos al final del archivo 'LOGTIMER.csv'
  sprintf(ol_buffer, "%d;%d;%d;%d",c,c,c,c);
  OpenLog.println(ol_buffer);

  Serial.println(ol_buffer);

  /*
  digitalWrite(statLED, HIGH);
  delay(1000);
  digitalWrite(statLED, LOW); 
  delay(1000);
  */ 

  delay(1000);
  
  c++;
}

void openlog_command_mode(){
  Serial.println("Setting up command mode");
  OpenLog.write(exitChar);//26
  OpenLog.write(exitChar);  
  OpenLog.write(exitChar);
  int count=0;
  while(count < TIMEOUT_SD) { // TIMEOUT_SD lo definií en 2200000 para evitar que se quede pegado ahí para siempre si es que hay cualquier error en la SD
    if(OpenLog.available()){
      if(OpenLog.read() == '>'){
        break;
      }
    }
    count++;  
  }
}

void blinkLed() {
  for (int i = 0; i < 10; i++) {
    digitalWrite(statLED, 1);
    delay(50);
    digitalWrite(statLED, 0);
    delay(50);
  }
}
