#define SYNTAX_OK	0
#define SEM_OK 0
#define SYNTAX_ERR	2
#define SEM_ERR 3
#define SEM_RUNTIME_ERR 4
#define ARGS_NUMBER_ERR 5
#define OTHER_SEM_ERR 6
#define ZERO_DIV_ERR 9
#define INTERNAL_ERR 99


static int PROGRAM(); // program
static int FUNC_DEF(); // function definition
static int STL(); // statements list
static int S(); // statement
static int PARAMS(); // function def params
static int NEXT_PARAM(); // next parameter in function def params
static int ASS(); // assignment
static int VALUE(); // value
static int FUNC_CALL(); // function call
static int INPUT_PARAMS(); // function call's input parameters
static int TERM(); // term
static int NEXT_TERM(); // next term
static int E(); // expression

int parse();