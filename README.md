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

    my $count = $tree->count_lt(25);
    # how many entries in the tree whose key is less than 25
    my $count = $tree->count_gt(25);
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

    ($k1, $v1, $k2, $v2) = $tree->find_min(2);
    ($k1, $v1, $k2, $v2, $k3, $v3) = $tree->find_min(3);
    ($k1, $v1, $k2, $v2, $k3, $v3, ...) = $tree->find_min(-1);

    ($k1, $v1, ...= $tree->find_lt_gt($lower_bound, $upper_bound);

    ...

    $tree->delete(1);

# DESCRIPTION

Quoted from [http://wcipeg.com/wiki/Size\_Balanced\_Tree](http://wcipeg.com/wiki/Size_Balanced_Tree):

\> A size balanced tree (SBT) is a self-balancing binary search tree (BBST) first published by Chinese student Qifeng Chen in 2007. The tree is rebalanced by examining the sizes of each node's subtrees. Its abbreviation resulted in many nicknames given by Chinese informatics competitors, including "sha bi" tree (Chinese: 傻屄树; pinyin: shǎ bī shù; literally meaning "dumb cunt tree") and "super BT", which is a homophone to the Chinese term for snot (Chinese: 鼻涕; pinyin: bítì) suggesting that it is messy to implement. Contrary to what its nicknames suggest, this data structure can be very useful, and is also known to be easy to implement. Since the only extra piece of information that needs to be stored is sizes of the nodes (instead of other "useless" fields such as randomized weights in treaps or colours in red–black tress), this makes it very convenient to implement the select-by-rank and get-rank operations (easily transforming it into an order statistic tree). It supports standard binary search tree operations such as insertion, deletion, and searching in O(log n) time. According to Chen's paper, "it works much faster than many other famous BSTs due to the tendency of a perfect BST in practice."

For performance consideration, this module provides trees with many stricter types.

If you choose any scalar as the key type, you must provide a comparing sub.
The comparing sub should exammed localized `$a` and `$b` (or `$::a` and `$::b` if there are introduced lexical &lt;$a> and &lt;$b> right outside the sub).
And if your comparing sub using an indirect way to judge the size of the keys,
don't do anything that will change the judge result. Or, the tree will be confused and give you incorrect results.

If you put more than one entries with equal-sized keys,
the insertion order is preserved by treating the first one as the smallest one among them.

PS. Qifeng Chen is 陈启峰 in simplified Chinese.

This module has been tested on perl version 5.8.9, 5.10.1, 5.12.5, 5.14.4, 5.16.3, 5.18.4, 5.20.3, 5.22.2

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
- $tree->insert\_after($key)

    Insert an entry into the tree.
    If there are any entries with the same key size,
    insert the new one after them.

- $tree->insert\_before($key)

    Insert an entry into the tree.
    If there are any entries with the same key size,
    insert the new one before them.

- $tree->delete($key)
- $tree->delete\_last($key)

    Delete one entry whose key is equal to $key.
    If there ary more than one entry with the same key size,
    delete the last inserted one.

- $tree->delete\_first($key)

    Delete one entry whose key is equal to $key.
    If there ary more than one entry with the same key size,
    delete the first inserted one.

- $size = $tree->size

    Get the number of entries in the tree

- $key or ($key1, $key2, ...) = $tree->find($key, $limit=1)
- $key or ($key1, $key2, ...) = $tree->find\_first($key, $limit=1)

    Get entries with key sizes equal to $key,
    from the first inserted one to the last inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = $tree->find\_last($key, $limit=1)

    Get entries with key sizes equal to $key,
    from the last inserted one to the first inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = $tree->find\_lt($key, $limit=1)

    Get entries, whose keys are smaller than $key, from the largest entry.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = $tree->find\_le($key, $limit=1)

    Get entries, whose keys are smaller than or equal to $key, from the largest entry.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = $tree->find\_gt($key, $limit=1)

    Get entries, whose keys are greater than $key, from the smallest one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = $tree->find\_ge($key, $limit=1)

    Get entries, whose keys are greater than or equal to $key, from the smallest one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = $tree->find\_gt\_lt($lower\_key, $upper\_key)

    Get entries, whose keys are greater than $lower\_key and smaller than $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $key2, ...) = $tree->find\_gt\_le($lower\_key, $upper\_key)

    Get entries, whose keys are greater than $lower\_key and smaller than or equal to $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $key2, ...) = $tree->find\_ge\_lt($lower\_key, $upper\_key)

    Get entries, whose keys are greater than or equal to $lower\_key and smaller than $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $key2, ...) = $tree->find\_ge\_le($lower\_key, $upper\_key)

    Get entries, whose keys are greater than or equal to $lower\_key and smaller than or equal to $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $key2, ...) = $tree->find\_min($limit=1)

    Get entries from the one with smallest key.
    If there are more than one entries with smallest key,
    begin from the first inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = $tree->find\_max($limit=1)

    Get entries from the one with largest key.
    If there are more than one entries with smallest key,
    begin from the last inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = &tree->skip\_l($offset, $limit=1)

    Get the first entry from one with the smallest key after skipping $offset entries.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = &tree->skip\_g($offset, $limit=1)

    Get the first entry from one with the largest key after skipping $offset entries.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $count = $tree->count\_lt($key)

    Get the number of entries whose keys are smaller than $key.

- $count = $tree->count\_le($key)

    Get the number of entries whose keys are smaller than or equal to $key.

- $count = $tree->count\_gt($key)

    Get the number of entries whose keys are greater than $key.

- $count = $tree->count\_ge($key)

    Get the number of entries whose keys are greater than or equal to $key.

- $dump\_str = $tree->dump

    Get a string which represent the whole tree structure. For debug use.

- ($order\_consistent, $size\_consistent, $balanced) = $tree->check

    Check the tree property. For debug use.

- $ever\_height = $tree->ever\_height

    Get the maximum height the tree has ever been. For debug use

### Tree::SizeBalanced::int\_int

Tree map with key type signed integers (32bits or 64bits according to your perl version) and value type signed integers (32bits or 64bits according to your perl version).

- $tree = Tree::SizeBalanced::int\_int->new
- $tree = sbtree\_int\_int
- $tree = sbtreeii

    Creat a new empty tree.

- $tree->insert($key, $value)
- $tree->insert\_after($key, $value)

    Insert an entry into the tree.
    If there are any entries with the same key size,
    insert the new one after them.

- $tree->insert\_before($key, $value)

    Insert an entry into the tree.
    If there are any entries with the same key size,
    insert the new one before them.

- $tree->delete($key)
- $tree->delete\_last($key)

    Delete one entry whose key is equal to $key.
    If there ary more than one entry with the same key size,
    delete the last inserted one.

- $tree->delete\_first($key)

    Delete one entry whose key is equal to $key.
    If there ary more than one entry with the same key size,
    delete the first inserted one.

- $size = $tree->size

    Get the number of entries in the tree

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find($key, $limit=1)
- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_first($key, $limit=1)

    Get entries with key sizes equal to $key,
    from the first inserted one to the last inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_last($key, $limit=1)

    Get entries with key sizes equal to $key,
    from the last inserted one to the first inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_lt($key, $limit=1)

    Get entries, whose keys are smaller than $key, from the largest entry.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_le($key, $limit=1)

    Get entries, whose keys are smaller than or equal to $key, from the largest entry.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt($key, $limit=1)

    Get entries, whose keys are greater than $key, from the smallest one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge($key, $limit=1)

    Get entries, whose keys are greater than or equal to $key, from the smallest one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt\_lt($lower\_key, $upper\_key)

    Get entries, whose keys are greater than $lower\_key and smaller than $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt\_le($lower\_key, $upper\_key)

    Get entries, whose keys are greater than $lower\_key and smaller than or equal to $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge\_lt($lower\_key, $upper\_key)

    Get entries, whose keys are greater than or equal to $lower\_key and smaller than $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge\_le($lower\_key, $upper\_key)

    Get entries, whose keys are greater than or equal to $lower\_key and smaller than or equal to $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_min($limit=1)

    Get entries from the one with smallest key.
    If there are more than one entries with smallest key,
    begin from the first inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_max($limit=1)

    Get entries from the one with largest key.
    If there are more than one entries with smallest key,
    begin from the last inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = &tree->skip\_l($offset, $limit=1)

    Get the first entry from one with the smallest key after skipping $offset entries.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = &tree->skip\_g($offset, $limit=1)

    Get the first entry from one with the largest key after skipping $offset entries.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $count = $tree->count\_lt($key)

    Get the number of entries whose keys are smaller than $key.

- $count = $tree->count\_le($key)

    Get the number of entries whose keys are smaller than or equal to $key.

- $count = $tree->count\_gt($key)

    Get the number of entries whose keys are greater than $key.

- $count = $tree->count\_ge($key)

    Get the number of entries whose keys are greater than or equal to $key.

- $dump\_str = $tree->dump

    Get a string which represent the whole tree structure. For debug use.

- ($order\_consistent, $size\_consistent, $balanced) = $tree->check

    Check the tree property. For debug use.

- $ever\_height = $tree->ever\_height

    Get the maximum height the tree has ever been. For debug use

### Tree::SizeBalanced::int\_num

Tree map with key type signed integers (32bits or 64bits according to your perl version) and value type numeric numbers (floating point numbers).

- $tree = Tree::SizeBalanced::int\_num->new
- $tree = sbtree\_int\_num
- $tree = sbtreein

    Creat a new empty tree.

- $tree->insert($key, $value)
- $tree->insert\_after($key, $value)

    Insert an entry into the tree.
    If there are any entries with the same key size,
    insert the new one after them.

- $tree->insert\_before($key, $value)

    Insert an entry into the tree.
    If there are any entries with the same key size,
    insert the new one before them.

- $tree->delete($key)
- $tree->delete\_last($key)

    Delete one entry whose key is equal to $key.
    If there ary more than one entry with the same key size,
    delete the last inserted one.

- $tree->delete\_first($key)

    Delete one entry whose key is equal to $key.
    If there ary more than one entry with the same key size,
    delete the first inserted one.

- $size = $tree->size

    Get the number of entries in the tree

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find($key, $limit=1)
- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_first($key, $limit=1)

    Get entries with key sizes equal to $key,
    from the first inserted one to the last inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_last($key, $limit=1)

    Get entries with key sizes equal to $key,
    from the last inserted one to the first inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_lt($key, $limit=1)

    Get entries, whose keys are smaller than $key, from the largest entry.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_le($key, $limit=1)

    Get entries, whose keys are smaller than or equal to $key, from the largest entry.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt($key, $limit=1)

    Get entries, whose keys are greater than $key, from the smallest one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge($key, $limit=1)

    Get entries, whose keys are greater than or equal to $key, from the smallest one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt\_lt($lower\_key, $upper\_key)

    Get entries, whose keys are greater than $lower\_key and smaller than $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt\_le($lower\_key, $upper\_key)

    Get entries, whose keys are greater than $lower\_key and smaller than or equal to $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge\_lt($lower\_key, $upper\_key)

    Get entries, whose keys are greater than or equal to $lower\_key and smaller than $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge\_le($lower\_key, $upper\_key)

    Get entries, whose keys are greater than or equal to $lower\_key and smaller than or equal to $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_min($limit=1)

    Get entries from the one with smallest key.
    If there are more than one entries with smallest key,
    begin from the first inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_max($limit=1)

    Get entries from the one with largest key.
    If there are more than one entries with smallest key,
    begin from the last inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = &tree->skip\_l($offset, $limit=1)

    Get the first entry from one with the smallest key after skipping $offset entries.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = &tree->skip\_g($offset, $limit=1)

    Get the first entry from one with the largest key after skipping $offset entries.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $count = $tree->count\_lt($key)

    Get the number of entries whose keys are smaller than $key.

- $count = $tree->count\_le($key)

    Get the number of entries whose keys are smaller than or equal to $key.

- $count = $tree->count\_gt($key)

    Get the number of entries whose keys are greater than $key.

- $count = $tree->count\_ge($key)

    Get the number of entries whose keys are greater than or equal to $key.

- $dump\_str = $tree->dump

    Get a string which represent the whole tree structure. For debug use.

- ($order\_consistent, $size\_consistent, $balanced) = $tree->check

    Check the tree property. For debug use.

- $ever\_height = $tree->ever\_height

    Get the maximum height the tree has ever been. For debug use

### Tree::SizeBalanced::int\_any

Tree map with key type signed integers (32bits or 64bits according to your perl version) and value type any scalars.

- $tree = Tree::SizeBalanced::int\_any->new
- $tree = sbtree\_int\_any
- $tree = sbtreeia

    Creat a new empty tree.

- $tree->insert($key, $value)
- $tree->insert\_after($key, $value)

    Insert an entry into the tree.
    If there are any entries with the same key size,
    insert the new one after them.

- $tree->insert\_before($key, $value)

    Insert an entry into the tree.
    If there are any entries with the same key size,
    insert the new one before them.

- $tree->delete($key)
- $tree->delete\_last($key)

    Delete one entry whose key is equal to $key.
    If there ary more than one entry with the same key size,
    delete the last inserted one.

- $tree->delete\_first($key)

    Delete one entry whose key is equal to $key.
    If there ary more than one entry with the same key size,
    delete the first inserted one.

- $size = $tree->size

    Get the number of entries in the tree

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find($key, $limit=1)
- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_first($key, $limit=1)

    Get entries with key sizes equal to $key,
    from the first inserted one to the last inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_last($key, $limit=1)

    Get entries with key sizes equal to $key,
    from the last inserted one to the first inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_lt($key, $limit=1)

    Get entries, whose keys are smaller than $key, from the largest entry.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_le($key, $limit=1)

    Get entries, whose keys are smaller than or equal to $key, from the largest entry.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt($key, $limit=1)

    Get entries, whose keys are greater than $key, from the smallest one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge($key, $limit=1)

    Get entries, whose keys are greater than or equal to $key, from the smallest one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt\_lt($lower\_key, $upper\_key)

    Get entries, whose keys are greater than $lower\_key and smaller than $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt\_le($lower\_key, $upper\_key)

    Get entries, whose keys are greater than $lower\_key and smaller than or equal to $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge\_lt($lower\_key, $upper\_key)

    Get entries, whose keys are greater than or equal to $lower\_key and smaller than $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge\_le($lower\_key, $upper\_key)

    Get entries, whose keys are greater than or equal to $lower\_key and smaller than or equal to $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_min($limit=1)

    Get entries from the one with smallest key.
    If there are more than one entries with smallest key,
    begin from the first inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_max($limit=1)

    Get entries from the one with largest key.
    If there are more than one entries with smallest key,
    begin from the last inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = &tree->skip\_l($offset, $limit=1)

    Get the first entry from one with the smallest key after skipping $offset entries.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = &tree->skip\_g($offset, $limit=1)

    Get the first entry from one with the largest key after skipping $offset entries.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $count = $tree->count\_lt($key)

    Get the number of entries whose keys are smaller than $key.

- $count = $tree->count\_le($key)

    Get the number of entries whose keys are smaller than or equal to $key.

- $count = $tree->count\_gt($key)

    Get the number of entries whose keys are greater than $key.

- $count = $tree->count\_ge($key)

    Get the number of entries whose keys are greater than or equal to $key.

- $dump\_str = $tree->dump

    Get a string which represent the whole tree structure. For debug use.

- ($order\_consistent, $size\_consistent, $balanced) = $tree->check

    Check the tree property. For debug use.

- $ever\_height = $tree->ever\_height

    Get the maximum height the tree has ever been. For debug use

### Tree::SizeBalanced::num\_void

Tree set with key type numeric numbers (floating point numbers).

- $tree = Tree::SizeBalanced::num\_void->new
- $tree = sbtree\_num\_void
- $tree = sbtreen

    Creat a new empty tree.

- $tree->insert($key)
- $tree->insert\_after($key)

    Insert an entry into the tree.
    If there are any entries with the same key size,
    insert the new one after them.

- $tree->insert\_before($key)

    Insert an entry into the tree.
    If there are any entries with the same key size,
    insert the new one before them.

- $tree->delete($key)
- $tree->delete\_last($key)

    Delete one entry whose key is equal to $key.
    If there ary more than one entry with the same key size,
    delete the last inserted one.

- $tree->delete\_first($key)

    Delete one entry whose key is equal to $key.
    If there ary more than one entry with the same key size,
    delete the first inserted one.

- $size = $tree->size

    Get the number of entries in the tree

- $key or ($key1, $key2, ...) = $tree->find($key, $limit=1)
- $key or ($key1, $key2, ...) = $tree->find\_first($key, $limit=1)

    Get entries with key sizes equal to $key,
    from the first inserted one to the last inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = $tree->find\_last($key, $limit=1)

    Get entries with key sizes equal to $key,
    from the last inserted one to the first inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = $tree->find\_lt($key, $limit=1)

    Get entries, whose keys are smaller than $key, from the largest entry.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = $tree->find\_le($key, $limit=1)

    Get entries, whose keys are smaller than or equal to $key, from the largest entry.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = $tree->find\_gt($key, $limit=1)

    Get entries, whose keys are greater than $key, from the smallest one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = $tree->find\_ge($key, $limit=1)

    Get entries, whose keys are greater than or equal to $key, from the smallest one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = $tree->find\_gt\_lt($lower\_key, $upper\_key)

    Get entries, whose keys are greater than $lower\_key and smaller than $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $key2, ...) = $tree->find\_gt\_le($lower\_key, $upper\_key)

    Get entries, whose keys are greater than $lower\_key and smaller than or equal to $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $key2, ...) = $tree->find\_ge\_lt($lower\_key, $upper\_key)

    Get entries, whose keys are greater than or equal to $lower\_key and smaller than $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $key2, ...) = $tree->find\_ge\_le($lower\_key, $upper\_key)

    Get entries, whose keys are greater than or equal to $lower\_key and smaller than or equal to $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $key2, ...) = $tree->find\_min($limit=1)

    Get entries from the one with smallest key.
    If there are more than one entries with smallest key,
    begin from the first inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = $tree->find\_max($limit=1)

    Get entries from the one with largest key.
    If there are more than one entries with smallest key,
    begin from the last inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = &tree->skip\_l($offset, $limit=1)

    Get the first entry from one with the smallest key after skipping $offset entries.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = &tree->skip\_g($offset, $limit=1)

    Get the first entry from one with the largest key after skipping $offset entries.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $count = $tree->count\_lt($key)

    Get the number of entries whose keys are smaller than $key.

- $count = $tree->count\_le($key)

    Get the number of entries whose keys are smaller than or equal to $key.

- $count = $tree->count\_gt($key)

    Get the number of entries whose keys are greater than $key.

- $count = $tree->count\_ge($key)

    Get the number of entries whose keys are greater than or equal to $key.

- $dump\_str = $tree->dump

    Get a string which represent the whole tree structure. For debug use.

- ($order\_consistent, $size\_consistent, $balanced) = $tree->check

    Check the tree property. For debug use.

- $ever\_height = $tree->ever\_height

    Get the maximum height the tree has ever been. For debug use

### Tree::SizeBalanced::num\_int

Tree map with key type numeric numbers (floating point numbers) and value type signed integers (32bits or 64bits according to your perl version).

- $tree = Tree::SizeBalanced::num\_int->new
- $tree = sbtree\_num\_int
- $tree = sbtreeni

    Creat a new empty tree.

- $tree->insert($key, $value)
- $tree->insert\_after($key, $value)

    Insert an entry into the tree.
    If there are any entries with the same key size,
    insert the new one after them.

- $tree->insert\_before($key, $value)

    Insert an entry into the tree.
    If there are any entries with the same key size,
    insert the new one before them.

- $tree->delete($key)
- $tree->delete\_last($key)

    Delete one entry whose key is equal to $key.
    If there ary more than one entry with the same key size,
    delete the last inserted one.

- $tree->delete\_first($key)

    Delete one entry whose key is equal to $key.
    If there ary more than one entry with the same key size,
    delete the first inserted one.

- $size = $tree->size

    Get the number of entries in the tree

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find($key, $limit=1)
- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_first($key, $limit=1)

    Get entries with key sizes equal to $key,
    from the first inserted one to the last inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_last($key, $limit=1)

    Get entries with key sizes equal to $key,
    from the last inserted one to the first inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_lt($key, $limit=1)

    Get entries, whose keys are smaller than $key, from the largest entry.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_le($key, $limit=1)

    Get entries, whose keys are smaller than or equal to $key, from the largest entry.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt($key, $limit=1)

    Get entries, whose keys are greater than $key, from the smallest one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge($key, $limit=1)

    Get entries, whose keys are greater than or equal to $key, from the smallest one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt\_lt($lower\_key, $upper\_key)

    Get entries, whose keys are greater than $lower\_key and smaller than $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt\_le($lower\_key, $upper\_key)

    Get entries, whose keys are greater than $lower\_key and smaller than or equal to $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge\_lt($lower\_key, $upper\_key)

    Get entries, whose keys are greater than or equal to $lower\_key and smaller than $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge\_le($lower\_key, $upper\_key)

    Get entries, whose keys are greater than or equal to $lower\_key and smaller than or equal to $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_min($limit=1)

    Get entries from the one with smallest key.
    If there are more than one entries with smallest key,
    begin from the first inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_max($limit=1)

    Get entries from the one with largest key.
    If there are more than one entries with smallest key,
    begin from the last inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = &tree->skip\_l($offset, $limit=1)

    Get the first entry from one with the smallest key after skipping $offset entries.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = &tree->skip\_g($offset, $limit=1)

    Get the first entry from one with the largest key after skipping $offset entries.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $count = $tree->count\_lt($key)

    Get the number of entries whose keys are smaller than $key.

- $count = $tree->count\_le($key)

    Get the number of entries whose keys are smaller than or equal to $key.

- $count = $tree->count\_gt($key)

    Get the number of entries whose keys are greater than $key.

- $count = $tree->count\_ge($key)

    Get the number of entries whose keys are greater than or equal to $key.

- $dump\_str = $tree->dump

    Get a string which represent the whole tree structure. For debug use.

- ($order\_consistent, $size\_consistent, $balanced) = $tree->check

    Check the tree property. For debug use.

- $ever\_height = $tree->ever\_height

    Get the maximum height the tree has ever been. For debug use

### Tree::SizeBalanced::num\_num

Tree map with key type numeric numbers (floating point numbers) and value type numeric numbers (floating point numbers).

- $tree = Tree::SizeBalanced::num\_num->new
- $tree = sbtree\_num\_num
- $tree = sbtreenn

    Creat a new empty tree.

- $tree->insert($key, $value)
- $tree->insert\_after($key, $value)

    Insert an entry into the tree.
    If there are any entries with the same key size,
    insert the new one after them.

- $tree->insert\_before($key, $value)

    Insert an entry into the tree.
    If there are any entries with the same key size,
    insert the new one before them.

- $tree->delete($key)
- $tree->delete\_last($key)

    Delete one entry whose key is equal to $key.
    If there ary more than one entry with the same key size,
    delete the last inserted one.

- $tree->delete\_first($key)

    Delete one entry whose key is equal to $key.
    If there ary more than one entry with the same key size,
    delete the first inserted one.

- $size = $tree->size

    Get the number of entries in the tree

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find($key, $limit=1)
- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_first($key, $limit=1)

    Get entries with key sizes equal to $key,
    from the first inserted one to the last inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_last($key, $limit=1)

    Get entries with key sizes equal to $key,
    from the last inserted one to the first inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_lt($key, $limit=1)

    Get entries, whose keys are smaller than $key, from the largest entry.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_le($key, $limit=1)

    Get entries, whose keys are smaller than or equal to $key, from the largest entry.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt($key, $limit=1)

    Get entries, whose keys are greater than $key, from the smallest one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge($key, $limit=1)

    Get entries, whose keys are greater than or equal to $key, from the smallest one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt\_lt($lower\_key, $upper\_key)

    Get entries, whose keys are greater than $lower\_key and smaller than $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt\_le($lower\_key, $upper\_key)

    Get entries, whose keys are greater than $lower\_key and smaller than or equal to $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge\_lt($lower\_key, $upper\_key)

    Get entries, whose keys are greater than or equal to $lower\_key and smaller than $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge\_le($lower\_key, $upper\_key)

    Get entries, whose keys are greater than or equal to $lower\_key and smaller than or equal to $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_min($limit=1)

    Get entries from the one with smallest key.
    If there are more than one entries with smallest key,
    begin from the first inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_max($limit=1)

    Get entries from the one with largest key.
    If there are more than one entries with smallest key,
    begin from the last inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = &tree->skip\_l($offset, $limit=1)

    Get the first entry from one with the smallest key after skipping $offset entries.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = &tree->skip\_g($offset, $limit=1)

    Get the first entry from one with the largest key after skipping $offset entries.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $count = $tree->count\_lt($key)

    Get the number of entries whose keys are smaller than $key.

- $count = $tree->count\_le($key)

    Get the number of entries whose keys are smaller than or equal to $key.

- $count = $tree->count\_gt($key)

    Get the number of entries whose keys are greater than $key.

- $count = $tree->count\_ge($key)

    Get the number of entries whose keys are greater than or equal to $key.

- $dump\_str = $tree->dump

    Get a string which represent the whole tree structure. For debug use.

- ($order\_consistent, $size\_consistent, $balanced) = $tree->check

    Check the tree property. For debug use.

- $ever\_height = $tree->ever\_height

    Get the maximum height the tree has ever been. For debug use

### Tree::SizeBalanced::num\_any

Tree map with key type numeric numbers (floating point numbers) and value type any scalars.

- $tree = Tree::SizeBalanced::num\_any->new
- $tree = sbtree\_num\_any
- $tree = sbtreena

    Creat a new empty tree.

- $tree->insert($key, $value)
- $tree->insert\_after($key, $value)

    Insert an entry into the tree.
    If there are any entries with the same key size,
    insert the new one after them.

- $tree->insert\_before($key, $value)

    Insert an entry into the tree.
    If there are any entries with the same key size,
    insert the new one before them.

- $tree->delete($key)
- $tree->delete\_last($key)

    Delete one entry whose key is equal to $key.
    If there ary more than one entry with the same key size,
    delete the last inserted one.

- $tree->delete\_first($key)

    Delete one entry whose key is equal to $key.
    If there ary more than one entry with the same key size,
    delete the first inserted one.

- $size = $tree->size

    Get the number of entries in the tree

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find($key, $limit=1)
- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_first($key, $limit=1)

    Get entries with key sizes equal to $key,
    from the first inserted one to the last inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_last($key, $limit=1)

    Get entries with key sizes equal to $key,
    from the last inserted one to the first inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_lt($key, $limit=1)

    Get entries, whose keys are smaller than $key, from the largest entry.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_le($key, $limit=1)

    Get entries, whose keys are smaller than or equal to $key, from the largest entry.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt($key, $limit=1)

    Get entries, whose keys are greater than $key, from the smallest one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge($key, $limit=1)

    Get entries, whose keys are greater than or equal to $key, from the smallest one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt\_lt($lower\_key, $upper\_key)

    Get entries, whose keys are greater than $lower\_key and smaller than $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt\_le($lower\_key, $upper\_key)

    Get entries, whose keys are greater than $lower\_key and smaller than or equal to $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge\_lt($lower\_key, $upper\_key)

    Get entries, whose keys are greater than or equal to $lower\_key and smaller than $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge\_le($lower\_key, $upper\_key)

    Get entries, whose keys are greater than or equal to $lower\_key and smaller than or equal to $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_min($limit=1)

    Get entries from the one with smallest key.
    If there are more than one entries with smallest key,
    begin from the first inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_max($limit=1)

    Get entries from the one with largest key.
    If there are more than one entries with smallest key,
    begin from the last inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = &tree->skip\_l($offset, $limit=1)

    Get the first entry from one with the smallest key after skipping $offset entries.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = &tree->skip\_g($offset, $limit=1)

    Get the first entry from one with the largest key after skipping $offset entries.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $count = $tree->count\_lt($key)

    Get the number of entries whose keys are smaller than $key.

- $count = $tree->count\_le($key)

    Get the number of entries whose keys are smaller than or equal to $key.

- $count = $tree->count\_gt($key)

    Get the number of entries whose keys are greater than $key.

- $count = $tree->count\_ge($key)

    Get the number of entries whose keys are greater than or equal to $key.

- $dump\_str = $tree->dump

    Get a string which represent the whole tree structure. For debug use.

- ($order\_consistent, $size\_consistent, $balanced) = $tree->check

    Check the tree property. For debug use.

- $ever\_height = $tree->ever\_height

    Get the maximum height the tree has ever been. For debug use

### Tree::SizeBalanced::str\_void

Tree set with key type strings.

- $tree = Tree::SizeBalanced::str\_void->new
- $tree = sbtree\_str\_void
- $tree = sbtrees

    Creat a new empty tree.

- $tree->insert($key)
- $tree->insert\_after($key)

    Insert an entry into the tree.
    If there are any entries with the same key size,
    insert the new one after them.

- $tree->insert\_before($key)

    Insert an entry into the tree.
    If there are any entries with the same key size,
    insert the new one before them.

- $tree->delete($key)
- $tree->delete\_last($key)

    Delete one entry whose key is equal to $key.
    If there ary more than one entry with the same key size,
    delete the last inserted one.

- $tree->delete\_first($key)

    Delete one entry whose key is equal to $key.
    If there ary more than one entry with the same key size,
    delete the first inserted one.

- $size = $tree->size

    Get the number of entries in the tree

- $key or ($key1, $key2, ...) = $tree->find($key, $limit=1)
- $key or ($key1, $key2, ...) = $tree->find\_first($key, $limit=1)

    Get entries with key sizes equal to $key,
    from the first inserted one to the last inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = $tree->find\_last($key, $limit=1)

    Get entries with key sizes equal to $key,
    from the last inserted one to the first inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = $tree->find\_lt($key, $limit=1)

    Get entries, whose keys are smaller than $key, from the largest entry.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = $tree->find\_le($key, $limit=1)

    Get entries, whose keys are smaller than or equal to $key, from the largest entry.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = $tree->find\_gt($key, $limit=1)

    Get entries, whose keys are greater than $key, from the smallest one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = $tree->find\_ge($key, $limit=1)

    Get entries, whose keys are greater than or equal to $key, from the smallest one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = $tree->find\_gt\_lt($lower\_key, $upper\_key)

    Get entries, whose keys are greater than $lower\_key and smaller than $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $key2, ...) = $tree->find\_gt\_le($lower\_key, $upper\_key)

    Get entries, whose keys are greater than $lower\_key and smaller than or equal to $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $key2, ...) = $tree->find\_ge\_lt($lower\_key, $upper\_key)

    Get entries, whose keys are greater than or equal to $lower\_key and smaller than $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $key2, ...) = $tree->find\_ge\_le($lower\_key, $upper\_key)

    Get entries, whose keys are greater than or equal to $lower\_key and smaller than or equal to $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $key2, ...) = $tree->find\_min($limit=1)

    Get entries from the one with smallest key.
    If there are more than one entries with smallest key,
    begin from the first inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = $tree->find\_max($limit=1)

    Get entries from the one with largest key.
    If there are more than one entries with smallest key,
    begin from the last inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = &tree->skip\_l($offset, $limit=1)

    Get the first entry from one with the smallest key after skipping $offset entries.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = &tree->skip\_g($offset, $limit=1)

    Get the first entry from one with the largest key after skipping $offset entries.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $count = $tree->count\_lt($key)

    Get the number of entries whose keys are smaller than $key.

- $count = $tree->count\_le($key)

    Get the number of entries whose keys are smaller than or equal to $key.

- $count = $tree->count\_gt($key)

    Get the number of entries whose keys are greater than $key.

- $count = $tree->count\_ge($key)

    Get the number of entries whose keys are greater than or equal to $key.

- $dump\_str = $tree->dump

    Get a string which represent the whole tree structure. For debug use.

- ($order\_consistent, $size\_consistent, $balanced) = $tree->check

    Check the tree property. For debug use.

- $ever\_height = $tree->ever\_height

    Get the maximum height the tree has ever been. For debug use

### Tree::SizeBalanced::str\_int

Tree map with key type strings and value type signed integers (32bits or 64bits according to your perl version).

- $tree = Tree::SizeBalanced::str\_int->new
- $tree = sbtree\_str\_int
- $tree = sbtreesi

    Creat a new empty tree.

- $tree->insert($key, $value)
- $tree->insert\_after($key, $value)

    Insert an entry into the tree.
    If there are any entries with the same key size,
    insert the new one after them.

- $tree->insert\_before($key, $value)

    Insert an entry into the tree.
    If there are any entries with the same key size,
    insert the new one before them.

- $tree->delete($key)
- $tree->delete\_last($key)

    Delete one entry whose key is equal to $key.
    If there ary more than one entry with the same key size,
    delete the last inserted one.

- $tree->delete\_first($key)

    Delete one entry whose key is equal to $key.
    If there ary more than one entry with the same key size,
    delete the first inserted one.

- $size = $tree->size

    Get the number of entries in the tree

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find($key, $limit=1)
- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_first($key, $limit=1)

    Get entries with key sizes equal to $key,
    from the first inserted one to the last inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_last($key, $limit=1)

    Get entries with key sizes equal to $key,
    from the last inserted one to the first inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_lt($key, $limit=1)

    Get entries, whose keys are smaller than $key, from the largest entry.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_le($key, $limit=1)

    Get entries, whose keys are smaller than or equal to $key, from the largest entry.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt($key, $limit=1)

    Get entries, whose keys are greater than $key, from the smallest one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge($key, $limit=1)

    Get entries, whose keys are greater than or equal to $key, from the smallest one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt\_lt($lower\_key, $upper\_key)

    Get entries, whose keys are greater than $lower\_key and smaller than $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt\_le($lower\_key, $upper\_key)

    Get entries, whose keys are greater than $lower\_key and smaller than or equal to $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge\_lt($lower\_key, $upper\_key)

    Get entries, whose keys are greater than or equal to $lower\_key and smaller than $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge\_le($lower\_key, $upper\_key)

    Get entries, whose keys are greater than or equal to $lower\_key and smaller than or equal to $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_min($limit=1)

    Get entries from the one with smallest key.
    If there are more than one entries with smallest key,
    begin from the first inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_max($limit=1)

    Get entries from the one with largest key.
    If there are more than one entries with smallest key,
    begin from the last inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = &tree->skip\_l($offset, $limit=1)

    Get the first entry from one with the smallest key after skipping $offset entries.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = &tree->skip\_g($offset, $limit=1)

    Get the first entry from one with the largest key after skipping $offset entries.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $count = $tree->count\_lt($key)

    Get the number of entries whose keys are smaller than $key.

- $count = $tree->count\_le($key)

    Get the number of entries whose keys are smaller than or equal to $key.

- $count = $tree->count\_gt($key)

    Get the number of entries whose keys are greater than $key.

- $count = $tree->count\_ge($key)

    Get the number of entries whose keys are greater than or equal to $key.

- $dump\_str = $tree->dump

    Get a string which represent the whole tree structure. For debug use.

- ($order\_consistent, $size\_consistent, $balanced) = $tree->check

    Check the tree property. For debug use.

- $ever\_height = $tree->ever\_height

    Get the maximum height the tree has ever been. For debug use

### Tree::SizeBalanced::str\_num

Tree map with key type strings and value type numeric numbers (floating point numbers).

- $tree = Tree::SizeBalanced::str\_num->new
- $tree = sbtree\_str\_num
- $tree = sbtreesn

    Creat a new empty tree.

- $tree->insert($key, $value)
- $tree->insert\_after($key, $value)

    Insert an entry into the tree.
    If there are any entries with the same key size,
    insert the new one after them.

- $tree->insert\_before($key, $value)

    Insert an entry into the tree.
    If there are any entries with the same key size,
    insert the new one before them.

- $tree->delete($key)
- $tree->delete\_last($key)

    Delete one entry whose key is equal to $key.
    If there ary more than one entry with the same key size,
    delete the last inserted one.

- $tree->delete\_first($key)

    Delete one entry whose key is equal to $key.
    If there ary more than one entry with the same key size,
    delete the first inserted one.

- $size = $tree->size

    Get the number of entries in the tree

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find($key, $limit=1)
- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_first($key, $limit=1)

    Get entries with key sizes equal to $key,
    from the first inserted one to the last inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_last($key, $limit=1)

    Get entries with key sizes equal to $key,
    from the last inserted one to the first inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_lt($key, $limit=1)

    Get entries, whose keys are smaller than $key, from the largest entry.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_le($key, $limit=1)

    Get entries, whose keys are smaller than or equal to $key, from the largest entry.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt($key, $limit=1)

    Get entries, whose keys are greater than $key, from the smallest one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge($key, $limit=1)

    Get entries, whose keys are greater than or equal to $key, from the smallest one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt\_lt($lower\_key, $upper\_key)

    Get entries, whose keys are greater than $lower\_key and smaller than $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt\_le($lower\_key, $upper\_key)

    Get entries, whose keys are greater than $lower\_key and smaller than or equal to $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge\_lt($lower\_key, $upper\_key)

    Get entries, whose keys are greater than or equal to $lower\_key and smaller than $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge\_le($lower\_key, $upper\_key)

    Get entries, whose keys are greater than or equal to $lower\_key and smaller than or equal to $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_min($limit=1)

    Get entries from the one with smallest key.
    If there are more than one entries with smallest key,
    begin from the first inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_max($limit=1)

    Get entries from the one with largest key.
    If there are more than one entries with smallest key,
    begin from the last inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = &tree->skip\_l($offset, $limit=1)

    Get the first entry from one with the smallest key after skipping $offset entries.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = &tree->skip\_g($offset, $limit=1)

    Get the first entry from one with the largest key after skipping $offset entries.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $count = $tree->count\_lt($key)

    Get the number of entries whose keys are smaller than $key.

- $count = $tree->count\_le($key)

    Get the number of entries whose keys are smaller than or equal to $key.

- $count = $tree->count\_gt($key)

    Get the number of entries whose keys are greater than $key.

- $count = $tree->count\_ge($key)

    Get the number of entries whose keys are greater than or equal to $key.

- $dump\_str = $tree->dump

    Get a string which represent the whole tree structure. For debug use.

- ($order\_consistent, $size\_consistent, $balanced) = $tree->check

    Check the tree property. For debug use.

- $ever\_height = $tree->ever\_height

    Get the maximum height the tree has ever been. For debug use

### Tree::SizeBalanced::str\_any

Tree map with key type strings and value type any scalars.

- $tree = Tree::SizeBalanced::str\_any->new
- $tree = sbtree\_str\_any
- $tree = sbtreesa

    Creat a new empty tree.

- $tree->insert($key, $value)
- $tree->insert\_after($key, $value)

    Insert an entry into the tree.
    If there are any entries with the same key size,
    insert the new one after them.

- $tree->insert\_before($key, $value)

    Insert an entry into the tree.
    If there are any entries with the same key size,
    insert the new one before them.

- $tree->delete($key)
- $tree->delete\_last($key)

    Delete one entry whose key is equal to $key.
    If there ary more than one entry with the same key size,
    delete the last inserted one.

- $tree->delete\_first($key)

    Delete one entry whose key is equal to $key.
    If there ary more than one entry with the same key size,
    delete the first inserted one.

- $size = $tree->size

    Get the number of entries in the tree

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find($key, $limit=1)
- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_first($key, $limit=1)

    Get entries with key sizes equal to $key,
    from the first inserted one to the last inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_last($key, $limit=1)

    Get entries with key sizes equal to $key,
    from the last inserted one to the first inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_lt($key, $limit=1)

    Get entries, whose keys are smaller than $key, from the largest entry.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_le($key, $limit=1)

    Get entries, whose keys are smaller than or equal to $key, from the largest entry.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt($key, $limit=1)

    Get entries, whose keys are greater than $key, from the smallest one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge($key, $limit=1)

    Get entries, whose keys are greater than or equal to $key, from the smallest one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt\_lt($lower\_key, $upper\_key)

    Get entries, whose keys are greater than $lower\_key and smaller than $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt\_le($lower\_key, $upper\_key)

    Get entries, whose keys are greater than $lower\_key and smaller than or equal to $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge\_lt($lower\_key, $upper\_key)

    Get entries, whose keys are greater than or equal to $lower\_key and smaller than $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge\_le($lower\_key, $upper\_key)

    Get entries, whose keys are greater than or equal to $lower\_key and smaller than or equal to $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_min($limit=1)

    Get entries from the one with smallest key.
    If there are more than one entries with smallest key,
    begin from the first inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_max($limit=1)

    Get entries from the one with largest key.
    If there are more than one entries with smallest key,
    begin from the last inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = &tree->skip\_l($offset, $limit=1)

    Get the first entry from one with the smallest key after skipping $offset entries.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = &tree->skip\_g($offset, $limit=1)

    Get the first entry from one with the largest key after skipping $offset entries.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $count = $tree->count\_lt($key)

    Get the number of entries whose keys are smaller than $key.

- $count = $tree->count\_le($key)

    Get the number of entries whose keys are smaller than or equal to $key.

- $count = $tree->count\_gt($key)

    Get the number of entries whose keys are greater than $key.

- $count = $tree->count\_ge($key)

    Get the number of entries whose keys are greater than or equal to $key.

- $dump\_str = $tree->dump

    Get a string which represent the whole tree structure. For debug use.

- ($order\_consistent, $size\_consistent, $balanced) = $tree->check

    Check the tree property. For debug use.

- $ever\_height = $tree->ever\_height

    Get the maximum height the tree has ever been. For debug use

### Tree::SizeBalanced::any\_void

Tree set with key type any scalars.

- $tree = Tree::SizeBalanced::any\_void->new sub { $a cmp $b }
- $tree = sbtree\_any\_void { $a cmp $b }
- $tree = sbtreea { $a cmp $b }

    Creat a new empty tree.

- $tree->insert($key)
- $tree->insert\_after($key)

    Insert an entry into the tree.
    If there are any entries with the same key size,
    insert the new one after them.

- $tree->insert\_before($key)

    Insert an entry into the tree.
    If there are any entries with the same key size,
    insert the new one before them.

- $tree->delete($key)
- $tree->delete\_last($key)

    Delete one entry whose key is equal to $key.
    If there ary more than one entry with the same key size,
    delete the last inserted one.

- $tree->delete\_first($key)

    Delete one entry whose key is equal to $key.
    If there ary more than one entry with the same key size,
    delete the first inserted one.

- $size = $tree->size

    Get the number of entries in the tree

- $key or ($key1, $key2, ...) = $tree->find($key, $limit=1)
- $key or ($key1, $key2, ...) = $tree->find\_first($key, $limit=1)

    Get entries with key sizes equal to $key,
    from the first inserted one to the last inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = $tree->find\_last($key, $limit=1)

    Get entries with key sizes equal to $key,
    from the last inserted one to the first inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = $tree->find\_lt($key, $limit=1)

    Get entries, whose keys are smaller than $key, from the largest entry.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = $tree->find\_le($key, $limit=1)

    Get entries, whose keys are smaller than or equal to $key, from the largest entry.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = $tree->find\_gt($key, $limit=1)

    Get entries, whose keys are greater than $key, from the smallest one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = $tree->find\_ge($key, $limit=1)

    Get entries, whose keys are greater than or equal to $key, from the smallest one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = $tree->find\_gt\_lt($lower\_key, $upper\_key)

    Get entries, whose keys are greater than $lower\_key and smaller than $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $key2, ...) = $tree->find\_gt\_le($lower\_key, $upper\_key)

    Get entries, whose keys are greater than $lower\_key and smaller than or equal to $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $key2, ...) = $tree->find\_ge\_lt($lower\_key, $upper\_key)

    Get entries, whose keys are greater than or equal to $lower\_key and smaller than $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $key2, ...) = $tree->find\_ge\_le($lower\_key, $upper\_key)

    Get entries, whose keys are greater than or equal to $lower\_key and smaller than or equal to $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $key2, ...) = $tree->find\_min($limit=1)

    Get entries from the one with smallest key.
    If there are more than one entries with smallest key,
    begin from the first inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = $tree->find\_max($limit=1)

    Get entries from the one with largest key.
    If there are more than one entries with smallest key,
    begin from the last inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = &tree->skip\_l($offset, $limit=1)

    Get the first entry from one with the smallest key after skipping $offset entries.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $key2, ...) = &tree->skip\_g($offset, $limit=1)

    Get the first entry from one with the largest key after skipping $offset entries.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $count = $tree->count\_lt($key)

    Get the number of entries whose keys are smaller than $key.

- $count = $tree->count\_le($key)

    Get the number of entries whose keys are smaller than or equal to $key.

- $count = $tree->count\_gt($key)

    Get the number of entries whose keys are greater than $key.

- $count = $tree->count\_ge($key)

    Get the number of entries whose keys are greater than or equal to $key.

- $dump\_str = $tree->dump

    Get a string which represent the whole tree structure. For debug use.

- ($order\_consistent, $size\_consistent, $balanced) = $tree->check

    Check the tree property. For debug use.

- $ever\_height = $tree->ever\_height

    Get the maximum height the tree has ever been. For debug use

### Tree::SizeBalanced::any\_int

Tree map with key type any scalars and value type signed integers (32bits or 64bits according to your perl version).

- $tree = Tree::SizeBalanced::any\_int->new sub { $a cmp $b }
- $tree = sbtree\_any\_int { $a cmp $b }
- $tree = sbtreeai { $a cmp $b }

    Creat a new empty tree.

- $tree->insert($key, $value)
- $tree->insert\_after($key, $value)

    Insert an entry into the tree.
    If there are any entries with the same key size,
    insert the new one after them.

- $tree->insert\_before($key, $value)

    Insert an entry into the tree.
    If there are any entries with the same key size,
    insert the new one before them.

- $tree->delete($key)
- $tree->delete\_last($key)

    Delete one entry whose key is equal to $key.
    If there ary more than one entry with the same key size,
    delete the last inserted one.

- $tree->delete\_first($key)

    Delete one entry whose key is equal to $key.
    If there ary more than one entry with the same key size,
    delete the first inserted one.

- $size = $tree->size

    Get the number of entries in the tree

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find($key, $limit=1)
- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_first($key, $limit=1)

    Get entries with key sizes equal to $key,
    from the first inserted one to the last inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_last($key, $limit=1)

    Get entries with key sizes equal to $key,
    from the last inserted one to the first inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_lt($key, $limit=1)

    Get entries, whose keys are smaller than $key, from the largest entry.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_le($key, $limit=1)

    Get entries, whose keys are smaller than or equal to $key, from the largest entry.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt($key, $limit=1)

    Get entries, whose keys are greater than $key, from the smallest one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge($key, $limit=1)

    Get entries, whose keys are greater than or equal to $key, from the smallest one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt\_lt($lower\_key, $upper\_key)

    Get entries, whose keys are greater than $lower\_key and smaller than $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt\_le($lower\_key, $upper\_key)

    Get entries, whose keys are greater than $lower\_key and smaller than or equal to $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge\_lt($lower\_key, $upper\_key)

    Get entries, whose keys are greater than or equal to $lower\_key and smaller than $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge\_le($lower\_key, $upper\_key)

    Get entries, whose keys are greater than or equal to $lower\_key and smaller than or equal to $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_min($limit=1)

    Get entries from the one with smallest key.
    If there are more than one entries with smallest key,
    begin from the first inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_max($limit=1)

    Get entries from the one with largest key.
    If there are more than one entries with smallest key,
    begin from the last inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = &tree->skip\_l($offset, $limit=1)

    Get the first entry from one with the smallest key after skipping $offset entries.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = &tree->skip\_g($offset, $limit=1)

    Get the first entry from one with the largest key after skipping $offset entries.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $count = $tree->count\_lt($key)

    Get the number of entries whose keys are smaller than $key.

- $count = $tree->count\_le($key)

    Get the number of entries whose keys are smaller than or equal to $key.

- $count = $tree->count\_gt($key)

    Get the number of entries whose keys are greater than $key.

- $count = $tree->count\_ge($key)

    Get the number of entries whose keys are greater than or equal to $key.

- $dump\_str = $tree->dump

    Get a string which represent the whole tree structure. For debug use.

- ($order\_consistent, $size\_consistent, $balanced) = $tree->check

    Check the tree property. For debug use.

- $ever\_height = $tree->ever\_height

    Get the maximum height the tree has ever been. For debug use

### Tree::SizeBalanced::any\_num

Tree map with key type any scalars and value type numeric numbers (floating point numbers).

- $tree = Tree::SizeBalanced::any\_num->new sub { $a cmp $b }
- $tree = sbtree\_any\_num { $a cmp $b }
- $tree = sbtreean { $a cmp $b }

    Creat a new empty tree.

- $tree->insert($key, $value)
- $tree->insert\_after($key, $value)

    Insert an entry into the tree.
    If there are any entries with the same key size,
    insert the new one after them.

- $tree->insert\_before($key, $value)

    Insert an entry into the tree.
    If there are any entries with the same key size,
    insert the new one before them.

- $tree->delete($key)
- $tree->delete\_last($key)

    Delete one entry whose key is equal to $key.
    If there ary more than one entry with the same key size,
    delete the last inserted one.

- $tree->delete\_first($key)

    Delete one entry whose key is equal to $key.
    If there ary more than one entry with the same key size,
    delete the first inserted one.

- $size = $tree->size

    Get the number of entries in the tree

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find($key, $limit=1)
- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_first($key, $limit=1)

    Get entries with key sizes equal to $key,
    from the first inserted one to the last inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_last($key, $limit=1)

    Get entries with key sizes equal to $key,
    from the last inserted one to the first inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_lt($key, $limit=1)

    Get entries, whose keys are smaller than $key, from the largest entry.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_le($key, $limit=1)

    Get entries, whose keys are smaller than or equal to $key, from the largest entry.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt($key, $limit=1)

    Get entries, whose keys are greater than $key, from the smallest one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge($key, $limit=1)

    Get entries, whose keys are greater than or equal to $key, from the smallest one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt\_lt($lower\_key, $upper\_key)

    Get entries, whose keys are greater than $lower\_key and smaller than $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt\_le($lower\_key, $upper\_key)

    Get entries, whose keys are greater than $lower\_key and smaller than or equal to $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge\_lt($lower\_key, $upper\_key)

    Get entries, whose keys are greater than or equal to $lower\_key and smaller than $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge\_le($lower\_key, $upper\_key)

    Get entries, whose keys are greater than or equal to $lower\_key and smaller than or equal to $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_min($limit=1)

    Get entries from the one with smallest key.
    If there are more than one entries with smallest key,
    begin from the first inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_max($limit=1)

    Get entries from the one with largest key.
    If there are more than one entries with smallest key,
    begin from the last inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = &tree->skip\_l($offset, $limit=1)

    Get the first entry from one with the smallest key after skipping $offset entries.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = &tree->skip\_g($offset, $limit=1)

    Get the first entry from one with the largest key after skipping $offset entries.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $count = $tree->count\_lt($key)

    Get the number of entries whose keys are smaller than $key.

- $count = $tree->count\_le($key)

    Get the number of entries whose keys are smaller than or equal to $key.

- $count = $tree->count\_gt($key)

    Get the number of entries whose keys are greater than $key.

- $count = $tree->count\_ge($key)

    Get the number of entries whose keys are greater than or equal to $key.

- $dump\_str = $tree->dump

    Get a string which represent the whole tree structure. For debug use.

- ($order\_consistent, $size\_consistent, $balanced) = $tree->check

    Check the tree property. For debug use.

- $ever\_height = $tree->ever\_height

    Get the maximum height the tree has ever been. For debug use

### Tree::SizeBalanced::any\_any

Tree map with key type any scalars and value type any scalars.

- $tree = Tree::SizeBalanced::any\_any->new sub { $a cmp $b }
- $tree = sbtree\_any\_any { $a cmp $b }
- $tree = sbtreeaa { $a cmp $b }

    Creat a new empty tree.

- $tree->insert($key, $value)
- $tree->insert\_after($key, $value)

    Insert an entry into the tree.
    If there are any entries with the same key size,
    insert the new one after them.

- $tree->insert\_before($key, $value)

    Insert an entry into the tree.
    If there are any entries with the same key size,
    insert the new one before them.

- $tree->delete($key)
- $tree->delete\_last($key)

    Delete one entry whose key is equal to $key.
    If there ary more than one entry with the same key size,
    delete the last inserted one.

- $tree->delete\_first($key)

    Delete one entry whose key is equal to $key.
    If there ary more than one entry with the same key size,
    delete the first inserted one.

- $size = $tree->size

    Get the number of entries in the tree

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find($key, $limit=1)
- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_first($key, $limit=1)

    Get entries with key sizes equal to $key,
    from the first inserted one to the last inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_last($key, $limit=1)

    Get entries with key sizes equal to $key,
    from the last inserted one to the first inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_lt($key, $limit=1)

    Get entries, whose keys are smaller than $key, from the largest entry.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_le($key, $limit=1)

    Get entries, whose keys are smaller than or equal to $key, from the largest entry.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt($key, $limit=1)

    Get entries, whose keys are greater than $key, from the smallest one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge($key, $limit=1)

    Get entries, whose keys are greater than or equal to $key, from the smallest one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt\_lt($lower\_key, $upper\_key)

    Get entries, whose keys are greater than $lower\_key and smaller than $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_gt\_le($lower\_key, $upper\_key)

    Get entries, whose keys are greater than $lower\_key and smaller than or equal to $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge\_lt($lower\_key, $upper\_key)

    Get entries, whose keys are greater than or equal to $lower\_key and smaller than $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_ge\_le($lower\_key, $upper\_key)

    Get entries, whose keys are greater than or equal to $lower\_key and smaller than or equal to $upper\_key,
    from the smallest one to the largest one.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_min($limit=1)

    Get entries from the one with smallest key.
    If there are more than one entries with smallest key,
    begin from the first inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = $tree->find\_max($limit=1)

    Get entries from the one with largest key.
    If there are more than one entries with smallest key,
    begin from the last inserted one.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = &tree->skip\_l($offset, $limit=1)

    Get the first entry from one with the smallest key after skipping $offset entries.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $key or ($key1, $value1, $key2, $value2, ...) = &tree->skip\_g($offset, $limit=1)

    Get the first entry from one with the largest key after skipping $offset entries.

    The optional $limit (default 1) indicates the maximum entry number you will get,
    $limit=-1 means unlimited.

- $count = $tree->count\_lt($key)

    Get the number of entries whose keys are smaller than $key.

- $count = $tree->count\_le($key)

    Get the number of entries whose keys are smaller than or equal to $key.

- $count = $tree->count\_gt($key)

    Get the number of entries whose keys are greater than $key.

- $count = $tree->count\_ge($key)

    Get the number of entries whose keys are greater than or equal to $key.

- $dump\_str = $tree->dump

    Get a string which represent the whole tree structure. For debug use.

- ($order\_consistent, $size\_consistent, $balanced) = $tree->check

    Check the tree property. For debug use.

- $ever\_height = $tree->ever\_height

    Get the maximum height the tree has ever been. For debug use

### Default type constructors

- $tree = Tree::SizeBalanced->new;

    equivalent to `$tree = Tree::SizeBalanced::int_any->new;`

- $tree = Tree::SizeBalanced->new sub { $a cmp $b };

    equivalent to `$tree = Tree::SizeBalanced::any_any->new;`

- $tree = sbtree;

    equivalent to `$tree = sbtreeia`

- $tree = sbtree { $a cmp $b };

    equivalent to `$tree = sbtreeaa`

# BENCHMARK

test result: (perl 5.22.2, Tree::SizeBalanced 2.6)

[incremental integer query](https://github.com/CindyLinz/Perl-Tree-SizeBalanced/blob/master/benchmark/incremental_integer_query.pl) seed\_count=10, data\_size=100\_000, verbose=0

    Benchmark: timing 1 iterations of Sorted array, Static array, tree set any, tree set int...
    Sorted array: 12 wallclock secs (12.60 usr +  0.00 sys = 12.60 CPU) @  0.08/s (n=1)
                (warning: too few iterations for a reliable count)
    ^CSIGINT!
    Static array: 737 wallclock secs (736.96 usr +  0.14 sys = 737.10 CPU) @  0.00/s (n=1)
                (warning: too few iterations for a reliable count)
    tree set any:  5 wallclock secs ( 4.70 usr +  0.01 sys =  4.71 CPU) @  0.21/s (n=1)
                (warning: too few iterations for a reliable count)
    tree set int:  1 wallclock secs ( 0.69 usr +  0.01 sys =  0.70 CPU) @  1.43/s (n=1)
                (warning: too few iterations for a reliable count)

    (Note that "Static array" didn't complete. It's interrupted)

[incremental string query](https://github.com/CindyLinz/Perl-Tree-SizeBalanced/blob/master/benchmark/incremental_string_query.pl) seed\_count=10, data\_size=100\_000, verbose=0

    Benchmark: timing 1 iterations of Sorted array, Static array, tree set any, tree set str...
    Sorted array: 15 wallclock secs (15.28 usr +  0.00 sys = 15.28 CPU) @  0.07/s (n=1)
                (warning: too few iterations for a reliable count)
    ^CSIGINT!
    Static array: 673 wallclock secs (672.08 usr +  0.15 sys = 672.23 CPU) @  0.00/s (n=1)
                (warning: too few iterations for a reliable count)
    tree set any:  6 wallclock secs ( 6.65 usr +  0.00 sys =  6.65 CPU) @  0.15/s (n=1)
                (warning: too few iterations for a reliable count)
    tree set str:  2 wallclock secs ( 1.88 usr +  0.00 sys =  1.88 CPU) @  0.53/s (n=1)
                (warning: too few iterations for a reliable count)

    (Note that "Static array" didn't complete. It's interrupted)

[bulk integer query](https://github.com/CindyLinz/Perl-Tree-SizeBalanced/blob/master/benchmark/bulk_integer_query.pl) seed\_count=10, data\_size=100\_000, verbose=0

    Benchmark: timing 1 iterations of Sorted array, Static array, tree set any, tree set int...
    Sorted array:  3 wallclock secs ( 2.99 usr +  0.00 sys =  2.99 CPU) @  0.33/s (n=1)
                (warning: too few iterations for a reliable count)
    ^CSIGINT!
    Static array: 251 wallclock secs (251.85 usr +  0.02 sys = 251.87 CPU) @  0.00/s (n=1)
                (warning: too few iterations for a reliable count)
    tree set any:  6 wallclock secs ( 5.24 usr +  0.00 sys =  5.24 CPU) @  0.19/s (n=1)
                (warning: too few iterations for a reliable count)
    tree set int:  1 wallclock secs ( 0.86 usr +  0.00 sys =  0.86 CPU) @  1.16/s (n=1)
                (warning: too few iterations for a reliable count)

    (Note that "Static array" didn't complete. It's interrupted)

[bulk string query](https://github.com/CindyLinz/Perl-Tree-SizeBalanced/blob/master/benchmark/bulk_string_query.pl) seed\_count=10, data\_size=100\_000, verbose=0

    Benchmark: timing 1 iterations of Sorted array, Static array, tree set any, tree set int...
    Sorted array:  5 wallclock secs ( 5.59 usr +  0.00 sys =  5.59 CPU) @  0.18/s (n=1)
                (warning: too few iterations for a reliable count)
    ^CSIGINT!
    Static array: 363 wallclock secs (361.56 usr +  0.07 sys = 361.63 CPU) @  0.00/s (n=1)
                (warning: too few iterations for a reliable count)
    tree set any:  8 wallclock secs ( 7.85 usr +  0.00 sys =  7.85 CPU) @  0.13/s (n=1)
                (warning: too few iterations for a reliable count)
    tree set int:  3 wallclock secs ( 3.27 usr +  0.01 sys =  3.28 CPU) @  0.30/s (n=1)
                (warning: too few iterations for a reliable count)

    (Note that "Static array" didn't complete. It's interrupted)

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

# ACKNOWLEDGEMENT

Thank TDYa127 [https://github.com/a127a127/](https://github.com/a127a127/) who tell me size balanced tree.
