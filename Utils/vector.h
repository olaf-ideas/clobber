#pragma once

#include <algorithm>

#include "random.h"

template <typename T>
class vector {
private:
    T*  m_data;
    unsigned int m_memo;
    unsigned int m_size;

public:

    vector(int size = 0) : m_size(size), m_memo(1) {
        while(m_memo < m_size)  m_memo <<= 1;
        m_data = (T*) malloc(m_memo * sizeof(T));
    }

    ~vector() {
        free(m_data);
    }

    inline void push_back(const T &value) {
        m_data[m_size++] = value;
        if(m_size >= m_memo) {
            m_memo <<= 1;
            m_data = (T*) realloc(m_data, m_memo * sizeof(T));
        }
    }

    inline void pop_back() {
        m_size--;
        if(m_size <= (m_memo >> 1)) {
            m_memo >>= 1;
            m_data = (T*) realloc(m_data, m_memo * sizeof(T));
        }
    }

    inline T& back() {
        return *(m_data + m_size - 1);
    }

    inline bool empty() const {
        return m_size == 0;
    }

    inline unsigned int size() const {
        return m_size;
    }

    inline void shuffle() {
        for(int i = m_size - 1; i > 0; i--) {
            std::swap(m_data[i], m_data[fast_rand(0, i)]);        
        }
    }
   
    const T& operator[] (unsigned int index) const {
        return *(m_data + index);
    }
 
};
