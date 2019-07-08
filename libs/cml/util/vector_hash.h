/* -*- C++ -*- ------------------------------------------------------------
 @@COPYRIGHT@@
 *-----------------------------------------------------------------------*/
/** @file
 */

#pragma once

#ifndef cml_util_vector_hash_h
#define cml_util_vector_hash_h

#include <boost/functional/hash.hpp>
#include <cml/vector/vector.h>

/* Need specialization in std: */
namespace std {
template<class Element, class StorageType>
struct hash<cml::vector<Element,StorageType>>
{
  typedef cml::vector<Element,StorageType> vector_type;
  inline std::size_t operator()(const vector_type& v) const {
    std::size_t seed = 0;
    for(int i = 0; i < v.size(); ++ i) boost::hash_combine(seed, v.get(i));
    return seed;
  }
};
}

#endif

// -------------------------------------------------------------------------
// vim:ft=cpp:sw=2
