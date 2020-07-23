#include "C:/Users/gbranche/Desktop/TestsArduinos/TestPB/src/MeasurementMessage.pb.h"
#include "pb_common.h"
#include "pb.h"
#include "pb_encode.h"
#include <DS18B20.h>
const int DS18B20_PIN=2;
DS18B20 ds(DS18B20_PIN);
float DS18B20_temperature; //réglages température


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
}


void loop() {
  // put your main code here, to run repeatedly:
  uint8_t buffer[30];

MeasurementMessage message = MeasurementMessage_init_zero;
pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
message.temperature = ds.getTempC();
bool status = pb_encode(&stream, MeasurementMessage_fields, &message);

if (!status)
{
    Serial.println("Failed to encode");
    return;
}
Serial.print("Temperature ");
Serial.println(message.temperature);

Serial.print("Message :");

for(int i=0; i<stream.bytes_written;i++){
  serial_printf(Serial1,"%x",buffer[i]);
}

Serial.println();
delay(10000);
}



void serial_printf(HardwareSerial& serial, const char* fmt, ...) { 
    va_list argv;
    va_start(argv, fmt);

    for (int i = 0; fmt[i] != '\0'; i++) {
        if (fmt[i] == '%') {
            // Look for specification of number of decimal places
            int places = 2;
            if (fmt[i+1] >= '0' && fmt[i+1] <= '9') {
                places = fmt[i+1] - '0';
                i++;
            }
            
            switch (fmt[++i]) {
                case 'B':
                    Serial.print("0b"); // Fall through intended
                case 'b':
                    Serial.print(va_arg(argv, int), BIN);
                    break;
                case 'c': 
                    Serial.print((char) va_arg(argv, int));
                    break;
                case 'd': 
                case 'i':
                    Serial.print(va_arg(argv, int), DEC);
                    break;
                case 'f': 
                    Serial.print(va_arg(argv, double), places);
                    break;
                case 'l': 
                    Serial.print(va_arg(argv, long), DEC);
                    break;
                case 'o':
                    Serial.print(va_arg(argv, int) == 0 ? "off" : "on");
                    break;
                case 's': 
                    Serial.print(va_arg(argv, const char*));
                    break;
                case 'X':
                    Serial.print("0x"); // Fall through intended
                case 'x':
                    Serial.print(va_arg(argv, int), HEX);
                    break;
                case '%': 
                    Serial.print(fmt[i]);
                    break;
                default:
                    Serial.print("?");
                    break;
            }
        } else {
            Serial.print(fmt[i]);
        }
    }
    va_end(argv);
}
