use File::Find;
find(\&wanted, '.');

%g_libdirs;

sub wanted {
  #print "$File::Find::name\n";
  if($File::Find::name=~m§./(lib(GIFT|MRML)[A-Za-z0-9]*)$§){
    $g_libdirs{$1}=$File::Find::name;
  }
}

print join("\n",sort {$a cmp $b} keys(%g_libdirs)),"\n";

for $i (sort {$a cmp $b} keys(%g_libdirs)){

  my $l_output_file_name="$g_libdirs{$i}/include/uses-declarations.h";

  $i=~s[/][_];

  my $l_define_name="_".uc($i)."_USES_DECLARATIONS_";


  unless(open OFILE, ">$l_output_file_name"){
    print STDERR "Could not create $l_output_file_name\n";
  }else{
    print OFILE << "EOF";
/* -*- mode: c++ -*- 

Path: $l_output_file_name
*/
/* 

    GIFT, a flexible content based image retrieval system.
    Copyright (C) 1998, 1999, 2000, 2001, 2002, CUI University of Geneva

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
/* 

 Author: Wolfgang Müller (via perl script)

  use namespace definitions for $i.
  to be included into all include files of this 
  library
*/
#ifndef $l_define_name
#define $l_define_name

using namespace std;

#endif

EOF
  }
}
