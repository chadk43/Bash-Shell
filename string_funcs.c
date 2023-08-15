#include <unistd.h>
#include <stdio.h>
#include "common.h"


/*
  int main(){
  char text[] = "this is a test";
  char target = '|';
  char targetString[] = "this is a test, oh my goodness";
  printf("%d",string_equal_check(&text[13], targetString));
  string_copy(text, targetString);
  printf("%s",targetString);
  printf("%s",text);
  return 0;
  }
*/


int search_string(char * input, char target){
    int index = -1;
    int i = 0;
    while (input[i] != target && input[i] != '\0'){
	i++;
    }
    if(input[i] == target){
	index = i;
    }
    return index;
}

int search_substring(char * input, char * target){
    int start_i = 0;
    int i = 0;
    int target_i = -1;
    while(input[i] != '\0'){
	while (input[i] != target[0] && input[i] != '\0'){
	    i++;
	}   
	if(input[i] == target[0]){
	    start_i = i;
	    target_i = 0;
	    while(input[start_i] == target[target_i] && input[start_i] != '\0'){
		start_i++;
		target_i++;
	    }
	    if (target[target_i] == '\0')
		return i;	
	}
	i++;
    }
    
    return -1;
}

int string_equal_check(char * first, char * second){
    int same = 1;
    int i = 0;
    while(first[i] != '\0' && second[i] != '\0'){
	if(first[i] != second[i]){
	    same = 0;
	}
	i++;
    }
//    if(first[i] != second[i]){
	
//	return 777; //this is a flag for error that I thought was recquired but apparently is not.
//	same = 0;
//    }
    return same;
}

void string_copy(char * source, char * destination){
    int index = 0;
    while(source[index] != '\0'){
	destination[index] = source[index];
	index++;
    }
    destination[index] = '\0';
    return;
}


void string_shorten(char * source, int index){
    while(source[index] != '\0'){
	source[index] = source[index + 1];
	index++;
    }
}

void string_remove_spaces(char * source){
    int index = 0;
    while(source[index] != '\0'){
	if(source[index] == ' ' || source[index] == '\n'){
	    string_shorten(source,index);
	}else{
	    index++;
	}
    }
}

void string_trim(char * source, int index){
    while(source[index] != '\0'){
	source[index] = '\0';
	index++;
    }
}

void remove_leading_spaces(char * source){
    int index = 0;
    while(source[index] == ' '){
	string_shorten(source,index);
    }
}
