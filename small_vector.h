//
//  small_vector.h
//  small_vector
//
//  Created by Ben Saratikyan on 11/9/19.
//  Copyright © 2019 Ben Saratikyan. All rights reserved.
//

#ifndef small_vector_h
#define small_vector_h

template<typename T, std::size_t N>
class small_vector {
    static_assert(N > 0, "Number of small vector elements in stack can't be 0!");
public:
    typedef std::random_access_iterator_tag iterator_category;
    typedef T value_type;

    small_vector()
    : _data()
    , _size(0)
    ,_isCArray(true) {}
    
    small_vector(std::size_t size) {
        if(size > N) {
            new (&_data._vec) std::vector<T>(size);
        }
        resize(size);
    }
    
    small_vector(const small_vector<T, N>& other) {
        if(!other._isCArray) {
            new (&_data._vec) std::vector<T>(other._data._vec);
        } else {
            for (std::size_t i = 0; i < other._size; ++i) {
                new (&_data._arr[i]) T(other._data._arr[i]);
            }
        }
        _size = other._size;
        _isCArray = other._isCArray;
    }
    
    small_vector(std::initializer_list<T> il)
    : _data()
    , _size(il.size())
    , _isCArray(false) {
        if(il.size() > N) {
            new (&_data) std::vector<T>(il.begin(), il.end());
        } else {
            std::size_t i = 0;
            for (auto it = il.begin(); it < il.end(); ++it, ++i) {
                new (&_data._arr[i]) T(*it);
            }
            _isCArray = true;
        }
    }
    
    small_vector<T, N>& operator=(const small_vector<T, N>& other) {
        if (this == &other) return *this;
          if(!other._isCArray) {
              new (&_data._vec) std::vector<T>(other._data._vec);
          } else {
              for (std::size_t i = 0; i < other._size; ++i) {
                  new (&_data._arr[i]) T(other._data._arr[i]);
              }
          }
        _size = other._size;
        _isCArray = other._isCArray;
        return *this;
    }
    
    bool operator==(const small_vector<T, N>& other) const {
        if (_size != other._size) return false;
        if (this == &other) return true;
        if (_isCArray) {
            for (std::size_t i = 0; i < _size; ++i) {
                if (other._data._arr[i] != _data._arr[i]) return false;
            }
            return true;
        } else {
            return _data._vec == other._data._vec;
        }
    }
    
    bool operator!=(const small_vector<T, N>& other) const {
        return !(*this == other);
    }
    
    const T& operator [](std::size_t idx) const noexcept {
        if (_isCArray) {
            return _data._arr[idx];
        } else {
            return _data._vec[idx];
        }
    }
       
    T& operator [](std::size_t idx) noexcept {
       return const_cast<T&>(static_cast<const small_vector<T, N>&>(*this)[idx]);
    }
    
    template<typename U>
    void push_back(U&& arg) noexcept {
        if (!_isCArray) {
            _data._vec.push_back(std::forward<U>(arg));
        } else if (_size == N && _isCArray) {
            std::vector<T> temp;
            temp.reserve(2 * N);
            for (T& t : _data._arr) {
                temp.push_back(std::move(t));
                if constexpr (!std::is_trivially_destructible_v<T>) {
                    t.~T();
                }
            }

            temp.push_back(std::forward<U>(arg));
            new (&_data._vec) std::vector<T>(std::move(temp));
            _isCArray = false;
        } else {
            new (&_data._arr[_size]) T(std::forward<U>(arg));
        }
        ++_size;
    }

    template<typename... Args>
    void emplace_back(Args&&... args) noexcept {
        push_back(T{std::forward<Args>(args)...});
    }
    
    void pop_back() noexcept {
        if(_size) {
        if(_isCArray) {
            if constexpr (!std::is_trivially_destructible_v<T>) {
                _data._arr[_size - 1].~T();
            }
        } else {
            _data._vec.pop_back();
        }
        --_size;
        }
    }
    
    void reserve(std::size_t n) noexcept {
        if(n <= N) return;
        
        if(!_isCArray) {
            _data._vec.reserve(n);
        } else {
            std::vector<T> temp;
            temp.reserve(n);
            for (std::size_t i = 0; i < _size; ++i) {
                temp.push_back(_data._arr[i]);
                if constexpr (!std::is_trivially_destructible_v<T>) {
                    _data._arr[i].~T();
                }
            }
            
            new (&_data._vec) std::vector<T>(std::move(temp));
            _isCArray = false;
        }
    }
    
    void resize(std::size_t n) noexcept {
        if(!_isCArray) {
            _data._vec.resize(n);
        } else if (n > N && _isCArray) {
            std::vector<T> temp(begin(), end());
            clear();
            new (&_data._vec) std::vector<T>(std::move(temp));
            _data._vec.resize(n);
            _isCArray = false;
        } else if (n < N && _isCArray) {
            std::vector<T> temp(begin(), end());
            for (std::size_t i = std::max(n, _size) - std::min(n, _size); i < _size; ++i) {
                _data._arr[i].~T();
            }
        }
        _size = n;
    }
    
    void clear() noexcept {
        if (_isCArray) {
            if constexpr (!std::is_trivially_destructible_v<T>) {
                for (std::size_t i = 0; i < _size; ++i) {
                    _data._arr[i].~T();
                }
            }
        } else {
            _data._vec.~vector<T>();
        }
        _isCArray = true;
        _size = 0;
    }
    
    bool empty() const {
        return !_size;
    }

    std::size_t size() const {
        return _size;
    }
    
    const T& front() const {
           return _isCArray ? _data._arr[0] : _data._vec.front();
    }
    
    const T& back() const {
        return _isCArray ? _data._arr[_size - 1] : _data._vec.back();
    }
    
    T* begin() {
        return _isCArray ? _data._arr : &_data._vec.front();
    }
    
    T* end() {
        return _isCArray ? _data._arr + _size : &_data._vec.back();
    }
    
    const T* begin() const {
        return _isCArray ? _data._arr : &_data._vec.front();
    }
       
    const T* end() const {
        return _isCArray ? _data._arr + _size : &_data._vec.back();
    }
    
    const T* data() const {
          return begin();
    }

    ~small_vector() {
        if(_size) clear();
    }

private:
    union Data {
        Data() {}
        
        ~Data() {}
        
        T _arr[N];
        std::vector<T> _vec;
    } _data;
    
    std::size_t _size;
    bool _isCArray;
};

#endif /* small_vector_h */
