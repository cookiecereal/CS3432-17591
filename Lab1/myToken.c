#include <stdlib.h>
#include <stdio.h>

//counts the number of characters for each word
int countLengthWord(char *str, char delim){
  int wordLength = 0;
  char* pStr = str; 
  for(*pStr; *pStr != delim && *pStr != '\0'; pStr++)//looks throught the the word one by one till it reaches the end
  {
    wordLength++; //adds to the number of words 
  }
  return wordLength; //returns the total length of the word
}
//counts the number of words
int countWords(char *str, char delim){
  int numWords, index;
  numWords = 0;
  char *pStr;
  pStr = str;
  if(*pStr == '\0')//if the word is empty or nothing is returned
  {
    numWords = 0;
  }
  else{
    if(pStr[0] != delim)//if their i a word it checks the position 
	{
      numWords++;
    }
    while(*pStr != '\0')//and keeps looking till theirs no more
	{
      if(*pStr == delim)//once it reaches the end of the word
	  {
	if(*(pStr + 1) != delim){
	  if(*(pStr + 1) != '\0'){
	    numWords++;
	  }
	}
      }
      pStr++;
    }
  }
  return numWords;
}
//my token that will take the two previus methods 
char ** mytoc(char *str, char delim){
  char ** vector;
  char * pStr;
  pStr = str;
  int index, wordLength, position;
  int numWords = countWords(str, delim);
  vector = (char **)calloc(numWords + 1, sizeof(char *));    //allocates n + 1
  position = 0;
  while(*pStr != '\0'){
    wordLength = countLengthWord(pStr, delim);
    if(wordLength > 0){                                      //just when there is an actual word
      vector[position] = (char *) malloc((wordLength + 1));  //allocate n + 1
      for(index = 0; index < wordLength; index++, pStr++){   //going from 0 to the length of the word
	vector[position][index] = *pStr;                     //store each character into array
	printf("%c", *pStr);	
      }
      printf("\n");
      vector[position][index] = '\0';                        //add 0 terminator
      position++;
    }
    else      //there is no word there
      pStr++;
  }
  vector[numWords] = '\0';
  return vector;
}


