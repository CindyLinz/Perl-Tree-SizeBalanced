use 5.008009;
use ExtUtils::MakeMaker;

my @key = qw(int num str any);
my @value = qw(void int num any);
my %shorter = (qw(
    int i
    num n
    str s
    any a
    void
), '');
my %type_name = (
    'int' => 'signed integers (32bits or 64bits according to your perl version)',
    'num' => 'numeric numbers (floating point numbers)',
    'str' => 'strings',
    'any' => 'any scalars',
);

my @generated_file;

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
            $member_proto =~ s[(\w+) *\((.*)\)]{
                my($method, $proto) = ($1, $2);
                my $args = $proto;
                $args =~ s/(^|,)[^,]*?(\w+)\s*(?:|=[^,]*)(?=$|,)/$1$2/g;

                "void\n".
                "$method\_$key\_$value($proto)\n".
                "    PPCODE:\n".
                "        SP = $method\_$key\_$value(aTHX_ SP, $args);\n"
            }ge;
            print $f $member_proto;
        }
    }
    close $f;

    push @generated_file, 'SizeBalanced.xs';
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

        push @generated_file, "lib/Tree/SizeBalanced/$key\_$value.pm";
    }
}

{
    my $insert_code = <<'.';
our @EXPORT_OK = qw(sbtree);
our %EXPORT_TAGS = (
    'all' => ['sbtree'],
    'short' => ['sbtree'],
    'long' => [],
);

.
    for my $key (@key) {
        for my $value (@value) {
            my $package = "Tree::SizeBalanced::$key\_$value";
            my $short = "sbtree$shorter{$key}$shorter{$value}";
            my $long = "sbtree_$key\_$value";
            my $proto = $key eq 'any' ? '&' : '';
            my $new_trailing = $key eq 'any' ? " sub { \$a cmp \$b }" : '';
            my $func_trailing = $key eq 'any' ? " { \$a cmp \$b }" : '';

            my($desc, $key_value_arg, $key_value_ret, $found_value);
            if( $value eq 'void' ) {
                $desc = "Tree set with key type $type_name{$key}.";
                $key_value_arg = '($key)';
                $key_value_ret = '$key';
                $found_value = '$found_or_not';
            } else {
                $desc = "Tree map with key type $type_name{$key} and value type $type_name{$value}.";
                $key_value_arg = '($key, $value)';
                $key_value_ret = '$key or ($key, $value)';
                $found_value = '($found_or_not, $value)';
            }

            $insert_code .= <<".";

=head3 $package

$desc

=over 4

=item \$tree = $package\->new$new_trailing

=item \$tree = $long$func_trailing

=item \$tree = $short$func_trailing

Creat a new empty tree.

=item \$tree->insert$key_value_arg

Insert an entry into the tree

=item \$tree->delete(\$key)

Delete one entry which is equal to \$key

=item \$size = \$tree->size

Get the number of entries in the tree

=item $found_value = \$tree->find(\$key)

Get the entry which is equal to \$key.

=item $key_value_ret = \$tree->find_lt(\$key)

Get the largest entry which is smaller than \$key.

=item $key_value_ret = \$tree->find_le(\$key)

Get the largest entry which is smaller than or equal to \$key.

=item $key_value_ret = \$tree->find_gt(\$key)

Get the smallest entry which is greater than \$key.

=item $key_value_ret = \$tree->find_ge(\$key)

Get the smallest entry which is greater than or equal to \$key.

=item $key_value_ret = \$tree->find_min

Get the smallest entry.

=item $key_value_ret = \$tree->find_max

Get the largest entry.

=item $key_value_ret = \&tree->skip_l(\$offset)

Get the first entry from the smallest one after skipping \$offset entries.

=item $key_value_ret = \&tree->skip_g(\$offset)

Get the first entry from the largest one after skipping \$offset entries.

=item \$count = \$tree->count_l(\$key)

How many entries which is smaller than \$key.

=item \$count = \$tree->count_g(\$key)

How many entries which is greater than \$key.

=item \$tree->dump

Print the whole tree to STDOUT. For debug use.

=item (\$order_consistent, \$size_consistent, \$balanced) = \$tree->check

Check the tree property. For debug use.

=back

=cut

use Tree::SizeBalanced::$key\_$value;

sub $long($proto) {
    unshift \@_, '$package';
    goto \\&$package\::new;
}

sub $short($proto) {
    unshift \@_, '$package';
    goto \\&$package\::new;
}

push \@EXPORT_OK, qw($long $short);
push \@{\$EXPORT_TAGS{'all'}}, qw($long $short);
push \@{\$EXPORT_TAGS{'short'}}, qw($long);
push \@{\$EXPORT_TAGS{'long'}}, qw($short);

.
        }
    }
    $insert_code .= <<".";
sub sbtree(;&) {
    if( ref($_[0]) eq 'CODE' ) {
        goto \\&sbtreeaa;
    } else {
        goto \\&sbtreeia;
    }
}

sub new {
    shift;
    if( ref($_[0]) eq 'CODE' ) {
        goto \\&sbtreeaa;
    } else {
        goto \\&sbtreeia;
    }
}
.

    my $f;
    open $f, 'lib/Tree/SizeBalanced.pm.gen';
    local $/;
    my $src = <$f>;
    close $f;

    $src =~ s/"INSERT POINT";/$insert_code/;

    open $f, '>lib/Tree/SizeBalanced.pm';
    print $f $src;
    close $f;

    push @generated_file, 'lib/Tree/SizeBalanced.pm';
}

# See lib/ExtUtils/MakeMaker.pm for details of how to influence
# the contents of the Makefile that is written.
WriteMakefile(
    NAME              => 'Tree::SizeBalanced',
    VERSION_FROM      => 'lib/Tree/SizeBalanced.pm', # finds $VERSION, requires EU::MM from perl >= 5.5
    PREREQ_PM         => {}, # e.g., Module::Name => 1.1
    ABSTRACT_FROM     => 'lib/Tree/SizeBalanced.pm', # retrieve abstract from module
    AUTHOR            => 'Cindy Wang (CindyLinz) <cindy@cpan.org>',
    #LICENSE           => 'perl',
    #Value must be from legacy list of licenses here
    #http://search.cpan.org/perldoc?Module%3A%3ABuild%3A%3AAPI
    LIBS              => [''], # e.g., '-lm'
    DEFINE            => '-std=c99', #'-g -O0', # e.g., '-DHAVE_SOMETHING'
    INC               => '-I.', # e.g., '-I. -I/usr/include/other'
	# Un-comment this if you add C files to link with later:
    OBJECT            => '$(O_FILES)', # link all the C files too
    clean             => {FILES => \@generated_file},
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

