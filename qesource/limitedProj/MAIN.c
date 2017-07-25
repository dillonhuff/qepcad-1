#include "qepcad.h"
#include <iostream>
#include "db/convenientstreams.h"
#include <signal.h>
#include "db/CAPolicy.h"

#include <cassert>

#ifndef _MSC_VER
static void SIGINT_handler(int i, siginfo_t *sip,void* uap);
static void init_SIGINT_handler();
#endif

void PRINT_PROJECTION_POLY_LIST(Word r, Word GVVL, Word poly_list) {
  while (poly_list != NIL) {
    //SWRITE("\tpoly list elem\n");
    Word poly;      
    ADV(poly_list, &poly, &poly_list);

    if (ISLIST(poly)) {
      printf("Length of list = %d\n", LENGTH(poly));

      for (int i = 1; i <= LENGTH(poly); i++) {
	Word e = LELTI(poly, i);
	printf("poly[%d] = %d\n", i, e);
	printf("ISLIST = %d\n", ISLIST(e));
	printf("ISATOM = %d\n", ISATOM(e));
	printf("ISOBJECT = %d\n", ISOBJECT(e));

	if (ISLIST(e)) {
	  PRINT_PROJECTION_POLY_LIST(r, GVVL, e);
	} else {
	  printf("%d is not a list\n", e);
	}
      }
    } else {
      printf("poly %d is not a list\n", poly);
    }

    //assert(LENGTH(poly) == 5);

    // This is the magic incantation that gets polynomials to print
    //IPDWRITE(r, LELTI(poly,PO_POLY), GVVL); SWRITE("\n");

  }

}

void PRINT_POLY_LIST(Word r, Word GVVL, Word poly_list) {

  while (poly_list != NIL) {
    //SWRITE("\tpoly list elem\n");
    Word poly;      
    ADV(poly_list, &poly, &poly_list);

    //printf("Length of poly = %d\n", LENGTH(poly));

    //assert(LENGTH(poly) == 5);

    // This is the magic incantation that gets polynomials to print
    IPDWRITE(r, LELTI(poly,PO_POLY), GVVL); SWRITE("\n");
    // Seems to be a list
    //Word p1 = LELTI(poly, 1);

    //printf("\t\tLength of p1 = %d\n", LENGTH(p1));

    // Seems to be a list
    //Word p2 = LELTI(poly, 2);

    //printf("\t\tLength of p2 = %d\n", LENGTH(p2));

    // Seems to be a one element something
    //Word p3 = LELTI(poly, 3);

    //printf("\t\tLength of p3 = %d\n", LENGTH(p3));

    // Seems to be an integer
    //Word p4 = LELTI(poly, 4);

    //printf("\t\tp4 = %d\n", p4);

    // Seems to be an integer
    //Word p5 = LELTI(poly, 5);

    //printf("\t\tp5 = %d\n", p5);

    /* while (p5 != NIL) { */
    /* 	Word dm; */
    /* 	ADV(p5, &dm, &p5); */
    /* 	SWRITE("portion of p5\n"); */
    /* } */

    /*   while (poly != NIL) { */
    /* 	SWRITE("\t\tPOLY ELEM\n"); */
    /* 	Word pelem; */
    /* 	ADV(poly, &pelem, &poly); */

    /* 	printf("pelem = %d\n", pelem); */
    /* 	while (pelem != NIL) { */
    /* 	  Word peel; */
    /* 	  ADV(pelem, &peel, &pelem); */
    /* 	  SWRITE("\t\t\tPortion of elem\n"); */
    /* 	} */
    /*   } */

    /*   /\* if (poly != NIL) { *\/ */
    /*   /\* 	IPDWRITE(r, poly, GVVL); *\/ */
    /*   /\* } *\/ */
  }

}

void PRINT_POLYS(Word r, Word GVVL, Word A) {
  Word Ap = A;
  while (Ap != NIL) {
    SWRITE("A level not NIL\n");
    Word poly_list;  
    ADV(Ap, &poly_list, &Ap);

    PRINT_POLY_LIST(r, GVVL, poly_list);


  }
}

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
  /*Int*/ if (PCCONTINUE == TRUE) { return; }
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


  QFFWR(GVVL, F);
  SWRITE("\n\n");
  
  printf("# of variables = %d\n\n", LENGTH(GVVL));

  SWRITE("Initial polynomial set\n");
  PRINT_POLYS(r, GVVL, A);
  SWRITE("\n\n");
  
  //FWRITE(GVVL, F);

  Word var_list = GVVL;
  while (var_list != NIL) {
    Word var;
    ADV(var_list, &var, &var_list);
    VWRITE(var);
    SWRITE("\n");
  }

  printf("r              = %d\n", r);

  PROJECT(r,A,&P,&J);

  printf("# of projection factors  = %d\n", LENGTH(P));
  printf("# of j level polynomials = %d\n", LENGTH(J));

  Word p1 = LELTI(P, 1);
  Word convp1 = DIPFP(r, A);

  cout << "p1 as distributive = " << convp1 << endl;

  PRINT_PROJECTION_POLY_LIST(r, GVVL, GVVL);
  //IPDWRITE(1, p1, LIST1(LIST1('a')));
  //IPDWRITE(r, p1, GVVL);
  
  //PRINT_PROJECTION_POLY_LIST(r, GVVL, P);

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
