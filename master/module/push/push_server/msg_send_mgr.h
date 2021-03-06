#pragma once


#include "../../include/push_interface.h"
#include <kthread/auto_lock.h>
#include <kthread/threadpool.h>
#include <core/timer_mgr.h>
#include <util/timeout_checker.h>
#include <pattern/object_pool.h>
using namespace klib::kthread;
using namespace klib::pattern;
using namespace klib::util;

#include <third/sigslot.h>

#define  DEFAULT_RETRY_INTERVAL     (6)         ///< 单位为秒
#define  DEFAULT_MAX_RETRY_TIMES    (4)         ///< 最大重试次数

namespace logic
{

//----------------------------------------------------------------------
// 消息确认信息
class msg_confirm_info
{
public:
    msg_confirm_info() : msg_id_(0), last_send_time_(0), sended_times_(0) {}
    
private:
    UINT64              msg_id_;                 ///< 消息的编号
    UINT64              last_send_time_;         ///< 最后发送的时间
    UINT64              sended_times_;           ///< 发送的次数
    push_msg_ptr        user_msg_;               ///< 用户消息的指针

    DEFINE_ACCESS_FUN_REF_CONST(UINT64, msg_id, msg_id_);
    DEFINE_ACCESS_FUN_REF_CONST(UINT64, last_send_time, last_send_time_);
    DEFINE_ACCESS_FUN_REF_CONST(UINT64, sended_times, sended_times_);
    DEFINE_ACCESS_FUN_REF_CONST(push_msg_ptr, user_msg, user_msg_);
};


//----------------------------------------------------------------------
// 消息发送管理
class msg_send_mgr : 
    public singleton<msg_send_mgr>,
    public sigslot::has_slots<>
{
public:
    msg_send_mgr(void);
    ~msg_send_mgr(void);

    // 需要外界提供发送的接口
    sigslot::signal3<ip_v4, USHORT, push_msg_ptr> s_send_msg;  // 
    handle_send_msg_result_callback handle_on_send_msg_result;

public:
    // 参数设置(最大发送次数，重试间隔)
    void set_max_retry_times(UINT32 uMaxRetryTimes);     ///< 设置最大重试次数
    void set_retry_send_interval(UINT32 uRetrySendInterval);        ///< 设置重试间隔

    UINT32 get_max_retry_time() { return max_retry_times_; }    ///< 获取最大重试次数
    UINT32 get_retry_send_interval() { return retry_send_interval; }    ///< 获取重试间隔

    void post_send_msg(ip_v4 addr, USHORT port, const push_msg_ptr msg);   ///< 投递发送消息
    void post_send_msg(ip_v4 addr, USHORT port, const std::vector<push_msg_ptr>& vecMsg); ///< 一次投递多个消息

    void on_client_msg_ack(DWORD addr, USHORT port, UINT64 uMsgID);  ///< 消息反馈处理
    BOOL remove_msg_confirm_info(UINT64 uMsgID);   ///< 删除正在发送确认的消息(针对报有客户端的特定消息)

protected:
    bool timer_check_resend_msg();  ///< 检查并重试发送消息

private:
    typedef std::map<client_addr_key, msg_confirm_info*>        MapClientConfirmMsgInfoType;
    typedef std::map<UINT64,  MapClientConfirmMsgInfoType>  MapMsgIDMsgConfirmType;
    MapMsgIDMsgConfirmType              confirm_msg_map_;                                ///< 保存确认消息列表
    mutex                               confirm_msg_mutex_;                              ///< 

    CObjectPool<msg_confirm_info, 10000, 5000>  confirm_msg_pool_;     ///< 待确认消息内存池
    timer_mgr                           timer_mgr_;

    UINT32                              max_retry_times_;                     ///< 最大重试发送次数
    UINT32                              retry_send_interval;                  ///< 重试发送的间隔

};

}