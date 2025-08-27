
void FRAMWrite32(int CS_pin, int address, int value, int N_BYTE) {
  
  unsigned int temp_data [N_BYTE];

  for (int i=0; i<N_BYTE; i++){
    temp_data [i] = (value) >> ((8*(i+1)) - 8);
  }

  // for Debug
  /*
  Serial.print("\n temp_4->" + String(temp_4) + "\n" );   
  Serial.print("\n temp_3->" + String(temp_3) + "\n" ); 
  Serial.print("\n temp_2->" + String(temp_2) + "\n" ); 
  Serial.print("\n temp_1->" + String(temp_1) + "\n" );
  */
  //Debug ends 

  // take the CS pin low to select the chip:
  digitalWrite(CS_pin, LOW); 
  myusdelay(10);

  //  send in the address and value via SPI:
  SPI.transfer(0x06);
  myusdelay(10);
  digitalWrite(CS_pin, HIGH);
  myusdelay(10);
  digitalWrite(CS_pin, LOW); 
  SPI.transfer(0x02);
  SPI.transfer(highByte(address));  // uncomment when paring with 48L640
  //SPI.transfer(midByte(address));     // comment when paring with 48L640
  SPI.transfer(lowByte(address));

  for(int i = 0; i<N_BYTE; i++){

    SPI.transfer(temp_data [i]);
  }
 
  // take the SS pin high to de-select the chip:
  myusdelay(10);
  digitalWrite(CS_pin, HIGH); 

  return;  
}


unsigned int FRAMRead32(int CS_pin, int address, int N_BYTE ) {

  unsigned int temp_data[N_BYTE];
  unsigned int result = 0;

  // take the SS pin low to select the chip:
  digitalWrite(CS_pin, LOW); 
  myusdelay(10);

  //  send in the address and value via SPI:
  SPI.transfer(0x03);
  SPI.transfer(highByte(address));  // uncomment when paring with 48L640
  //SPI.transfer(midByte(address));     // comment when paring with 48L640
  SPI.transfer(lowByte(address));
  
  for(int i=0; i< N_BYTE; i++){

    temp_data[i] = SPI.transfer(0x00);

  }

  // for Debug
  /*
  Serial.print("\n read temp_4->" + String(temp_4) + "\n" ); // for Debug  
  Serial.print("\n read temp_3->" + String(temp_3) + "\n" ); // for Debug
  Serial.print("\n read temp_2->" + String(temp_2) + "\n" ); // for Debug
  Serial.print("\n read temp_1->" + String(temp_1) + "\n" ); // for Debug 
  */
  //Debug ends 
  
  // take the SS pin high to de-select the chip:
  myusdelay(10);
  digitalWrite(CS_pin, HIGH); 

  //calculate 8/16/24/32 bit result
  result = (((temp_data[3]) << 24) & (N_BYTE >= 4 ? 0xFFFFFFFF : 0x00000000)) | (((temp_data[2]) << 16) & (N_BYTE >= 3 ? 0xFFFFFFFF : 0x00000000)) | (((temp_data[1]) << 8) & (N_BYTE >= 2 ? 0xFFFFFFFF : 0x00000000)) | temp_data[0];  

  return (result);
  
}

int ADC_READ(int CS_pin, uint8_t address)   // SPI read func for ADC
{
  int temp_data1, temp_data2;
  int temp_data=0;
  address = 4;
  //Serial.print("\n address = " + String(temp_data1));
  
  digitalWrite(CS_pin, LOW); 
  myusdelay(1);
  temp_data1 = SPI.transfer(0x04);
  temp_data2 = SPI.transfer(0x00);
  myusdelay(1);
  digitalWrite(CS_pin, HIGH); 

  //Serial.print("\n temp_data1 = " + String(temp_data1));
  //Serial.print("\n temp_data2 = " + String(temp_data2));

  temp_data = temp_data + ((temp_data1) << 8); //update 3rd MSB byte of error counter
  
  temp_data = temp_data + temp_data2; //update LSB byte of error counter  

  return temp_data;

}