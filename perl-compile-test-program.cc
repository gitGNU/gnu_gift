// -*- mode: c++ -*- 
/* 

   GIFT, a flexible content based image retrieval system.
   Copyright (C) 1998, 1999, 2000 CUI, University of Geneva

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/
/**
   This program should compile using a decently installed
   Perl 5. see the perlembed manpage for details. The program
   is directly snipped from the perl 5 perlembed manpage.

   See the perlembed manpage for author details.
*/

#if defined(__cplusplus) && !defined(PERL_OBJECT)
#define is_cplusplus
#endif

#ifdef is_cplusplus
extern "C" {
#endif
  
#include <EXTERN.h>
#include <perl.h>
#ifdef PERL_OBJECT
#define NO_XSLOCKS
#include <XSUB.h>
#include "win32iop.h"
#include <fcntl.h>
#include <perlhost.h>
#endif
#ifdef is_cplusplus
}
#  ifndef EXTERN_C
#    define EXTERN_C extern "C"
#  endif
#else
#  ifndef EXTERN_C
#    define EXTERN_C extern
#  endif
#endif

EXTERN_C void xs_init _((void));

EXTERN_C void boot_DynaLoader _((CV* cv));

EXTERN_C void
xs_init(void)
{
  char *file = __FILE__;
  dXSUB_SYS;
  
  /* DynaLoader is a special case */
  newXS("DynaLoader::boot_DynaLoader", boot_DynaLoader, file);
}

void test(void){
  dSP ;
  int count;
  
  ENTER;
  SAVETMPS;
  PUSHMARK(SP) ;

  XPUSHs(sv_2mortal(newSViv(10)));
  XPUSHs(sv_2mortal(newSVpv("hii",
			    3)));
  XPUSHs(sv_2mortal(newSVpv("hoo",
			    3)));

  int lResultSize(perl_call_pv((char*)("huhu"), G_ARRAY));
  
  SPAGAIN;
  
  for(int i=0;
      i<lResultSize;
      i+=3){
    
    SV*   lPoppedInstruction=POPs;
    int   lInstruction=SvIV(lPoppedInstruction);
    
    STRLEN lLengthOfPopped;
    
    SV*   lPoppedName=POPs;
    char* lName=SvPV(lPoppedName,lLengthOfPopped);
    
    SV*   lPoppedValue=POPs;
    char* lValue=SvPV(lPoppedValue,lLengthOfPopped);
    
  }
  
  PUTBACK;
  FREETMPS;
  LEAVE;
  
}

int main(
	 int argc, 
	 char **argv,
	 char **env
	 ) {
  
  static PerlInterpreter *my_perl;  /***    The Perl interpreter    ***/
  
  my_perl = perl_alloc();
  perl_construct(my_perl);
  perl_parse(my_perl, NULL, argc, argv, (char **)NULL);
  perl_run(my_perl);
  perl_destruct(my_perl);
  perl_free(my_perl);

  xs_init();

  test();
 	
  ; return 0; }
