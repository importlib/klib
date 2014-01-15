#ifndef _klib_net_archive_h_
#define _klib_net_archive_h_


#include <string>
#include <list>
#include <map>





#if (!defined(WIN32) || !(defined(_WIN32)))
typedef  unsigned int UINT;
typedef    int   BOOL;
typedef  unsigned short USHORT;
typedef  unsigned char  UCHAR;
typedef  unsigned long  DWORD;


#include "string.h"
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <inttypes.h>
#include <endian.h>

#else
#define __BYTE_ORDER  __LITTLE_ENDIAN

#if _MSC_VER > 1200
#include <cstdint>
#endif

#endif

#include "../../macro.h"
#include "../../inttype.h"



namespace klib {
namespace net {


// 定义长度标记的长度
#define  length_mark_len  (2)

// 网络序列化类
class net_archive {
public:
	net_archive(char* buff, UINT buffLen) {
		set_buff(buff);
		error_flag_ = false;
		buff_len_ = buffLen;
	}

	// 设置要序列化的缓冲区
    void set_buff(char* buff) {
        orig_pos_ = buff;
		op_pos_ = (char*) orig_pos_ + length_mark_len;
        writed_length_ = false;
	}

	// 返回设置的缓冲区指针
	char* get_buff() {
        if (writed_length_) {
            return (char*) orig_pos_;
        }
		return (char*) orig_pos_ + length_mark_len;
	}

    // 返回写入的长度
	UINT length() {
        if (writed_length_) {
            return buff_len_ + length_mark_len;
        }
		return buff_len_;
	}

    // 写入长度，并记录标记
    void write_length_mark() {
        *(USHORT*)orig_pos_ = KHTON16(buff_len_ + length_mark_len);
        writed_length_ = true;
    }
    
	/*
	  重新回到起点
	*/
	void reset() {
		set_buff((char*)orig_pos_);
	}
	
	//返回是否出错标志
	bool get_error() {
		return error_flag_;
	}

	//////////////////////////////////////////////////////////////////////////
	// 以下是一些序列化函数
	net_archive& operator << (uint8_t a) {
		if (this->get_data_len() + sizeof(a) > buff_len_) {
			error_flag_ = true;
			return *this;
		}
		memcpy(op_pos_, &a, sizeof(a));
		op_pos_ += sizeof(a);
		return *this;
	}

	net_archive& operator >> (uint8_t& a) {
		if (this->get_data_len() + sizeof(a) > buff_len_) {
			error_flag_ = true;
			return *this;
		}
		memcpy(&a, op_pos_, sizeof(a));
		op_pos_ += sizeof(a);
		return *this;
	}

	net_archive& operator << (int8_t a) {
		if (this->get_data_len() + sizeof(a) > buff_len_) {
			error_flag_ = true;
			return *this;
		}
		memcpy(op_pos_, &a, sizeof(a));
		op_pos_ += sizeof(a);
		return *this;
	}

	net_archive& operator >> (int8_t& a) {
		if (this->get_data_len() + sizeof(a) > buff_len_) {
			error_flag_ = true;
			return *this;
		}
		memcpy(&a, op_pos_, sizeof(a));
		op_pos_ += sizeof(a);
		return *this;
	}

	net_archive& operator << (unsigned int a) {
		if (this->get_data_len() + sizeof(a) > buff_len_) {
			error_flag_ = true;
			return *this;
		}
		a = KHTON32(a);
		memcpy(op_pos_, &a, sizeof(a));
		op_pos_ += sizeof(a);
		return *this;
	}

	net_archive& operator >> (unsigned int& a) {
		if (this->get_data_len() + sizeof(a) > buff_len_) {
			error_flag_ = true;
			return *this;
		}
		memcpy(&a, op_pos_, sizeof(a));
		a = KHTON32(a);
		op_pos_ += sizeof(a);
		return *this;
	}

	net_archive& operator << (unsigned short a) {
		if (this->get_data_len() + sizeof(a) > buff_len_) {
			error_flag_ = true;
			return *this;
		}
		a = KHTON16(a);
		memcpy(op_pos_, &a, sizeof(a));
		op_pos_ += sizeof(a);
		return *this;
	}

	net_archive& operator >> (unsigned short& a) {
		if (this->get_data_len() + sizeof(a) > buff_len_) {
			error_flag_ = true;
			return *this;
		}
		memcpy(&a, op_pos_, sizeof(a));
		a = KNTOH16(a);
		op_pos_ += sizeof(a);
		return *this;
	}

	net_archive& operator << (BOOL a) {
		if (this->get_data_len() + sizeof(a) > buff_len_) {
			error_flag_ = true;
			return *this;
		}
		a = KHTON32(a);
		memcpy(op_pos_, &a, sizeof(a));
		op_pos_ += sizeof(a);
		return *this;
	}

	net_archive& operator >> (BOOL& a) {
		if (this->get_data_len() + sizeof(a) > buff_len_) {
			error_flag_ = true;
			return *this;
		}
		memcpy(&a, op_pos_, sizeof(a));
		a = ntohl(a);
		op_pos_ += sizeof(a);
		return *this;
	}

	net_archive& operator << (long a) {
		if (this->get_data_len() + sizeof(a) > buff_len_) {
			error_flag_ = true;
			return *this;
		}
		a = KHTON32(a);
		memcpy(op_pos_, &a, sizeof(a));
		op_pos_ += sizeof(a);
		return *this;
	}

	net_archive& operator << (unsigned long a) {
		if (this->get_data_len() + sizeof(a) > buff_len_) {
			error_flag_ = true;
			return *this;
		}
		a = KNTOH32(a);
		memcpy(op_pos_, &a, sizeof(a));
		op_pos_ += sizeof(a);
		return *this;
	}

	net_archive& operator >> (unsigned long& a) {
		if (this->get_data_len() + sizeof(a) > buff_len_) {
			error_flag_ = true;
			return *this;
		}
		memcpy(&a, op_pos_, sizeof(a));
		a = KNTOH32(a);
		op_pos_ += sizeof(a);
		return *this;
	}

	net_archive& operator << (uint64_t a) {
		if (this->get_data_len() + sizeof(a) > buff_len_) {
			error_flag_ = true;
			return *this;
		}
		a = KNTOH64(a);
		memcpy(op_pos_, &a, sizeof(a));
		op_pos_ += sizeof(a);
		return *this;
	}

	net_archive& operator >> (uint64_t& a) {
		if (this->get_data_len() + sizeof(a) > buff_len_) {
			error_flag_ = true;
			return *this;
		}
		memcpy(&a, op_pos_, sizeof(a));
		a = KNTOH64(a);
		op_pos_ += sizeof(a);
		return *this;
	}

	net_archive& operator << (::std::string& str) {
		if (this->get_data_len() + sizeof(USHORT) + str.size() > buff_len_) {
			error_flag_ = true;
			return *this;
		}
		(*this). operator << ((USHORT) str.size());
		memcpy(op_pos_, str.c_str(), str.size());
		op_pos_ += str.size();
		return *this;
	}

	net_archive& operator << (const ::std::string& str) {
		if (this->get_data_len() + sizeof(USHORT) + str.size() > buff_len_) {
			error_flag_ = true;
			return *this;
		}
		(*this). operator << ((USHORT) str.size());
		memcpy(op_pos_, str.c_str(), str.size());
		op_pos_ += str.size();
		return *this;
	}

	net_archive& operator >> (::std::string& str) {
		if (this->get_data_len() + sizeof(USHORT) + str.size() > buff_len_) {
			error_flag_ = true;
			return *this;
		}
		USHORT len;
		(*this). operator >> (len);
		str = ::std::string(op_pos_, len);
		op_pos_ += str.size();
		return *this;
	}

	template <class T>
	net_archive& operator << (::std::list<T>& theList) {
		if (this->get_data_len() + sizeof(USHORT) >= buff_len_) {
			error_flag_ = true;
			return *this;
		}

		this->operator << ((USHORT)theList.size());
		typename std::list<T>::const_iterator itr = theList.begin();
		for (; itr != theList.end(); ++itr) {
			if (this->get_data_len() + sizeof(USHORT) > buff_len_) {
				error_flag_ = true;
				return *this;
			}

			this->operator << (*itr);
		}
		return *this;
	}

	template <typename T>
	net_archive& operator >> (::std::list<T>& theList) {
		USHORT len;
		T target;

		if (this->get_data_len() + sizeof(USHORT) >= buff_len_) {
			error_flag_ = true;
			return *this;
		}

		this->operator >> (len);
		for (int i=0; i<len; ++i) {
			this->operator >> (target);
			theList.push_back(target);
		}
		return *this;
	}

	template <typename TKey, typename TVal>
	net_archive& operator << (::std::map<TKey, TVal>& theMap) {
		if (this->get_data_len() + sizeof(USHORT) >= buff_len_) {
			error_flag_ = true;
			return *this;
		}

		this->operator << ((USHORT)theMap.size());
		typename ::std::map<TKey, TVal>::iterator itr = theMap.begin();
		for (; itr != theMap.end(); ++itr) {
			this->operator << (itr->first);
			this->operator << (itr->second);
		}
		return *this;
	}

	template <typename TKey, typename TVal>
	net_archive& operator >> (::std::map<TKey, TVal>& theMap) {
		if (this->get_data_len() + sizeof(USHORT) >= buff_len_) {
			error_flag_ = true;
			return *this;
		}

		USHORT len;
		this->operator >> (len);
		TKey theKey;
		TVal theVal;
		for (int i=0; i<len; ++i) {
			this->operator >> (theKey);
			this->operator >> (theVal);
			theMap[theKey] = theVal;
		}
		return *this;
	}

	//返回写入数据的长度
	unsigned int get_data_len() {
		return op_pos_ - orig_pos_;
	}

private:
	char*   op_pos_;		///< 当前在缓冲区的位置
	const char* orig_pos_;	///< 缓冲区的初始位置
	UINT buff_len_;		    ///< 缓冲区的总长度
	bool error_flag_;       ///< 是否出错(缓冲区大小不够情况)
    bool writed_length_;    ///< 是否写入的长度
};



}}



#endif  // _klib_net_archive_h_