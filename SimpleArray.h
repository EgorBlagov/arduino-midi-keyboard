#ifndef SIMPLE_ARRAY_H
#define SIMPLE_ARRAY_H

typedef word simple_array_size_t;
template <typename T, simple_array_size_t SIZE>
class SimpleArray {
  public:
    SimpleArray() {}
    SimpleArray(const T* items)
    {
      for (simple_array_size_t i = 0; i < SIZE; i++) {
        this->data[i] = items[i];
      }
    }

    T& operator[](unsigned int i) {
      return this->data[i];
    }
    const T& operator[](unsigned int i)const {
      return this->data[i];
    }

    const simple_array_size_t size() {
      return SIZE;
    }

  private:
    T data[SIZE];
};

#endif
