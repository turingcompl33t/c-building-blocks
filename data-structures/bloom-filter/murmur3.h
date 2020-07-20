//-----------------------------------------------------------------------------
// MurmurHash3 was written by Austin Appleby, and is placed in the public domain. 
// The author hereby disclaims copyright to this source code.

#ifndef _MURMURHASH3_H_
#define _MURMURHASH3_H_

#include <stdint.h>

// MurmurHash3_x86_32()
//
// Arguments:
//  key  - the data to be hashed
//  len  - the length of the data
//  seed - a seed for this invocation of the hash function
//  out  - buffer into which hash digest will be written
//
// Returns:
//  A 32-bit hash digest in buffer `out`
void MurmurHash3_x86_32 (const void *key, int len, uint32_t seed, void *out);

// MurmurHash3_x86_128()
//
// Arguments:
//  key  - the data to be hashed
//  len  - the length of the data
//  seed - a seed for this invocation of the hash function
//  out  - buffer into which hash digest will be written
//
// Returns:
//  A 128-bit hash digest in buffer `out`
void MurmurHash3_x86_128(const void *key, int len, uint32_t seed, void *out);

// MurmurHash3_x64_128()
//
// Arguments:
//  key  - the data to be hashed
//  len  - the length of the data
//  seed - a seed for this invocation of the hash function
//  out  - buffer into which hash digest will be written
//
// Returns:
//  A 128-bit hash digest in buffer `out`
void MurmurHash3_x64_128(const void *key, int len, uint32_t seed, void *out);

#endif // _MURMURHASH3_H_
