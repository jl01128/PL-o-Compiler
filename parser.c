//Jackie Lin

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "compiler.h"

#define MAX_CODE_LENGTH 1000
#define MAX_SYMBOL_COUNT 100

instruction *code;
//array of instruction for the vm for code generation
int cIndex;
//keep track of how many instruction are kept track
symbol *table;
//our symbol table
int tIndex;
//keep track how many items are in the symbol table

//our token list
lexeme *tokens;
//index of current index
int listIdx = 0;
int symIdx = 0;
int level = 0;

void program();
void block();
void const_Declaration();
int var_Declaration();
void procedure_Declaration();
void statement();
void condition();
void expression();
void term();
void factor();

void nextToken();
int multipleDeclarationCheck(lexeme *tokens);
void mark();
int findSymbol(lexeme *tokens, int kind);


void emit(int opname, int level, int mvalue);
void addToSymbolTable(int k, char n[], int v, int l, int a, int m);
void printparseerror(int err_code);
void printsymboltable();
void printassemblycode();



instruction *parse(lexeme *list, int printTable, int printCode){
	code = NULL;

  //allocate memory array
  code = malloc(sizeof(code) * MAX_CODE_LENGTH);
	table = malloc(sizeof(table) * MAX_SYMBOL_COUNT);

  //current token set to beginning elememnt of token list
	tokens = list;

	program();

  /* this line is EXTREMELY IMPORTANT, you MUST uncomment it
		when you test your code otherwise IT WILL SEGFAULT in 
		vm.o THIS LINE IS HOW THE VM KNOWS WHERE THE CODE ENDS
		WHEN COPYING IT TO THE PAS*/
	code[cIndex].opcode = -1;
  
  //print symbol table if given command
  if(printTable == 1){
    printsymboltable();
  }
  //print assembly table if given command
  if(printCode == 1){
    printassemblycode();
  }
	
	return code;
}

void program(){
	
  int i;

  //emit JMP
  emit(7, 0 , 0);
	
	addToSymbolTable(3, "main", 0, 0, 0, 0);

	level = -1;
	
	block();

	if(tokens[listIdx].type != periodsym){
	 	printparseerror(1);
	}

  //emit HAlT
	emit(9,0,3);

	for(i = 0; i < MAX_CODE_LENGTH; i++){
    //OPR 5(CALL)
		if(code[i].opcode == 5){
			code[i].m = table[code[i].m].addr;
		}
	}
	code[0].m = table[0].addr;

}

void block(){
	
	level++;
	int procedure_idx = tIndex - 1;
	
	const_Declaration();
	
	int x = var_Declaration();
	procedure_Declaration();
	table[procedure_idx].addr = cIndex * 3;

	if(level == 0){
    //emit INC
	 	emit(6, 0, x);
	}
	else{
    //emit INC
		emit(6, 0, x + 4);
	}
  
	statement();
  
	mark(tokens);
 
	level--;
	
}

void const_Declaration(){
	
	if(tokens[listIdx].type == constsym){
	 	do{
       
	 		nextToken();
	 		if(tokens[listIdx].type != identsym){
	 			printparseerror(2);
	 		}

	 		symIdx = multipleDeclarationCheck(tokens);

	 		if(symIdx != -1){
	 			printparseerror(18);
	 		}
      //saved indent
	 		char ident[12];
      strcpy(ident, tokens[listIdx].name);

	 		nextToken();

	 		if(tokens[listIdx].type != assignsym){
	 			printparseerror(2);
	 		}

	 		nextToken();

	 		if(tokens[listIdx].type != numbersym){
	 			printparseerror(2);
	 		}

	 		addToSymbolTable(1, ident, tokens[listIdx].value, level, 0, 0);
	 		nextToken();
			
	 	} while(tokens[listIdx].type == commasym);

	 	if(tokens[listIdx].type != semicolonsym){
	 		if(tokens[listIdx].type == identsym){
				printparseerror(13);
	 		}
	 		else{
	 			printparseerror(14);
	 		}
			
	 	}
		nextToken();
	}
}

int var_Declaration(){

	int numVars = 0;

	if(tokens[listIdx].type == varsym){
	  do{
 
	 	  numVars++;

	 		nextToken();
	 		if(tokens[listIdx].type != identsym){
	 			printparseerror(3);
	 		}

	 		symIdx = multipleDeclarationCheck(tokens);

	 		if(symIdx != -1){
	 			printparseerror(18);
			}

			if(level == 0){
	 			addToSymbolTable(2, tokens[listIdx].name, 0, level, numVars - 1, 0);
	 		}
	 		else{
	 			addToSymbolTable(2, tokens[listIdx].name, 0, level, numVars + 3, 0);
	 		}

	 		nextToken();		

	 	} while(tokens[listIdx].type == commasym);
	 	if(tokens[listIdx].type != semicolonsym){

	 		if(tokens[listIdx].type == identsym){
	 			printparseerror(13);
	 		}
	 		else{
				printparseerror(14);
	 		}
	 	}
    
    nextToken();
	}
	return numVars;
}

void procedure_Declaration(){

	while(tokens[listIdx].type == procsym){

		nextToken();
    if(tokens[listIdx].type != identsym){
      printparseerror(4);
    }

    symIdx = multipleDeclarationCheck(tokens);
    
    if (symIdx != -1){
      printparseerror(18);
    }

    addToSymbolTable(3, tokens[listIdx].name, 0, level, 0, 0);

    nextToken();

    if(tokens[listIdx].type != semicolonsym){
      printparseerror(4);
    }
  
    nextToken();
  
    block();
    if(tokens[listIdx].type != semicolonsym){
      printparseerror(18);
    }

    nextToken();
    emit(2,0,0);
	}
}

void statement(){
  int loopIdx;
  int jpcIdx;
  int jmpIdx;
 
	if (tokens[listIdx].type == identsym){
   
		symIdx = findSymbol(tokens, 2);

		if(symIdx == -1){
			if (findSymbol(tokens, 1) != findSymbol(tokens, 3)){
				printparseerror(6);
			}
			else{
				printparseerror(19);
			}
		}

		nextToken();
		if(tokens[listIdx].type != assignsym){
			printparseerror(5);
		}

		nextToken();
		expression();

    //emit STO
		emit(4, level - table[symIdx].level, table[symIdx].addr);
		return;
	}
	if(tokens[listIdx].type == dosym){
  
		do{
			nextToken();
			statement();
		} while (tokens[listIdx].type == semicolonsym);
   
		if(tokens[listIdx].type != odsym){
			if(tokens[listIdx].type == identsym || tokens[listIdx].type == dosym || tokens[listIdx].type == whensym || tokens[listIdx].type == whilesym || tokens[listIdx].type == readsym || tokens[listIdx].type == writesym || tokens[listIdx].type == callsym){
				printparseerror(15);
			}
			else{
				printparseerror(16);
			}
		}
  
		nextToken();
		return;
    
	}
	if(tokens[listIdx].type == whensym){
   
		nextToken();
		condition();
		jpcIdx = cIndex;

    //emit JPC
		emit(8, 0, 0);
		if(tokens[listIdx].type != dosym){
			printparseerror(8);
		}

		nextToken();
		statement();
		if(tokens[listIdx].type == elsedosym){

			jmpIdx = cIndex;
      //emit JMP
			emit(7,0,0);
			code[jpcIdx].m = cIndex * 3;
     
			nextToken();
			statement();
			code[jmpIdx].m = cIndex * 3;
		}
		else{
			code[jpcIdx].m = cIndex * 3;
		}
		return;
	}

	if(tokens[listIdx].type == whilesym){

		nextToken();
		loopIdx = cIndex;
		condition();

		if(tokens[listIdx].type != dosym){
			printparseerror(9);
		}

		nextToken();
		jpcIdx = cIndex;
    //emit JPC
		emit(8, 0, 0);
		statement();
    //emit JMP
		emit(7, 0, loopIdx  * 3);
		code[jpcIdx].m = cIndex * 3;
		return;	
	}
	if(tokens[listIdx].type == readsym){
  
		nextToken();
		if(tokens[listIdx].type != identsym){
			printparseerror(6);
		}

		symIdx = findSymbol(tokens, 2);
		if(symIdx == -1){
			if(findSymbol(tokens, 1) != findSymbol(tokens, 3)){
				printparseerror(7);
			}
			else{	
				printparseerror(19);
			}
		}

		nextToken();
    //emit READ
		emit(9, 0, 2);

    //emit STO
		emit(4, (level - table[symIdx].level) ,table[symIdx].addr);
		return;
	}

	if(tokens[listIdx].type == writesym){
    
		nextToken();
		expression();
    //emit WRITE
		emit(9, 0, 1);
		return;
	}	
	if(tokens[listIdx].type == callsym){
  
		nextToken();

		symIdx = findSymbol(tokens, 3);
		if(symIdx == -1){
			if(findSymbol(tokens, 1) != findSymbol(tokens, 2)){
				printparseerror(7);
			}
			else{
				printparseerror(19);
			}
		}
  
		nextToken();
    //emit CAL
		emit(5,(level - table[symIdx].level), symIdx);
	}
}

void condition(){

	if(tokens[listIdx].type == oddsym){
    
		nextToken();
		expression();
    //emti ODD
		emit(2, 0, 6);
	}
	else{
		expression();
		if(tokens[listIdx].type == eqlsym){
  
			nextToken();
			expression();
      //emit EQL
			emit(2, 0, 8);
		}
		else if(tokens[listIdx].type == neqsym){
  
			nextToken();
			expression();
      //emit NEQ
			emit(2, 0, 9);
		}
		else if(tokens[listIdx].type == lsssym){
 
			nextToken();
			expression();
      //emit LSS
			emit(2, 0, 10);
		}
		else if(tokens[listIdx].type == leqsym) {
  
			nextToken();
			expression();
      //emit LEQ
			emit(2, 0, 11);
		}
		else if(tokens[listIdx].type == gtrsym){
      
			nextToken();
			expression();
      //emit GTR
			emit(2, 0, 12);
		}
		else if(tokens[listIdx].type == geqsym){
    
			nextToken();
			expression();
      //emit GEQ
	    emit(2, 0, 13);
		}
		else{
			printparseerror(10);
		}
			
	}
}

void expression(){
	if(tokens[listIdx].type == subsym){
      
		nextToken();
		term();
    //emit NEG
		emit(2, 0, 1);

		while(tokens[listIdx].type == addsym || tokens[listIdx].type  == subsym){

			if(tokens[listIdx].type  == addsym){
				nextToken();
				term();
        //emit ADD
				emit(2, 0,  2);
			}
			else{
				nextToken();
				term();
        //emit SUB
				emit(2, 0, 3);
			}
		}
	}
	else{
			
    if(tokens[listIdx].type  ==  addsym){
			nextToken();
		}
		term();
		while(tokens[listIdx].type ==addsym || tokens[listIdx].type  == subsym){
				
      if(tokens[listIdx].type  == addsym){
          
				nextToken();
				term();
        //emit ADD
				emit(2, 0,  2);
			}
			else{
				nextToken();
				term();
        //emit SUB
				emit(2, 0, 3);
			}
		}
	}
  if(tokens[listIdx].type == lparensym || tokens[listIdx].type  == identsym || tokens[listIdx].type  == numbersym || tokens[listIdx].type == oddsym){
		printparseerror(17);
	}
}

void term(){

	factor();

	while(tokens[listIdx].type  == multsym || tokens[listIdx].type == divsym || tokens[listIdx].type == modsym){

		if(tokens[listIdx].type == multsym){
    
			nextToken();
			factor();
      //emit MUL
			emit(2, 0, 4);
		}
		else  if(tokens[listIdx].type == divsym){
     
			nextToken();
			factor();
      //emit DIV
			emit(2, 0, 5);
		}
		else{
			nextToken();
			factor();
      //emit MOD
			emit(2, 0, 7);
		}

	}
}

void factor(){
	if(tokens[listIdx].type == identsym){
      
		int symIdx_var = findSymbol(tokens, 2);
		int symIdx_const  = findSymbol(tokens, 1);

		if((symIdx_var  == -1) && (symIdx_const == -1)){
			if(findSymbol(tokens, 3) != -1){
				printparseerror(11);
			}
				else{
					printparseerror(19);
				}
			}
			if(symIdx_var == -1){
        //emit LIT
				emit(1, 0, table[symIdx_const].val);

			}
			else if(symIdx_const == -1 || table[symIdx_var].level > table[symIdx_const].level){
        //emit LOD
				emit(3, level - table[symIdx_var].level, table[symIdx_var].addr);
			}
			else{
        //emit LIT
				emit(1, 0, table[symIdx_const].val);
			}
			nextToken();
		}
		else if(tokens[listIdx].type == numbersym){
      //emit LIT
			emit(1, 0, tokens[listIdx].value);

			nextToken();
		}	
		else if(tokens[listIdx].type == lparensym){
      
			nextToken();
			expression();
			if(tokens[listIdx].type != rparensym){
				printparseerror(12);
			}
			nextToken();
		}
	else{
		printparseerror(19);
	}
}

void nextToken(){
  //increment the index of the index within the token array
	listIdx++;
}

int multipleDeclarationCheck(lexeme *tokens){
  int i;
	
  //search through symbol table
	for(i = 0; i < MAX_SYMBOL_COUNT; i++){
    //find identifier
		if(strcmp(tokens[i].name, table[i].name) == 0 && strcmp(tokens[i].name," ") == 1){
      //unmarked
			if(table[i].mark == 0){
        //equals current level
				if(table[i].level == level){
					return i;
				}
			}
		}
	}
  return -1;
}

int findSymbol(lexeme *tokens, int kind){
  int i;

	for(i = tIndex - 1; i >= 0; i--){
    
    //Matched correct name AND kind value AND unmarked
		if(strcmp(tokens[listIdx].name, table[i].name) == 0 && (kind == table[i].kind) && table[i].mark == 0){
      		return i;
		}
	}
  return -1;
}

void mark(lexeme *tokens){
  int i;

	for(i = MAX_SYMBOL_COUNT; i >= 0; i--){
    //ignore marked entires
		if(table[i].mark == 0){
      //equals current level
			if(table[i].level == level){
				table[i].mark = 1;
			}
		}
    //stops if unmarked or less than current level
		else if(table[i].level < level && table[i].mark == 0){
			break;
		}
	}
}


void emit(int opname, int level, int mvalue)
{

	code[cIndex].opcode = opname;
	code[cIndex].l = level;
	code[cIndex].m = mvalue;
	cIndex++;

}

void addToSymbolTable(int k, char n[], int v, int l, int a, int m)
{
	table[tIndex].kind = k;
	strcpy(table[tIndex].name, n);
	table[tIndex].val = v;
	table[tIndex].level = l;
	table[tIndex].addr = a;
	table[tIndex].mark = m;
	tIndex++;
}


void printparseerror(int err_code)
{
	switch (err_code)
	{
		case 1:
			printf("Parser Error: Program must be closed by a period\n");
			break;
		case 2:
			printf("Parser Error: Constant declarations should follow the pattern 'ident := number {, ident := number}'\n");
			break;
		case 3:
			printf("Parser Error: Variable declarations should follow the pattern 'ident {, ident}'\n");
			break;
		case 4:
			printf("Parser Error: Procedure declarations should follow the pattern 'ident ;'\n");
			break;
		case 5:
			printf("Parser Error: Variables must be assigned using :=\n");
			break;
		case 6:
			printf("Parser Error: Only variables may be assigned to or read\n");
			break;
		case 7:
			printf("Parser Error: call must be followed by a procedure identifier\n");
			break;
		case 8:
			printf("Parser Error: when must be followed by do\n");
			break;
		case 9:
			printf("Parser Error: while must be followed by do\n");
			break;
		case 10:
			printf("Parser Error: Relational operator missing from condition\n");
			break;
		case 11:
			printf("Parser Error: Arithmetic expressions may only contain arithmetic operators, numbers, parentheses, constants, and variables\n");
			break;
		case 12:
			printf("Parser Error: ( must be followed by )\n");
			break;
		case 13:
			printf("Parser Error: Multiple symbols in variable and constant declarations must be separated by commas\n");
			break;
		case 14:
			printf("Parser Error: Symbol declarations should close with a semicolon\n");
			break;
		case 15:
			printf("Parser Error: Statements within do-od must be separated by a semicolon\n");
			break;
		case 16:
			printf("Parser Error: do must be followed by od\n");
			break;
		case 17:
			printf("Parser Error: Bad arithmetic\n");
			break;
		case 18:
			printf("Parser Error: Confliciting symbol declarations\n");
			break;
		case 19:
			printf("Parser Error: Undeclared identifier\n");
			break;
		default:
			printf("Implementation Error: unrecognized error code\n");
			break;
	}
	
	free(code);
	free(table);
  
  exit(0);
}

void printsymboltable()
{
	int i;
	printf("Symbol Table:\n");
	printf("Kind | Name        | Value | Level | Address | Mark\n");
	printf("---------------------------------------------------\n");
	for (i = 0; i < tIndex; i++)
		printf("%4d | %11s | %5d | %5d | %5d | %5d\n", table[i].kind, table[i].name, table[i].val, table[i].level, table[i].addr, table[i].mark); 
	
	free(table);
	table = NULL;
}

void printassemblycode()
{
	int i;
	printf("Line\tOP Code\tOP Name\tL\tM\n");
	for (i = 0; i < cIndex; i++)
	{
		printf("%d\t", i);
		printf("%d\t", code[i].opcode);
		switch (code[i].opcode)
		{
			case 1:
				printf("LIT\t");
				break;
			case 2:
				switch (code[i].m)
				{
					case 0:
						printf("RTN\t");
						break;
					case 1:
						printf("NEG\t");
						break;
					case 2:
						printf("ADD\t");
						break;
					case 3:
						printf("SUB\t");
						break;
					case 4:
						printf("MUL\t");
						break;
					case 5:
						printf("DIV\t");
						break;
					case 6:
						printf("ODD\t");
						break;
					case 7:
						printf("MOD\t");
						break;
					case 8:
						printf("EQL\t");
						break;
					case 9:
						printf("NEQ\t");
						break;
					case 10:
						printf("LSS\t");
						break;
					case 11:
						printf("LEQ\t");
						break;
					case 12:
						printf("GTR\t");
						break;
					case 13:
						printf("GEQ\t");
						break;
					default:
						printf("err\t");
						break;
				}
				break;
			case 3:
				printf("LOD\t");
				break;
			case 4:
				printf("STO\t");
				break;
			case 5:
				printf("CAL\t");
				break;
			case 6:
				printf("INC\t");
				break;
			case 7:
				printf("JMP\t");
				break;
			case 8:
				printf("JPC\t");
				break;
			case 9:
				switch (code[i].m)
				{
					case 1:
						printf("WRT\t");
						break;
					case 2:
						printf("RED\t");
						break;
					case 3:
						printf("HAL\t");
						break;
					default:
						printf("err\t");
						break;
				}
				break;
			default:
				printf("err\t");
				break;
		}
		printf("%d\t%d\n", code[i].l, code[i].m);
	}
	if (table != NULL)
		free(table);
}