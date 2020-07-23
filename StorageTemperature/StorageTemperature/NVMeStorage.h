#pragma once
#include <string>
#include <winnt.h>
using namespace std;

class NVMeStorage
{
private:
	LPCWSTR wszDrive;

public:
	NVMeStorage();
	NVMeStorage(LPCWSTR wszDrive);
	int setReadDrive(LPCWSTR wszDrive);
	bool getDriveGeometry(DISK_GEOMETRY* pdg);
	bool getTemperature(STORAGE_TEMPERATURE_DATA_DESCRIPTOR* stdd);
private:
	bool getDriveQuery(void* obj, int size);
};

