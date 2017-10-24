//
//  main.c
//  Joseph_Monkey_King
//
//  Created by LI YANZHE on 25/10/2017.
//  Copyright © 2017 Yanzhe Lee. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#define OK 0
#define OVERFLOW -1
#define ERROR -2

typedef struct{
	int next;
	int data;
}SCirList;

int INIT_MEMORY(SCirList* mem,int len)
{
	for (int i=0; i<len-1; ++i) {
		mem[i].next=i+1;
	}
	mem[len-1].next=0;
	return OK;
}

int NEW_MEMORY(SCirList* mem)
{
	int top=mem[0].next;
	if (top) {
		mem[0].next=mem[top].next;
	}
	return top;
}

void FREE_MEM(SCirList* mem,int index)
{
	mem[index].next=mem[0].next;
	mem[0].next=index;
}

int CREATE_CIRCLE(SCirList* circle,int n)
{
	int prev=0,q=0;
	for (int i=n; i>0; --i) {
		int p=NEW_MEMORY(circle);
		if (!p){
			return OVERFLOW;
		}
		circle[p].data=i;
		circle[p].next=prev;
		prev=p;
		if (i==n){
			q=p;
		}
	}
	circle[q].next=prev;
	return OK;
}

//int CIRCLE_LENGTH(SCirList* circle)
//{
//	int count=0,p=1;
//	while (circle[p].next!=1) {
//		++count;
//		p=circle[p].next;
//	}
//	return count+1;
//}

/**
 Find the last one in circle and get store stack out order
 @param queue : store stack out order, length=persons
 */
int FIND_KING(int startPos,int persons,int startDiv,int* queue)
{
	if (startPos>persons) return ERROR;
	SCirList* circle=(SCirList*)calloc(persons+1, sizeof(SCirList));
	if (!circle) return OVERFLOW;
	if (INIT_MEMORY(circle,persons+1)!=OK||CREATE_CIRCLE(circle, persons)!=OK) return ERROR;
	int head=circle[1].next;
	int p=head,curDiv=startDiv,resultCount=0;
	while (p) {
		for (int i=1; i<curDiv-1; ++i) {
			p=circle[p].next;
		}
		const int q=circle[p].next;
		queue[resultCount]=circle[q].data;
		++resultCount;
		if (circle[p].next==p) {
			p=0;
		}else{
			circle[p].next=circle[q].next;
			if (curDiv!=2) {
				p=circle[p].next;
			}
			FREE_MEM(circle,q);
		}
		if (curDiv>1) {
			--curDiv;
		}else{
			curDiv=startDiv;
		}
	}
	free(circle);
	return OK;
}

/* Safe flush stdin buffer */
void Safe_Flush(FILE *fp)
{
	int ch;
	while ((ch = fgetc(fp)) != EOF && ch != '\n');
}

/* Safe input integer */
void SafeInputParameter(char *display,int *para)
{
	while (1) {
		puts(display);
		if (scanf("%d",para)!=1) {
			puts("Input error");
			Safe_Flush(stdin);
			continue;
		}
		break;
	}
}

int main(int argc, const char * argv[]) {
	int startPos,div,persons;
	SafeInputParameter("Please input number of monkeys", &persons);
	SafeInputParameter("Please input start position", &startPos);
	SafeInputParameter("Please input division", &div);
	int *que=(int*)calloc(persons, sizeof(int)),status;
	if ((status=FIND_KING(startPos, persons, div,que))!=OK){
		switch (status) {
			case OVERFLOW:
				puts("Static memory overflow");
				break;
			case ERROR:
				puts("Operation error");
				break;
		}
		return status;
	};
	printf("The king is No.%d\n",que[persons-1]);
	puts("Stack out order:");
	if (que) {
		for (int i=0; i<persons; ++i) {
			printf("%-3d\t",que[i]);
		}
		puts("");
	}
	free(que);
	return 0;
}
