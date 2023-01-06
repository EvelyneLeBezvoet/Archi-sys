typedef enum { C_PLAIN // commande : 0 args
, C_VOID, // parenthèses : 1 arg -> left
// left right sont les args à gauche et à droite de la commande
C_AND // si la première réussi -> exec
, C_OR // si la première fail -> exec
, C_PIPE // redirection input
, C_SEQ // executer après première (;)
 } cmdtype;

struct cmd {
    int type;
    struct cmd *left; //
    struct cmd *right; // 
    // Suite pour PLAIN / VOID (?)
    // *input > cmd > *output
    // ... >> *append
    // ... 2> *error
    char **args; // plain  -> args
    char *input; // cat.txt > ls
    char *output; // > *output
    char *append; // ???
    char *error;
};

struct arglist { // useless for us
    char *arg;
    struct arglist *next;
};

extern struct cmd* parser (char*);
extern void output (struct cmd*,int);
