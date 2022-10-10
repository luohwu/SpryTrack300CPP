#include "helpers.hpp"
#include "geometryHelper.hpp"
#include <iostream>
#define FORCED_DEVICE_DLL_PATH "G:\spryTrack SDK x64\bin"

#ifdef FORCED_DEVICE_DLL_PATH
#include <Windows.h>
#endif

using namespace std;
ftkLibrary lib=NULL;
bool isNotFromConsole = false;
ftkError err;



int main(int argc, char** argv)
{
#ifdef FORCED_DEVICE_DLL_PATH
	SetDllDirectory((LPCTSTR)FORCED_DEVICE_DLL_PATH);
#endif

	// initialize the library
	ftkBuffer errBuffer;
	lib = ftkInit();
	if (lib == nullptr)
	{
		error("Cannot initialize driver");
	}

	// check current global options
	enumerateOptions(lib, 0uLL);

	//retrieve the device and device number
	DeviceData device(retrieveLastDevice(lib));
	uint64 sn(device.SerialNumber);

	//check all device-related options
	cout << "List available options:" << endl << endl;
	//enumerateOptions(lib, sn);


	// set custon option values
	setOptionValue(lib, sn, 10, 2173);
	setOptionValue(lib, sn, 11, 110);
	//waitForKeyboardHit();

	cout << "List available options:" << endl << endl;
	enumerateOptions(lib, sn);
	waitForKeyboardHit();

	// load and set geometry from file
	ftkRigidBody geom{};
	string geomFile = "geometry110.ini";
	loadAndSetGeometryFile(lib, sn, geomFile, &geom);

	//initialize a frame to get marker pose
	ftkFrameQuery* frame = ftkCreateFrame();
	if (frame == 0)
	{
		cerr << "cannot create frame instance" << endl;
		waitForKeyboardHit();
	}

	err = ftkSetFrameOptions(false, 0u, 16u, 16u, 0u, 16u, frame);
	if (err != ftkError::FTK_OK)
	{
		ftkDeleteFrame(frame);
		checkError(lib);
	}
	uint32 counter(50u);
	cout.setf(ios::fixed, ios::floatfield);
	cout.precision(2u);
	for (uint32 u(0u), i; u < 100u; u++)
	{
		err = ftkGetLastFrame(lib, sn, frame, 100);
		if (err > ftkError::FTK_OK)
		{
			cout << ".";
			continue;
		}
		else if (err < ftkError::FTK_OK)
		{
			cout << "warning: " <<  int32(err) << endl;
			if (err == ftkError::FTK_WAR_NO_FRAME)
			{
				continue;
			}
		}

		cout << "get frame"<<endl;
		switch (frame->markersStat)
		{
		case ftkQueryStatus::QS_WAR_SKIPPED:
			ftkDeleteFrame(frame);
			cerr << "marker fields in the frame are not set correctly" << endl;
			checkError(lib);
			break;
		case ftkQueryStatus::QS_ERR_INVALID_RESERVED_SIZE:
			ftkDeleteFrame(frame);
			cerr << "marker reserved size is invalid" << endl;
			checkError(lib);
			break;
		case ftkQueryStatus::QS_OK:
			break;
		default:
			ftkDeleteFrame(frame);
			cerr << "invalid query status" << endl;
			checkError(lib);
		}

		if (frame->markersCount == 0)
		{
			cout << "0 detected marker" << endl;
			sleep(1000);
			continue;
		}

		if (frame->markersStat == ftkQueryStatus::QS_ERR_OVERFLOW)
		{
			cerr << "marker's buffer size is too small" << endl;

		}

		for (i = 0; i < frame->markersCount; i++)
		{
			ftkMarker marker = frame->markers[i];
			cout.precision(2);
			cout << endl;
			cout << "geometry: " << marker.geometryId << ", trans(" <<
				marker.translationMM[0] << " " << marker.translationMM[1] <<
				" " << marker.translationMM[2] << "), error: ";
			cout.precision(3u);
			cout << marker.registrationErrorMM << endl;

		}
		if (--counter == 0u)
		{
			break;
		}
		sleep(1000L);
	}

	if (counter != 0u)
	{
		cout << endl << "loop aborted after too many invalid trials" << endl;

	}
	else
	{
		cout << "success to get marker data from the camere" << endl;
	}
	
	
	//close driver
	ftkDeleteFrame(frame);
	if (ftkClose(&lib) != ftkError::FTK_OK)
	{
		checkError(lib);
	}


	waitForKeyboardHit();
	return 0;


}