#!/usr/bin/perl -w # -*- mode: perl -*-

#     GIFT, a flexible content based image retrieval system.
#     Copyright (C) 1998, 1999, 2000, 2001, 2002, CUI University of Geneva

#     This program is free software; you can redistribute it and/or modify
#     it under the terms of the GNU General Public License as published by
#     the Free Software Foundation; either version 2 of the License, or
#     (at your option) any later version.

#     This program is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU General Public License for more details.

#     You should have received a copy of the GNU General Public License
#     along with this program; if not, write to the Free Software
#     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#
# This file finds out which perl modules needed are missing from your system
# the perl modules are specified as arguments
#

my $lModule;
my $lReturnValue;
while($lModule=shift){
  print "Checking for module $lModule in your perl setup\n";
  eval "use $lModule;";
  my $lErrorString=join("\n    ",split("\n",$@));
  if($@){
    $lReturnValue=1;
    print STDERR << "ENDSTRING" 
      $lModule is missing. 
	Perl gives us the following error message:
	$lErrorString;
ENDSTRING
  }else{
    print "$lModule: OK\n"
  };
}

die q(There were one or more missing Perl modules. Please install them
      or adjust your Perl library path befor trying again.

     This error message was generated ) if $lReturnValue
