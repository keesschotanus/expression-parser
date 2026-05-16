typedef enum { token_operand, token_operator } TokenType;

typedef struct {
        TokenType type;
        union {
                double operand;
                char operator;
        } value;
} Token;

void convert(const char *infix);
double evaluate();

void create_symbol_stack();
void push_symbol(char symbol);
char pop_symbol();
char peek_symbol();
void free_symbol_stack();

void create_operand_stack();
void push_operand(double operand);
double pop_operand();
double peek_operand();
void free_operand_stack();

void create_postfix_list();
void add_postfix_token(Token token);
void print_postfix_list();
void visit_postfix_list(void (*visitor)(const Token *));
void free_postfix_list();

