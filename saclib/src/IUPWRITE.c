/*===========================================================================
			       IUPWRITE(A,v)

Integral  univariate polynomial write.

Inputs
  A : in Z[x].
  v : a variable.

Side effects
  The polynomial A is written in the output stream in recursive
  format using the variable v.
===========================================================================*/
#include "saclib.h"

void IUPWRITE(A,v)
       Word A,v;
{
       Word Ap,e,a;

Step1: /* A = 0 */
       if (A == 0) {
	  CWRITE('0');
	  goto Return; }

Step2: /* Initialize. */
       Ap = A;
       CWRITE('(');

Step3: /* Write coefficient */
       FIRST2(Ap,&e,&a);
       if (a == -1 && e > 0) {
	  CWRITE('-');
	  goto Step4; }
       if (a == 1 && e > 0) {
	  if (A != Ap)
	     CWRITE('+');
	  goto Step4; }
       if (ISIGNF(a) > 0 && A != Ap)
	  CWRITE('+');
       IWRITE(a);

Step4: /* Write variable and exponent */
       if (e > 0)
	  VWRITE(v);
       if (e > 1) {
	  CWRITE('^');
	  AWRITE(e); }
       Ap = RED2(Ap);
       if (Ap != NIL)
	  goto Step3;

Step5: /* Close. */
       CWRITE(')');

Return: /* Prepare for return. */
       return;
}
