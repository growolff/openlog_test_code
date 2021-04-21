/*
  por Gonzalo Olave Wolff
  Programa de ejemplo para configurar un OpenLog y logear mensajes
  Basado en el ejemplo "Recording to OpenLog example" de Nathan Seidle y en un ejemplo que me envio Juan Carlos Pina de Timer

  This is an example of basic recording to OpenLog using software serial. This example is best for users who
  want to use OpenLog with an Uno or other platform capable of softwareSerial.

  Connect the following OpenLog to ESP32:
  GRN to pin 4
  RXI of OpenLog to pin 15
  TXI of OpenLog to pin 2
  VCC to 5V
  GND to GND

  This example records in the LOGFILE.csv file whatever the its printed using OpenLog.print().
*/

#define TIMEOUT_SD 2200000
#define TIMEOUT_LOG 10000

// archivo config.txt en la raiz del openlog debe tener las siguientes dos lineas
//19200,26,3,0,1,1,0
//baud,escape,esc#,mode,verb,echo,ignoreRX
#define OL_BAUDRATE 9600 // open log serial comm
#define MS_BAUDRATE 115200 // arduino serial comm

//HardwareSerial Serial2(2);
// configuracion uart1 para comunicar el openlog
#define OpenLog Serial2
// redefine los pines del uart1
// los que vienen por defecto en el esp32 (9,10) no se pueden utilizar para uart externo
#define RX2 4
#define TX2 0

// caracter de salida, configurado en el archivo config.txt almacenado en la tarjeta micro SD
#define OL_EXIT_CHAR  26
// pines para configurar el serial1 del esp32
#define OL_RX         RX2
#define OL_TX         TX2

// pin para reiniciar el OpenLog conectado al pin GRN
#define OL_RESET      2

// contador de ciclos
float c = 0.2;

// variables para configurar nombre del archivo a escribir y para los mensajes que se enviaran
char fileName[18];
char ol_buffer[18];

void setup() {
  pinMode(OL_RESET, OUTPUT);

  Serial.begin(MS_BAUDRATE); //Open serial port

  Serial.println("Open software serial port");
  OpenLog.begin(OL_BAUDRATE, SERIAL_8N1, OL_RX, OL_TX); //Open software serial port at 9600bps

  // envia comandos al openlog para entrar al modo lecutra/escritura
  if (setupOpenLog() == true) {
    Serial.println("OpenLog setup OK");
    // crea un nuevo archivo llamado 'LOGTIMER.csv'd
    //Serial.println("create file LOGTIMER.csv");
    //sprintf(fileName, "append LOGTIMER.csv\r");
    //OpenLog.print(fileName);
  }
  else {
    Serial.println("Could not setup OpenLog correctly");
    while (true) {

    }
  }

  Serial.println("__fin_setup__");
}

void loop() {

  if (OpenLog.available()) {
    // escribe los datos al final del archivo 'LOGTIMER.csv'
    sprintf(ol_buffer, "%.2f;%.2f;%.2f;%.2f;%.2f;%.2f;%.2f;%.2f", c, c, c, c, c, c, c, c);
    OpenLog.println(ol_buffer);
  }
  else {
    Serial.println("OpenLog not available");
  }
  Serial.println(ol_buffer);
  delay(1000);

  c++;
}

bool setupOpenLog() {
  //Reset OpenLog
  digitalWrite(OL_RESET, LOW);
  delay(100);
  digitalWrite(OL_RESET, HIGH);

  // espera que el OpenLog responda un caracter '<' para continuar
  long t = millis();
  Serial.println("setting up OpenLog ...");
  while (millis() - t < TIMEOUT_LOG) {
    Serial.print(".");
    if (OpenLog.available()) {
      Serial.println("OpenLog available...");
      if (OpenLog.read() == '<') {
        Serial.println("OpenLog setup OK");
        // hasta aca deberia funcionar para loguear en .txt
        // si queremos trabajar con comandos para crear archivos u otros se puede usar la siguiente funcion
        //if (openlogCommandMode() == true) {
        //  Serial.println("OpenLog command mode OK");
        //  return true;
        //}
      }
    }
    delay(500);
  }
  Serial.println("OpenLog setup ERROR");
  return false;
}

bool openlogCommandMode() {
  OpenLog.write(OL_EXIT_CHAR);
  OpenLog.write(OL_EXIT_CHAR);
  OpenLog.write(OL_EXIT_CHAR);
  int count = 0;
  while (count < TIMEOUT_SD) { // TIMEOUT_SD lo definií en 2200000 para evitar que se quede pegado ahí para siempre si es que hay cualquier error en la SD
    if (OpenLog.available()) {
      if (OpenLog.read() == '>') {
        return true;
      }
    }
    count++;
  }
  Serial.println("OpenLog command mode ERROR");
  return false;
}
