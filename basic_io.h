/*
 * Copyright (c) 2020, Olivier Valentin
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef BASIC_IO_H_
#define BASIC_IO_H_

// Prints a character
void basic_io_outc(char c);

// Waits for a character to be available and return it
char basic_io_inc();

#endif /* BASIC_IO_H_ */
