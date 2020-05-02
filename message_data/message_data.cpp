#include "message_data.h"

bool MessageData::putInt(const DataId& key_id, const int& value)
{
    //判断Data_ID是否已经使用
    if(map_index.find(key_id) != map_index.end())
    {
        return false;
    }
    vec_int_item.push_back(value);
    //记录消息的位置
    int index = vec_int_item.size() - 1;

    //返回下标位置
    transformIndex(Type_Int, index);

    map_index[key_id] = index;

    std::cout<<"Put key_id: "<<key_id<<std::endl;

    return true;
}

bool MessageData::getInt(const DataId& key_id, int& value)
{
    //判断Data_ID是否有对应数据
    if(map_index.find(key_id) == map_index.end())
    {
        return false;
    }
    //根据key_id获取下标
    int index = map_index[key_id];

    //判断类型
    if(!transformType(index, Type_Int))
    {
        return false;
    }

    transformPos(index);

    value = vec_int_item[index];

    std::cout<<"Get vaule: "<<value<<std::endl;

    return true;
}

bool MessageData:: putMDataArray(const DataId& key_id, MessageData** message_data, const int& message_size)
{
    if(map_index.find(key_id) != map_index.end() || message_size == 0)
    {
        return false;
    }
    std::vector<MessageData*> vec_data;
    vec_data.reserve(message_size);

    std::cout<<message_size<<std::endl;

    for(int i = 0; i < message_size; i++)
    {
        vec_data.push_back(message_data[i]);
    }

    vec_mdata_item.push_back(vec_data);

    int index = vec_mdata_item.size() - 1;

    transformIndex(Type_Array, index);

    map_index[key_id] = index;

    return true;
}

bool MessageData::getMDataArraySize(const DataId& key_id, int& vec_size)
{
    if(map_index.find(key_id) == map_index.end())
    {
        return false;
    }
    
    int index = map_index[key_id];

    if(!transformType(index, Type_Array))
    {
        return false;
    }
    transformPos(index);

    vec_size = vec_mdata_item[index].size();
    return (vec_size > 0);
}

bool MessageData::getMDataArray(const DataId& key_id, MessageData** data_value, const int& elem_index)
{
    if(map_index.find(key_id) == map_index.end())
    {
        return false;
    }
    
    int index = map_index[key_id];

    if(!transformType(index, Type_Array))
    {
        return false;
    }

    transformPos(index);
    
    std::vector<MessageData*>vec_data;
    vec_data = vec_mdata_item[index];
    *data_value = vec_data[elem_index];
    return true;

}