//Func to flush out Serial1 RX buffer
void serial1_Flush(){
  while(Serial1.available()) {
    char t = Serial1.read();
    
  }
}

//Func to flush out Serial RX buffer
void serial_Flush(){
  while(Serial.available()) {
    char t = Serial.read();
    
  }
}

//Custom us Delay
void myusdelay(int period){

  unsigned long time = micros();
  while ( micros() < time+period){};

}


//Custom ms Delay
void mymsdelay(int period){

  unsigned long time = millis();
  while ( millis() < time+period){};

}

int fram_test(){
  unsigned int A = 0XAA;
  int t_address =5000;
  unsigned int data0 =0 ;
  int temp_data =0;

  mymsdelay(1);
  SPI.beginTransaction(mySetting);
  FRAMWrite32(CS_FRAM, t_address, A);
  SPI.endTransaction();
  mymsdelay(1);

  SPI.beginTransaction(mySetting);
  data0  = FRAMRead32(CS_FRAM, t_address);
  Serial.print("\n fram testdata ->" + String(data0) + "\n" );  //for debug
  SPI.endTransaction();

  if (data0 == A) return 1;

  else return 0;

}