#!/usr/bin/perl -w # -*- mode: perl -*-

#     GIFT, a flexible content based image retrieval system.
#     Copyright (C) 1998, 1999, 2000 CUI, University of Geneva

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

require HTTP::Request;
require LWP::UserAgent;
use CPAN;
use File::Copy;

sub unlinkAndWrite( $ ){
  my $inFile=shift;
  print "Deleting $inFile ...";
  if(unlink $inFile){
    print " done.\n";
  }else{
    print "FAILED (Error string: $!)\n";
  }
}

sub getAndUntar( $$$ ){
  my $inURL=shift;
  my $inVersion=shift;
  my $inExtension=shift;
  my $ua = LWP::UserAgent->new;
  
  my $request = HTTP::Request->new(GET => $inURL);
  print $request->as_string();
  my $response = $ua->request($request);

  if($response->{_content}){
    
    my $lFile="/tmp/$inVersion$inExtension";
    unlinkAndWrite($lFile);
    open EXPAT,">$lFile" or die "could not open temporary file";
    print EXPAT $response->{_content};
    close EXPAT;
    $lTar=$lFile;
    $lTar=~s/(tar.gz)|(tgz)/tar/;
    unlinkAndWrite($lTar);
    system("gunzip $lFile");
    my $lDir=$lTar;
    $lDir=~s/\.tar$//;
    unlinkAndWrite($lDir);
    system("cd /tmp;tar xvf $lTar");
    return $lDir;
  }else{
    print STDERR "FAILED: $inVersion\n";
    return "";
  }
}


sub getAndCompile( $$$ ){

  my $inURL=shift;
  my $inVersion=shift;
  my $inExtension=shift;

  my $lFile=getAndUntar($inURL,$inVersion,$inExtension);
  
  if($lFile){
    $ENV{PWD}=$lFile;
    system("cd $ENV{PWD}; ./configure;make;make install");
    unlink $lFile;
  }
}

sub getExpat( $$ ){
  my $inVersion=shift;
  my $inExtension=shift;
  getAndCompile("http://download.sourceforge.net/expat/$inVersion$inExtension",
		$inVersion,
		$inExtension);
}
sub getCharmer( $$ ){
  

  my $inVersion=shift;
  my $inExtension=shift;
  my $lFile=getAndUntar("http://www.mrml.net/download/packages/",
			$inVersion,
			$inExtension);
  if($lFile){

    my $lNewFile=$lFile;
    
    if($inChar){
      $lNewFile=$ENV{bindir}.$inVersion;
      move($lFile,$lNewFile);
    }

    $ENV{PWD}=$lNewFile;
    
    print join("\n",%ENV);
    
    print "Writing applet frame $ENV{PWD}/Charmer.html";
    system("cd $ENV{PWD};perl write-applet-frame.pl");
  }
}



print "For successfully running this script you need to have 
superuser privileges. Otherwise we suggest you to install by hand:

HTTP::Request
the xml parser library expat (you get it at 
http://download.sourceforge.net/expat/$lVersion)
XML::Parser
XML::Writer

this you can do using 

perl -MCPAN -e \"install 'XML::Writer'\" 

etc.
";

print "Downloading the current Charmer version\n";
getCharmer("Charmer-0.2b",".tgz");
print "\nInstalled Charmer\n ";

my $lVersion='expat-1.95.1';
my $lParameter;
my $lDoInstallExpat=1;
while($_=shift){
  if(m/HAVE\_LIBEXPAT/){
    $lDoInstallExpat=0;
    last;
  }
}
# get expat, if it's not already there
# this is needed by XML::Parser
if($lDoInstallExpat){
  print "Downloading and installing expat \n";
  getExpat($lVersion,".tar.gz");
  print "FINISHED Downloading and installing expat\n";
}
    
# Install the perl prerequisites
#system("perl -MCPAN -e \"install 'XML::Writer'\" -e \"install 'XML::Parser'\"");

CPAN::install('Bundle::CPAN');
#CPAN::reload('cpan');
CPAN::install('XML::Parser');
CPAN::install('XML::Writer');

print "
---
As of 31 Jan 2001, there the test
of XML::Writer fails. As a consequence,
you have to install this module by hand.
This is NOT our 'fault', but a bug in 
XML::Writer (I guess).

WM
";

