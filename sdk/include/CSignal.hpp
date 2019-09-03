#ifndef CSIGNAL_HPP
#define CSIGNAL_HPP

#include <functional>
#include <map>
#include <vector>

/**
 * @brief The CSignal class 信号类，用于异步触发回调，一个信号可以触发多个回调
 */

template <typename... Args>
class CSignal
{
public:
    CSignal():m_nCurrentId(0){}
    CSignal(CSignal const & other):m_nCurrentId(0){}

    template <typename T>
    /**
     * @brief connect_member 信号链接
     * @param inst 类对象
     * @func 函数成员
     * @return 连接id，不用时必须手动断开链接
     */
    int connect_member(T * inst,void (T::*func)(Args...)){
        return s_Connect([=](Args... args){
            (inst->*func)(args...);
        });
    }

    template <typename T>
    /**
     * @brief connect_member 信号链接
     * @param inst 类对象
     * @func 函数成员
     * @return 连接id，不用时必须手动断开链接
     */
    int connect_member(T * inst, void (T::*func)(Args...)const){
        return s_Connect([=](Args... args){
            (inst->*func)(args...);
        });
    }
    /**
     * @brief s_Connect 信号链接
     * @param slot 链接槽函数
     * @return 连接id，不用时必须手动断开链接
     */
    int s_Connect(std::function<void(Args...)>const & slot)const{
        m_slots.insert(std::make_pair(++m_nCurrentId,slot));
        return m_nCurrentId;
    }
    /**
     * @brief s_Disconnect 断开信号链接
     * @param id 信号连接id
     */
    void s_Disconnect(int id)const{
        m_slots.erase(id);
    }
    /**
     * @brief s_DisconnectAll 断开所有信号连接
     */
    void s_DisconnectAll()const{
        m_slots.clear();
    }
    /**
     * @brief s_Emit 发送信号
     * @param p 参数，必须与信号类型符合
     */
    void s_Emit(Args... p){
        for (auto it : m_slots) {
            it.second(p...);
        }
    }
    /**
     * @brief s_Disconnect 断开信号连接
     * @param idList 信号连接id列表
     */
    void s_Disconnect(const std::vector<int> idList)const{
        for(int i=0;i<idList.size();++i)
            s_Disconnect(idList[i]);
    }
//    CSignal & operator =(CSignal const & other){
//        s_DisconnectAll();
//        return *this;
//    }

private:
    mutable int m_nCurrentId;
    mutable std::map<int,std::function<void (Args...)>> m_slots;
};
#endif // CSIGNAL_HPP
