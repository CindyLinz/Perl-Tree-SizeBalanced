#include "tree_key.h"

#define VALUE void
#  include "tree_key_value.h"
#  include "xs.h"
#undef VALUE

#define VALUE int
#  include "tree_key_value.h"
#  include "xs.h"
#undef VALUE

#define VALUE num
#  include "tree_key_value.h"
#  include "xs.h"
#undef VALUE
