#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define IDSIZE 10
#define NAMESIZE 20
#define BLOCK 2
#define LINESIZE 1024

typedef struct { 			/* a name consists of */
	char last[NAMESIZE];	/* a last name (1 word) */
	char first[NAMESIZE];	/* a first name (1 word) */
} name;

typedef struct {			/* a record consists of */
	char id[IDSIZE];		/* an ID 'a' followed by 8 digits */
	name name;				/* a name e.g., {"simpson", "homer"}, */
	int score;				/* a score between 0 & 100 inclusive */
} record;

typedef struct {	/* a record list consists of */
	record *data;	/* a pointer to a dynamic array of records */
	size_t nalloc;	/* the number of records allocated, and */
	size_t nused;	/* the number of records in use */
} record_list;


void list_init(record_list *plist){
	
	plist->data = 0;
	plist->nalloc = 0;
	plist->nused = 0;
	
	
}

void list_destroy(record_list *plist){
	/* size_t i; */
	plist->nalloc = 0;
	plist->nused = 0;
	
	free(plist->data); 
	/* for(i=0; i < plist->nused; i++){
		free(plist->data[i]);
	} */
	
	/* free(plist); */
	
	printf("list destroyed\n"); 
	
}

int list_insert(record_list *plist, const record *prec){

	if(plist->nalloc == plist->nused){
		record *qlist = realloc(plist->data, (plist->nalloc+BLOCK)*sizeof(record)); /* THIS IS SUPPOSED TO BE A RECORD POINTER NOT A RECORD_LIST POINTER !!!! */
		plist->nalloc += BLOCK;
		if(qlist =='\0'){
			printf("unable to allocate memory\n");
			return 0;
		}
		
		/* plist->data = qlist->data; is the same as (*)plist.data = (*)qlist.data; which is dereferencing */
		plist->data = qlist; /* assignment of address */
		
		
		printf("more memory allocated\n");  
		plist->data[plist->nused++] = *prec; /* insert record into data[] and increment */
		
		printf("record stored\n");
	
	}
	else if(plist->nalloc > plist->nused){
		plist->data[plist->nused++] = *prec; /* insert record into data[] and increment */
		
		printf("record stored\n");
	}
	return 1;
}

int isValid(const char s[]){
	size_t i;
	if(strlen(s) != 9 || s[0]!= 'a'){
		return 0;
	} 
	for(i = 1; s[i]!='\0'; i++){
		if(!isdigit(s[i]))
		return 0;
	}
	
	return 2;
}


int parse_record(record_list *plist, record *prec){  /* parameters changed to accept a record_list */
	char tempId[IDSIZE];
	char tempFirst[NAMESIZE];
	char tempLast[NAMESIZE];
	char buffer[LINESIZE];
	int tempScore;
	size_t i;
	
	printf("Enter ID, Last Name, First Name, Score\n"); 
	while(fgets(buffer, LINESIZE, stdin)){
		printf("Enter ID, Last Name, First Name, Score\n"); 
		
		sscanf(buffer, "%s %s %s %d", tempId, tempLast, tempFirst, &tempScore);
		
		if(isValid(tempId) != 2 || tempScore > 100 || tempScore < 0){
			printf("ID or score invalid, try again\n");
			continue;
		}
		
		if(strlen(tempFirst) > NAMESIZE || strlen(tempLast) > NAMESIZE){
			printf("Name too long");
			continue;
		}
		
		for(i = 0; tempLast[i]!='\0'; i++){
			tempLast[i] = tolower(tempLast[i]);
		}
		
		for(i = 0; i < strlen(tempFirst); i++){
			tempFirst[i] = tolower(tempFirst[i]);
		}
		
		
		strcpy(prec->id, tempId);
		strcpy(prec->name.last, tempLast);
		strcpy(prec->name.first, tempFirst);
		prec->score = tempScore;
		
		list_insert(plist, prec);  
	}
	
	return 1;

}

void print_record(const record_list *plist){
	size_t j;
	for(j = 0; j < plist->nused; j++){
		printf("Record number %lu\n", j+1);
		printf("%s : %s, %s: %d\n", plist->data[j].id, plist->data[j].name.last, plist->data[j].name.first, plist->data[j].score); 
	}
}


int main(){
	record_list a; 
	record r; 
	list_init(&a);
	parse_record(&a, &r); 
	print_record(&a); 
	
	list_destroy(&a);  
	/* print_record(&a);  */
	
	return 0;
}