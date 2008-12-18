

#. make a perl prog for FindWindowLike
# ie call from ruby via cmdline / system cmd.
# return string list of window hwnds.
# or blank string if none found.
# so can tell if msdev is open

# a function &getopt can be used to read the command line and any options provided on the command line. &getopt is like the C equivalent. It is a library function. The values of the command line get in Perl assigned to an array called @ARGV. &getopt only takes this @ARGV and evaluates the elements.
# Unlike in C the content of the first element in the array is not the program name but the first command line argument.
# check if we have 2 arguments:
die "USAGE: add number1 number2\n" unless ($ARGV[1]);
print "$ARGV[0] + $ARGV[1] is:", $ARGV[0] + $ARGV[1] ,"\n";



use strict;

# REFERENCES
# * cpan->Win32::GuiTest
# * cpan->Win32::GuiTest::Examples
# (to install, run ppm) 

# window partial name to search for (case-sensitive)
my $winname = "Monkey";

# include Windows speciffic modules
use Win32;
use Win32::Process;

# include the testing library
# we import ALL exported symbols for convenience
# is wiser though to import just what you need in 
# production code 
use Win32::GuiTest qw(:ALL);

# list of windows; we use this to keep all found windows
# returned by FindWindowLike()
my @windows;

# we're starting
print "* Tester starting ... \n";

# search for windows matching the title "Tested". Could be
# "am I Tested?" ;-)
@windows = FindWindowLike( undef, $winname, "" );

# print the number of windows fond (it should be zero!)
print "* Number of $winname windows found: "
  . ( $#windows + 1 ) . "\n";
  
if ( $#windows >= 0 ) {
    print "* The $winname program is started more than once!\n";
    
    #~ print "@windows"; # print array of hwnds
    #~ print "\n";
    
    #~ foreach my $i (0..$#windows) {
      #~ my $hwnd = $windows[$i];
      #~ print $hwnd . "\n";
      #~ my $s = GetWindowText($hwnd);
      #~ print $s . "\n";
    #~ }
    
    print "\n";
    foreach my $hwnd (@windows){
      my $title = GetWindowText($hwnd);
      print "$hwnd: $title\n";
    }
}

print "* Done. \n";




