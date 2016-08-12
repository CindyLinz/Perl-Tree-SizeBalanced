use 5.022001;
use ExtUtils::MakeMaker;

my @key = qw(int num);
my @value = qw(void int num any);
my @method;
{
    my $f;
    open $f, 'SizeBalanced.xs.gen';
    local $/;
    my $xs = <$f>;
    close $f;

    my($static_code, $member_proto) = $xs =~ /(.*\nINCLUDE:.*?\n)(.*)/s;

    @method = $member_proto =~ /(\w+) *\(.*\)/g;

    open $f, '>SizeBalanced.xs';
    print $f $static_code;
    for my $key (@key) {
        for my $value (@value) {
            my $member_proto = $member_proto;
            $member_proto =~ s((\w+) *\((.*)\)){
                my($method, $proto) = ($1, $2);
                my $args = $proto;
                $args =~ s/(^|,)[^,]*?(\w+)\s*(?:|=[^,]*)(?=$|,)/$1$2/g;

                <<".";
void
$method\_$key\_$value($proto)
    PPCODE:
        SP = $method\_$key\_$value(aTHX_ SP, $args);
.
            }ge;
            print $f $member_proto;
        }
    }
    close $f;
}

mkdir 'lib/Tree/SizeBalanced';
for my $key (@key) {
    for my $value (@value) {
        my $f;
        open $f, ">lib/Tree/SizeBalanced/$key\_$value.pm";

        print $f <<".";
package Tree::SizeBalanced::$key\_$value;

use strict;
use warnings;

use Tree::SizeBalanced;

.
        for my $method (@method) {
            print $f "*$method = \\&Tree::SizeBalanced::Core::$method\_$key\_$value;\n";
        }
        print $f "\n1;\n";

        close $f;
    }
}

# See lib/ExtUtils/MakeMaker.pm for details of how to influence
# the contents of the Makefile that is written.
WriteMakefile(
    NAME              => 'Tree::SizeBalanced',
    VERSION_FROM      => 'lib/Tree/SizeBalanced.pm', # finds $VERSION, requires EU::MM from perl >= 5.5
    PREREQ_PM         => {}, # e.g., Module::Name => 1.1
    ABSTRACT_FROM     => 'lib/Tree/SizeBalanced.pm', # retrieve abstract from module
    AUTHOR            => 'A. U. Thor <cindy@nonet>',
    #LICENSE           => 'perl',
    #Value must be from legacy list of licenses here
    #http://search.cpan.org/perldoc?Module%3A%3ABuild%3A%3AAPI
    LIBS              => [''], # e.g., '-lm'
    DEFINE            => '-g -O0', # e.g., '-DHAVE_SOMETHING'
    INC               => '-I.', # e.g., '-I. -I/usr/include/other'
	# Un-comment this if you add C files to link with later:
    OBJECT            => '$(O_FILES)', # link all the C files too
);
if  (eval {require ExtUtils::Constant; 1}) {
  # If you edit these definitions to change the constants used by this module,
  # you will need to use the generated const-c.inc and const-xs.inc
  # files to replace their "fallback" counterparts before distributing your
  # changes.
  my @names = (qw());
  ExtUtils::Constant::WriteConstants(
                                     NAME         => 'Tree::SizeBalanced',
                                     NAMES        => \@names,
                                     DEFAULT_TYPE => 'IV',
                                     C_FILE       => 'const-c.inc',
                                     XS_FILE      => 'const-xs.inc',
                                  );

}
else {
  use File::Copy;
  use File::Spec;
  foreach my $file ('const-c.inc', 'const-xs.inc') {
    my $fallback = File::Spec->catfile('fallback', $file);
    copy ($fallback, $file) or die "Can't copy $fallback to $file: $!";
  }
}

