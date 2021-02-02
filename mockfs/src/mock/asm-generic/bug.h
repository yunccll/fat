
#ifndef  ASM_GENERIC_BUG_H
#define  ASM_GENERIC_BUG_H

#include <assert.h>
#include "mock/linux/compiler.h"

#define BUG_ON(x)  assert(x);

#define WARN_ON(condition) ({						\
	int __ret_warn_on = !!(condition);				\
	if (unlikely(__ret_warn_on))					\
		pr_warn("warn on"#condition);				\
	unlikely(__ret_warn_on);						\
})


#endif   /* ASM_GENERIC_BUG_H */
