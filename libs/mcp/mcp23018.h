#ifndef MCP23018_H
#define MCP23018_H
#include "../../defines.h"

#define MCP23018_TWI_ADDRESS 0b0100000

uint8_t mcp23018_init(void);
uint8_t mcp23018_update_matrix(bool[ROWS][COLS]);


#endif /* end of include guard: MCP23018_H */
