use File::Find;
find(\&wanted,'/home/wolfgang/Viper');

sub wanted{
  
  s/CVL/CGL/g;
  s/Viper/GIFT/g;
  s/viper/gift/g;
  s/^v\-/gift\-/g;

  my $dostring="mv $File::Find::name $File::Find::dir/$_\n";

  print $dostring;
  system($dostring);
}
