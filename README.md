# NAME

Tree::SizeBalanced - Size balanced binary search tree (XS implementation)

Handy for in-memory realtime ranking systems.

# SYNOPSIS

    use Tree::SizeBalanced; # use $tree = Tree::SizeBalanced::int_any->new
    use Tree::SizeBalanced qw(:long); # use $tree = sbtree_int_any;
    use Tree::SizeBalanced qw(:short); # use $tree = sbtreeia;
    use Tree::SizeBalanced qw(:all); # export :short and :long

    my $tree_map_any_any = sbtree_any_any { length($b) <=> length($a) };
    my $tree_map_any_any = sbtreeaa { length($b) <=> length($a) }; # shorter form
    my $tree_map_any_any = Tree::SizeBalanced::any_any->new
      sub { length($b) <=> length($a) }; # full form
    # tree map with any scalars as keys

    my $tree_set_num = sbtree_num_void;
    my $tree_set_num = sbtreen; # shorter form
    my $tree_set_num = Tree::SizeBalanced::num_void->new; # full form
    # or set version.
    #  This one use numeric values (floating point numbers) as keys

    my $tree = sbtree_int_any;
    my $tree = sbtreeia; # shorter form
    my $tree = Tree::SizeBalanced::int_any->new; # full form
    # tree map (key value pairs)
    #  the keys are signed integers
    #  the values are any scalars

    $tree->insert(1, {a => 3});

    ...

    my $count = $tree->count_l(25);
    # how many entries in the tree whose key is less than 25
    my $count = $tree->count_g(25);
    # how many entries in the tree whose key is greater than 25

    ($key, $value) = $tree->skip_l(23);
    $key = $tree->skip_l(23);
    # Get the first (smallest) entry whose key is greater than 23

    ($key, $value) = $tree->skip_g(23);
    $key = $tree->skip_g(23);
    # Get the first (largest) entry whose key is less than 23

    ($key, $value) = $tree->find_min;
    $key = $tree->find_min;
    ($key, $value) = $tree->find_max;
    $key = $tree->find_max;

    ...

    $tree->delete(1);

# DESCRIPTION

Quoted from [http://wcipeg.com/wiki/Size\_Balanced\_Tree](http://wcipeg.com/wiki/Size_Balanced_Tree):

> A size balanced tree (SBT) is a self-balancing binary search tree (BBST) first published by Chinese student Qifeng Chen in 2007. The tree is rebalanced by examining the sizes of each node's subtrees. Its abbreviation resulted in many nicknames given by Chinese informatics competitors, including "sha bi" tree (Chinese: 傻屄树; pinyin: shǎ bī shù; literally meaning "dumb cunt tree") and "super BT", which is a homophone to the Chinese term for snot (Chinese: 鼻涕; pinyin: bítì) suggesting that it is messy to implement. Contrary to what its nicknames suggest, this data structure can be very useful, and is also known to be easy to implement. Since the only extra piece of information that needs to be stored is sizes of the nodes (instead of other "useless" fields such as randomized weights in treaps or colours in red–black tress), this makes it very convenient to implement the select-by-rank and get-rank operations (easily transforming it into an order statistic tree). It supports standard binary search tree operations such as insertion, deletion, and searching in O(log n) time. According to Chen's paper, "it works much faster than many other famous BSTs due to the tendency of a perfect BST in practice."

For performance consideration, this module provides trees with many stricter types.

If you choose integers as the key type, don't use the least number (-2147483647 or -9223372036854775808 according to your perl version) as a key. The tree uses it to indicate \`not found' internally.

If you choose integers as the key type, don't use "-nan" as a key. The tree uses it to indicate \`not found' internally.

If you choose any scalar as the key type, you must provide a comparing sub.
The comparing sub should examed localized `$a` and `$b` (or `$::a` and `$::b` if there are introduced lexical &lt;$a> and &lt;$b> right outside the sub).
And if your comparing sub using an indirect way to judge the size of the keys,
don't do anything that will change the judge result. Or, the tree will be confused and give you incorrect results.

If you put more than one entries with equal-sized keys, the order between them is not garenteed.

The `find` and `delete` methods will try to pick any entries with the same key size as the provided one,
it won't try to find the identical one.

PS. Qifeng Chen is 陈启峰 in simplified Chinese.

This module has been tested on perl version 5.8.9, 5.10.1, 5.12.5, 5.14.4, 5.16.3, 5.18.4, 5.20.3, 5.22.1, 5.22.2

## EXPORT

All exported subs are different style ways to create new trees.

- (nothing)

    Without importing anything, you can use the full form to obtain a new tree:

        my $tree = Tree::SizeBalanced::str_any->new;

- :long

    With the long form:

        my $tree = sbtree_any_str { length($a) <=> length($b) || $a cmp $b };

- :short

    With the short form:

        my $tree = sbtreei;

- :all = :short + :long

## Different trees with different types

### Tree::SizeBalanced::int\_void

Tree set with key type signed integers (32bits or 64bits according to your perl version).

- $tree = Tree::SizeBalanced::int\_void->new
- $tree = sbtree\_int\_void
- $tree = sbtreei

    Creat a new empty tree.

- $tree->insert($key)

    Insert an entry into the tree

- $tree->delete($key)

    Delete one entry which is equal to $key

- $size = $tree->size

    Get the number of entries in the tree

- $found\_or\_not = $tree->find($key)

    Get the entry which is equal to $key.

- $key = $tree->find\_lt($key)

    Get the largest entry which is smaller than $key.

- $key = $tree->find\_le($key)

    Get the largest entry which is smaller than or equal to $key.

- $key = $tree->find\_gt($key)

    Get the smallest entry which is greater than $key.

- $key = $tree->find\_ge($key)

    Get the smallest entry which is greater than or equal to $key.

- $key = $tree->find\_min

    Get the smallest entry.

- $key = $tree->find\_max

    Get the largest entry.

- $key = &tree->skip\_l($offset)

    Get the first entry from the smallest one after skipping $offset entries.

- $key = &tree->skip\_g($offset)

    Get the first entry from the largest one after skipping $offset entries.

- $count = $tree->count\_l($key)

    How many entries which is smaller than $key.

- $count = $tree->count\_g($key)

    How many entries which is greater than $key.

- $tree->dump

    Print the whole tree to STDOUT. For debug use.

- ($order\_consistent, $size\_consistent, $balanced) = $tree->check

    Check the tree property. For debug use.

### Tree::SizeBalanced::int\_int

Tree map with key type signed integers (32bits or 64bits according to your perl version) and value type signed integers (32bits or 64bits according to your perl version).

- $tree = Tree::SizeBalanced::int\_int->new
- $tree = sbtree\_int\_int
- $tree = sbtreeii

    Creat a new empty tree.

- $tree->insert($key, $value)

    Insert an entry into the tree

- $tree->delete($key)

    Delete one entry which is equal to $key

- $size = $tree->size

    Get the number of entries in the tree

- ($found\_or\_not, $value) = $tree->find($key)

    Get the entry which is equal to $key.

- $key or ($key, $value) = $tree->find\_lt($key)

    Get the largest entry which is smaller than $key.

- $key or ($key, $value) = $tree->find\_le($key)

    Get the largest entry which is smaller than or equal to $key.

- $key or ($key, $value) = $tree->find\_gt($key)

    Get the smallest entry which is greater than $key.

- $key or ($key, $value) = $tree->find\_ge($key)

    Get the smallest entry which is greater than or equal to $key.

- $key or ($key, $value) = $tree->find\_min

    Get the smallest entry.

- $key or ($key, $value) = $tree->find\_max

    Get the largest entry.

- $key or ($key, $value) = &tree->skip\_l($offset)

    Get the first entry from the smallest one after skipping $offset entries.

- $key or ($key, $value) = &tree->skip\_g($offset)

    Get the first entry from the largest one after skipping $offset entries.

- $count = $tree->count\_l($key)

    How many entries which is smaller than $key.

- $count = $tree->count\_g($key)

    How many entries which is greater than $key.

- $tree->dump

    Print the whole tree to STDOUT. For debug use.

- ($order\_consistent, $size\_consistent, $balanced) = $tree->check

    Check the tree property. For debug use.

### Tree::SizeBalanced::int\_num

Tree map with key type signed integers (32bits or 64bits according to your perl version) and value type numeric numbers (floating point numbers).

- $tree = Tree::SizeBalanced::int\_num->new
- $tree = sbtree\_int\_num
- $tree = sbtreein

    Creat a new empty tree.

- $tree->insert($key, $value)

    Insert an entry into the tree

- $tree->delete($key)

    Delete one entry which is equal to $key

- $size = $tree->size

    Get the number of entries in the tree

- ($found\_or\_not, $value) = $tree->find($key)

    Get the entry which is equal to $key.

- $key or ($key, $value) = $tree->find\_lt($key)

    Get the largest entry which is smaller than $key.

- $key or ($key, $value) = $tree->find\_le($key)

    Get the largest entry which is smaller than or equal to $key.

- $key or ($key, $value) = $tree->find\_gt($key)

    Get the smallest entry which is greater than $key.

- $key or ($key, $value) = $tree->find\_ge($key)

    Get the smallest entry which is greater than or equal to $key.

- $key or ($key, $value) = $tree->find\_min

    Get the smallest entry.

- $key or ($key, $value) = $tree->find\_max

    Get the largest entry.

- $key or ($key, $value) = &tree->skip\_l($offset)

    Get the first entry from the smallest one after skipping $offset entries.

- $key or ($key, $value) = &tree->skip\_g($offset)

    Get the first entry from the largest one after skipping $offset entries.

- $count = $tree->count\_l($key)

    How many entries which is smaller than $key.

- $count = $tree->count\_g($key)

    How many entries which is greater than $key.

- $tree->dump

    Print the whole tree to STDOUT. For debug use.

- ($order\_consistent, $size\_consistent, $balanced) = $tree->check

    Check the tree property. For debug use.

### Tree::SizeBalanced::int\_any

Tree map with key type signed integers (32bits or 64bits according to your perl version) and value type any scalars.

- $tree = Tree::SizeBalanced::int\_any->new
- $tree = sbtree\_int\_any
- $tree = sbtreeia

    Creat a new empty tree.

- $tree->insert($key, $value)

    Insert an entry into the tree

- $tree->delete($key)

    Delete one entry which is equal to $key

- $size = $tree->size

    Get the number of entries in the tree

- ($found\_or\_not, $value) = $tree->find($key)

    Get the entry which is equal to $key.

- $key or ($key, $value) = $tree->find\_lt($key)

    Get the largest entry which is smaller than $key.

- $key or ($key, $value) = $tree->find\_le($key)

    Get the largest entry which is smaller than or equal to $key.

- $key or ($key, $value) = $tree->find\_gt($key)

    Get the smallest entry which is greater than $key.

- $key or ($key, $value) = $tree->find\_ge($key)

    Get the smallest entry which is greater than or equal to $key.

- $key or ($key, $value) = $tree->find\_min

    Get the smallest entry.

- $key or ($key, $value) = $tree->find\_max

    Get the largest entry.

- $key or ($key, $value) = &tree->skip\_l($offset)

    Get the first entry from the smallest one after skipping $offset entries.

- $key or ($key, $value) = &tree->skip\_g($offset)

    Get the first entry from the largest one after skipping $offset entries.

- $count = $tree->count\_l($key)

    How many entries which is smaller than $key.

- $count = $tree->count\_g($key)

    How many entries which is greater than $key.

- $tree->dump

    Print the whole tree to STDOUT. For debug use.

- ($order\_consistent, $size\_consistent, $balanced) = $tree->check

    Check the tree property. For debug use.

### Tree::SizeBalanced::num\_void

Tree set with key type numeric numbers (floating point numbers).

- $tree = Tree::SizeBalanced::num\_void->new
- $tree = sbtree\_num\_void
- $tree = sbtreen

    Creat a new empty tree.

- $tree->insert($key)

    Insert an entry into the tree

- $tree->delete($key)

    Delete one entry which is equal to $key

- $size = $tree->size

    Get the number of entries in the tree

- $found\_or\_not = $tree->find($key)

    Get the entry which is equal to $key.

- $key = $tree->find\_lt($key)

    Get the largest entry which is smaller than $key.

- $key = $tree->find\_le($key)

    Get the largest entry which is smaller than or equal to $key.

- $key = $tree->find\_gt($key)

    Get the smallest entry which is greater than $key.

- $key = $tree->find\_ge($key)

    Get the smallest entry which is greater than or equal to $key.

- $key = $tree->find\_min

    Get the smallest entry.

- $key = $tree->find\_max

    Get the largest entry.

- $key = &tree->skip\_l($offset)

    Get the first entry from the smallest one after skipping $offset entries.

- $key = &tree->skip\_g($offset)

    Get the first entry from the largest one after skipping $offset entries.

- $count = $tree->count\_l($key)

    How many entries which is smaller than $key.

- $count = $tree->count\_g($key)

    How many entries which is greater than $key.

- $tree->dump

    Print the whole tree to STDOUT. For debug use.

- ($order\_consistent, $size\_consistent, $balanced) = $tree->check

    Check the tree property. For debug use.

### Tree::SizeBalanced::num\_int

Tree map with key type numeric numbers (floating point numbers) and value type signed integers (32bits or 64bits according to your perl version).

- $tree = Tree::SizeBalanced::num\_int->new
- $tree = sbtree\_num\_int
- $tree = sbtreeni

    Creat a new empty tree.

- $tree->insert($key, $value)

    Insert an entry into the tree

- $tree->delete($key)

    Delete one entry which is equal to $key

- $size = $tree->size

    Get the number of entries in the tree

- ($found\_or\_not, $value) = $tree->find($key)

    Get the entry which is equal to $key.

- $key or ($key, $value) = $tree->find\_lt($key)

    Get the largest entry which is smaller than $key.

- $key or ($key, $value) = $tree->find\_le($key)

    Get the largest entry which is smaller than or equal to $key.

- $key or ($key, $value) = $tree->find\_gt($key)

    Get the smallest entry which is greater than $key.

- $key or ($key, $value) = $tree->find\_ge($key)

    Get the smallest entry which is greater than or equal to $key.

- $key or ($key, $value) = $tree->find\_min

    Get the smallest entry.

- $key or ($key, $value) = $tree->find\_max

    Get the largest entry.

- $key or ($key, $value) = &tree->skip\_l($offset)

    Get the first entry from the smallest one after skipping $offset entries.

- $key or ($key, $value) = &tree->skip\_g($offset)

    Get the first entry from the largest one after skipping $offset entries.

- $count = $tree->count\_l($key)

    How many entries which is smaller than $key.

- $count = $tree->count\_g($key)

    How many entries which is greater than $key.

- $tree->dump

    Print the whole tree to STDOUT. For debug use.

- ($order\_consistent, $size\_consistent, $balanced) = $tree->check

    Check the tree property. For debug use.

### Tree::SizeBalanced::num\_num

Tree map with key type numeric numbers (floating point numbers) and value type numeric numbers (floating point numbers).

- $tree = Tree::SizeBalanced::num\_num->new
- $tree = sbtree\_num\_num
- $tree = sbtreenn

    Creat a new empty tree.

- $tree->insert($key, $value)

    Insert an entry into the tree

- $tree->delete($key)

    Delete one entry which is equal to $key

- $size = $tree->size

    Get the number of entries in the tree

- ($found\_or\_not, $value) = $tree->find($key)

    Get the entry which is equal to $key.

- $key or ($key, $value) = $tree->find\_lt($key)

    Get the largest entry which is smaller than $key.

- $key or ($key, $value) = $tree->find\_le($key)

    Get the largest entry which is smaller than or equal to $key.

- $key or ($key, $value) = $tree->find\_gt($key)

    Get the smallest entry which is greater than $key.

- $key or ($key, $value) = $tree->find\_ge($key)

    Get the smallest entry which is greater than or equal to $key.

- $key or ($key, $value) = $tree->find\_min

    Get the smallest entry.

- $key or ($key, $value) = $tree->find\_max

    Get the largest entry.

- $key or ($key, $value) = &tree->skip\_l($offset)

    Get the first entry from the smallest one after skipping $offset entries.

- $key or ($key, $value) = &tree->skip\_g($offset)

    Get the first entry from the largest one after skipping $offset entries.

- $count = $tree->count\_l($key)

    How many entries which is smaller than $key.

- $count = $tree->count\_g($key)

    How many entries which is greater than $key.

- $tree->dump

    Print the whole tree to STDOUT. For debug use.

- ($order\_consistent, $size\_consistent, $balanced) = $tree->check

    Check the tree property. For debug use.

### Tree::SizeBalanced::num\_any

Tree map with key type numeric numbers (floating point numbers) and value type any scalars.

- $tree = Tree::SizeBalanced::num\_any->new
- $tree = sbtree\_num\_any
- $tree = sbtreena

    Creat a new empty tree.

- $tree->insert($key, $value)

    Insert an entry into the tree

- $tree->delete($key)

    Delete one entry which is equal to $key

- $size = $tree->size

    Get the number of entries in the tree

- ($found\_or\_not, $value) = $tree->find($key)

    Get the entry which is equal to $key.

- $key or ($key, $value) = $tree->find\_lt($key)

    Get the largest entry which is smaller than $key.

- $key or ($key, $value) = $tree->find\_le($key)

    Get the largest entry which is smaller than or equal to $key.

- $key or ($key, $value) = $tree->find\_gt($key)

    Get the smallest entry which is greater than $key.

- $key or ($key, $value) = $tree->find\_ge($key)

    Get the smallest entry which is greater than or equal to $key.

- $key or ($key, $value) = $tree->find\_min

    Get the smallest entry.

- $key or ($key, $value) = $tree->find\_max

    Get the largest entry.

- $key or ($key, $value) = &tree->skip\_l($offset)

    Get the first entry from the smallest one after skipping $offset entries.

- $key or ($key, $value) = &tree->skip\_g($offset)

    Get the first entry from the largest one after skipping $offset entries.

- $count = $tree->count\_l($key)

    How many entries which is smaller than $key.

- $count = $tree->count\_g($key)

    How many entries which is greater than $key.

- $tree->dump

    Print the whole tree to STDOUT. For debug use.

- ($order\_consistent, $size\_consistent, $balanced) = $tree->check

    Check the tree property. For debug use.

### Tree::SizeBalanced::str\_void

Tree set with key type strings.

- $tree = Tree::SizeBalanced::str\_void->new
- $tree = sbtree\_str\_void
- $tree = sbtrees

    Creat a new empty tree.

- $tree->insert($key)

    Insert an entry into the tree

- $tree->delete($key)

    Delete one entry which is equal to $key

- $size = $tree->size

    Get the number of entries in the tree

- $found\_or\_not = $tree->find($key)

    Get the entry which is equal to $key.

- $key = $tree->find\_lt($key)

    Get the largest entry which is smaller than $key.

- $key = $tree->find\_le($key)

    Get the largest entry which is smaller than or equal to $key.

- $key = $tree->find\_gt($key)

    Get the smallest entry which is greater than $key.

- $key = $tree->find\_ge($key)

    Get the smallest entry which is greater than or equal to $key.

- $key = $tree->find\_min

    Get the smallest entry.

- $key = $tree->find\_max

    Get the largest entry.

- $key = &tree->skip\_l($offset)

    Get the first entry from the smallest one after skipping $offset entries.

- $key = &tree->skip\_g($offset)

    Get the first entry from the largest one after skipping $offset entries.

- $count = $tree->count\_l($key)

    How many entries which is smaller than $key.

- $count = $tree->count\_g($key)

    How many entries which is greater than $key.

- $tree->dump

    Print the whole tree to STDOUT. For debug use.

- ($order\_consistent, $size\_consistent, $balanced) = $tree->check

    Check the tree property. For debug use.

### Tree::SizeBalanced::str\_int

Tree map with key type strings and value type signed integers (32bits or 64bits according to your perl version).

- $tree = Tree::SizeBalanced::str\_int->new
- $tree = sbtree\_str\_int
- $tree = sbtreesi

    Creat a new empty tree.

- $tree->insert($key, $value)

    Insert an entry into the tree

- $tree->delete($key)

    Delete one entry which is equal to $key

- $size = $tree->size

    Get the number of entries in the tree

- ($found\_or\_not, $value) = $tree->find($key)

    Get the entry which is equal to $key.

- $key or ($key, $value) = $tree->find\_lt($key)

    Get the largest entry which is smaller than $key.

- $key or ($key, $value) = $tree->find\_le($key)

    Get the largest entry which is smaller than or equal to $key.

- $key or ($key, $value) = $tree->find\_gt($key)

    Get the smallest entry which is greater than $key.

- $key or ($key, $value) = $tree->find\_ge($key)

    Get the smallest entry which is greater than or equal to $key.

- $key or ($key, $value) = $tree->find\_min

    Get the smallest entry.

- $key or ($key, $value) = $tree->find\_max

    Get the largest entry.

- $key or ($key, $value) = &tree->skip\_l($offset)

    Get the first entry from the smallest one after skipping $offset entries.

- $key or ($key, $value) = &tree->skip\_g($offset)

    Get the first entry from the largest one after skipping $offset entries.

- $count = $tree->count\_l($key)

    How many entries which is smaller than $key.

- $count = $tree->count\_g($key)

    How many entries which is greater than $key.

- $tree->dump

    Print the whole tree to STDOUT. For debug use.

- ($order\_consistent, $size\_consistent, $balanced) = $tree->check

    Check the tree property. For debug use.

### Tree::SizeBalanced::str\_num

Tree map with key type strings and value type numeric numbers (floating point numbers).

- $tree = Tree::SizeBalanced::str\_num->new
- $tree = sbtree\_str\_num
- $tree = sbtreesn

    Creat a new empty tree.

- $tree->insert($key, $value)

    Insert an entry into the tree

- $tree->delete($key)

    Delete one entry which is equal to $key

- $size = $tree->size

    Get the number of entries in the tree

- ($found\_or\_not, $value) = $tree->find($key)

    Get the entry which is equal to $key.

- $key or ($key, $value) = $tree->find\_lt($key)

    Get the largest entry which is smaller than $key.

- $key or ($key, $value) = $tree->find\_le($key)

    Get the largest entry which is smaller than or equal to $key.

- $key or ($key, $value) = $tree->find\_gt($key)

    Get the smallest entry which is greater than $key.

- $key or ($key, $value) = $tree->find\_ge($key)

    Get the smallest entry which is greater than or equal to $key.

- $key or ($key, $value) = $tree->find\_min

    Get the smallest entry.

- $key or ($key, $value) = $tree->find\_max

    Get the largest entry.

- $key or ($key, $value) = &tree->skip\_l($offset)

    Get the first entry from the smallest one after skipping $offset entries.

- $key or ($key, $value) = &tree->skip\_g($offset)

    Get the first entry from the largest one after skipping $offset entries.

- $count = $tree->count\_l($key)

    How many entries which is smaller than $key.

- $count = $tree->count\_g($key)

    How many entries which is greater than $key.

- $tree->dump

    Print the whole tree to STDOUT. For debug use.

- ($order\_consistent, $size\_consistent, $balanced) = $tree->check

    Check the tree property. For debug use.

### Tree::SizeBalanced::str\_any

Tree map with key type strings and value type any scalars.

- $tree = Tree::SizeBalanced::str\_any->new
- $tree = sbtree\_str\_any
- $tree = sbtreesa

    Creat a new empty tree.

- $tree->insert($key, $value)

    Insert an entry into the tree

- $tree->delete($key)

    Delete one entry which is equal to $key

- $size = $tree->size

    Get the number of entries in the tree

- ($found\_or\_not, $value) = $tree->find($key)

    Get the entry which is equal to $key.

- $key or ($key, $value) = $tree->find\_lt($key)

    Get the largest entry which is smaller than $key.

- $key or ($key, $value) = $tree->find\_le($key)

    Get the largest entry which is smaller than or equal to $key.

- $key or ($key, $value) = $tree->find\_gt($key)

    Get the smallest entry which is greater than $key.

- $key or ($key, $value) = $tree->find\_ge($key)

    Get the smallest entry which is greater than or equal to $key.

- $key or ($key, $value) = $tree->find\_min

    Get the smallest entry.

- $key or ($key, $value) = $tree->find\_max

    Get the largest entry.

- $key or ($key, $value) = &tree->skip\_l($offset)

    Get the first entry from the smallest one after skipping $offset entries.

- $key or ($key, $value) = &tree->skip\_g($offset)

    Get the first entry from the largest one after skipping $offset entries.

- $count = $tree->count\_l($key)

    How many entries which is smaller than $key.

- $count = $tree->count\_g($key)

    How many entries which is greater than $key.

- $tree->dump

    Print the whole tree to STDOUT. For debug use.

- ($order\_consistent, $size\_consistent, $balanced) = $tree->check

    Check the tree property. For debug use.

### Tree::SizeBalanced::any\_void

Tree set with key type any scalars.

- $tree = Tree::SizeBalanced::any\_void->new sub { $a cmp $b }
- $tree = sbtree\_any\_void { $a cmp $b }
- $tree = sbtreea { $a cmp $b }

    Creat a new empty tree.

- $tree->insert($key)

    Insert an entry into the tree

- $tree->delete($key)

    Delete one entry which is equal to $key

- $size = $tree->size

    Get the number of entries in the tree

- $found\_or\_not = $tree->find($key)

    Get the entry which is equal to $key.

- $key = $tree->find\_lt($key)

    Get the largest entry which is smaller than $key.

- $key = $tree->find\_le($key)

    Get the largest entry which is smaller than or equal to $key.

- $key = $tree->find\_gt($key)

    Get the smallest entry which is greater than $key.

- $key = $tree->find\_ge($key)

    Get the smallest entry which is greater than or equal to $key.

- $key = $tree->find\_min

    Get the smallest entry.

- $key = $tree->find\_max

    Get the largest entry.

- $key = &tree->skip\_l($offset)

    Get the first entry from the smallest one after skipping $offset entries.

- $key = &tree->skip\_g($offset)

    Get the first entry from the largest one after skipping $offset entries.

- $count = $tree->count\_l($key)

    How many entries which is smaller than $key.

- $count = $tree->count\_g($key)

    How many entries which is greater than $key.

- $tree->dump

    Print the whole tree to STDOUT. For debug use.

- ($order\_consistent, $size\_consistent, $balanced) = $tree->check

    Check the tree property. For debug use.

### Tree::SizeBalanced::any\_int

Tree map with key type any scalars and value type signed integers (32bits or 64bits according to your perl version).

- $tree = Tree::SizeBalanced::any\_int->new sub { $a cmp $b }
- $tree = sbtree\_any\_int { $a cmp $b }
- $tree = sbtreeai { $a cmp $b }

    Creat a new empty tree.

- $tree->insert($key, $value)

    Insert an entry into the tree

- $tree->delete($key)

    Delete one entry which is equal to $key

- $size = $tree->size

    Get the number of entries in the tree

- ($found\_or\_not, $value) = $tree->find($key)

    Get the entry which is equal to $key.

- $key or ($key, $value) = $tree->find\_lt($key)

    Get the largest entry which is smaller than $key.

- $key or ($key, $value) = $tree->find\_le($key)

    Get the largest entry which is smaller than or equal to $key.

- $key or ($key, $value) = $tree->find\_gt($key)

    Get the smallest entry which is greater than $key.

- $key or ($key, $value) = $tree->find\_ge($key)

    Get the smallest entry which is greater than or equal to $key.

- $key or ($key, $value) = $tree->find\_min

    Get the smallest entry.

- $key or ($key, $value) = $tree->find\_max

    Get the largest entry.

- $key or ($key, $value) = &tree->skip\_l($offset)

    Get the first entry from the smallest one after skipping $offset entries.

- $key or ($key, $value) = &tree->skip\_g($offset)

    Get the first entry from the largest one after skipping $offset entries.

- $count = $tree->count\_l($key)

    How many entries which is smaller than $key.

- $count = $tree->count\_g($key)

    How many entries which is greater than $key.

- $tree->dump

    Print the whole tree to STDOUT. For debug use.

- ($order\_consistent, $size\_consistent, $balanced) = $tree->check

    Check the tree property. For debug use.

### Tree::SizeBalanced::any\_num

Tree map with key type any scalars and value type numeric numbers (floating point numbers).

- $tree = Tree::SizeBalanced::any\_num->new sub { $a cmp $b }
- $tree = sbtree\_any\_num { $a cmp $b }
- $tree = sbtreean { $a cmp $b }

    Creat a new empty tree.

- $tree->insert($key, $value)

    Insert an entry into the tree

- $tree->delete($key)

    Delete one entry which is equal to $key

- $size = $tree->size

    Get the number of entries in the tree

- ($found\_or\_not, $value) = $tree->find($key)

    Get the entry which is equal to $key.

- $key or ($key, $value) = $tree->find\_lt($key)

    Get the largest entry which is smaller than $key.

- $key or ($key, $value) = $tree->find\_le($key)

    Get the largest entry which is smaller than or equal to $key.

- $key or ($key, $value) = $tree->find\_gt($key)

    Get the smallest entry which is greater than $key.

- $key or ($key, $value) = $tree->find\_ge($key)

    Get the smallest entry which is greater than or equal to $key.

- $key or ($key, $value) = $tree->find\_min

    Get the smallest entry.

- $key or ($key, $value) = $tree->find\_max

    Get the largest entry.

- $key or ($key, $value) = &tree->skip\_l($offset)

    Get the first entry from the smallest one after skipping $offset entries.

- $key or ($key, $value) = &tree->skip\_g($offset)

    Get the first entry from the largest one after skipping $offset entries.

- $count = $tree->count\_l($key)

    How many entries which is smaller than $key.

- $count = $tree->count\_g($key)

    How many entries which is greater than $key.

- $tree->dump

    Print the whole tree to STDOUT. For debug use.

- ($order\_consistent, $size\_consistent, $balanced) = $tree->check

    Check the tree property. For debug use.

### Tree::SizeBalanced::any\_any

Tree map with key type any scalars and value type any scalars.

- $tree = Tree::SizeBalanced::any\_any->new sub { $a cmp $b }
- $tree = sbtree\_any\_any { $a cmp $b }
- $tree = sbtreeaa { $a cmp $b }

    Creat a new empty tree.

- $tree->insert($key, $value)

    Insert an entry into the tree

- $tree->delete($key)

    Delete one entry which is equal to $key

- $size = $tree->size

    Get the number of entries in the tree

- ($found\_or\_not, $value) = $tree->find($key)

    Get the entry which is equal to $key.

- $key or ($key, $value) = $tree->find\_lt($key)

    Get the largest entry which is smaller than $key.

- $key or ($key, $value) = $tree->find\_le($key)

    Get the largest entry which is smaller than or equal to $key.

- $key or ($key, $value) = $tree->find\_gt($key)

    Get the smallest entry which is greater than $key.

- $key or ($key, $value) = $tree->find\_ge($key)

    Get the smallest entry which is greater than or equal to $key.

- $key or ($key, $value) = $tree->find\_min

    Get the smallest entry.

- $key or ($key, $value) = $tree->find\_max

    Get the largest entry.

- $key or ($key, $value) = &tree->skip\_l($offset)

    Get the first entry from the smallest one after skipping $offset entries.

- $key or ($key, $value) = &tree->skip\_g($offset)

    Get the first entry from the largest one after skipping $offset entries.

- $count = $tree->count\_l($key)

    How many entries which is smaller than $key.

- $count = $tree->count\_g($key)

    How many entries which is greater than $key.

- $tree->dump

    Print the whole tree to STDOUT. For debug use.

- ($order\_consistent, $size\_consistent, $balanced) = $tree->check

    Check the tree property. For debug use.

# SEE ALSO

This mod's github [https://github.com/CindyLinz/Perl-Tree-SizeBalanced](https://github.com/CindyLinz/Perl-Tree-SizeBalanced).
It's welcome to discuss with me when you encounter bugs, or
if you think that some patterns are also useful but the mod didn't provide them yet.

Introduction to Size Balanced Tree [http://wcipeg.com/wiki/Size\_Balanced\_Tree](http://wcipeg.com/wiki/Size_Balanced_Tree).

陈启峰's original paper [https://drive.google.com/file/d/0B6NYSy8f6mQLOEpHdHh4U2hRcFk/view?usp=sharing](https://drive.google.com/file/d/0B6NYSy8f6mQLOEpHdHh4U2hRcFk/view?usp=sharing),
I found it from [http://sunmoon-template.blogspot.tw/2015/01/b-size-balanced-tree.html](http://sunmoon-template.blogspot.tw/2015/01/b-size-balanced-tree.html).

# AUTHOR

Cindy Wang (CindyLinz) <cindy@cpan.org>

# COPYRIGHT AND LICENSE

Copyright (C) 2016 by CindyLinz

This library is free software; you can redistribute it and/or modify
it under the same terms as Perl itself, either Perl version 5.22.1 or,
at your option, any later version of Perl 5 you may have available.
