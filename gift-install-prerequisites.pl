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
use strict;

#
# unlink a file, write if that succeeded
#
sub unlinkAndWrite( $ ){
  my $inFile=shift;
  if($inFile){
    print "Deleting $inFile ...";
    if(unlink $inFile){
      print " done.\n";
    }elsif(system("rm","-r",$inFile)){
      
      print "FAILED (Error string: $!)\n";
    }
  }
}

#
# get a file from an URL, and untar/ungzip it
#
sub getAndUntar( $$$ ){
  my $inURL=shift;
  my $inVersion=shift;
  my $inExtension=shift;
  my $ua = LWP::UserAgent->new;
  
  my $request = HTTP::Request->new(GET => $inURL);
  print $request->as_string();
  my $response = $ua->request($request);

  if($response->{_content}){

    #
    # get ans write the item
    #
    my $lFile="/tmp/$inVersion$inExtension";
    {
      unlinkAndWrite($lFile);
      open RETRIEVED,">$lFile" or die "could not open temporary file";
      print RETRIEVED $response->{_content};
      close RETRIEVED;
    }

    #
    # gunzip the file
    #
    my $lTar=$lFile;
    $lTar=~s/(tar.gz)|(tgz)/tar/;
    unlinkAndWrite($lTar);
    system("gunzip $lFile");


    #
    # untar
    #
    my $lDir=$lTar;
    $lDir=~s/\.tar$//;
    unlinkAndWrite($lDir);
    system("cd /tmp;tar xvf $lTar");
    #
    # if the program is gnu compliant, the tar
    # results will be in the directory given by $lDir
    #
    return $lDir;
  }else{
    print STDERR "FAILED: $inVersion\n";
    return "";
  }
}


#
# get a file from an URL and compile it
#
sub getAndCompile( $$$$ ){
  my $inFlags=shift;
  my $inURL=shift;
  my $inVersion=shift;
  my $inExtension=shift;
  
  #
  # get the file
  #
  my $lFile=getAndUntar($inURL,$inVersion,$inExtension);

  if($lFile){
    my $lOptions="";
    my($lKey,$lValue);
    #
    # build an option list for configure
    #
    while (($lKey,$lValue) = each %{$inFlags}) {
      if($lKey){
	if($lValue){
	  $lOptions.=" $lKey $lValue " 
	}else{
	  $lOptions.=" $lKey "
	}
      }
    }
    #
    # file is now unpacked
    # go into the corresponding directory
    # configure, and install
    #
    $ENV{PWD}=$lFile;
    system("cd $ENV{PWD}; ./configure $lOptions ; make all install");
    unlink $lFile;
  }
}

sub getGNUInstallationCompliantPackage( $@ ){
  my $inFlags=shift;
  my $inURLBase=shift;
  my $inVersion=shift;
  my $inExtension=shift;
  getAndCompile($inFlags,
                "$inURLBase$inVersion$inExtension",
		$inVersion,
		$inExtension);
}
sub getCharmer( $$ ){
  

  my $inVersion=shift;
  my $inExtension=shift;
  my $lDirectory=getAndUntar("http://www.mrml.net/download/packages/",
			     $inVersion,
			     $inExtension);
  if($lDirectory){

    my $lInstallationLocation=$lDirectory;
    
    if($lDirectory){
      $lInstallationLocation=$ENV{bindir}.$inVersion;
      move($lDirectory,$lInstallationLocation);
    }

    $ENV{PWD}=$lInstallationLocation;
    
    #print join("\n",%ENV);
    
    print "Writing applet frame $ENV{PWD}/Charmer.html";
    system("cd $ENV{PWD};perl write-applet-frame.pl");
  }
}

#
# install a list of perl modules
#
sub installPerlModules{
  my $lPrefix=shift;
  my $lModuleName;

  while($lModuleName=shift){
    my$mod=CPAN::Shell->expand("Module",$lModuleName);
    if($lPrefix->{LIB}){
      CPAN::Shell->o('conf','makepl_arg',qq[ LIB=$lPrefix->{LIB} ]);
    }
    $mod->install;
    CPAN::Shell->o('conf','makepl_arg');
  }
}


#
# get the current SnakeCharmer version
#
print "Downloading the current Charmer version\n";
getCharmer("Charmer-0.2b",".tgz");
print "\nInstalled Charmer\n ";

my $lBinDir=shift;
my $lLibDir=shift;

$lBinDir=`echo $lBinDir` if $lBinDir=~m/\$/;
$lLibDir=`echo $lBinDir` if $lLibDir=~m/\$/;

my $lPrefixHash={};
$lPrefixHash={"--libdir"=>$lLibDir,
	      "--bindir"=>$lBinDir} if $lBinDir;

my $lPerlPrefixHash={};
$lPerlPrefixHash={"LIB"=>$lBinDir} if $lBinDir;

installPerlModules($lPerlPrefixHash,'HTTP::Request');

if(1==1){
  # among the packages to be found is
  # expat
  # this is needed by XML::Parser
  
  my $lToBeInstalled={'EXPAT'=>["http://download.sourceforge.net/expat/",
				'expat-1.95.1',
				".tar.gz"],
		      'MAGICK'=>["http://imagemagick.sourceforge.net/http/",
				 "ImageMagick-5.4.0",
				 ".tar.gz"],
		      'DBI'=>["http://prdownloads.sourceforge.net/libdbi/",
			      "libdbi-0.6.2",
			      ".tar.gz"]
		     };
  my $i;
  foreach $i (sort {$a cmp $b} keys(%{$lToBeInstalled})){
    unless($ENV{$i}){
      #if package with associated to key $i is there,
      #then the variable $ENV{$i} has been set by
      #./configure
      getGNUInstallationCompliantPackage($lPrefixHash,
					 @{$lToBeInstalled->{$i}});
    }
  }
}


installPerlModules($lPerlPrefixHash,
		   'XML::Parser',
		   'XML::Writer',
		   'XML::Parser::EasyTree');
