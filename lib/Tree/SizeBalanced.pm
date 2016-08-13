package Tree::SizeBalanced;

use 5.022001;
use strict;
use warnings;
use Carp;

require Exporter;
use AutoLoader;

our @ISA = qw(Exporter);

# Items to export into callers namespace by default. Note: do not export
# names by default without a very good reason. Use EXPORT_OK instead.
# Do not simply export all your public functions/methods/constants.

# This allows declaration	use Tree::SizeBalanced ':all';
# If you do not need this, moving things directly into @EXPORT or @EXPORT_OK
# will save memory.

our $VERSION = '0.01';

for my $key (qw(int num str any)) {
    for my $value (qw(void int num any)) {
        require "Tree/SizeBalanced/$key\_$value.pm";

        no strict 'refs';
        my $new_func = \&{"Tree::SizeBalanced::$key\_$value\::new"};
        my $class = "Tree::SizeBalanced::$key\_$value";
        *{"new_$key\_$value"} = sub {
            shift;
            $new_func->($class, @_);
        };
    }
}
sub new {
    if( ref($_[0]) eq 'SUB' ) {
        goto \&new_any_void;
    } else {
        goto \&new_int_void;
    }
}

require XSLoader;
XSLoader::load('Tree::SizeBalanced', $VERSION);

# Preloaded methods go here.

# Autoload methods go after =cut, and are processed by the autosplit program.

1;
__END__
# Below is stub documentation for your module. You'd better edit it!

=head1 NAME

Tree::SizeBalanced - Size balanced binary search tree (XS implementation)

=head1 SYNOPSIS

  use Tree::SizeBalanced;

  my $tree = Tree::SizeBalanced;
  $tree->insert(1);

=head1 DESCRIPTION

Stub documentation for Tree::SizeBalanced, created by h2xs. It looks like the
author of the extension was negligent enough to leave the stub
unedited.

Blah blah blah.

=head2 EXPORT

None by default.



=head1 SEE ALSO

Mention other useful documentation such as the documentation of
related modules or operating system documentation (such as man pages
in UNIX), or any relevant external documentation such as RFCs or
standards.

If you have a mailing list set up for your module, mention it here.

If you have a web site set up for your module, mention it here.

=head1 AUTHOR

A. U. Thor, E<lt>cindy@nonetE<gt>

=head1 COPYRIGHT AND LICENSE

Copyright (C) 2016 by A. U. Thor

This library is free software; you can redistribute it and/or modify
it under the same terms as Perl itself, either Perl version 5.22.1 or,
at your option, any later version of Perl 5 you may have available.


=cut
