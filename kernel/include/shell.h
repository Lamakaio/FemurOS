enum commandes {
  NONE,
  ECHO,
  CALC,
  PAGEFAULT,
  EXIT,
  SLEEP,
  PRIME,
  GAME
};

struct strWithLen {
  char * str;
  int len;
};
struct Args {
  struct strWithLen* args;
  int numArgs;
  int background;
};
struct Command {
  const char *text;
  struct strWithLen (*function)(struct Args args);
  int mandatoryArgs;
  const char *opts; //ex : ioap
}; /*Convention : les arguments sont passés dans l'ordre suivant : arguments obligatoires,
puis quelles options ont été choisies (ex : ip) dans l'ordre spécifié, puis options dans le même ordre. */


struct PrefixTree {
  enum commandes id;
  char letter;
  struct PrefixTree **children;
  int numChildren;
};

void init_shell(void);
void shellProcess(void);
