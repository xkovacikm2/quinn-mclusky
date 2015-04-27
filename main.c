/* 
 * File:   main.c
 * Author: kovko
 *
 * Created on Streda, 2015, apríla 22, 15:29
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

typedef struct expression{
    char *expression;
    char to_final_flag;
    struct expression *next;
}EXPRESSION;

EXPRESSION *final_first=NULL, *final_last=NULL;

void add_to_final_list(EXPRESSION *adder);
EXPRESSION *load_entry_list(int n_variables);
void optimalize(EXPRESSION *first, int n_variables);
void print_final_list();
EXPRESSION *expression();
void add_to_list(EXPRESSION **first, EXPRESSION **last, EXPRESSION *new);
char *reducable_to(EXPRESSION *active, EXPRESSION *compared, int n_variables);
void free_final_list();

int main() {
    int n_variables;
    EXPRESSION *entry_list_first=NULL;
    
    printf("Number of variables: ");
    scanf("%d", &n_variables);
    getchar();
    
    entry_list_first=load_entry_list(n_variables);    
    optimalize(entry_list_first, n_variables);
    print_final_list();
    
    return (EXIT_SUCCESS);
}

void add_to_final_list(EXPRESSION *adder){
    EXPRESSION *add=NULL;
    
    add=expression();
    *add=*adder;
    add->next=NULL;
    
    if(final_first==NULL){
        final_first=final_last=add;
    }
    else{
        final_last->next=add;
        final_last=add;
    }
}

EXPRESSION *load_entry_list(int n_variables){
    int i;
    char c, buf[n_variables+1];
    EXPRESSION *first=NULL, *last=NULL, *new=NULL;
    
    while(scanf("%[^+\n]+", buf)>0){
        new=expression();
        new->expression=strdup(buf);
        add_to_list(&first, &last, new);
    }
    
    return first;
}

void optimalize(EXPRESSION *first, int n_variables){
    EXPRESSION *active=NULL, *compared=NULL, *new_list_first=NULL, *new_list_last=NULL, *new=NULL, *temp=NULL;
    char *buf;
    
    active=first;
    while(active!=NULL){
        compared=active->next;
        while(compared!=NULL){
            
            if((buf=reducable_to(active, compared, n_variables))!=NULL){
                active->to_final_flag=FALSE;
                compared->to_final_flag=FALSE;
                new=expression();
                new->expression=strdup(buf);
                add_to_list(&new_list_first, &new_list_last, new);
            }
            
            compared=compared->next;
        }
        active=active->next;
    }
    
    while(first!=NULL){
        if(first->to_final_flag)
            add_to_final_list(first);
        temp=first->next;
        free(first);
        first=temp;
    }
    
    if(new_list_first!=NULL)
        optimalize(new_list_first, n_variables);
}

void print_final_list(){
    EXPRESSION *temp=final_first;
    while(temp!=NULL){
        printf("%s", temp->expression);
        temp=temp->next;
        if(temp!=NULL)
            putchar('+');
    }
}

EXPRESSION *expression(){
    EXPRESSION *new=(EXPRESSION*)malloc(sizeof(EXPRESSION));
    new->next=NULL;
    new->to_final_flag=TRUE;
    return new;
}

void add_to_list(EXPRESSION **first, EXPRESSION **last, EXPRESSION *new){
    if (*first==NULL){
        *first=*last=new;
    }
    else{
        (*last)->next=new;
        *last=new;
    }
}

char *reducable_to(EXPRESSION *active, EXPRESSION *compared, int n_variables){
    char *buf;
    int diff_position=-1, i;
    
    //if equal return first - easiest case
    if(strcmp(active->expression, compared->expression)==0)
        return active->expression;
    
    //if not equal 
    for(i=0; i<n_variables; i++){
        //if # not on same position
        if((active->expression[i]=='#' && compared->expression[i]!='#') || (active->expression[i]!='#' && compared->expression[i]=='#')){
            return NULL;
        }
        //if they differ on same position
        if(active->expression[i]!=compared->expression[i]){
            //if they differ more than once
            if(diff_position>=0)
                return NULL;
            diff_position=i;
        }
    }
    
    buf=strdup(active->expression);
    buf[diff_position]='#';
    return buf;
}
