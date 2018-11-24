#define SYNTAX_OK	0
#define SEM_OK 0
#define SYNTAX_ERR	2
#define SEM_ERR 3
#define SEM_RUNTIME_ERR 4
#define ARGS_NUMBER_ERR 5
#define OTHER_SEM_ERR 6
#define ZERO_DIV_ERR 9
#define INTERNAL_ERR 99


int PROGRAM(); // program
int FUNC_DEF(); // function definition
int STL(); // statements list
int S(); // statement
int PARAMS(); // function def params
int NEXT_PARAM(); // next parameter in function def params
int ASS(); // assignment
int VALUE(); // value
int FUNC_CALL(); // function call
int INPUT_PARAMS(); // function call's input parameters
int TERM(); // term
int NEXT_TERM(); // next term
int E(); // expression

int parse();