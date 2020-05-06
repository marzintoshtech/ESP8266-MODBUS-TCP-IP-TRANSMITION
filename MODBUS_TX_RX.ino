void TxRx(){
  // Check if a client has connected // Modbus TCP/IP
  WiFiClient client = MBServer.available();

  if (!client) {
    return;
  }


  boolean flagClientConnected = 0;
  byte byteFN = MB_FC_NONE;
  int Start;
  int WordDataLength;
  int ByteDataLength;
  int MessageLength;

  // Modbus TCP/IP
  while (client.connected()) {
    digitalWrite(ServerConnected, HIGH);
    if (client.available())
    {
      flagClientConnected = 1;
      int i = 0;
      while (client.available())
      {
        ByteArray[i] = client.read();
        i++;
      }

      client.flush();
      ///// code here
      ///////// Holding Register [0] A [9] = 10 Holding Registers Writing
      ReadTemp();
      MBHoldingRegister[0] = TempBuffer1;
      MBHoldingRegister[1] = TempBuffer2;
      MBHoldingRegister[2] = TempBuffer3;
      MBHoldingRegister[3] = TempBuffer4;
      MBHoldingRegister[4] = TempBuffer5;
      MBHoldingRegister[5] = analogRead(analogInPin);
      MBHoldingRegister[6] = analogRead(analogInPin) * 10;
      MBHoldingRegister[7] = TempBuffer6;
      MBHoldingRegister[8] = TempBuffer7;
      MBHoldingRegister[9] = TempBuffer2 / 100;
      //--------------------------------------------------------
      int Temporal[10];

      Temporal[0] = MBHoldingRegister[10];
      Temporal[1] = MBHoldingRegister[11];
      Temporal[2] = MBHoldingRegister[12];
      Temporal[3] = MBHoldingRegister[13];
      Temporal[4] = MBHoldingRegister[14];
      Temporal[5] = MBHoldingRegister[15];
      Temporal[6] = MBHoldingRegister[16];
      Temporal[7] = MBHoldingRegister[17];
      Temporal[8] = MBHoldingRegister[18];
      Temporal[9] = MBHoldingRegister[19];

      if (Temporal[0] != 0) {
      Serial.println(Temporal[0]); 
      Temporal[0] = 0;
      digitalWrite(PreseceBT, HIGH);
      } else { 
      digitalWrite(PreseceBT, LOW);  
      }



      //// rutine Modbus TCP
      byteFN = ByteArray[MB_TCP_FUNC];
      Start = word(ByteArray[MB_TCP_REGISTER_START], ByteArray[MB_TCP_REGISTER_START + 1]);
      WordDataLength = word(ByteArray[MB_TCP_REGISTER_NUMBER], ByteArray[MB_TCP_REGISTER_NUMBER + 1]);
    }

    // Handle request

    switch (byteFN) {
      case MB_FC_NONE:
        break;

      case MB_FC_READ_REGISTERS: // 03 Read Holding Registers
        ByteDataLength = WordDataLength * 2;
        ByteArray[5] = ByteDataLength + 3; //Number of bytes after this one.
        ByteArray[8] = ByteDataLength; //Number of bytes after this one (or number of bytes of data).
        for (int i = 0; i < WordDataLength; i++)
        {
          ByteArray[ 9 + i * 2] = highByte(MBHoldingRegister[Start + i]);
          ByteArray[10 + i * 2] = lowByte(MBHoldingRegister[Start + i]);
        }
        MessageLength = ByteDataLength + 9;
        client.write((const uint8_t *)ByteArray, MessageLength);

        byteFN = MB_FC_NONE;

        break;


      case MB_FC_WRITE_REGISTER: // 06 Write Holding Register
        MBHoldingRegister[Start] = word(ByteArray[MB_TCP_REGISTER_NUMBER], ByteArray[MB_TCP_REGISTER_NUMBER + 1]);
        ByteArray[5] = 6; //Number of bytes after this one.
        MessageLength = 12;
        client.write((const uint8_t *)ByteArray, MessageLength);
        byteFN = MB_FC_NONE;
        break;

      case MB_FC_WRITE_MULTIPLE_REGISTERS: //16 Write Holding Registers
        ByteDataLength = WordDataLength * 2;
        ByteArray[5] = ByteDataLength + 3; //Number of bytes after this one.
        for (int i = 0; i < WordDataLength; i++)
        {
          MBHoldingRegister[Start + i] = word(ByteArray[ 13 + i * 2], ByteArray[14 + i * 2]);
        }
        MessageLength = 12;
        client.write((const uint8_t *)ByteArray, MessageLength);
        byteFN = MB_FC_NONE;

        break;
    }
  }
  digitalWrite(ServerConnected, LOW);  
}
