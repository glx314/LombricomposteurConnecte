#include <Base64.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "lorasender.pb.h"

#include <TheThingsNetwork.h>

// TTN_FP_EU868 or TTN_FP_US915
#define freqPlan TTN_FP_EU868
const char *appEui = "0000000000000000";
const char *appKey = "be4bb8bb192893920451f5db555d7952";

#define loraSerial Serial1
#define debugSerial Serial

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

String input_buffer ;
pb_byte_t decoded_buffer[100] ; // to be refined
char* lora_bin_buffer ;
int lora_bin_buffer_size = 0 ;

lorasender_request request = lorasender_request_init_default ;
pb_ostream_t ostream ; 

bool decode_data (pb_istream_t *stream, const pb_field_t *field, void **arg) {
    Serial.println("Decoding data field");
    Serial.print("size:"); Serial.println(stream->bytes_left);
    Serial.print("buffer:"); Serial.println((char*)stream->state);
    lora_bin_buffer = (char*)stream->state ;
    lora_bin_buffer_size = stream->bytes_left ;
    stream->bytes_left = 0 ;
    return true;
}

void setup() {
  Serial.begin(9600);
  loraSerial.begin(57600);
  ostream = pb_ostream_from_buffer(decoded_buffer, sizeof(decoded_buffer));
  request.data.funcs.decode = &decode_data;

  ttn.onMessage(cb_ttn_message);

  debugSerial.println("-- STATUS");
  ttn.showStatus();

  debugSerial.println("-- JOIN");
  //ttn.join(appEui, appKey);
  ttn.join(appEui, appKey);}

void loop() {
  if (Serial.available()) {
    input_buffer = Serial.readStringUntil('\n');
    if (input_buffer.length()>0) {
      Serial.println("------------------------------");
      Serial.print("- Data received - ");
      Serial.println(input_buffer);
      process_input_buffer();
    }
  }
}

void process_input_buffer() {
    size_t bufsize = Base64.decode((char*)decoded_buffer, (char*)input_buffer.c_str(), input_buffer.length());
    pb_istream_t stream = pb_istream_from_buffer(decoded_buffer, bufsize);

    pb_decode(&stream, lorasender_request_fields, &request);
	Serial.println("Decoded Message:");
	Serial.print("cmd.command : "); Serial.println(request.command);
    if (request.command == _lorasender_request_command_type.SEND) {
    	Serial.print("SEND request received")
		Serial.print("lora_data   : "); Serial.println(lora_bin_buffer);
		Serial.print("inbuf size  : "); Serial.println(lora_bin_buffer_size);
		ttn.sendBytes(lora_bin_buffer,lora_bin_buffer_size);
    }
}

void cb_ttn_message(const byte *payload, size_t length, port_t port)
{
  debugSerial.println("-- MESSAGE RECEIVED");
}