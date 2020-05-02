#ifndef MESSAGE_DATA
#define MESSAGE_DATA

#include <iostream>
#include <cstdio>
#include <map>
#include <vector>

//消息的id
typedef unsigned short DataId;
const DataId Data_PositionX     = 1;
const DataId Data_PositionY     = 2;
const DataId Data_Seatno        = 3;
const DataId Data_ActionType    = 4;
const DataId Data_Array         = 5;

const int Type_Int              = 1;
const int Type_Array            = 2;

//消息数据类
class  MessageData
{
public:
    MessageData()
    {

    }
    ~ MessageData()
    {

    }

public: 
    //打包或解开消息的方法
    bool putInt(const DataId& key_id, const int& value);

    bool putMDataArray(const DataId& key_id, MessageData** message_data, const int& message_size);

    bool getInt(const DataId& key_id, int& value);

    bool getMDataArraySize(const DataId& key_id, int& vec_size);

    bool getMDataArray(const DataId& key_id, MessageData** data_value, const int& elem_index);

private:
    
    //根据消息类型与下标返回位置
    inline void transformIndex(const int& data_type ,int& index)
    {
       index = (data_type << 16 ) + index;
    }

    //根据index返回消息类型
    inline bool transformType(const int& index, const int& date_type)
    {

        return (((index >> 16) == date_type) ? true : false);
    }

    //根据下标返回在数组中的位置
    inline void transformPos(int& index)
    {
        index = index & 0xFFFF;
    }

private:

    std::map<DataId, int>        map_index;    //存放消息的索引
    
    std::vector<int>             vec_int_item;   //存放整形消息

    std::vector<std::vector<MessageData* > >    vec_mdata_item; //存放消息数组
    
};




#endif