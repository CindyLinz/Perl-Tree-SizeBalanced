use 5.022001;
use ExtUtils::MakeMaker;
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

for my $gen_filename ( <*.gen> ) {
    my($filename) = $gen_filename =~ /^(.*)\.gen$/;

    my($inf, $outf);

    open $inf, $gen_filename;
    my $source = do{
        local $/;
        <$inf>;
    };
    close $inf;

    my @macro;
    $source =~ s(#\s*define\s+(\w+)\s*\(([^()]*)\)\s*\{{3}(.*?)\}{3}){
        my($name, $args, $body) = ($1, $2, $3);
        my @arg = $args =~ /(\w+)/g;
        push @macro, [$name, \@arg, $body];
        ''
    }ges;

    for my $macro (@macro) {
        my($name, $args, $code) = @$macro;
        $source =~ s(([ \t]*)$name\s*\(([^()]*)\)){
            my $indent = $1;
            my $arg_values = $2;
            my @arg_value = split /,/, $arg_values;

            my $code = $code;
            for my $i (0..$#{$args}) {
                my $arg_label = $args->[$i];
                my $arg_value = $arg_value[$i];
                $code =~ s/(\s*##\s*|\b)$arg_label(\s*##\s*|\b)/$arg_value/g;
            }
            $code =~ s/^/$indent/gm;
            $code . $indent;
        }ges;
    }

    open $outf, ">$filename";
    print $outf $source;
    close $outf;
}
