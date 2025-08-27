// This program is only for SEL Demo
#include <stdio.h>
#include <stdlib.h>
#include <SPI.h>


#define speedMaximum 10000000
#define dataOrder MSBFIRST
#define dataMode SPI_MODE0

SPISettings mySetting(speedMaximum, dataOrder, dataMode);

const int CS_ADC  = 3;    
const int CS_FRAM = 5;
const int CS_OBC  = 7;

const int LDAP_PG = 4;

int temp_SEL_count = 0;
int temp_data_req  = 0;
int temp_d = 0;
int temp_tm = 0;

unsigned long current_time  = 0; // To check current time
unsigned long prev_time  = 0; // To check current time
unsigned long start_time  = 0; // To check current time
unsigned long time_diff  = 0; // To check current time
unsigned long last_trg  = 0;
unsigned long current_trg = 0;
unsigned long trg_diff = 0;
//Counters use FRAM address
// Counters for SEL 
const int add_SEL = 1100;  // FRAM addresses
const int add_data_req = 2100;
const int add_d = 3100;
const int add_tm = 4100;
//const int add_RST_RF_1 = 4100;  // FRAM addresses
//const int add_RST_RF_2 = 5100;  // FRAM addresses

bool startLoop = false;  // flag to control loop execution
String inputString = ""; // a string to hold incoming serial data

void FRAMWrite32(int CS_pin, int address, int value, int N_BYTE = 1);
unsigned int FRAMRead32(int CS_pin, int address, int N_BYTE = 1 );
int fram_test();

void interruptHandler();

void setup() {
  
  // put your setup code here, to run once:

  pinMode(CS_ADC, OUTPUT);      
  pinMode(CS_FRAM, OUTPUT);
  pinMode(CS_OBC, OUTPUT);
  pinMode(A1, OUTPUT);    // for ADC input

  pinMode(LDAP_PG, INPUT_PULLUP);   
  attachInterrupt(digitalPinToInterrupt(LDAP_PG), interruptHandler, CHANGE); // set LDAP_PG(D4) as interrupt

  digitalWrite(CS_ADC, HIGH);
  digitalWrite(CS_FRAM, HIGH);
  digitalWrite(CS_OBC, HIGH);
  digitalWrite(A1, HIGH);
  
  
  Serial1.begin(9600); // use for comm with FPGA
  while (! Serial1); 

  Serial.begin(9600);
  while (! Serial); 
  //mymsdelay(100);
  Serial.print ("\n Serial done! \n"); 

  SPI.begin();
  Serial.print ("\n SPI done! \n"); 
  //int fram_check = fram_test(); // simple test to write and read data from SRAM to check if it is connected properly
  
  /*
  while (!fram_check){
    
    if(!fram_check){

        //Serial.print("\n FRAM error!"); // for debug
        Serial.print("\n ."); // for debug
        digitalWrite(LED_BUILTIN, HIGH); 
        mymsdelay(100);
        digitalWrite(LED_BUILTIN, LOW); 

    }


  fram_check = fram_test(); // simple test to write and read data from SRAM to check if it is connected properly  

  }
  */
  //mymsdelay(5000);  // for debug
  /*
  if(fram_check ){

    digitalWrite(LED_BUILTIN, HIGH); 
    mymsdelay(10000);
    digitalWrite(LED_BUILTIN, LOW); 

  }
  */
 // for debug
 Serial.print ("\n Startup done! \n"); 

}

void loop() {
  // put your main code here, to run repeatedly:
  
  // Check for incoming serial data
  if (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;

    // If newline received, process the command
    if (inChar == '\n') {
      inputString.trim(); // remove \r or extra spaces

      if (inputString == "status") {
        //Serial.println("Command received. Starting loop actions...");
        startLoop = true;
        start_time = millis();
        prev_time = start_time;
        Serial.print ("\n Ready! \n"); 
        Serial.print (start_time); 
        
      } else {
        Serial.println("Unknown command: " + inputString);
      }

      inputString = ""; // clear for next command
    }
  }

  if (startLoop) {
  current_time =  millis();
  time_diff = current_time - prev_time ;
 // Serial.print ("\n");   
 // Serial.print (time_diff); 
  if (time_diff > 10000)
  {
    
    Serial.print ("\n Runtime:"); 
    int rounded_current_time = round((current_time - start_time)/1000);
    Serial.print (rounded_current_time); 
    Serial.print ("\n"); 
    prev_time = current_time;
    ADC_data_fetch ();


  }

// Check if any Serial data receive from FPGA
  if (Serial1.available()){

    int first_val = Serial1.peek();

    switch (first_val){

      //Check by OBC if the comm is up
      case ':': BP_ACK_UART(); // 0x3A
                break;
      //Request data by OBC     
      case '*': BP_GET_DATA(); // 0x25
                break;
      
      default:  serial1_Flush();          

    }
  }
 
  //  function below will only be invoked by USB uart from PC
  if (Serial.available()){  

    int first_val = Serial.peek();

    switch (first_val){

      //clear FRAM
      case '^': FRAM_CLR(); // 0x5E
                break;
      //Request data by OBC     
      case '*': Test_ADC(); // 0x25
                break;
      //FRAM check for debug     
      case '#': FRAM_test1(); // 
                break;
      
      default:  serial_Flush();          

    }

  }
 
  }

    
}

void ADC_data_fetch (){

  uint8_t address = 0;
  int result = 0;
  int err_count = 0;
  Serial.print ("\n Fetching data from ADC! \n"); // for debug

  SPI.beginTransaction(mySetting); 

  result = ADC_READ(CS_ADC, address);
  SPI.endTransaction();  

  Serial.println(); // for debug
  Serial.print("\n ADC result = " + String(result) + "\n"); // for debug
  //RD_ADC(); 

  /*
  SPI.beginTransaction(mySetting);
  err_count = FRAMRead32( CS_FRAM, add_adc, 4); // reading previous counter value
  FRAMWrite32( CS_FRAM, add_adc, err_count, 4);
  FRAMWrite32( CS_FRAM, add_adc_last_data, result, 4);
  SPI.endTransaction();
  */
  return;
}



void interruptHandler() {
  // This function will be called when the interrupt is triggered
  // Add your interrupt handling code here
  int pin_status;
  current_trg =  millis();
  trg_diff = current_trg - last_trg;


  pin_status = digitalRead(LDAP_PG);
  if(pin_status == 0 && trg_diff > 100)
  {
    last_trg = current_trg;
    Serial.print ("\n SEL detected! \n"); 

  }
  

  temp_SEL_count =  FRAMRead32( CS_FRAM, add_SEL);

  temp_SEL_count++;

  FRAMWrite32( CS_FRAM, add_SEL, temp_SEL_count);

  return;
  
}
