#include <Windows.h>
#include <winioctl.h>
#include "NVMeStorage.h"

NVMeStorage::NVMeStorage()
{
}

NVMeStorage::NVMeStorage(LPCWSTR wszDrive)
{
	this->wszDrive = wszDrive;
}

int NVMeStorage::setReadDrive(LPCWSTR wszDrive)
{
	this->wszDrive = wszDrive;
	if (this->wszDrive == NULL) {
		return 1;
	}
    return 0;
}

bool NVMeStorage::getDriveGeometry(DISK_GEOMETRY* pdg)
{
	HANDLE hDevice = INVALID_HANDLE_VALUE;  // handle to the drive to be examined 
	BOOL bResult = FALSE;                 // results flag
	DWORD junk = 0;                     // discard results

	hDevice = CreateFileW(this->wszDrive,          // drive to open
		0,                // no access to the drive
		FILE_SHARE_READ | // share mode
		FILE_SHARE_WRITE,
		NULL,             // default security attributes
		OPEN_EXISTING,    // disposition
		0,                // file attributes
		NULL);            // do not copy file attributes

	if (hDevice == INVALID_HANDLE_VALUE)    // cannot open the drive
	{
		return (FALSE);
	}

	bResult = DeviceIoControl(hDevice,                       // device to be queried
		IOCTL_DISK_GET_DRIVE_GEOMETRY, // operation to perform
		NULL, 0,                       // no input buffer
		pdg, sizeof(*pdg),            // output buffer
		&junk,                         // # bytes returned
		(LPOVERLAPPED)NULL);          // synchronous I/O

	CloseHandle(hDevice);

	return (bResult);
}

bool NVMeStorage::getTemperature(STORAGE_TEMPERATURE_DATA_DESCRIPTOR* stdd)
{
	if (stdd == nullptr)
	{
		return false;
	}
	BOOL bResult = FALSE;      // generic results flag
	bResult = getDriveQuery(&(*stdd), sizeof(*stdd));
	return bResult;
}

bool NVMeStorage::getDriveQuery(void* obj, int size)
{
	HANDLE hDevice = INVALID_HANDLE_VALUE;
	BOOL bResult = FALSE;
	DWORD junk = 0;

	hDevice = CreateFileW(this->wszDrive,	// drive to open
		0,							// no access to the drive
		FILE_SHARE_READ |	// share mode
		FILE_SHARE_WRITE,
		NULL,						// default security attributes
		OPEN_EXISTING,				// disposition
		0,							// file attributes
		NULL);			// do not copy file attributes

	if(hDevice == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	
	STORAGE_PROPERTY_QUERY spq;
	spq.PropertyId = StorageDeviceTemperatureProperty;
	spq.QueryType = PropertyStandardQuery;

	bResult = DeviceIoControl(hDevice,                       // device to be queried
		IOCTL_STORAGE_QUERY_PROPERTY, // operation to perform
		&spq, sizeof(spq),                       // no input buffer
		obj, size,            // output buffer
		&junk,                         // # bytes returned
		(LPOVERLAPPED)NULL);          // synchronous I/O

	CloseHandle(hDevice);

	return (bResult);
}
