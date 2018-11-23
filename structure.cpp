#include <cstring>
#include <iostream>
#include <Windows.h>
#include <winioctl.h>

#include "utils.h"
#include "structure.h"

using namespace std;

#define IOCTL_STORAGE_QUERY_PROPERTY CTL_CODE(IOCTL_STORAGE_BASE, 0x0500, METHOD_BUFFERED, FILE_ANY_ACCESS)

typedef struct _STORAGE_DEVICE_DESCRIPTOR {
    ULONG Version;
    ULONG Size;
    UCHAR DeviceType;
    UCHAR DeviceTypeModifier;
    BOOLEAN RemovableMedia;
    BOOLEAN CommandQueueing;
    ULONG VendorIdOffset;
    ULONG ProductIdOffset;
    ULONG ProductRevisionOffset;
    ULONG SerialNumberOffset;
    STORAGE_BUS_TYPE BusType;
    ULONG RawPropertiesLength;
    UCHAR RawDeviceProperties[1];
} STORAGE_DEVICE_DESCRIPTOR, *PSTORAGE_DEVICE_DESCRIPTOR;

typedef enum _STORAGE_PROPERTY_ID {
  StorageDeviceProperty,
  StorageAdapterProperty,
  StorageDeviceIdProperty,
  StorageDeviceUniqueIdProperty,
  StorageDeviceWriteCacheProperty,
  StorageMiniportProperty,
  StorageAccessAlignmentProperty,
  StorageDeviceSeekPenaltyProperty,
  StorageDeviceTrimProperty,
  StorageDeviceWriteAggregationProperty,
  StorageDeviceDeviceTelemetryProperty,
  StorageDeviceLBProvisioningProperty,
  StorageDevicePowerProperty,
  StorageDeviceCopyOffloadProperty,
  StorageDeviceResiliencyProperty,
  StorageDeviceMediumProductType,
  StorageAdapterRpmbProperty,
  StorageAdapterCryptoProperty,
  StorageDeviceIoCapabilityProperty,
  StorageAdapterProtocolSpecificProperty,
  StorageDeviceProtocolSpecificProperty,
  StorageAdapterTemperatureProperty,
  StorageDeviceTemperatureProperty,
  StorageAdapterPhysicalTopologyProperty,
  StorageDevicePhysicalTopologyProperty,
  StorageDeviceAttributesProperty,
  StorageDeviceManagementStatus,
  StorageAdapterSerialNumberProperty,
  StorageDeviceLocationProperty,
  StorageDeviceNumaProperty,
  StorageDeviceZonedDeviceProperty,
  StorageDeviceUnsafeShutdownCount
} STORAGE_PROPERTY_ID, *PSTORAGE_PROPERTY_ID;

typedef enum _STORAGE_QUERY_TYPE {
  PropertyStandardQuery,
  PropertyExistsQuery,
  PropertyMaskQuery,
  PropertyQueryMaxDefined
} STORAGE_QUERY_TYPE, *PSTORAGE_QUERY_TYPE;

typedef struct _STORAGE_PROPERTY_QUERY {
  STORAGE_PROPERTY_ID PropertyId;
  STORAGE_QUERY_TYPE  QueryType;
  BYTE                AdditionalParameters[1];
} STORAGE_PROPERTY_QUERY, *PSTORAGE_PROPERTY_QUERY;


void get_launch_path(ENVIRON* env) {
	char* buffer = new char[MAX_PATH_SIZE];
	GetModuleFileName(NULL, buffer, MAX_PATH_SIZE);
	env->launch_path = string(buffer);
}


string get_offset(PUCHAR p, ULONG Length, ULONG Offset) {
    if (Offset && p[Offset]) {
        string line = "";
        for (ULONG  i = Offset; p[i] != '\0' && i < Length; i++) 
            line += p[i];
        while (line[0] == ' ')
        	line = line.substr(1);
        return line;
    }
    return "";
}


void get_drive_info(ENVIRON* env) {
    // Составим название устройства: '\\.\С:, '\\.\D:', типа того
    char x[512];
    string temp = "\\\\.\\" + string(1, env->launch_path[0]) + ":";
    strncpy(x, temp.c_str(), sizeof(x));
 
    // Хэндл к устройству
    HANDLE hDevice = CreateFile(&(x[0]), 0, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    
    // Создаем всякие буферы
    STORAGE_PROPERTY_QUERY query;
    ZeroMemory(&query, sizeof(STORAGE_PROPERTY_QUERY));
    query.PropertyId = StorageDeviceProperty;
    query.QueryType = PropertyStandardQuery;

    UCHAR buffer[512];
    ULONG length;
    BOOL status = DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY, &query, sizeof(query), &buffer, 512, &length, NULL);

    if (hDevice) {
        PSTORAGE_DEVICE_DESCRIPTOR devDesc = (PSTORAGE_DEVICE_DESCRIPTOR) buffer;
        env->drive_serial = get_offset(buffer, length, devDesc->SerialNumberOffset);
    }
    else wcout << L"Ошибка определения устройства\n";
}


void get_struct(ENVIRON* env) {
	get_launch_path(env);
	get_drive_info(env);
}


void read_struct(ENVIRON* env, std::ifstream* f) {
	char path[MAX_PATH_SIZE];
	f->read(path, MAX_PATH_SIZE);
	env->launch_path = string(path);

	char drive_serial[MAX_PATH_SIZE];
	f->read(drive_serial, sizeof(drive_serial));
	env->drive_serial = string(drive_serial);
}


void write_struct(ENVIRON* env, std::ofstream* f) {
	char x[MAX_PATH_SIZE];
	strcpy(x, env->launch_path.c_str());
	f->write(x, MAX_PATH_SIZE);

	char y[MAX_PATH_SIZE];
	strcpy(y, env->drive_serial.c_str());
	f->write(y, MAX_PATH_SIZE);
}


void print_struct(ENVIRON* env) {
	wcout << L"-----------ОКРУЖЕНИЕ-----------\n";
	wcout << L"Путь запуска: "; cout << env->launch_path << endl;
	wcout << L"Серийник устройства: "; cout << env->drive_serial << endl;
}
