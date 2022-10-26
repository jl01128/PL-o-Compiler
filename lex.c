//Jackie Lin 

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "compiler.h"
#define MAX_NUMBER_TOKENS 1000
#define MAX_IDENT_LEN 11
#define MAX_NUMBER_LEN 5

lexeme *list;
int lex_index;

int counter = 0;

void printlexerror(int type);
void printtokens();

//find given variable
int getVariable(char* input);
//find given num
int getNumber(char* input);


//lexanalyzer returns the list if no errors
//returns null if there are errors in input
lexeme *lexanalyzer(char *input, int printFlag){
  
  list = NULL;

  int val;
	lex_index = 0;

  //allocate memory, to make list array 
	list = malloc(MAX_NUMBER_TOKENS * sizeof(lexeme));

  char* tokens[] = {"const", "var", "procedure", "call", "when", "do", "elsedo", "while", "do", "do", "od", "read", "write", "odd", "=", "!=", "<", "<=", ">", ">=", "%", "*", "/", "+", "-", "(", ")", ",", ".", ";", ":=", " ", "\n", "//", "\t"};

	char* temp = malloc(1 * sizeof(char));

  // big while loop, will look through every single character one by one to check for type or error
  //continues until zero character is reached from input
	while(1){

    //until end of list is reached
		if(input[counter] == '\0'){
			break;
		}

		// checks up to 9 characters
		temp = realloc(temp, (strlen(tokens[2]) + 1) * sizeof(char));
		strncpy(temp, &input[counter], strlen(tokens[2]));
		temp[9] = '\0';

    //procedure
    if(strcmp(tokens[2], temp) == 0){     //compare given string with reserved words

			counter += strlen(temp);

      //check if variable
			if(isalnum(input[counter])){
				counter -= strlen(temp);
				getVariable(input);
				continue;
			}

      //store given enum in list for return      
			list[lex_index].type = procsym;
			lex_index++;
			continue;
		}

    //checks up to 6 characters
    temp = realloc(temp, (strlen(tokens[6]) + 1) * sizeof(char));
		strncpy(temp, &input[counter], strlen(tokens[6]));
		temp[6] = '\0';

    // elsedo
 		if(strcmp(tokens[6], temp) == 0){

			counter += strlen(temp);
	
			if(isalnum(input[counter])){
				
        counter -= strlen(temp);
				getVariable(input);
				continue;
			}

			list[lex_index].type = elsedosym;
			lex_index++;
			continue;
		}


		//checks up to 5 characters
		temp = realloc(temp, (strlen(tokens[0]) + 1) * sizeof(char));
		strncpy(temp, &input[counter], strlen(tokens[0]));
		temp[5] = '\0';
    
    //const
		if(strcmp(tokens[0], temp) == 0){

			counter += strlen(temp);
	
			if(isalnum(input[counter])){
				counter -= strlen(temp);
				getVariable(input);
				continue;
			}
          
			list[lex_index].type = constsym;
			lex_index++;
			continue;
		}

		// while
		if(strcmp(tokens[7], temp) == 0){

			counter += strlen(temp);
			
			if(isalnum(input[counter])){

				counter -= strlen(temp);
				getVariable(input);
				continue;
			}
          
			list[lex_index].type = whilesym;
			lex_index++;
			continue;
		}

		// write
		if(strcmp(tokens[12], temp) == 0){
			
      counter += strlen(temp);
			
			if(isalnum(input[counter])){
				counter -= strlen(temp);
				getVariable(input);
				continue;
			}
           
			list[lex_index].type = writesym;
			lex_index++;
			continue;
		}

		//checks up to 4 characters
		temp = realloc(temp, (strlen(tokens[11]) + 1) * sizeof(char));
		strncpy(temp, &input[counter], strlen(tokens[11]));
		temp[4] = '\0';

    //read
		if(strcmp(tokens[11], temp) == 0){
			
      counter += strlen(temp);
		
			if(isalnum(input[counter])){

				counter -= strlen(temp);
				getVariable(input);
				continue;
			}
           
			list[lex_index].type = readsym;
			lex_index++;
			continue;
		}

		// call
		if(strcmp(tokens[3], temp) == 0){
			
      counter += strlen(temp);
			
			if(isalnum(input[counter])){
				counter -= strlen(temp);
				getVariable(input);
				continue;
			}
     
			list[lex_index].type = callsym;
			lex_index++;
			continue;
		}
		
    // when 
		if(strcmp(tokens[4], temp) == 0){

			counter += strlen(temp);
			
			if(isalnum(input[counter])){

				counter -= strlen(temp);
				getVariable(input);
				continue;
			}
           
			list[lex_index].type = whensym;
			lex_index++;
			continue;
		}

    //checks up to 3 characters
		temp = realloc(temp, (strlen(tokens[1]) + 1) * sizeof(char));
		strncpy(temp, &input[counter], strlen(tokens[2]));
		temp[3] = '\0';

		// var
		if(strcmp(tokens[1], temp) == 0){

			counter += strlen(temp);
			
			if(isalnum(input[counter])){
				counter -= strlen(temp);
				getVariable(input);
				continue;
			}
    
			list[lex_index].type = varsym;
			lex_index++;
			continue;
		}

		// odd
		if(strcmp(tokens[13], temp) == 0){
			counter += strlen(temp);
		
			if(isalnum(input[counter])){

				counter -= strlen(temp);
				getVariable(input);
				continue;
			}
    
			list[lex_index].type = oddsym;
			lex_index++;
			continue;
		}

    //checks up to 2 characters
	  temp = realloc(temp, (strlen(tokens[10]) + 1) * sizeof(char));
	  strncpy(temp, &input[counter], strlen(tokens[10]));
	  temp[2] = '\0';

    // od
	  if(strcmp(tokens[10], temp) == 0){

		  counter += strlen(temp);
		  
		  if(isalnum(input[counter])){
			  counter -= strlen(temp);
			  getVariable(input);
			  continue;
		  }
        
		  list[lex_index].type = odsym;
		  lex_index++;
		  continue;
	  }

		// do
		if(strcmp(tokens[8], temp) == 0){

			counter += strlen(temp);
		
			if(isalnum(input[counter])){
				counter -= strlen(temp);
				getVariable(input);
				continue;
			}
           
			list[lex_index].type = dosym;
			lex_index++;
			continue;
		}

		// !=
		if(strcmp(tokens[15], temp) == 0){

			counter += strlen(temp);
            
			list[lex_index].type = neqsym;
			lex_index++;
			continue;
		}

		// <=
		if(strcmp(tokens[17], temp) == 0){

			counter += strlen(temp);
         
			list[lex_index].type = leqsym;
			lex_index++;
			continue;
		}

		// >=
		if(strcmp(tokens[19], temp) == 0){

			counter += strlen(temp);
       
			list[lex_index].type = geqsym;
			lex_index++;
			continue;
		}

		// :=
		if(strcmp(tokens[30], temp) == 0){

			counter += strlen(temp);
        
			list[lex_index].type = assignsym;
			lex_index++;
			continue;
		}

		// //
		if(strcmp(tokens[33], temp) == 0){

			counter++;
			while(1){
				counter++;
				if(input[counter] == '\n'){
					break;
				}
			}
			continue;
		}

		//checks up to 1 character or symbol
		temp = realloc(temp, (strlen(tokens[16]) + 1) * sizeof(char));
		strncpy(temp, &input[counter], strlen(tokens[16]));
		temp[1] = '\0';

    // <
		if(strcmp(tokens[16], temp) == 0){
			counter += strlen(temp);
           
			list[lex_index].type = lsssym;
			lex_index++;
			continue;
		}

		// >
		if(strcmp(tokens[18], temp) == 0){

			counter += strlen(temp);
            
			list[lex_index].type = gtrsym;
			lex_index++;
			continue;
		}

    // =
	  if(strcmp(tokens[14], temp) == 0){

		  counter += strlen(temp);
        
		  list[lex_index].type = eqlsym;
		  lex_index++;
		  continue;
	  }

		// %
		if(strcmp(tokens[20], temp) == 0){

			counter += strlen(temp);
          
			list[lex_index].type = modsym;
			lex_index++;
			continue;
		}

		// *
		if(strcmp(tokens[21], temp) == 0){

			counter += strlen(temp);
           
			list[lex_index].type = multsym;
			lex_index++;
			continue;
		}

		// /
		if(strcmp(tokens[22], temp) == 0){

			counter += strlen(temp);
     
			list[lex_index].type = divsym;
			lex_index++;
			continue;
		}

		// +
		if(strcmp(tokens[23], temp) == 0){

			counter += strlen(temp);
            
			list[lex_index].type = addsym;
			lex_index++;
			continue;
		}

		// -
		if(strcmp(tokens[24], temp) == 0){

			counter += strlen(temp);
          
			list[lex_index].type = subsym;
			lex_index++;
			continue;
		}

		// (
		if(strcmp(tokens[25], temp) == 0){
			counter += strlen(temp);
          
			list[lex_index].type = lparensym;
			lex_index++;
			continue;
		}

		// )
		if(strcmp(tokens[26], temp) == 0){
			counter += strlen(temp);
        
			list[lex_index].type = rparensym;
			lex_index++;
			continue;
		}

		// ,
		if(strcmp(tokens[27], temp) == 0){

			counter += strlen(temp);

			list[lex_index].type = commasym;
			lex_index++;
			continue;
		}

		// .
		if(strcmp(tokens[28], temp) == 0){

			counter += strlen(temp);

			list[lex_index].type = periodsym;
			lex_index++;
			continue;
		}

		// ;
		if(strcmp(tokens[29], temp) == 0){

			counter += strlen(temp);
  
			list[lex_index].type = semicolonsym;
			lex_index++;
			continue;
		} 

    //checks space, newline, tabs pass this 

    // ' '
		if(strcmp(tokens[31], temp) == 0){

			counter += strlen(temp);
			continue;
		}

		// '\n'
		if(strcmp(tokens[32], temp) == 0){
			counter += strlen(temp);
			continue;
		}

		// '\t'
		if(strcmp(tokens[34], temp) == 0){
			counter += strlen(temp);
			continue;
		}

		//check if alphabet var
    if(isalpha(input[counter])){
      
      val = getVariable(input);

			if(val){
        
				printlexerror(val);
				free(temp);
				return NULL;
			}

			continue;
		}
  
    //check if number var
		if(isdigit(input[counter])){

			val = getNumber(input);
			if(val){     
				printlexerror(val);
				free(temp);
				return NULL;
			}
			continue;
		}

		printlexerror(1);
		free(temp);
		return NULL;
	}

	free(temp);

  if(printFlag){
	  printtokens();
  }

  list[lex_index++].type = -1;

	return list;
}

//cehcks if given character is alphabet
int getVariable(char* input){
	
  //length control varibale
  int j = 0;

	while(isalnum(input[counter])){
		
    j++;
		counter++;

    //checks if identifier is within identifier length
    if(j > MAX_IDENT_LEN){
			return 4;
		}
	}
  
  //allocate array to store identifier
	char* temp = malloc((j + 1) * sizeof(char));
	
  strncpy(temp, &input[counter - j], j);
	temp[j] = '\0';

  list[lex_index].type = identsym;
  strcpy(list[lex_index].name, temp);

	lex_index++;
	free(temp);
	return 0;
}

//check if given character is a num
int getNumber(char* input){

  int j = 0;


	while(isalnum(input[counter])){

    j++;
		counter++;

		//check if each given character is a num for a large number
    if(isalpha(input[counter])){
			return 2;
		}

    //check if num length is greater than 5, error has occured
		if(j > MAX_NUMBER_LEN){
			return 3;
		}
	}
  //allocate array to store number
	char* temp = malloc((j + 1) * sizeof(char));

  strncpy(temp, &input[counter - j], j);
	temp[j] = '\0';
 
	list[lex_index].type = numbersym;
	list[lex_index].value = atoi(temp);
	lex_index++;
	free(temp);
	return 0;
}

void printtokens()
{
	int i;
	printf("Lexeme Table:\n");
	printf("lexeme\t\ttoken type\n");
	for (i = 0; i < lex_index; i++)
	{
		switch (list[i].type)
		{
			case oddsym:
				printf("%11s\t%d", "odd", oddsym);
				break;
			case eqlsym:
				printf("%11s\t%d", "==", eqlsym);
				break;
			case neqsym:
				printf("%11s\t%d", "!=", neqsym);
				break;
			case lsssym:
				printf("%11s\t%d", "<", lsssym);
				break;
			case leqsym:
				printf("%11s\t%d", "<=", leqsym);
				break;
			case gtrsym:
				printf("%11s\t%d", ">", gtrsym);
				break;
			case geqsym:
				printf("%11s\t%d", ">=", geqsym);
				break;
			case modsym:
				printf("%11s\t%d", "%", modsym);
				break;
			case multsym:
				printf("%11s\t%d", "*", multsym);
				break;
			case divsym:
				printf("%11s\t%d", "/", divsym);
				break;
			case addsym:
				printf("%11s\t%d", "+", addsym);
				break;
			case subsym:
				printf("%11s\t%d", "-", subsym);
				break;
			case lparensym:
				printf("%11s\t%d", "(", lparensym);
				break;
			case rparensym:
				printf("%11s\t%d", ")", rparensym);
				break;
			case commasym:
				printf("%11s\t%d", ",", commasym);
				break;
			case periodsym:
				printf("%11s\t%d", ".", periodsym);
				break;
			case semicolonsym:
				printf("%11s\t%d", ";", semicolonsym);
				break;
			case assignsym:
				printf("%11s\t%d", ":=", assignsym);
				break;
			case odsym:
				printf("%11s\t%d", "od", odsym);
				break;
			case whensym:
				printf("%11s\t%d", "when", whensym);
				break;
			case elsedosym:
				printf("%11s\t%d", "elsedo", elsedosym);
				break;
			case whilesym:
				printf("%11s\t%d", "while", whilesym);
				break;
			case dosym:
				printf("%11s\t%d", "do", dosym);
				break;
			case callsym:
				printf("%11s\t%d", "call", callsym);
				break;
			case writesym:
				printf("%11s\t%d", "write", writesym);
				break;
			case readsym:
				printf("%11s\t%d", "read", readsym);
				break;
			case constsym:
				printf("%11s\t%d", "const", constsym);
				break;
			case varsym:
				printf("%11s\t%d", "var", varsym);
				break;
			case procsym:
				printf("%11s\t%d", "procedure", procsym);
				break;
			case identsym:
				printf("%11s\t%d", list[i].name, identsym);
				break;
			case numbersym:
				printf("%11d\t%d", list[i].value, numbersym);
				break;
		}
		printf("\n");
	}
	printf("\n");
	printf("Token List:\n");
	for (i = 0; i < lex_index; i++)
	{
		if (list[i].type == numbersym)
			printf("%d %d ", numbersym, list[i].value);
		else if (list[i].type == identsym)
			printf("%d %s ", identsym, list[i].name);
		else
			printf("%d ", list[i].type);
	}
	printf("\n");
}

void printlexerror(int type)
{
	if (type == 1)
		printf("Lexical Analyzer Error: Invalid Symbol\n");
	else if (type == 2)
		printf("Lexical Analyzer Error: Invalid Identifier\n");
	else if (type == 3)
		printf("Lexical Analyzer Error: Excessive Number Length\n");
	else if (type == 4)
		printf("Lexical Analyzer Error: Excessive Identifier Length\n");
	else
		printf("Implementation Error: Unrecognized Error Type\n");
	
	free(list);
	return;
}