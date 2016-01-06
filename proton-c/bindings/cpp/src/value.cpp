/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include "proton_bits.hpp"

#include "proton/data.hpp"
#include "proton/value.hpp"
#include "proton/scalar.hpp"

#include <ostream>

namespace proton {

value::value() : data_(data::create()) {}

value::value(const value& x) : data_(data::create()) { data_.copy(x.data_); }

#if PN_HAS_CPP11
value::value(value&& x) : data_(0) { swap(x); }
#endif

// Referencing an external value
value::value(data d) : data_(d) {}

// Referencing an external value
value& value::ref(data d) { data_ = d; return *this; }

value& value::operator=(const value& x) { data_.copy(x.data_); return *this; }

void value::swap(value& x) { std::swap(data_, x.data_); }

void value::clear() { data_.clear(); }

bool value::empty() const { return data_.empty(); }

class encoder value::encoder() { clear(); return data_.encoder(); }

class decoder value::decoder() const { data_.decoder().rewind(); return data_.decoder(); }

type_id value::type() const { return decoder().type(); }

bool operator==(const value& x, const value& y) { return x.data_.equal(y.data_); }

bool operator<(const value& x, const value& y) { return x.data_.less(y.data_); }

std::ostream& operator<<(std::ostream& o, const value& v) {
    // pn_inspect prints strings with quotes which is not normal in C++.
    if (v.empty())
        return o << "<empty>";
    switch (v.type()) {
      case STRING:
      case SYMBOL:
        return o << v.get<std::string>();
      default:
        return o << v.data_;
    }
}

int64_t value::as_int() const { return get<scalar>().as_int(); }
uint64_t value::as_uint() const { return get<scalar>().as_uint(); }
double value::as_double() const { return get<scalar>().as_double(); }
std::string value::as_string() const { return get<scalar>().as_string(); }

}