#include <aJSON.h>

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
#define SIZE_BUFFER_ENTRADA 256

RF24 radio(9,10);

char buffer_entrada[256];
int ultimo_byte_entrada = 0;

void setup(void)
{ 
  	Serial.begin(115200);
	printf_begin();

	radio.begin();
  	radio.setRetries(15,15);
  	radio.setPayloadSize(8);
		
	radio.openReadingPipe(1,0xF0F0F0F0D1LL);
	radio.openReadingPipe(2,0xF0F0F0F0D2LL);

  	radio.startListening();
	blanquearBufferEntrada();
	
  	radio.printDetails();	
}

void blanquearBufferEntrada(void){
	for(int i=0; i<SIZE_BUFFER_ENTRADA; i++){
		buffer_entrada[i] = '\0';	
	}	
}

void loop(void)
{
	if (radio.available()){
		char char_recibido_radio;
		bool done = false;
		while (!done)
		{
			done = radio.read( &char_recibido_radio, 1);
			Serial.write(char_recibido_radio);
		}
	}
}

void serialEvent() {
	while (Serial.available()) {  
		char byte_leido = Serial.read();
		if(byte_leido == '|'){
			aJsonObject *msg = aJson.parse(buffer_entrada);	
			if (msg) {
				aJsonObject *idNodo = aJson.getObjectItem(msg, "idNodo");
				if (idNodo) {
					uint64_t pipe_escritura;
					if(idNodo->valueint==1) pipe_escritura = 0xF0F0F0F0E1LL;
					if(idNodo->valueint==2) pipe_escritura = 0xF0F0F0F0E2LL;
					
					char *json = aJson.print(msg);
		
					radio.stopListening();
					radio.openWritingPipe(pipe_escritura);
					for(int i=0; i<strlen(json); i++){
						bool ok = false;
						while(!ok){
							ok = radio.write((json + i), 1);
						}
					}
					char final = '|';
					bool ok = false;
					while(!ok){
						ok = radio.write(&final, 1);
					}
					radio.startListening();

					free(json);
					
				}else{
					Serial.println("el mensaje no tiene idNodo");
				}				
			}else{
				Serial.println("error al interpretar json");
			}
			blanquearBufferEntrada();
			ultimo_byte_entrada = 0;
			aJson.deleteItem(msg);
		}else{      
			buffer_entrada[ultimo_byte_entrada] = byte_leido;	
			ultimo_byte_entrada++;
		}
	}
}


