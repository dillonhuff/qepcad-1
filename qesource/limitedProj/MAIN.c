#include "qepcad.h"
#include <iostream>
#include "db/convenientstreams.h"
#include <signal.h>
#include "db/CAPolicy.h"

#ifndef _MSC_VER
static void SIGINT_handler(int i, siginfo_t *sip,void* uap);
static void init_SIGINT_handler();
#endif

void QepcadCls::PROJECT_QVARS(Word Fs, Word *t_, Word *F_e_, Word *F_n_, Word *F_s_)
{

  Word A,D,F,F_e,F_n,F_s,Fh,J,P,Q,Ths,f,i,r,t, T;

  // To satisfy Visual Studio Debugger.
  t = 0;
  F_e = 0;
  F_n = 0;
  F_s = 0;

  /* hide Ths,i,t; */
  Word cL,**cC,cr,ce,ci,*cT,cj,cs,cl,ct; /* Chris variables. */
  Word Cs,Ps,Qs,Pps,Cps,Qps,SF; /* Chris variables. */
  char c1,c2; /* Chris variables. */
 Step1: /* Normalize. */
  FIRST4(Fs,&r,&f,&Q,&Fh);
  /*Int*/ PCNSTEP = 1;
  /*Int*/ if (INTERACT()) USERINT(LFS("Before Normalization"),'a');
  /*Int*/ if (PCCONTINUE == TRUE) { goto Return; }
  /*Int*/ Ths = ACLOCK();
  F = NORMQFF(Fh);
  if (GVUA != NIL) GVNA = NORMQFF(GVUA);
  /*Int*/ Ths = ACLOCK() - Ths;
  /*Int*/ TMNORMQFF = Ths;
  /*Int*/ GVNQFF = F;
  //       if (TYPEQFF(F) != UNDET) { t = EQU; F_e = F; goto Return; }

 Step2: /* Projection. */
  if (GVUA != NIL) F = LIST3(ANDOP,GVNA,F);
  A = EXTRACT(r,F);

  if (GVUA != NIL) {
    GVNA = SECOND(F);
    F = THIRD(F); }
  /*Int*/ GVNIP = A;
  /*Int*/ for (i = 1; i <= r; i++) NMNIP[i] = LENGTH(LELTI(A,i));
  /*Int*/ GVPF = LLCOPY(A);
  /*Int*/ GVLV = r;
  /*Int*/ PCNSTEP = 1;
  /*Int if (INTERACT()) USERINT(LFS("After Normalization"),'A'); */
  /*Int PCNSTEP = 1; */


  printf("# of variables = %d\n", LENGTH(GVVL));
  while (A != NIL) {
    SWRITE("A not NIL\n");
    Word dummy;  
    ADV(A, &dummy, &A);

    if (A != NIL) {
      IPDWRITE(r, dummy, GVVL);
    }

  }
  return;

  PROJECT(r,A,&P,&J);

  /*Int*/ if (PCCONTINUE == TRUE) { goto Return; }

 Return: /* Prepare for return. */
  *t_ = t;
  *F_e_ = F_e;
  *F_n_ = F_n;
  *F_s_ = F_s;
  return;
  
}

/*====================================================================
                 main(argc,argv)

Main Algorithm which drives the QEPCAD sytem. 
====================================================================*/
int main(int argc, char **argv) {
  Word Fs,F_e,F_n,F_s,V,t,ac;
  char **av;

 Step1: /* Set up the system. */
  ARGSACLIB(argc,argv,&ac,&av);
  BEGINSACLIB((Word *)&argc);
  BEGINQEPCAD(ac,av);

#ifndef _MSC_VER
  init_SIGINT_handler(); /* A special handler for SIGINT is needed
			    to shut down child processes. Also used
			    for SIGTERM. */
#endif

 Step2: /* Read input, create CAD, write result */
  PCCONTINUE = FALSE;
  PRINTBANNER();
  do {
    // Reinitialize system in between runs
    if (PCCONTINUE == TRUE) 
      { 
	INITSYS();
	PCCONTINUE = FALSE;
      }
    INPUTRD(&Fs,&V);
    QepcadCls Q(V,Fs);
    BTMQEPCAD = ACLOCK();
    Q.PROJECT_QVARS(Fs, &t, &F_e, &F_n, &F_s);

  } while (PCCONTINUE == TRUE);
       
 Step3: /* Clean up the system. */
  SWRITE("\n=====================  The End  =======================\n");
  STATSACLIB();
  ENDQEPCAD();
  ENDSACLIB(SAC_FREEMEM);
  free(av); /* Frees the array malloc'd by ARGSACLIB */

 Return: /* Prepare for return. */
  return 0;
}

#ifndef _MSC_VER
static void SIGINT_handler(int i, siginfo_t *sip,void* uap)
{  
  ENDQEPCAD(); // Kill child CAServer processes
  ENDSACLIB(SAC_FREEMEM);
  exit(1);
}

static void init_SIGINT_handler() 
{
  struct sigaction *p;
  p = (struct sigaction *)malloc(sizeof(struct sigaction));
  p->sa_handler = NULL;
  p->sa_sigaction = SIGINT_handler;
  sigemptyset(&(p->sa_mask));
  p->sa_flags = SA_SIGINFO;
  sigaction(SIGINT,p,NULL);
  sigaction(SIGTERM,p,NULL);
  free(p);
}
#endif
