# STACK
Modern C-style stack with templates, protection, debug and dumping. You can also use this stack instead of your implementation to catch the errors.

-------------
## Signature of stack methods:
| Method | Arg1 | Arg2 | Arg3 | Arg4 |
|:----------------|:---------:|:---------:|:---------:|:---------:|
| ```StackCtor(TYPE, STACK, CAPACITY, PRINT)``` | type of Stack | [&] Stack var | Start capacity | [&] Print fucntion |
| ```StackDtor(TYPE, STACK)``` | type of Stack | [&] Stack var | | |
| ```StackPush(TYPE, STACK, VALUE)``` | type of Stack | [&] Stack var | value to push |  |
| ```StackPop(TYPE, STACK, VALUE)``` | type of Stack | [&] Stack var | [&] input variable for answer |  |
| ```StackSize(TYPE, STACK, VALUE)``` | type of Stack | [&] Stack var | [&] input variable for answer |  |
| ```StackDump(TYPE, STACK)``` | type of Stack | [&] Stack var |  |  |
| ```StackVerify(TYPE, STACK)``` | type of Stack | [&] Stack var |  |  |

--------------
## Description of stack methods:
* StackCtor - Creates new stack, allocates memory and prepares stack for work. `Accepts only empty zeroed stack. Please use {0} / calloc`
* StackDtor - Deletes stack, frees up memory. You can use variable after destructor to create new stack.
* StackPush - Puts a variable on the stack.
* StackPop - Takes a variable out of the stack and puts it in value arguments, which should be transmitted by a link.
* StackSize - Gets size of the stack and puts it in value arguments, which should be transmitted by a link.
* StackDump - Prints dump of stack. [STACK_DUMPING] needed.
* StackVerify - Verifies stack and returns errors.
* [EachMethod] returns error code.

## Flags
* ```STACK_CANARY_PROTECT``` - Turns on canary protect
* ```STACK_HASH_PROTECT``` - Turns on hash protect
* ```STACK_DUMPING``` - Turns on dumping. Dumps every error!

------------------------------------------------------
## Examples:

### SimpleStack
```
#include <stdio.h>

#define STACK_CANARY_PROTECT
#define STACK_HASH_PROTECT
#define STACK_DUMPING

#define STACK_TYPE int
#include "includes/stack.h"
#undef STACK_TYPE

void PrintInt(int i) { printf("%d", i); }

int main()
{
    Stack_int stk = {}; // Zeroing out the structure
    StackCtor(int, &stk, 5, PrintInt); // Constructs the stack
    StackPush(int, &stk, 249);
    StackPush(int, &stk, 810);
    StackPush(int, &stk, 666);

    int temp = 0;
    StackPop(int, &stk, &temp);
    PrintInt(temp); // 666

    StackDtor(int, &stk); // Deletes the stack
    return 0;
}
```