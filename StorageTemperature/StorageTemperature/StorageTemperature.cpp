
#define UNICODE 1
#define _UNICODE 1


#include <windows.h>
#include <cstdio>
#include <ctime>
#include <cstring>
#include "NVMeStorage.h"


#define wszDrive L"\\\\.\\PhysicalDrive0"


int wmain(int argc, wchar_t* argv[])
{
	auto errcode = 0;
    auto Temperature = false;
    auto Geometry = false;
	
	for(auto i = 0; i < argc; i++)
	{
        wprintf(L"%s\r\n", argv[i]);
		wstring x = argv[i];
		if(x.compare(L"TEMP") == 0)
		{
			Temperature = true;
			printf("Temperature: true");
		}
		else if(x.compare(L"GEO") == 0)
		{
			Geometry = true;
			printf("Geometry: true");
		}
	}

    DISK_GEOMETRY pdg = { 0 }; // disk drive geometry structure
    BOOL bResult = FALSE;      // generic results flag
    ULONGLONG DiskSize = 0;    // size of the drive, in bytes


	// 読み出すドライブを設定
    NVMeStorage ssd(wszDrive);


	// ドライブのジオメトリ読み出し
	if(Geometry)
	{
		bResult = ssd.getDriveGeometry(&pdg);

	    if (bResult)
	    {
	        wprintf(L"Drive path      = %ws\n", wszDrive);
	        wprintf(L"Cylinders       = %I64d\n", pdg.Cylinders);
	        wprintf(L"Tracks/cylinder = %ld\n", (ULONG)pdg.TracksPerCylinder);
	        wprintf(L"Sectors/track   = %ld\n", (ULONG)pdg.SectorsPerTrack);
	        wprintf(L"Bytes/sector    = %ld\n", (ULONG)pdg.BytesPerSector);

	        DiskSize = pdg.Cylinders.QuadPart * (ULONG)pdg.TracksPerCylinder *
	            (ULONG)pdg.SectorsPerTrack * (ULONG)pdg.BytesPerSector;
	        wprintf(L"Disk size       = %I64d (Bytes)\n"
	            L"                = %.2f (Gb)\n",
	            DiskSize, (double)DiskSize / (1024 * 1024 * 1024));
	    }
	    else
	    {
	        wprintf(L"GetDriveGeometry failed. Error %ld.\n", GetLastError());
	    }
		errcode &= bResult;
    }


	// ドライブの温度を読み出し
	if(Temperature)
	{
		STORAGE_TEMPERATURE_DATA_DESCRIPTOR stdd;
		bResult = ssd.getTemperature(&stdd);


		time_t now = time(NULL);
		tm pnow;
		localtime_s(&pnow, &now);

		if (bResult)
		{
			wprintf(L"%2d/%2d/%2d %2d:%2d:%2d Temperature = %d\n", pnow.tm_year + 1900, pnow.tm_mon + 1, pnow.tm_mday, pnow.tm_hour, pnow.tm_min, pnow.tm_sec, stdd.TemperatureInfo->Temperature);
		}
		else
		{
			wprintf(L"GetDriveQuery failed. Error %ld.\n", GetLastError());
		}
		errcode &= bResult;
	}

    return errcode;
}

