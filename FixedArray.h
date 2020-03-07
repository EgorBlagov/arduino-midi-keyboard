#ifndef FIXED_ARRAY_H
#define FIXED_ARRAY_H


typedef word simple_array_size_t;
template <typename T, simple_array_size_t SIZE>
class FixedArray {
public:
  template <typename... K>
  FixedArray(K... items) : data{ items... } {}

  T& operator[](unsigned int i) {
    return data[i];
  }

  const T& operator[](unsigned int i) const {
    return data[i];
  }

  const simple_array_size_t size() const {
    return SIZE;
  }

private:
  T data[SIZE];
};




#endif
