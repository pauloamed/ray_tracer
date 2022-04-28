#ifndef PARAMSET_H
#define PARAMSET_H

#include <iostream>
#include <map>
#include <memory>
#include <string>

/// Pure virtual basic type. The map stores a pointer to the base class
class ValueBase {
public:
  ValueBase() {}
  virtual ~ValueBase() {}
};

/// We must convert the base class object to the proper derived class object.
template <typename T> class Value : public ValueBase {
private:
  T m_tValue; // The stored data.:w

public:
  Value(T tValue) : m_tValue{tValue} {}
  ~Value() {}

  Value(const Value &src) : m_tValue{src.m_tValue} {}

  Value &operator=(const Value &src) {
    if (this != &src)
      m_tValue = src.m_tValue;
    return *this;
  }

  T value() { return m_tValue; }
};

namespace rt3 {
// The ParamSet is just a heterogeneous hash table. All keys are strings.
using ParamSet = std::map<std::string, std::shared_ptr<ValueBase>>;

/*!
 * This is an auxiliary function to avoid the *verbose* access associated
 * with the std::map<>.
 *
 * Tries to retrieve the value associated with `key` from the ParamSet `ps`.
 * In case there is no such key/value pair stored in `ps`, the function returns
 * the default value `default_value` provided by the client him/herself.
 * @param ps The ParamSet we want to extract the value from.
 * @param key The key stored in the ParamSet.
 * @param default_value The default value returned, in case the key is not in
 * the ParamSet.
 * @return The value associated with `key`, if such key is stored in the `ps`,
 * or the given default value otherwise.
 */
template <typename T>
T retrieve(const ParamSet &ps, std::string key, const T &default_value = T()) {
  // Try to retrieve key/data item from the map.
  auto result = ps.find(key);
  if (result != ps.end())
    std::cout << "-->ParamSet: Found [\"" << result->first << "\"].\n";
  else
    std::cout << "-->ParamSet: Key [\"" << key << "\"] not present.\n";
  // Assign a default value in case type is not in the ParamSet object.
  return (result == ps.end()) ? default_value : // default value.
             dynamic_cast<Value<T> &>(*result->second).value();
}
} // namespace rt3

#endif
