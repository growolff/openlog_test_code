# openlog_test_code
código de prueba de un arduino logeando en un openlog


## troubleshooting

A veces cuando se conecta un openlog por primera vez viene configurado con un baudrate diferente al que declara en el archivo config.txt

links de referencia:
* [entrar al modo de comandos en arduino](https://github.com/sparkfun/OpenLog/blob/master/firmware/Arduino_Examples/Performance_Testing/CommandPrompt_Power/CommandPrompt_Power.ino)
* [openlog hookup guide](https://learn.sparkfun.com/tutorials/openlog-hookup-guide)
* [ejemplo útil](https://www.hobbyist.co.nz/?q=openlog-serial-datalogger)

1. conectar el openlog directamente al puerto serial mediante un USB to UART 3.3v de la siguiente forma

| OpenLog | USB2UART   |
|---------|------------|
| GRN     | DTR        |
| RX0     | TX         |
| TX0     | RX         |
| VCC     | VCC (3.3v) |
| GND     | GND        |
| -       | BLK        |

2. Abrir el monitor serial de Arduino IDE y probar los diferentes baudrates hasta que el openlog responda con caracteres conocidos (cuando se ve "12<" el openlog recibira cualquier caracter o string que se envíe y lo logueará en un archivo por defecto LOGFILE.txt).

  ```
12<
```
Si se envía el caracter de salida 3 veces <code>exitchar, 26 -> CTRL+Z o 36 -> $) </code> el openlog entra a un modo donde el sistema de archivos puede ser modificado por comandos (reivsar la [info sobre comandos](https://learn.sparkfun.com/tutorials/openlog-hookup-guide/all#command-set)).

  ```
12<~>
```
se puede probar enviando el caracter '?' para ver el menú de comandos del OpenLog

  ```
12<~>?
OpenLog v4.1
Basic commands:
new <file>              : Creates <file>
append <file>           : Appends text to end of <file>
md <directory>          : Creates a <directory>
ls                      : Shows the content of the current directory..
read <file> <start> <length> <type>: Outputs <length> bytes of <file> to the terminal starting at <start>. Omit <start> and <length> to read whole file. <type> 1 prints in ASCII, 2 in HEX.
size <file>             : Write size of <file> to terminal
disk                    : Shows card information
reset                   : Causes unit to reset, uses parameters in config file
set                     : Menu to configure system mode
baud                    : Menu to configure baud rate
```

3. usando el comando 'baud' se entra a un menú que permite configurar el baudrate. Al parecer esta configuración es más importante que la del archivo config.txt (modifica epprom)
4. usando el comando 'set' se puede cambiar el <code>exitchar</code> en la opción 5
5. una vez configurados el nuevo baudrate y el nuevo exitchar, se puede volver a revisar en el SerialMonitor de arduino cual de los baudrates permite una correcta comunicación con el openlog.
