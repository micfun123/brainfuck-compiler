#include "bfc.h"

// TODO maybe clean these up
#if defined(CASSERT)
#undef CASSERT
#endif
#define CASSERT(exp) do{if(!(exp)){fprintf(stderr,"%s:%d failed assert '%s'\n",__FILE__,__LINE__,#exp);abort(1);}}while(0)
#define CASSERT(exp, cleanup) do{if(!(exp)){fprintf(stderr,"%s:%d failed assert '%s'\n",__FILE__,__LINE__,#exp);goto cleanup;}}while(0)
#define CASSERT_MSG(exp, msg) do{if(!(exp)){fprintf(stderr,"%s",msg);abort(1);}}while(0)
#define CASSERT_MSG_CLEANUP(exp, msg, cleanup) do{if(!(exp)){fprintf(stderr,"%s",msg);goto cleanup;}}while(0)
#define CASSERT_PRNT(exp, prnt) do{if(!(exp)){prnt;abort(1);}}while(0)
#define CASSERT_PRNT_CLEANUP(exp, prnt, cleanup) do{if(!(exp)){prnt;cleanup;}}while(0)

int main(int argc, char* argv[])
{

    return 0;
}