// =============================================================================

/*!
 *
 *   This file is part of the ATRACSYS fusionTrack library.
 *   Copyright (C) 2003-2021 by Atracsys LLC. All rights reserved.
 *
 *   \file stereo1_ListOptions.cpp
 *   \brief Display fusionTrack/spryTrack  options in the console
 *
 *   This sample aims to present the following driver features:
 *   - Open/close the driver
 *   - Enumerate devices
 *   - List options
 *
 *   How to compile this example:
 *   - Follow instructions in README.txt
 *
 *   How to run this example:
 *   - Install the fusionTrack/spryTrack driver (see documentation)
 *   - Switch on device
 *   - Run the resulting executable
 *
 */
 // =============================================================================

#include "helpers.hpp"

#include <algorithm>
#include <deque>
#include <iomanip>
#include <iostream>

#ifdef FORCED_DEVICE_DLL_PATH
#include <Windows.h>
#endif

using namespace std;

//ftkLibrary lib = NULL;
//bool isNotFromConsole = true;

// ---------------------------------------------------------------------------
// Callback function for options



// ---------------------------------------------------------------------------
// main function

int main2(int argc, char** argv)
{
    isNotFromConsole = isLaunchedFromExplorer();

    // -----------------------------------------------------------------------
    // Defines where to find Atracsys SDK dlls when FORCED_DEVICE_DLL_PATH is
    // set.
#ifdef FORCED_DEVICE_DLL_PATH
    SetDllDirectory((LPCTSTR)FORCED_DEVICE_DLL_PATH);
#endif

    cout << "This is a demonstration on how to enumerate options." << endl;

    deque< string > args;
    for (int i(1); i < argc; ++i)
    {
        args.emplace_back(argv[i]);
    }

    bool showHelp(false);

    if (!args.empty())
    {
        showHelp = (find_if(args.cbegin(), args.cend(), [](const string& val) {
            return val == "-h" || val == "--help";
            }) != args.cend());
    }

    string cfgFile("");

    if (showHelp || args.empty())
    {
        cout << setw(30u) << "[-h/--help] " << flush << "Displays this help and exits." << endl;
        cout << setw(30u) << "[-c/--config F] " << flush << "JSON config file. Type "
            << "std::string, default = none" << endl;
    }

    cout << "Copyright (c) Atracsys LLC 2003-2021" << endl;
    if (showHelp)
    {
#ifdef ATR_WIN
        if (isLaunchedFromExplorer())
        {
            cout << "Press the \"ANY\" key to quit" << endl;
            waitForKeyboardHit();
        }
#endif
        return 0;
    }

    auto pos(find_if(args.cbegin(), args.cend(),
        [](const string& val) { return val == "-c" || val == "--config"; }));
    if (pos != args.cend() && ++pos != args.cend())
    {
        cfgFile = *pos;
    }

    // ----------------------------------------------------------------------
    // Initialize driver

    ftkBuffer buffer;

    lib = ftkInitExt(cfgFile.empty() ? nullptr : cfgFile.c_str(), &buffer);
    if (lib == nullptr)
    {
        cerr << buffer.data << endl;
        error("Cannot initialize driver", !isNotFromConsole);
    }

    // ----------------------------------------------------------------------
    // List global options (i.e. options which don't require a device)

    cout << "List global options:" << endl << endl;
    enumerateOptions(lib, 0uLL);

    // ----------------------------------------------------------------------
    // Retrieve the device

    DeviceData device(retrieveLastDevice(lib, true, false, !isNotFromConsole));
    uint64 sn(device.SerialNumber);

    // Set current temperature index
    ftkSetInt32(lib, sn, 30, 1);

    cout << "Waiting for 2 seconds" << endl;
    sleep(2500L);
    ftkFrameQuery* frame(ftkCreateFrame());
     (lib, sn, frame, 1000u);
    ftkDeleteFrame(frame);

    // Copying the counter of lost frames
    ftkSetInt32(lib, sn, 68, 1);

    // ----------------------------------------------------------------------
    // Enumerate options of the device

    cout << "List available options:" << endl << endl;
    enumerateOptions(lib, sn);

    cout << "\tSUCCESS" << endl;

    // ----------------------------------------------------------------------
    // Close driver

    if (ftkError::FTK_OK != ftkClose(&lib))
    {
        checkError(lib, !isNotFromConsole);
    }
    #ifdef ATR_WIN
    if (isNotFromConsole)
    {
        cout << endl << "*** Hit a key to exit ***" << endl;
        waitForKeyboardHit();
    }
#endif
    return EXIT_SUCCESS;
}
