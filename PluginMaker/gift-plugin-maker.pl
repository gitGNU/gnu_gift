use strict;
use vars qw($g_destination_directory);
use File::Find;
use File::Copy;
use File::Path;
use Getopt::Long;

my $l_plugin_name;
my $l_plugin_type;

print qq(gift-plugin-maker.pl: Plugin frame generator for GIFT plugins. 
Usage:
$0 --plugin-name=NAME [--prefix DIR] [--plugin-type=TYPE]

This program is part of the GNU Image Finding Tool package.


);

my $g_destination_prefix;
GetOptions("plugin-name=s" => \$l_plugin_name,
	   "plugin-type=s" => \$l_plugin_type,
	   "prefix=s" => \$g_destination_prefix,
	  );

die "$0: ERROR: You have to specify a plugin name!" 
  unless $l_plugin_name;
$l_plugin_type=lc($l_plugin_name)     unless $l_plugin_type;
$g_destination_prefix="$ENV{HOME}"    unless $g_destination_prefix;


$g_destination_directory=$g_destination_prefix."/GIFTPlugin-".$l_plugin_name."/";

sub replace_list( $ $ ){
  my $in_string=shift;

  my $l_patterns=shift or die;
  my($l_pattern,$l_replacement);
  while(($l_pattern,$l_replacement)=each (%$l_patterns)){
    $in_string =~ s/$l_pattern/$l_replacement/gx;
  }

  return $in_string;
}
sub wanted {
  my $l_file=$_;

  my $l_dir=$File::Find::dir;
  
  if( -f "$l_file"){
    my $l_destination_file=replace_list("$g_destination_directory$l_dir/$l_file",
					{
					 "\@PLUGIN_NAME\@" => $l_plugin_name,
					 "\@PLUGIN_TYPE\@" => $l_plugin_type,
					 ' \.in$ ' => ""
					});
    mkpath(replace_list("$g_destination_directory$l_dir",
			{
			 "\@PLUGIN_NAME\@" => $l_plugin_name,
			 "\@PLUGIN_TYPE\@" => $l_plugin_type,
			 ' \.in$ ' => ""
			}));
    
    
    print "Treating file: $l_dir/$l_file --> $l_destination_file ";
    open SOURCE_FILE,"$l_file" or 
      die "Could not open $_\n";
    open DESTINATION_FILE,">$l_destination_file" or 
      die "could not open $_ for writing\n"; 
    my $l_count=0;
    print "|";
    while(<SOURCE_FILE>){
      print DESTINATION_FILE replace_list($_,{"\@PLUGIN_MAKER_PWD\@" => "$ENV{PWD}/../",
					      "\@PLUGIN_NAME\@" => $l_plugin_name,
					      "\@PLUGIN_TYPE\@" => $l_plugin_type});
      
      $l_count++;                          #
      print "/"  if($l_count % 40 == 10);# turning line 
      print "-"  if($l_count % 40 == 20);# for showing  
      print "\\" if($l_count % 40 == 30);# progress 
      print "|"  if($l_count % 40 == 00);# 
    }
    print " \n";
    close DESTINATION_FILE;
    if(($l_destination_file=~m/ \.sh$ /x)
       ||($l_destination_file=~m/ \.pl$ /x)){
      chmod 0744,$l_destination_file;
    }
  }
}

print "I am now preparing the GIFT Plugins libGIFTAc$l_plugin_name and libGIFTQu$l_plugin_name\n";
finddepth(\&wanted, '.');
print "done";
