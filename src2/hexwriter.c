/////////////////////////////////////////////////////////////////////////////
///  hexwriter.c
///  copyright 2024 William R Cooke
///  Writes Intel hex format file
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdint.h>

#define MAX_LINE (16)
static uint16_t address = 0;
static uint16_t lineStart = 0;
static uint16_t lineType = 0;
static uint16_t bytesThisLine = 0;
static uint8_t line[MAX_LINE];

FILE* hexfile;

int hex_newFile(char* filename)
{
  int rtn = 0;
  hexfile = fopen(filename, "w");
  if(hexfile == NULL) 
  {
    rtn = -1;
  }
  else
  {
    address = 0;
    lineStart = 0;
    bytesThisLine = 0;
  }

  return rtn;
}

void hex_writeRecord(void)
{
  uint8_t sum = 0;
  fprintf(hexfile,":");
  fprintf(hexfile,"%02X ",bytesThisLine);  // number data bytes
  sum += bytesThisLine;
  fprintf(hexfile, "%04X ", lineStart);    // record start address
  sum += (lineStart >> 8) & 0xff;
  sum += lineStart & 0xff;
  fprintf(hexfile, "00 "); // record type
  for(int h = 0; h < bytesThisLine; h++)
  {
    fprintf(hexfile, "%02X ", line[h]);
    sum += line[h];
  }
  uint8_t checksum = -sum;
  fprintf(hexfile, "%02X\r\n", checksum);
}

void hex_writeEOF(void)
{
  // Standard is : 00 0000 01 1f
  // but : 00 0000 00 00
  // makes concatenating files easier
  hex_writeRecord();
  fprintf(hexfile, ":0000000000\r\n");
  fclose(hexfile);
}

int hex_setAddress(uint16_t addr)
{
  if(addr != address)
  {
    hex_writeRecord();
    address = addr;
    lineStart = addr;
    bytesThisLine = 0;
  }
}

void hex_addByte(uint8_t byt)
{
  if(bytesThisLine == MAX_LINE)
  {
    hex_writeRecord();
    lineStart = address;
    bytesThisLine = 0;
  }
  line[bytesThisLine++] = byt;
  address++;
}







int main(int argc, char* argv[])
{
  int rtn = 0;
  hex_newFile("test.hex");
  for(int i = 0; i < 507; i++)
  {
    hex_addByte((uint8_t) (i & 0xff));
  }
  hex_setAddress(17200);
  for(int i = 99; i > 0; i -= 3)
  {
    hex_addByte((uint8_t) (i & 0xff));
    printf("%d\n", i & 0xff);
  }
  hex_writeEOF();




  return rtn;
}