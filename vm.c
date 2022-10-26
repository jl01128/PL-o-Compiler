//Jackie Lin 

#include <stdio.h>
#include "compiler.h"

#define MAX_PAS_LENGTH 3000

int pas[MAX_PAS_LENGTH];

//initial values
int IC;                     //INSTRUCTION COUNTER
int GP;                     //GLOBAL POINTER - POINTS TO DATA SEGMENT
int DP;                     //DATA POINTER  - TO ACESS VARIABLE IN MAIN
int FREE;                   //FREE - POINTS TO HEAP
int BP;                     //POINTS TO BASE OF DATA OR ACTIVATION RECORDS
int PC;                     //PROGRAM COUNTER
int SP;                     //STACK POINTER - POINTS TO TOP OF STACK

int base(int L, int BP, int *pas);
void print_execution(int line, char* opname, int *IR, int PC, int BC, int SP, int DP, int *pas, int GP);  


void execute_program(instruction *code, int printFlag){

  //halt condition for P-Machine Cycle
  int flag = 1;

  int i;

  // Set entire pas to 0
  for (i = 0; i < MAX_PAS_LENGTH; i++){
    pas[i] = 0;
  }
  
  int j = 0;
  
  // read in program

	IC = 0;
  //continue until reached end of list(-1)
  while(code[j].opcode != -1){
    pas[IC] = code[j].opcode;
		IC++;
		pas[IC] = code[j].l;
		IC++;
		pas[IC] = code[j].m;
		IC++;
		j++;
  }

  // Initialize the pointers.
  GP = IC;
  DP = IC - 1;
  FREE = IC + 40;
  BP = IC;
  PC = 0;
  SP = MAX_PAS_LENGTH;
    
  if(printFlag){
    printf("\t\t\t\tPC\tBP\tSP\tDP\tdata\n");
    printf("Initial values:\t\t\t%d\t%d\t%d\t%d\n", PC, BP, SP, DP);
  }

  //(UNITL 903 IS REACHED)
  while(flag){
    
    int IR[3];

    //Fetch cycle
    IR[0] = pas[PC];          //OP
    IR[1] = pas[PC+1];        //L
    IR[2] = pas[PC+2];        //M

    //line num of given instruction
    int line = PC / 3;

    //Moves on to next instruction
    PC += 3;

    //Execute cycle
    switch(IR[0]){
      // LIT
      case 1:          
        if(BP == GP){
          DP++;
          pas[DP] = IR[2];
        }
        else{
          SP--;
          pas[SP] = IR[2];
        }

        if(printFlag){
          print_execution(line, "LIT", IR, PC, BP, SP, DP, pas, GP);
        }

        break;

      // OPR
      case 2:
        switch(IR[2]){
          // RTN
          case 0:
                        
            SP = BP + 1;
            PC = pas[SP - 4];
            BP = pas[SP - 3];
            if(printFlag){
              print_execution(line, "RTN", IR, PC, BP, SP, DP, pas, GP);
            }
            break;

          // NEG
          case 1:
                        
            if(BP == GP){
              pas[DP] = -1 * pas[DP];
            }
            else{
              pas[SP] = -1 * pas[SP];
            }
            if(printFlag){
              print_execution(line, "NEG", IR, PC, BP, SP, DP, pas, GP);
            }
            break;

          // ADD
          case 2:
                        
            if(BP == GP){
                DP--;
                pas[DP] = pas[DP] + pas[DP + 1];
            }
            else{
              SP++;
              pas[SP] = pas[SP] + pas[SP - 1];
            }
            if(printFlag){
              print_execution(line, "ADD", IR, PC, BP, SP, DP, pas, GP);
            }
            break;

          // SUB
          case 3:
                      
            if(BP == GP){
              DP--;
              pas[DP] = pas[DP] - pas[DP + 1];
            }
            else{
              SP++;
              pas[SP] = pas[SP] - pas[SP - 1];
            }
            if(printFlag){
              print_execution(line, "SUB", IR, PC, BP, SP, DP, pas, GP);
            }
            break;

          // MUL
          case 4:
                
            if(BP == GP){
              DP--;
              pas[DP] = pas[DP] * pas[DP + 1];
            }
            else{
              SP++;
              pas[SP] = pas[SP] * pas[SP - 1];
            }
            if(printFlag){
              print_execution(line, "MUL", IR, PC, BP, SP, DP, pas, GP);
            }
            break;

          // DIV
          case 5:

            if(BP == GP){
              DP--;
              pas[DP] = pas[DP] / pas[DP + 1];
            }
            else{
              SP++;
              pas[SP] = pas[SP] / pas[SP - 1];
            }
            if(printFlag){
              print_execution(line, "DIV", IR, PC, BP, SP, DP, pas, GP);
            }
            break;

          // ODD
          case 6:
                       
            if(BP == GP){
              pas[DP] = pas[DP] % 2;
            }
            else{
              pas[SP] = pas[SP] % 2;
            }
            if(printFlag){
              print_execution(line, "ODD", IR, PC, BP, SP, DP, pas, GP);
            }
            break;

          // MOD
          case 7:
                        
            if(BP == GP){
              DP--;
              pas[DP] = pas[DP] % pas[DP + 1];
            }
            else{
              SP++;
              pas[SP] = pas[SP] % pas[SP - 1];
            }
            if(printFlag){
              print_execution(line, "MOD", IR, PC, BP, SP, DP, pas, GP);
            }
            break;

          // EQL
          case 8:

            if(BP == GP){
              DP--;
              pas[DP] = pas[DP] == pas[DP + 1];
            }
            else{
              SP++;
              pas[SP] = pas[SP] == pas[SP - 1];
            }
            if(printFlag){
              print_execution(line, "EQL", IR, PC, BP, SP, DP, pas, GP);
            }
            break;

            // NEQ
          case 9:
                        
            if(BP == GP){
              DP--;
              pas[DP] = pas[DP] != pas[DP + 1];
            }
            else{
              SP++;
              pas[SP] = pas[SP] != pas[SP - 1];
            }
            if(printFlag){
              print_execution(line, "NEQ", IR, PC, BP, SP, DP, pas, GP);
            }
            break;

          // LSS
          case 10:
      
            if(BP == GP){
              DP--;
              pas[DP] = pas[DP] < pas[DP + 1];
            }
            else{
              SP++;
              pas[SP] = pas[SP] < pas[SP - 1];
            }
            if(printFlag){
              print_execution(line, "LSS", IR, PC, BP, SP, DP, pas, GP);
            }
            break;

          // LEQ
          case 11:
                        
            if(BP == GP){
              DP--;
              pas[DP] = pas[DP] <= pas[DP + 1];
            }
            else{
              SP++;
              pas[SP] = pas[SP] <= pas[SP - 1];
            }
            if(printFlag){
              print_execution(line, "LEQ", IR, PC, BP, SP, DP, pas, GP);
            }
            break;

          // GTR
          case 12:
  
            if(BP == GP){
              DP--;
              pas[DP] = pas[DP] > pas[DP + 1];
            }
            else{
              SP++;
              pas[SP] = pas[SP] > pas[SP - 1];
            }
            if(printFlag){
              print_execution(line, "GTR", IR, PC, BP, SP, DP, pas, GP);
            }
            break;

          // GEQ
          case 13:
                        
            if(BP == GP){
              DP--;
              pas[DP] = pas[DP] >= pas[DP + 1];
            }
            else{
              SP++;
              pas[SP] = pas[SP] >= pas[SP - 1];
            }
            if(printFlag){
              print_execution(line, "GEQ", IR, PC, BP, SP, DP, pas, GP);
            }
            break;
        }
        break;

      // LOD
      case 3:
                
        if(BP == GP){
          DP++;
          pas[DP] = pas[GP + IR[2]];
        }
        else{
                    
          if(base(IR[1], BP, pas) == GP){
            SP--;
            pas[SP] = pas[GP + IR[2]];
          }
                    
          else{
            SP--;
            pas[SP] = pas[base(IR[1], BP, pas) - IR[2]];
          }
        }
        
        if(printFlag){
          print_execution(line, "LOD", IR, PC, BP, SP, DP, pas, GP);
        }
        break;

      // STO
      case 4:
                
        if(BP == GP){
          pas[GP + IR[2]] = pas[DP];
          DP--;
        }

        else{
                    
          if(base(IR[1], BP, pas) == GP){
            pas[GP + IR[2]] = pas[SP];
            SP++;
          }
                    
          else{
            pas[base(IR[1], BP, pas) - IR[2]] = pas[SP];
            SP++;
          }
        }
        
        if(printFlag){
          print_execution(line, "STO", IR, PC, BP, SP, DP, pas, GP);
        }
        break;

      // CAL
      case 5:
          
        pas[SP - 1] = 0;  
        pas[SP - 2] = base(IR[1], BP, pas);  
        pas[SP - 3] = BP;
        pas[SP - 4] = PC;

        BP = SP - 1;
        PC = IR[2];

          
        if(printFlag){
          print_execution(line, "CAL", IR, PC, BP, SP, DP, pas, GP);
        }
        break;

      //INC
      case 6:
               
        if(BP == GP){
          DP = DP + IR[2];  
        }
        else{
          SP = SP - IR[2];  
        }
                
        if(printFlag){
          print_execution(line, "INC", IR, PC, BP, SP, DP, pas, GP);
        }
        break;

      //JMP
      case 7:
                
        PC = IR[2]; 
        
        if(printFlag){
          print_execution(line, "JMP", IR, PC, BP, SP, DP, pas, GP);
        }
        break;

      //JPC
      case 8:
                
        if(BP == GP){
          
          if(pas[DP] == 0){
            PC = IR[2]; 
          }
                    
          DP--;
        }
        
        else{
          
          if(pas[SP] == 0){
                        
            PC = IR[2];   
          }
                   
          SP++;
        }

        if(printFlag){
          print_execution(line, "JPC", IR, PC, BP, SP, DP, pas, GP);
        }
        break;

      // SYS
      case 9:
        
        switch(IR[2]){

          // (9 0 1)
          case 1:
            
            printf("Top of Stack Value: ");
                       
            if(BP == GP){
              printf("%d\n", pas[DP]);
              DP--;
            }
            else{
              printf("%d\n", pas[SP]);
              SP++;
            }

            if(printFlag){
              print_execution(line, "SYS", IR, PC, BP, SP, DP, pas, GP);
            }
            break;

          // (9 0 2)          
          case(2):
            
            printf("Please Enter an Integer: ");
                        
            if(BP == GP){
              DP++;
              scanf("%d", &pas[DP]);
            }
            else{
              SP--;
              scanf("%d", &pas[SP]);
            }
              
            if(printFlag){
              print_execution(line, "SYS", IR, PC, BP, SP, DP, pas, GP);
            }
            break;

          // (9 0 3)
          case(3):
            
            if(printFlag){
              print_execution(line, "SYS", IR, PC, BP, SP, DP, pas, GP);
            }
                      
            flag = 0;
            break;
        }
          
        break;
    }
  }
}

int base(int L, int BP, int *pas)
{
	int ctr = L;
	int rtn = BP;
	while (ctr > 0)
	{
		rtn = pas[rtn-1];
		ctr--;
	}
	return rtn;
}

void print_execution(int line, char*opname, int *IR, int PC, int BP, int SP, int DP, int *pas, int GP){

  int i;
  //print out instruction and registers
  printf("%2d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t", line, opname, IR[1], IR[2], PC, BP, SP, DP);


  //print data section
  for(i = GP; i <= DP; i++){
    printf("%d ", pas[i]);
  }
  printf("\n");

  //print stack
  printf("\tstack : ");
  for(i = MAX_PAS_LENGTH - 1; i >= SP; i--){
    printf("%d ", pas[i]);
  }
  printf("\n");
}
