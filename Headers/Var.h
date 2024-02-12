#ifndef Var
#define Var

#define MAX_CHAR_LABEL 31
#define LINE_LENGTH 81
#define START_POINT 100

enum directive { DATA, STRING, ENTRY, EXTERN, NOT_DIR };
enum operation { MOV, CMP, ADD, SUB, NOT, CLR, LEA, INC, DEC, JMP, BNE, RED, PRN, JSR, RTS, STOP, NOT_OPERATION };
enum parameter { REGISTER, LABEL, NUMBER, NOT_PARAM, NOT_EXIST };

/* There are 3 tables in the file:
 * labels table, data table, external labels table.
 * The data table is one of the fields of the labels table.
 * All tables are implemented using a linked list.
 * We will list each in its place. */
 /*A.1 Data table*/
struct nodeD {
	int x; /* A signal data. Both number data and string data will be stored according to their ascii value.*/
	struct nodeD* next;
};
typedef struct nodeD nodeD;

/*A.2 Labels table*/
struct nodeL {
	char name[MAX_CHAR_LABEL]; /* Label name. */
	int address; /* Label address. If the label has been declared as an entry and not yet initialized, the number -1 will be a flag for us to know that.*/
	nodeD* type; /* Label type: Data label - the pointer will be the head of the data table. Source label - the pointer will be NULL.*/
	enum directive e; /* This field is the beginning of two attributes that can be set to a label: entry, extern. It has three options: ENTRY, EXTERN, NOT_DIR.*/
	struct nodeL* next;
};
typedef struct nodeL nodeL;

typedef struct {
	nodeL* head; /* Head of label table. */
} labelTable;

/*A.3 Extern labels table*/
struct nodeEX {
	char name[MAX_CHAR_LABEL]; /* Label name.*/
	int address; /* Address where a call was made to the extern label*/
	struct nodeEX* next;
};
typedef struct nodeEX nodeEX;

typedef struct {
	nodeEX* head;
}externTable; /* Head of extern labels table.*/

#endif
