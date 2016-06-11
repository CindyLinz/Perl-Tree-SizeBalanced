# Before 'make install' is performed this script should be runnable with
# 'make test'. After 'make install' it should work as 'perl Tree-SizeBalanced.t'

#########################

# change 'tests => 1' to 'tests => last_test_to_print';

use strict;
use warnings;

use Test::More tests => 2001;
BEGIN { use_ok('Tree::SizeBalanced') };

#########################

# Insert your test code below, the Test::More module is use()ed here so read
# its man page ( perldoc Test::More ) for help writing this test script.

my $tree = Tree::SizeBalanced->new;

for(1..100) {
    $tree->insert($_);
    my @check = $tree->check;
    is_deeply([$tree->check], [1,1,1]);
    is($tree->size, $_);
}

for(reverse(1..100)) {
    $tree->insert($_);
    is_deeply([$tree->check], [1,1,1]);
}

for(0..199) {
    is($tree->skip_l($_), int($_/2+1));
    is($tree->skip_g($_), int((199-$_)/2+1));
}

for(1..100) {
    is($tree->count_lt($_), ($_ - 1) * 2);
    is($tree->count_gt($_), (100 - $_) * 2);
    is($tree->count_le($_), $_ * 2);
    is($tree->count_ge($_), (101 - $_) * 2);
}
is($tree->find_max, 100);
is($tree->find_min, 1);
is($tree->find_lt(1), undef);
is($tree->find_gt(100), undef);
is($tree->find_le(0), undef);
is($tree->find_ge(101), undef);
is($tree->find(0), '');
is($tree->find(101), '');
for(2..99) {
    is($tree->find_lt($_), $_-1);
    is($tree->find_gt($_), $_+1);
    is($tree->find_le($_), $_);
    is($tree->find_ge($_), $_);
    is($tree->find($_), 1);
}
is($tree->find_le(999), 100);
is($tree->find_ge(-10), 1);

for(1..100) {
    $tree->delete($_);
    is_deeply([$tree->check], [1,1,1]);
    is($tree->find($_), 1, "delete $_ once");
}
for(1..100) {
    $tree->delete($_);
    is_deeply([$tree->check], [1,1,1]);
    is($tree->find($_), '', "deleted $_ twice");
}
