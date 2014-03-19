
//*****************************************************************************
// RCF - Remote Call Framework
// Copyright (c) 2005, Jarl Lindrud.
// Contact: jlindrud@hotmail.com .
//
// Distributed under the so-called MIT license, see accompanying file license.txt.
//*****************************************************************************

#ifndef _SF_DATAPTR_HPP_
#define _SF_DATAPTR_HPP_

#include <string>

#include <SF/PortableTypes.hpp>

namespace SF {

    //************************************************************************
    // DataPtr class holds a pointer to a buffer of data. It includes an internal 
    // buffer in order to avoid dynamic memory allocation for small buffer sizes, < 64bytes.


    // TODO: need to be able to pass a preallocated buffer to DataPtr<T>, and use that if its big enough
    class DataPtr 
    {
    private:
        typedef Byte8 T;
    public:
        DataPtr();
        DataPtr(const T *sz);
        DataPtr(const T *sz, UInt32 length);
        DataPtr(const DataPtr &rhs);
        DataPtr &operator=(const DataPtr &rhs);
        ~DataPtr();

        void assign(const T *sz, UInt32 length);
        void assign(const T *sz);
        void assign(const std::string &s);

        // TODO
        //void attach(T *sz, void (*pfn_deleter)(T *) ) {}
        //void attach(T *sz, UInt32 length, void (*pfn_deleter)(T *)) {}
        //void detach() { whichDeleter = 0; }

        void release();
        UInt32 allocate(UInt32 length);
        void terminatebufferwithzero() const;
        void update_length();
        T *get() const;
        UInt32 length() const;
        bool empty() const;
        std::string cpp_str() const;

    private:
        T *ptr_;
        UInt32 length_;
        UInt32 allocatedLength_;
        int whichDeleter_;
        void (*pfn_deleter_)(T *);
        T buffer_[64];
        UInt32 length(const T *sz);
    };

    bool operator<(const DataPtr &lhs, const DataPtr &rhs);
    bool operator==(const DataPtr &lhs, const DataPtr &rhs);
    //bool operator==(const DataPtr &lhs, const Byte8 *rhs);
    bool operator!=(const DataPtr &lhs, const DataPtr &rhs);
    //bool operator!=(const DataPtr &lhs, const Byte8 *rhs);
    
} // namespace SF

#endif // ! _SF_DATAPTR_HPP_
