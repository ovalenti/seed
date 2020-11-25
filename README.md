# seed
Seed is an interpreted language designed to maximize the ratio between the size of the interpreter (lowest) and its potential as a programming language (highest).
The ultimate goal is to bootstrap an autonomous (self-hosted) system made almost entirely of source code; the only binary part being the interpreter. Hence the name `seed`.

Needless to say that the language is rough, but it is quite extensible and I believe that the most complex constructs can be implemented in the language itself.

Being an interpreted language does not mean it is interractive: **seed is not an interactive language**. This is not an implementation issue, but is related to the nature of the language itself.

# Getting to hack

The file program.c defines a C string which is the Seed code to be executed. Replace it with your code, build and run the interpreter.

# Language basics
- Seed is a stack based language and uses [RPN](https://en.wikipedia.org/wiki/Reverse_Polish_notation).
- all values are of integer type, with no string typing.
- a plain value gets pushed to the top of the stack.
- an operator will consumme inputs from the stack and push back the result.
- operators are strings enclosed with brackets.

> [DROP] [DUP] [ADD] [SUB] ...

*Example:*

> 2 2 [ADD]

| before [ADD] | after [ADD] |
| -:| -:|
| 2 | 4 |
| 2 | |

# Stack manipulation

| Function | Description |
| - |:- |
| [DROP] | pops the top value |
| [DUP] | read the top value and push it |
| [ROT] | pop the top value N, and move the N'th stack element top the top |
| [SWAP] | swap 2 top most values (implemented with [ROT]) |

# Code block

Braces `{}` can be used to enclose parts of the source code.

When an opening brace `{` is read, the interpreter pushes the current location (memory pointer) on the stack, and stops executing the code until a matching closing brace `}` is reached.

This construct can be used to create functions or reference to litteral values. When the interpreter executes a closing brace `}`, it returns to a calling context.

*Example:* (note that `!` is a call to the code pointed to the top most value of the stack)

```
2
{
  2
  [ADD]
}
!
```

At the end of execution, one value remains on the stack: 4

This code will push on the stack the address of the string "Hello!" (right where it is in the memory):

```
{Hello!}
``` 

# Variables

Seed provides with the concept of simple variables. They bind a name to an integer value.

`[STORE]` creates/updates a variable. It will consume 2 elements of the stack: a pointer to the name of the variable, and the value to store.

`*` dereferences a variable. It will consume a pointer to the name of the variable, and push its current value.

*Example:* store 42 in variable UNIVERSE:

```
42
{UNIVERSE}
[STORE]
```

Example: read the value of variable UNIVERSE:

```
{UNIVERSE} *
```

*Example:* create a function DOUBLE and call it:

```
{ 2 [MUL] } {DOUBLE} [STORE]

2
{DOUBLE} * !
```

As a shortcut, enclosing a string with square brackets is equivalent to dereferencing the variable and calling-it.

*Example:* this code is equivalent to the previous example:

```
{ 2 [MUL] } {DOUBLE} [STORE]

2 [DOUBLE]
```

# Execution flow

Functions are implemented by enclosing their code between ̀`{}` and storing the address in a variable. A call to a function is achieved by `!` on the address or using the sqare brackets around its name.

Functions can be nested.

The conditionnal execution is achieved by two means:
- `?` consumes the 2 top most values of the stack and calls code pointed by the second when the first does not equal 0. This is typically the usual IF statement.
- `??` consumes the top value and calls it. At the end of execution, a stacked value is expected and consumed. The code is executed again until the resulting value equals 0. This is typically a sort of WHILE statement.

# How would I...

*build an if-then-else construct:*

```
{ {A is smaller than 42\0} [PRINTSTR] }
{ {A is greater than 42\0} [PRINTSTR] }
{[SWAP]}
{A} * 42 [GT]
? [DROP] !
```

*build a while(<test>) construct (while not using the stack to store the test result):*

Use the ?? builtin and wrap the body in a conditional.

```
{
	{ 0 }
	{ <body> 1 }
	{[SWAP]}
	<test> ? [DROP] !
} ??
```

*Compute Fibonacci numbers:*

```
{
	[DUP]
	2 [GE]
	{
		[DUP]
		1 [SUB]
		[FIBO]
		[SWAP]
		2 [SUB]
		[FIBO]
		[ADD]
	}
	[SWAP] ?
} {FIBO} [STORE]

10 [FIBO] [PRINTINT]
```

*Load the ascii value of the 'c' character:*

```
{c} [PEEK]
```

