#include "modbus.h"

/***********************Input/Output Coils and Registers***********************/
#if ((READ_HOLDING_REGISTERS_ENABLED > 0)|| (SWRITE_SINGLE_REGISTER_ENABLED > 0) || (WRITE_MULTIPLE_REGISTERS_ENABLED > 0))
PetitRegStructure  Registers       [NUMBER_OF_OUTPUT_REGISTERS];
#endif
