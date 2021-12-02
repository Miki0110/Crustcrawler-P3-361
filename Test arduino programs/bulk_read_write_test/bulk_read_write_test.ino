#include <DynamixelShield.h>

//Motor IDs
const uint8_t DXL_ID[6] = {0, 1, 2, 3, 4, 5};

const float DXL_PROTOCOL_VERSION = 2.0;
DynamixelShield dxl(Serial3);

//Namespace for motor control table entries
using namespace ControlTableItem;

//for bulk reads and writes
ParamForBulkReadInst_t bulk_readPos_param, bulk_readVel_param;
ParamForBulkWriteInst_t bulk_writePos_param, bulk_writeVel_param;
RecvInfoFromStatusInst_t read_result;

void setup() {
  // put your setup code here, to run once:
  dxl.begin(1000000);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);

  // fill the members of structure for bulkRead
  for(int i=0; i < 3; i++){
     bulk_readPos_param.xel[i].id = DXL_ID[i+1];
     bulk_readPos_param.xel[i].addr = 132; //Present Position on mX
     bulk_readPos_param.xel[i].length = 4;
     bulk_readPos_param.id_count = 3;
    }
   for(int i=0; i < 3; i++){
     bulk_readVel_param.xel[i].id = DXL_ID[i+1];
     bulk_readVel_param.xel[i].addr = 128; //Present Position on mX
     bulk_readVel_param.xel[i].length = 4;
     bulk_readVel_param.id_count = 3;
    }

  
  Serial.begin(1000000);
}

void loop() {
  // put your main code here, to run repeatedly:
  int32_t recv_position_1 = 0, recv_position_2 = 0, recv_position_3 = 0;
  // Print the read data using bulkRead
  for(int i = 0; i < 3;i++){
  Serial.println(dxl.readControlTableItem(RETURN_DELAY_TIME, DXL_ID[i+1]));
  }
  unsigned long start = millis();
  for(int i = 0; i < 100;i++){
  dxl.bulkRead(bulk_readPos_param, read_result);
  }
  unsigned long slut = millis();
  Serial.println("Time to finish bulk read");
  Serial.println((slut - start)/100);

  start = millis();
  for(int i = 0; i < 1000;i++){
  for (int n = 0; n < 3; n++) {
    dxl.getPresentPosition(DXL_ID[n+1], UNIT_RAW);
  }
  }
  slut = millis();
  Serial.println("Time to finish normal call");
  Serial.println((slut - start)/1000);

    start = millis();
  for(int i = 0; i < 1000;i++){
  for (int n = 0; n < 3; n++) {
    dxl.readControlTableItem(PRESENT_POSITION, DXL_ID[n+1]);
    dxl.writeControlTableItem(LED_GREEN, DXL_ID[n+1], 1);
  }
  }
  slut = millis();
  Serial.println("Time to finish readtableitem call");
  Serial.println((slut - start)/1000);
  
  /*
  Serial.println("======= Bulk Read ========");
  memcpy(&recv_position_1, read_result.xel[0].data, read_result.xel[0].length);
  memcpy(&recv_position_2, read_result.xel[1].data, read_result.xel[1].length);
  memcpy(&recv_position_3, read_result.xel[2].data, read_result.xel[2].length);
  Serial.print("ID: ");Serial.print(read_result.xel[0].id);Serial.print(" ");
  Serial.print(", Present Position: ");Serial.print(recv_position_1);Serial.print(" ");
  Serial.print(", Packet Error: ");Serial.print(read_result.xel[0].error);Serial.print(" ");
  Serial.print(", Param Length: ");Serial.print(read_result.xel[0].length);Serial.print(" ");
  Serial.println();

    Serial.print("ID: ");Serial.print(read_result.xel[1].id);Serial.print(" ");
  Serial.print(", Present Position: ");Serial.print(recv_position_2);Serial.print(" ");
  Serial.print(", Packet Error: ");Serial.print(read_result.xel[1].error);Serial.print(" ");
  Serial.print(", Param Length: ");Serial.print(read_result.xel[1].length);Serial.print(" ");
  Serial.println();

    Serial.print("ID: ");Serial.print(read_result.xel[2].id);Serial.print(" ");
  Serial.print(", Present Position: ");Serial.print(recv_position_3);Serial.print(" ");
  Serial.print(", Packet Error: ");Serial.print(read_result.xel[2].error);Serial.print(" ");
  Serial.print(", Param Length: ");Serial.print(read_result.xel[2].length);Serial.print(" ");
  Serial.println();
  delay(200);*/
}
