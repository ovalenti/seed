/*
 * Copyright (c) 2020, Olivier Valentin
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef INTERPRETER_H_
#define INTERPRETER_H_

// the stack values (stack[0] is the first value pushed)
extern unsigned long stack[];
// index of first free location in stack
extern unsigned long stack_ptr;

// scope management
unsigned long* scope_find(char* name);

// start interpreting at loc
void execute_at(char *loc);

// hook to extend builtin commands
extern int (*extra_builtin)(char** loc);


int eat(const char *word, char **location);


#endif /* INTERPRETER_H_ */
