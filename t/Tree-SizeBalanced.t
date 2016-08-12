# Before 'make install' is performed this script should be runnable with
# 'make test'. After 'make install' it should work as 'perl Tree-SizeBalanced.t'

#########################

# change 'tests => 1' to 'tests => last_test_to_print';

use strict;
use warnings;

use Test::More tests => 881;
BEGIN { use_ok('Tree::SizeBalanced') };

#########################

# Insert your test code below, the Test::More module is use()ed here so read
# its man page ( perldoc Test::More ) for help writing this test script.

{
    my $tree = Tree::SizeBalanced->new_int_void;

    for(1..20) {
        $tree->insert($_);
        my @check = $tree->check;
        is_deeply([$tree->check], [1,1,1]);
        is($tree->size, $_);
    }

    for(reverse(1..20)) {
        $tree->insert($_);
        is_deeply([$tree->check], [1,1,1]);
    }

    for(0..39) {
        is($tree->skip_l($_), int($_/2+1));
        is($tree->skip_g($_), int((39-$_)/2+1));
    }

    for(1..20) {
        is($tree->count_lt($_), ($_ - 1) * 2);
        is($tree->count_gt($_), (20 - $_) * 2);
        is($tree->count_le($_), $_ * 2);
        is($tree->count_ge($_), (21 - $_) * 2);
    }
    is($tree->find_max, 20);
    is($tree->find_min, 1);
    is($tree->find_lt(1), undef);
    is($tree->find_gt(20), undef);
    is($tree->find_le(0), undef);
    is($tree->find_ge(21), undef);
    is($tree->find(0), '');
    is($tree->find(21), '');
    for(2..19) {
        is($tree->find_lt($_), $_-1);
        is($tree->find_gt($_), $_+1);
        is($tree->find_le($_), $_);
        is($tree->find_ge($_), $_);
        is($tree->find($_), 1);
    }
    is($tree->find_le(999), 20);
    is($tree->find_ge(-10), 1);

    for(1..20) {
        $tree->delete($_);
        is_deeply([$tree->check], [1,1,1]);
        is($tree->find($_), 1, "delete $_ once");
    }
    for(1..20) {
        $tree->delete($_);
        is_deeply([$tree->check], [1,1,1]);
        is($tree->find($_), '', "deleted $_ twice");
    }
}

{
    my $tree = Tree::SizeBalanced->new_num_void;

    for(1..20) {
        $tree->insert($_ / 2);
        my @check = $tree->check;
        is_deeply([$tree->check], [1,1,1]);
        is($tree->size, $_);
    }

    for(reverse(1..20)) {
        $tree->insert($_ / 2);
        is_deeply([$tree->check], [1,1,1]);
    }

    for(0..39) {
        is($tree->skip_l($_), int($_/2+1)/2);
        is($tree->skip_g($_), int((39-$_)/2+1)/2);
    }

    for(1..20) {
        is($tree->count_lt($_/2), ($_ - 1) * 2);
        is($tree->count_gt($_/2), (20 - $_) * 2);
        is($tree->count_le($_/2), $_ * 2);
        is($tree->count_ge($_/2), (21 - $_) * 2);
    }
    is($tree->find_max, 10);
    is($tree->find_min, .5);
    is($tree->find_lt(.5), undef);
    is($tree->find_gt(10), undef);
    is($tree->find_le(0), undef);
    is($tree->find_ge(10.5), undef);
    is($tree->find(0), '');
    is($tree->find(10.5), '');
    for(2..19) {
        is($tree->find_lt($_/2), ($_-1)/2);
        is($tree->find_gt($_/2), ($_+1)/2);
        is($tree->find_le($_/2), $_/2);
        is($tree->find_ge($_/2), $_/2);
        is($tree->find($_/2), 1);
    }
    is($tree->find_le(999), 10);
    is($tree->find_ge(-10), .5);

    for(1..20) {
        $tree->delete($_/2);
        is_deeply([$tree->check], [1,1,1]);
        is($tree->find($_/2), 1, "delete $_/2 once");
    }
    for(1..20) {
        $tree->delete($_/2);
        is_deeply([$tree->check], [1,1,1]);
        is($tree->find($_/2), '', "deleted $_/2 twice");
    }
}

{
    my $tree = Tree::SizeBalanced->new_int_int;
    for(1..10) {
        $tree->insert($_*2, 10-$_);
    }
    for(1..10) {
        my($found, $value) = $tree->find($_*2);
        is($found, 1);
        is($value, 10-$_);
    }
    for(1..10) {
        my($found, $value) = $tree->find_lt($_*2+1);
        is($found, $_*2);
        is($value, 10-$_);
    }
}

{
    my $tree = Tree::SizeBalanced->new_int_any;
    for(1..10) {
        $tree->insert($_, 'a'.$_);
    }
    for(1..10) {
        my($found, $value) = $tree->find($_);
        is($found, 1);
        is($value, 'a'.$_);
    }
}

{
    my $tree = Tree::SizeBalanced->new_str_any;
    for('a'..'g') {
        $tree->insert($_, 'x'.$_);
    }
    for('a'..'g') {
        my($found, $value) = $tree->find($_);
        is($found, 1);
        is($value, 'x'.$_);
    }
    is($tree->count_lt('f'), 5);
    is($tree->count_le('f'), 6);
    is($tree->count_gt('f'), 1);
    is($tree->count_ge('f'), 2);
    is_deeply([$tree->find_lt('a')], [undef]);
    is_deeply([$tree->find_lt('c')], ['b', 'xb']);
}

