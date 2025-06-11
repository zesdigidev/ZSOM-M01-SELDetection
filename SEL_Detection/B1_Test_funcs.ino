void FRAM_CLR(){

  Serial.print ("\n Code to clear FRAM received! \n"); 
  serial_Flush();
  INI_FRAM(); 

  return;
}


// Initialize FRAM (0 to 5000 locations)
void INI_FRAM(){

  int temp;
  
  //Serial.print("\n FRAM initialization started! \n"); // for debug
  SPI.beginTransaction(mySetting);  
  for(int i=0; i<5000; i++){

    FRAMWrite32( CS_FRAM, i, 0);
    myusdelay(1);

  }
  SPI.endTransaction();  
  //Serial.print("\n FRAM initialization ended! \n"); // for debug
  
  //check if FRAM is cleared successfully
  Serial.print("\n FRAM check started! \n");  
  SPI.beginTransaction(mySetting);
  for(int i=0; i<5000; i++){

    temp = temp + FRAMRead32( CS_FRAM, i);
    myusdelay(1);

  }
  SPI.endTransaction();  

  if (temp != 0) Serial.print("\n FRAM is not cleared!");
  else Serial.print("\n FRAM cleared!");
  //checking FRAM ended
  
  return;
}

void Test_ADC (){

  Serial.print ("\n Test: Fetch data from ADC! \n"); 
  serial_Flush();
  //RD_ADC(); 

  return;
}

void FRAM_test1(){

  serial_Flush();

  for(int i=0; i<10; i++)
    {
      int fram_check = fram_test();

      //Serial.print(" fram_check = ");
      //Serial.println(fram_check);
      Serial.print("\n fram_check = " + String(fram_check));
      mymsdelay(1000);
    }

  return;
  


}





