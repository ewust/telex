/* Copyright 2012, 2013, 2014  University of Michigan

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

// Modified for Telex Anticensorship 2011, telex.cc
//  Based on "int128.h", Apache license, original
//  Copyright 2004 Google Inc.
//  All Rights Reserved.

#ifndef BASE_INT128_H_
#define BASE_INT128_H_

#include <iostream>
using std::ostream;
using std::cout;
using std::endl;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

#include <assert.h>
#define DCHECK_GE(x,y) assert((x) >= (y))

//#include "base/integral_types.h"
//#include "base/logging.h"

// An unsigned 128-bit integer type. Thread-compatible.
class uint128 {
public:
  uint128();  // Sets to 0, but don't trust on this behavior.
  uint128(uint64 top, uint64 bottom);
#ifndef SWIG
  uint128(int bottom);
  uint128(uint32 bottom);   // Top 96 bits = 0
#endif
  uint128(uint64 bottom);   // hi_ = 0
  uint128(const uint128 &val);

  void Initialize(uint64 top, uint64 bottom);

  bool operator==(const uint128& b) const;
  bool operator!=(const uint128& b) const;
  uint128& operator=(const uint128& b);

  bool operator<(const uint128& b) const;
  bool operator>(const uint128& b) const;
  bool operator<=(const uint128& b) const;
  bool operator>=(const uint128& b) const;

  // Logical operators.
  uint128 operator~() const;
  uint128 operator|(const uint128& b) const;
  uint128 operator&(const uint128& b) const;
  uint128 operator^(const uint128& b) const;
  uint128 operator|=(const uint128& b);
  uint128 operator&=(const uint128& b);
  uint128 operator^=(const uint128& b);

  // Shift operators.
  uint128 operator<<(int amount) const;
  uint128 operator>>(int amount) const;

  // Arithmetic operators.
  // TODO: division, etc.
  uint128 operator*(const uint128& b) const;
  uint128 operator+(const uint128& b) const;
  uint128 operator-(const uint128& b) const;
  uint128 operator*=(const uint128& b);
  uint128 operator+=(const uint128& b);
  uint128 operator-=(const uint128& b);
  uint128 operator++(int);
  uint128 operator--(int);
  uint128 operator++();
  uint128 operator--();

  friend uint64 Uint128Low64(const uint128& v);
  friend uint64 Uint128High64(const uint128& v);

  friend ostream& operator<<(ostream& o, const uint128& b);

private:
  // Little-endian memory order optimizations can benefit from
  // having lo_ first, hi_ last.
  // See util/endian/endian.h and Load128/Store128 for storing a uint128.
  uint64        lo_;
  uint64        hi_;

  // Not implemented, just declared for catching automatic type conversions.
  uint128(uint8);
  uint128(uint16);
  uint128(float v);
  uint128(double v);
};

extern const uint128 kuint128max;

// allow uint128 to be logged
extern ostream& operator<<(ostream& o, const uint128& b);

// Methods to access low and high pieces of 128-bit value.
// Defined externally from uint128 to facilitate conversion
// to native 128-bit types when compilers support them.
inline uint64 Uint128Low64(const uint128& v) { return v.lo_; }
inline uint64 Uint128High64(const uint128& v) { return v.hi_; }

// TODO: perhaps it would be nice to have int128, a signed 128-bit type?

// --------------------------------------------------------------------------
//                      Implementation details follow
// --------------------------------------------------------------------------
inline bool uint128::operator==(const uint128& b) const {
  return (lo_ == b.lo_) && (hi_ == b.hi_);
}
inline bool uint128::operator!=(const uint128& b) const {
  return !(*this == b);
}
inline uint128& uint128::operator=(const uint128& b) {
  lo_ = b.lo_;
  hi_ = b.hi_;
  return *this;
}

inline uint128::uint128(): lo_(0), hi_(0) { }
inline uint128::uint128(uint64 top, uint64 bottom) : lo_(bottom), hi_(top) { }
inline uint128::uint128(const uint128 &v) : lo_(v.lo_), hi_(v.hi_) { }
inline uint128::uint128(uint64 bottom) : lo_(bottom), hi_(0) { }
#ifndef SWIG
inline uint128::uint128(uint32 bottom) : lo_(bottom), hi_(0) { }
inline uint128::uint128(int bottom) : lo_(bottom), hi_(0) {
  if (bottom < 0) {
    --hi_;
  }
}
#endif
inline void uint128::Initialize(uint64 top, uint64 bottom) {
  hi_ = top;
  lo_ = bottom;
}

// Comparison operators.

#define CMP128(op)                                              \
inline bool uint128::operator op(const uint128& b) const {      \
  return (hi_ == b.hi_) ? (lo_ op b.lo_) : (hi_ op b.hi_);      \
}

CMP128(<)
CMP128(>)
CMP128(>=)
CMP128(<=)

#undef CMP128

// Logical operators.

inline uint128 uint128::operator~() const {
  return uint128(~hi_, ~lo_);
}

#define LOGIC128(op)                                             \
inline uint128 uint128::operator op(const uint128& b) const {    \
  return uint128(hi_ op b.hi_, lo_ op b.lo_);                    \
}

#define ULOGIC128(uop)                                           \
inline uint128 uint128::operator uop(const uint128& b) {         \
  hi_ uop b.hi_;                                                 \
  lo_ uop b.lo_;                                                 \
  return *this;                                                  \
}

LOGIC128(|)
LOGIC128(&)
LOGIC128(^)
ULOGIC128(|=)
ULOGIC128(&=)
ULOGIC128(^=)

#undef LOGIC128
#undef ULOGIC128

// Shift operators.

inline uint128 uint128::operator<<(int amount) const {
  DCHECK_GE(amount, 0);

  // uint64 shifts of >= 64 are undefined, so we will need some special-casing.
  if (amount < 64) {
    if (amount == 0) {
      return *this;
    }
    uint64 new_hi = (hi_ << amount) | (lo_ >> (64 - amount));
    uint64 new_lo = lo_ << amount;
    return uint128(new_hi, new_lo);
  } else if (amount < 128) {
    return uint128(lo_ << (amount - 64), 0);
  } else {
    return uint128(0, 0);
  }
}

inline uint128 uint128::operator>>(int amount) const {
  DCHECK_GE(amount, 0);

  // uint64 shifts of >= 64 are undefined, so we will need some special-casing.
  if (amount < 64) {
    if (amount == 0) {
      return *this;
    }
    uint64 new_hi = hi_ >> amount;
    uint64 new_lo = (lo_ >> amount) | (hi_ << (64 - amount));
    return uint128(new_hi, new_lo);
  } else if (amount < 128) {
    return uint128(0, hi_ >> (amount - 64));
  } else {
    return uint128(0, 0);
  }
}

inline uint128 uint128::operator*(const uint128& b) const {
  return uint128(*this) *= b;
}

inline uint128 uint128::operator+(const uint128& b) const {
  return uint128(*this) += b;
}

inline uint128 uint128::operator-(const uint128& b) const {
  return uint128(*this) -= b;
}

/**
 From G3D/uint128.cpp
 
 @maintainer Morgan McGuire, http://graphics.cs.williams.edu
 @author Kyle Whitson
 
 @created 2008-07-17
 @edited  2008-07-17
 */
static void addAndCarry(const uint64& _a, const uint64& _b, uint64& carry, uint64& result) {
        
    // Break each number into 4 32-bit chunks. Since we are using uints, right-shifting will fill with zeros.
    // This eliminates the need to and with 0xFFFFFFFF.
    uint32 a [2] = {_a & 0xFFFFFFFF, _a >> 32};
    uint32 b [2] = {_b & 0xFFFFFFFF, _b >> 32};

    uint64 tmp = uint64(a[0]) + b[0];

    result = tmp & 0xFFFFFFFF;
    uint32 c = tmp >> 32;

    tmp = uint64(c) + a[1] + b[1];
    result += tmp << 32;
    carry = (tmp >> 32);
}

void multiplyAndCarry(const uint64& _a, const uint64& _b, uint64& carry, uint64& result) {

    // Break each number into 4 32-bit chunks. Since we are using uints, right-shifting will fill with zeros.
    // This eliminates the need to and with 0xFFFFFFFF.
    uint32 a [2] = {_a & 0xFFFFFFFF, _a >> 32};
    uint32 b [2] = {_b & 0xFFFFFFFF, _b >> 32};

    uint64 prod [2][2];
    for(int i = 0; i < 2; ++i) {
        for(int j = 0; j < 2; ++j) {
            prod[i][j] = uint64(a[i]) * b[j];
        }
    }

    // The product of the low bits of a and b will always fit into the result
    result = prod[0][0];

    // The product of the high bits of a and b will never fit into the result
    carry = prod[1][1];

    // The high 32 bits of prod[0][1] and prod[1][0] will never fit into the result
    carry += prod[0][1] >> 32;
    carry += prod[1][0] >> 32;

    uint64 tmp;
    addAndCarry(result, (prod[0][1] << 32), tmp, result);
    carry += tmp;
    addAndCarry(result, (prod[1][0] << 32), tmp, result);
    carry += tmp;
}

inline uint128 uint128::operator*=(const uint128& b) {
    // The low bits will get overwritten when doing the multiply, so back up both (in case &x == this)
    const uint64 oldLo = lo_;
    const uint64 oldBLo = b.lo_;

    uint64 carry;
    multiplyAndCarry(oldLo, oldBLo, carry, lo_);

    // Overflow doesn't matter here because the result is going into hi - any overflow will exceed the capacity of a 128-bit number
    // Note: hi * x.hi will always overflow, since (x * 2^64) * (y * 2^64) = x*y*(2^128). The largest number expressable in 128 bits is
    // 2^128 - 1.
    hi_ = carry + (oldLo * b.hi_) + (hi_ * oldBLo);

    return *this;

}

inline uint128 uint128::operator+=(const uint128& b) {
  hi_ += b.hi_;
  lo_ += b.lo_;
  if (lo_ < b.lo_)
    ++hi_;
  return *this;
}

inline uint128 uint128::operator-=(const uint128& b) {
  hi_ -= b.hi_;
  if (b.lo_ > lo_)
    --hi_;
  lo_ -= b.lo_;
  return *this;
}

inline uint128 uint128::operator++(int) {
  uint128 tmp(*this);
  *this += 1;
  return tmp;
}

inline uint128 uint128::operator--(int) {
  uint128 tmp(*this);
  *this -= 1;
  return tmp;
}

inline uint128 uint128::operator++() {
  *this += 1;
  return *this;
}

inline uint128 uint128::operator--() {
  *this -= 1;
  return *this;
}

#endif  // BASE_INT128_H_
