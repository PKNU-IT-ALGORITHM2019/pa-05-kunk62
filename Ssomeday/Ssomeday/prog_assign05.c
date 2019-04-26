#pragma warning (disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define SIZE 200000
#define com_buf 50
#define w_size 50
#define c_size 15
#define m_size 500
#define buf_len 565

typedef struct node {
	char* word;
	char* wclass;
	char* meaning;

	struct node* left;
	struct node* right;
} Node;

int read_line(FILE* fp, char str[], int n);
void read();
Node* create_node(char* word, char* wclass, char* mean);
Node* find(char* s, int mode);
int insert(Node* n);
void add();
void discard(char* s);
void discardall(char* title);
void upper_lower(char* s);

int count = 0;
Node* head = NULL;

int main() {
	char command_line[com_buf];
	char* cmd, * arg;
	Node* n;
	printf("Wait a sec...\n");
	read();
	printf("dictionary loaded.\n\n");
	
	while (1) {
		printf("$ ");
		if (read_line(stdin, command_line, com_buf) <= 0) continue;
		cmd = strtok(command_line, " ");
		if (strcmp(cmd, "size") == 0)
			printf("%d\n\n", count);
		else if (strcmp(cmd, "exit") == 0)
			exit(1);
		else if (strcmp(cmd, "add") == 0)
			add();
		else if (strcmp(cmd, "find") == 0) {
			arg = strtok(NULL, " ");
			n = find(arg, 1);
		}
		else if (strcmp(cmd, "discard") == 0) {
			arg = strtok(NULL, " ");
			discard(arg);
		}
		else if (strcmp(cmd, "discardall") == 0) {
			arg = strtok(NULL, " ");
			discardall(arg);
		}
		else printf("Command do not exist.\n\n");

	}
	free(n);
	return 0;
}
Node* create_node(char* word, char* wclass, char* mean) {//create & initializing 
	Node* n = (Node*)malloc(sizeof(Node));
	n->word = word;
	n->wclass = wclass;
	n->meaning = mean;
	n->left = NULL;
	n->right = NULL;
	return n;
}

void read() {
	FILE* fp;
	Node* n;
	char buffer[buf_len];
	char* token;
	char* word; char* wclass; char* mean;
	int r;

	if ((fp = fopen("shuffled_dict.txt", "r")) == NULL) {
		printf("error\n"); exit(1);
	}
	while (!feof(fp)) {
		read_line(fp, buffer, buf_len - 1);
		if (strlen(buffer) == 0) break;
		token = strtok(buffer, " ");
		word = _strdup(token);
		token = strtok(NULL, ")");
		wclass = _strdup(token);
		strcat(wclass, ")");
		token = strtok(NULL, "");
		mean = _strdup(token);

		n = create_node(word, wclass, mean);

		if (head == NULL) {
			head = n; count = 1;
		}
		else r = insert(n);
	}
	fclose(fp);
}

int read_line(FILE* fp, char str[], int n) {
	int ch, i = 0;
	while ((ch = fgetc(fp)) != '\n' && ch != EOF)
		if (i < n)
			str[i++] = ch;
	str[i] = '\0';
	return i;
}

int insert(Node * n) {
	Node* p = head;
	while (p != NULL) {
		if (strcmp(n->word, p->word) < 0) {
			if (p->left == NULL) {
				p->left = n;
				count++;
				return 1;
			}
			else { p = p->left; continue; }
		}
		else {
			if (p->right == NULL) {
				p->right = n;
				count++;
				return 2;
			}
			else { p = p->right; continue; }
		}
	}
	return -1;
}

void add() {
	Node* N;
	char* word = (char*)malloc(sizeof(char)*w_size); 
	char* wclass= (char*)malloc(sizeof(char) * c_size);
	char* mean= (char*)malloc(sizeof(char) * m_size);
	char buffer[buf_len];
	int r;

	printf("word: ");
	read_line(stdin, buffer, w_size);
	strcpy(word, buffer);
	printf("wclass: ");
	if (read_line(stdin, buffer, c_size) > 0) {
		strcat(wclass, buffer);
		strcat(wclass, ".)");
	}
	else strcpy(wclass, "");
	printf("meaning: ");
	read_line(stdin, buffer, m_size);
	strcpy(mean, buffer);
	printf("\n");

	N = create_node(word, wclass, mean);
	r = insert(N);
}

Node* find(char* s, int mode) {
	Node* n = head;

	upper_lower(s);
	while ((n != NULL) && (strcmp(n->word, s) != 0)) {
		if (strcmp(n->word, s) > 0)
			n = n->left;
		else
			n = n->right;
	}
	if (mode == 1) {
		if (n == NULL) printf("Not found.\n\n");
		else printf("%s\n\n", n->meaning);
	}
	return n;
}

void discard(char* s) {
	Node* n;
	n = find(s, 0);

}

void discardall(char* title) {
	FILE* fp;
	char buffer[w_size];
	char* s;
	int d_count = 0;

	if ((fp = fopen(title, "r")) == NULL) {
		printf("error\n"); exit(1);
	}
	while (!feof(fp)) {
		read_line(fp, buffer, w_size);
		s = _strdup(buffer);
		discard(s);
		d_count++;
	}
	printf("%d words were discardd successfully.\n", d_count);
	fclose(fp);
}

void upper_lower(char* s) {
	int i = 1;
	char buffer[w_size];
	if (islower(s[0])) buffer[0] = s[0] - 32;
	else buffer[0] = s[0];
	while (s[i] != '\0') {
		if (isupper(s[i])) buffer[i] = s[i] + 32;
		else buffer[i] = s[i];
		i++;

	}
	buffer[i] = '\0';
	strcpy(s, buffer);
}