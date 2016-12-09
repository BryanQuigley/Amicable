///
/// @file   SieveOfEratosthenes-inline.hpp
/// @brief  Inline methods of the SieveOfEratosthenes class.
///
/// Copyright (C) 2016 Kim Walisch, <kim.walisch@gmail.com>
///
/// This file is distributed under the BSD License. See the COPYING
/// file in the top level directory.
///

#ifndef SIEVEOFERATOSTHENES_INLINE_HPP
#define SIEVEOFERATOSTHENES_INLINE_HPP

#include "config.hpp"
#include "SieveOfEratosthenes.hpp"
#include "EratSmall.hpp"
#include "EratMedium.hpp"
#include "EratBig.hpp"
#include "pmath.hpp"

#include <stdint.h>

namespace primesieve {

inline uint64_t SieveOfEratosthenes::getStart() const
{
  return start_;
}

inline uint64_t SieveOfEratosthenes::getStop() const
{
  return stop_;
}

inline uint_t SieveOfEratosthenes::getSieveSize() const
{
  return sieveSize_;
}

/// This method must be called consecutively for all primes up to
/// sqrt(stop) in order to sieve the primes within the
/// interval [start, stop].
///
inline void SieveOfEratosthenes::addSievingPrime(uint_t prime)
{
  uint64_t square = isquare<uint64_t>(prime);

  // This loop is executed once all primes <= sqrt(segmentHigh_)
  // required to sieve the next segment have been
  // added to the erat* objects further down.
  while (segmentHigh_ < square)
    sieveSegment();

       if (prime > limitEratMedium_)   eratBig_->addSievingPrime(prime, segmentLow_);
  else if (prime > limitEratSmall_) eratMedium_->addSievingPrime(prime, segmentLow_);
  else /* (prime > limitPreSieve) */ eratSmall_->addSievingPrime(prime, segmentLow_);
}

} // namespace primesieve

#endif
