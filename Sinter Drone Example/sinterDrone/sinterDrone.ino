#include <sinter_config.h>
#include <sinter.h>
#include "internal_functions.h"
#pragma GCC diagnostic warning "-fpermissive"
#pragma GCC diagnostic ignored "-Wwrite-strings"

/*
drone_pair();
takeoff();
land();
 */

const unsigned char prog_svm[] = 
{0xAD, 0xAC, 0x05, 0x50, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x44, 0x03, 0x00, 0x0E, 0x44, 0x06, 0x00, 0x0E, 0x44, 0x05, 0x00, 0x46};

const unsigned int prog_svm_len = (int) sizeof(prog_svm)/sizeof(prog_svm[0]);
char heap[0x4000];

// Setup of the program. Only run once at the beginning
void setup() {
  // setting up serial
  
  // or can hardcode the address new byte[6]{0x26, 0xC5, 0xD, 0xD7, 0x93, 0x18});
  delay(1000);
  setupInternals();

  sinter_setup_heap(heap, 0x4000);
  sinter_value_t result;
  sinter_fault_t fault = sinter_run(prog_svm, prog_svm_len, &result);

  Serial.print("Program exited with fault ");
  Serial.print(fault);
  Serial.print(" and result type ");
  Serial.print(result.type);
  Serial.print(" (");
  Serial.print(result.integer_value);
  Serial.print(", ");
  Serial.print(result.float_value);
  Serial.print(")\n");
  Serial.print("Result : ");
  Serial.println(result.string_value);    
}

void loop() {

}
