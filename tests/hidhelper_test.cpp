#include "hidhelper.h"
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace {
std::vector<unsigned char> sentReport;
int sendCount = 0;
int freeCount = 0;
hid_device *device = reinterpret_cast<hid_device *>(1);
hid_device_info interfaceInfo{};

void check(bool condition, const char *message) {
    if(!condition) {
        std::fprintf(stderr, "%s\n", message);
        std::exit(1);
    }
}
}

extern "C" {
hid_device *hid_open(unsigned short, unsigned short, const wchar_t *) { return device; }
hid_device_info *hid_enumerate(unsigned short, unsigned short) { return &interfaceInfo; }
void hid_free_enumeration(hid_device_info *) { ++freeCount; }
hid_device *hid_open_path(const char *path) {
    check(std::strcmp(path, "test-interface") == 0, "Wrong interface opened");
    return device;
}
int hid_send_feature_report(hid_device *, const unsigned char *data, size_t length) {
    ++sendCount;
    sentReport.assign(data, data + length);
    return static_cast<int>(length);
}
const wchar_t *hid_error(hid_device *) { return L"test error"; }
}

int main() {
    HIDHelper::sendMouseReport(device, HIDHelper::RGB, 0x7f, 0x17);
    check(sentReport == std::vector<unsigned char>({7, 0x13, 0x7f, 0x17, 0, 0, 0, 0}),
          "Lighting command must be the eight-byte HID report without USB setup bytes");

    unsigned char media[] = {0x00, 0xcd};
    HIDHelper::sendRawMouseReport(device, HIDHelper::MakeInputMediaKey, media, sizeof(media));
    check(sentReport == std::vector<unsigned char>({7, 0x17, 0x00, 0xcd, 0, 0, 0, 0}),
          "Media command must be padded without reading beyond its payload");

    unsigned char keys[] = {1, 2, 3, 4, 5, 6};
    HIDHelper::sendRawMouseReport(device, HIDHelper::MakeInputKeyboard, keys, sizeof(keys));
    check(sentReport == std::vector<unsigned char>({7, 0x15, 1, 2, 3, 4, 5, 6}),
          "Six-key input must fit the report");

    int before = sendCount;
    HIDHelper::sendRawMouseReport(nullptr, HIDHelper::RGB, keys, 1);
    HIDHelper::sendRawMouseReport(device, HIDHelper::RGB, keys, 7);
    HIDHelper::sendRawMouseReport(device, HIDHelper::RGB, keys, -1);
    HIDHelper::sendRawMouseReport(device, HIDHelper::RGB, nullptr, 1);
    check(sendCount == before, "Invalid reports must not reach the device");

    char path[] = "test-interface";
    interfaceInfo.interface_number = 1;
    interfaceInfo.path = path;
    check(HIDHelper::openKeyboardInterface() == device, "Expected keyboard interface");
    check(freeCount == 1, "Enumeration must be freed after opening an interface");
    return 0;
}
