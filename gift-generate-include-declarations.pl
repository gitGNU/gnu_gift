use File::Find;
find(\&wanted, '.');

%g_libdirs;

sub wanted {
  #print "$File::Find::name\n";
  if($File::Find::name=~m§\.h$§){
    my $l_out_file="$_";
    my $l_in_file="$_.bak";
    rename $l_out_file,$l_in_file;
    open INFILE,$l_in_file or die "could not open $l_in_file $!";
    open OUTFILE,">$l_out_file" or die "could not open $l_out_file $!";
    select(OUTFILE);

    my $l_added=0;

    while(<INFILE>){
      print $_;
      if((!$l_added)
	 && (m/\#define\s+_C/)){
	print '#include "../include/uses-declarations.h"';
	$l_added=1;
      }
    }
  }
}
