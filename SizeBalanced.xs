#define PERL_NO_GET_CONTEXT
#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "ppport.h"

#include "const-c.inc"

MODULE = Tree::SizeBalanced		PACKAGE = Tree::SizeBalanced		

INCLUDE: const-xs.inc
