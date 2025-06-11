 void BP_ACK_UART()
  {

    Serial1.write(0x5A); // send back acknowledge to BP
    mymsdelay(1);
    serial1_Flush();

    return;
  }


  void BP_GET_DATA()
  {

    serial1_Flush();
    temp_SEL_count =  FRAMRead32( CS_FRAM, add_SEL);

    Serial1.write(0x2A);

    Serial1.write(temp_SEL_count);

    Serial1.write(0xAA);

    temp_data_req = FRAMRead32( CS_FRAM, add_data_req);

    temp_data_req++;

    FRAMWrite32( CS_FRAM, add_data_req, temp_data_req);

    if (temp_data_req/4 != 0) temp_tm = temp_data_req/4;
      else temp_tm = 0;

    if (temp_tm/24 != 0) temp_d = temp_tm/24;
      else temp_d = 0;

    FRAMWrite32( CS_FRAM, add_tm, temp_tm);
    FRAMWrite32( CS_FRAM, add_d, temp_d);

    return;

  }