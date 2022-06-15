#ifndef ADU218_HPP
#define ADU218_HPP

#include <string>
#include <array>
#include <hidapi/hidapi.h>


#define VENDOR_ID 0x0a07
#define PRODUCT_ID 218
#define COMMAND_SIZE 8

class ADU218{
public:
    ADU218();
    ~ADU218();
    std::array<unsigned char,COMMAND_SIZE> input_buffer_;
    int SetOutput(int channel, bool state);
    int ReadInputs(char port, int timeout);
private:
    hid_device *device_handle_;
};

ADU218::ADU218(){
    if (hid_init() == -1){
        //Handle error
    }
    device_handle_ = hid_open(VENDOR_ID, PRODUCT_ID, NULL);
    if (device_handle_ == NULL){
        //Handle Error
        hid_exit();
    }
}

ADU218::~ADU218(){
    hid_close(device_handle_);
    hid_exit();
}

int ADU218::SetOutput(int channel, bool state){
    unsigned char relay_state = 'R';
    const unsigned char output = channel + '0';
    if (state){
        relay_state = 'S';
    }
    const std::array<unsigned char,COMMAND_SIZE> buffer{0x01,relay_state,0x4B,channel,0x00,0x00,0x00,0x00};
    return hid_write(device_handle_, buffer.data(), buffer.size());
}

int ADU218::ReadInputs(char port, int timeout){
    int bytes_read{};
    
    const std::array<unsigned char, COMMAND_SIZE> read_command{0x01,'R','P',port,0x00,0x00,0x00,0x00};
    if(hid_write(device_handle_, read_command.data(), read_command.size()) != -1){
        return  hid_read_timeout(device_handle_,input_buffer_.data(),input_buffer_.size(), timeout);
    }
}
#endif