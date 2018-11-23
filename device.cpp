#include <string>
#include <iostream>
#include <Windows.h>
#include <winioctl.h>

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


string print_prop(PUCHAR p, ULONG Length, string Name, ULONG Offset)
{
    if (Offset && p[Offset]) {
        string line = Name + ": ";
        for (ULONG  i = Offset; p[i] != '\0' && i < Length; i++) 
            line += p[i];
        return string(line);
    }
    return "";
}


string GetPhysicalDriveSerialNumber(char path[]) {
    DWORD dwRet = NO_ERROR;
    string strSerialNumber;
 
    // Format physical drive path (may be '\\.\С:, '\\.\D:' and so on).
    char x[512];
    string temp = "\\\\.\\" + string(1, path[0]) + ":";
    strncpy(x, temp.c_str(), sizeof(x));
 
    // Get a handle to physical drive
    HANDLE hDevice = CreateFile(&(x[0]), 0, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    
    STORAGE_PROPERTY_QUERY query;
    ZeroMemory(&query, sizeof(STORAGE_PROPERTY_QUERY));
    query.PropertyId = StorageDeviceProperty;
    query.QueryType = PropertyStandardQuery;

    UCHAR buffer[512];
    ULONG length;
    BOOL status = DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY, &query, sizeof(query), &buffer, 512, &length, NULL);

    if (hDevice) {
        PSTORAGE_DEVICE_DESCRIPTOR devDesc = (PSTORAGE_DEVICE_DESCRIPTOR) buffer;
        cout << print_prop(buffer, length, "Vendor ID",        devDesc->VendorIdOffset) << endl;
        cout << print_prop(buffer, length, "Product ID",       devDesc->ProductIdOffset) << endl;
        cout << print_prop(buffer, length, "Product Revision", devDesc->ProductRevisionOffset) << endl;
        cout << print_prop(buffer, length, "Serial Number",    devDesc->SerialNumberOffset) << endl;
    }
    return "";
}

int main() {
    char path[] = "C:/lol/kek";
    string kek = GetPhysicalDriveSerialNumber(path);
    return 0;
}
