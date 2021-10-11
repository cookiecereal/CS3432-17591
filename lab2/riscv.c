#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h> // malloc & free
#include <stdint.h> // use guaranteed 64-bit integers
#include "tokenizer.h" // Create header file and reference that
#include "memory.h" // built-in functions to read and write to a specific file
#define INPUT_LIM 50

int32_t* reg; // Array of 32 32-bit registers
int lines=0;

void init_regs();
void print_regs();
bool interpret(char* instr);
void write_read_demo();

/**
 * Initialize register array for usage.
 * Malloc space for each 32-bit register then initializes each register to 0.
 * Do not alter this function!
 */
void init_regs(){
	int reg_amount = 32;
	reg = malloc(reg_amount * sizeof(int32_t)); // 32 * 4 bytes
	for(int i = 0; i < 32; i++)
		reg[i] = i;
}

void print_regs(){
	int col_size = 10;
	for(int i = 0; i < 8; i++){
		printf("X%02i:%.*lld", i, col_size, (long long int) reg[i]);
		printf(" X%02i:%.*lld", i+8, col_size, (long long int) reg[i+8]);
		printf(" X%02i:%.*lld", i+16, col_size, (long long int) reg[i+16]);
		printf(" X%02i:%.*lld\n", i+24, col_size, (long long int) reg[i+24]);
	}
}


int string_compare(char str1[], char str2[]){
  int ctr = 0;
  while(str1[ctr] == str2[ctr]){
    if(str1[ctr] == '\0' || str2[ctr] == '\0')
      break;
    ctr++;
  }
  if(str1[ctr] == '\0' && str2[ctr] == '\0')
    return 0;
  else
    return -1;
}

/**
 * Fill out this function and use it to read interpret user input to execute RV64 instructions.
 * You may expect that a single, properly formatted RISC-V instruction string will be passed
 * as a parameter to this function.
 */
bool interpret(char* instr){
  char **token = tokenize(instr); //tokenizes the string
  //created an array containing the first POSSIBLE words
  char *first[] = {"LW", "LD", "SW", "SD", "ADD", "ADDI", "SLLI", "SRLI", "AND", "OR", "XOR"};
  int check_instr;
  char *instru[1];
  char *firstReg[1];
  char *secReg;
 
  int size_first = sizeof first / sizeof *first; //size of of array of first

  //TAKING CARE OF FINDING THE INSTRUCTION
  for(int i = 0; i < size_first; i++){ //will iterate through the list and check if it exists
    check_instr = string_compare(token[0], first[i]);
    if(check_instr == 0){ //if found, it equals 0
      instru[0] = first[i];
      break;
    }
  }
  if(check_instr == 0){
    printf("-> INSTRUCTION: %s \n", instru[0]);
  }
  else{
    return false;
  }
  //THE FIRST REGISTER WILL BE THE SECOND TOKEN
  firstReg[0] = token[1];
  printf("-> DEST REGISTER: %s \n", firstReg[0]);
  char *xOut = strtok(firstReg[0], "X"); //still not tokenized
  int xOut2 = atoi(xOut); //converts string to int
  
  //CHECK IF INSTR = LW or LD or SW or SW 
  if(instru[0] == "LW" || instru[0] == "LD" || instru[0] == "SW" || instru[0] =="SD"){
    secReg = strtok(token[2], "("); //still not tokenized
    char *imm = secReg; //the immediate of the second register
    char *secondReg; //actual second register
    char *copySec;
    while(secReg != NULL){
      secondReg = secReg;
      secReg = strtok(NULL, "(");
    }
    secondReg = strtok(secondReg, ")");
    while(secondReg != NULL){
      copySec = secondReg;
      secondReg = strtok(NULL, ")");
    }
    
    printf("-> IMMEDIATE FOR SECOND REGISTER: %s\n", imm);
    printf("-> SECOND REGISTER: %s\n", copySec);
    char *se = strtok(copySec, "X"); //still not tokenized
    int toInt = atoi(se); //converts string to int
    int Immed = atoi(imm);

    int lines;//this is the error

    if(instru[0] == "LW"){
      reg[xOut2] = reg[toInt]+Immed;
      write_address(reg[xOut2], lines, "mem.txt");
    }
    if(instru[0] == "LD"){
      reg[xOut2] = reg[toInt]+Immed;
      write_address(reg[xOut2], lines, "mem.txt");
    }
    if(instru[0] == "SW"){
      reg[toInt] = reg[toInt] + Immed;
      reg[toInt] = reg[xOut2];
      write_address(reg[toInt], lines, "mem.txt");
    }
    if(instru[0] =="SD"){
      reg[toInt] = reg[toInt] + Immed;
      reg[toInt] = reg[xOut2];
      write_address(reg[toInt], lines, "mem.txt");
    }

    if(lines == 0 || lines%10 == 0){
      //put this in like 0 by creating a variable "0x" then add 08
      lines = lines + 8;
    }else{
      //increment lines + 10
      lines = lines + 10;
    }
    
  } 
  //CHECK "ADD"  or  "AND"  or   "OR"  or   "XOR"
  char *secondRegExtra[1];
  char *thirdRegExtra[1];
  
  //int lines;//this is the error

  if(instru[0] == "ADD" || instru[0] == "AND" || instru[0] == "OR" || instru[0] == "XOR"){
    secondRegExtra[0] = token[2];
    printf("-> SECOND REGISTER: %s\n", secondRegExtra[0]);
    char *xOut3 = strtok(secondRegExtra[0], "X"); //still not tokenized
    int xOut4 = atoi(xOut3); //converts string to int
   
    thirdRegExtra[0] = token[3];
    printf("-> THIRD REGISTER: %s\n", thirdRegExtra[0]);
    char *xOut6;
    xOut6 = strtok(thirdRegExtra[0], "X"); //still not tokenized
    int xOut7 = atoi(xOut6); //converts string to int
    
    if(instru[0] == "ADD"){
      reg[xOut2] = reg[xOut4] + reg[xOut7]; //ADD INSTRUCTION
    }
    if(instru[0] == "AND"){ //AND INSTRUCTION
      reg[xOut2] = reg[xOut4] & reg[xOut7]; 
    }
    if(instru[0] == "OR"){ //OR INSTRUCTION
      reg[xOut2] = reg[xOut4] | reg[xOut7]; 
    }
    if(instru[0] == "XOR"){ //XOR INSTRUCTION
      reg[xOut2] = reg[xOut4] ^ reg[xOut7];
    }
    write_address(reg[xOut2], lines, "mem.txt");
    
    if(lines == 0 || lines%10 == 0){
      //put this in like 0 by creating a variable "0x" then add 08
      lines = lines + 8;
    }else{
      //increment lines + 10
      lines = lines + 10;
    }
    printf("\n");
  }
  //CHECKING IF INSTRUCTION IS   "ADDI"   or   "SLLI"   or   "SRLI" 
  char *immediates[1];
  char *secondRegImm[1];
  if(instru[0] == "ADDI"){
    secondRegImm[0] = token[2];
    immediates[0] = token[3];
    printf("-> SECOND REGISTER %s\n", secondRegImm[0]);
    char *xOut8= strtok(secondRegImm[0], "X"); //still not tokenized
    int xOut9 = atoi(xOut8); //converts string to int
    
    printf("-> IMMEDIATE: %s\n", immediates[0]);
    //write_address(immediates[0], 0x20, "mem.txt");
    reg[xOut9] = atoi(immediates[0]);
    write_address(reg[xOut9],lines, "mem.txt");

    if(lines == 0 || lines%10 == 0){
      //put this in like 0 by creating a variable "0x" then add 08
      lines = lines + 8;
    }else{
      //increment lines + 10
      lines = lines + 10;
    }

  } //---------------------------------------------------------------
  if(instru[0]== "SLLI" || instru[0] == "SRLI"){
    char *sec[1];
    sec[0] = token[2]; //REGISTER 2
    printf("-> SECOND REGISTER %s\n", sec[0]);
    char *xOut10= strtok(sec[0], "X"); //still not tokenized
    int xOut11 = atoi(xOut10); //converts string to int
    
    char *immediates[1];
    immediates[0] = token[3];
    printf("-> IMMEDIATE: %s\n", immediates[0]);
    int howMuch = atoi(immediates[0]);
    char *new;
    if(instru[0] == "SLLI"){
      reg[xOut2] = reg[xOut11] << howMuch;
    }
    
    if(instru[0] == "SRLI"){
      reg[xOut2] = reg[xOut11] >> howMuch;
    }
    
    write_address(reg[xOut2], lines, "mem.txt");
    
    if(lines == 0 || lines%10 == 0){
      //put this in like 0 by creating a variable "0x" then add 08
      lines = lines + 8;
    }else{
      //increment lines + 10
      lines = lines + 10;
    }

  }
  return true;
}


/**
 * Simple demo program to show the usage of read_address() and write_address() found in memory.c
 * Before and after running this program, look at mem.txt to see how the values change.
 * Feel free to change "data_to_write" and "address" variables to see how these affect mem.txt
 * Use 0x before an int in C to hardcode it as text, but you may enter base 10 as you see fit.
 */
void write_read_demo(){
	// Write 4095 (or "0000000 00000FFF") in the 20th address (address 152 == 0x98)
  int32_t data_to_write = 0xFFF;
  int32_t address = 0x98;
  char* mem_file = "mem.txt";

	int32_t write = write_address(data_to_write, address, mem_file);
//	if(write == (int32_t) NULL)
//		printf("ERROR: Unsucessful write to address %0X\n", 0x40);
	int32_t read = read_address(address, mem_file);

//	printf("Read address %lu (0x%lX): %lu (0x%lX)\n", address, address, read, read); // %lu -> format as an long-unsigned
}

/**
 * Your code goes in the main
 *
 */
int main(){
	// Do not write any code between init_regs
	init_regs(); // DO NOT REMOVE THIS LINE

// printf("_______________________________");
// printf("it takes spaces no commas");
// printf("_______________________________");

// char user_input [INPUT_LIM];
// init_regs();//do not reomove
// while(1){
//   printf("Enter an isntruction or use cntr-D to exit");
//   printf("$ ");
//   fgets(user_input,INPUT_LIM,stdin); //get user input
//   char *str = user_input;
//       if (str[0] == '#'){ //FOR NOW EOF = /n
//       break;
//     }
//      bool correctlyExec =  interpret(str); //calls interpret function from riscv.c file
//     if (correctlyExec == 0){ //if not executed correctly
//       printf("---> Sorry. This was not executed correctly.\n");
//     }
// }
//   printf("------------------------\n");
//   printf("-- Printing registers: -- \n");
//   printf("------------------------\n");
//   print_regs();
//   printf("\n");
//   return 0;

//	Below is a sample program to a write-read. Overwrite this with your own code.
	write_read_demo();
	printf(" RV32 Interpreter.\nType RV32 instructions. Use upper-case letters and space as a delimiter.\nEnter 'EOF' character to end program\n");

	char* instruction = malloc(1000 * sizeof(char));
	bool is_null = false;
	// fgets() returns null if EOF is reached.
	is_null = fgets(instruction, 1000, stdin) == NULL;
	while(!is_null){
		interpret(instruction);
		printf("\n");
		print_regs();
		printf("\n");

		is_null = fgets(instruction, 1000, stdin) == NULL;
	}

	printf("Good bye!\n");

	return 0;
}
