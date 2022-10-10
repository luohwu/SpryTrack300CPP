// ============================================================================

/*!
 *
 *   This file is part of the ATRACSYS fusionTrack library.
 *   Copyright (C) 2003-2018 by Atracsys LLC. All rights reserved.
 *
 *   \file helpers_windows.cpp
 *   \brief Helping functions used by sample applications
 *
 */
// ============================================================================

#include "helpers.hpp"

#include <conio.h>
#include <windows.h>
#include <iostream>
#include <stdio.h>

using namespace std;

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

bool isLaunchedFromExplorer()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if ( ! GetConsoleScreenBufferInfo( GetStdHandle(
                                           STD_OUTPUT_HANDLE ), &csbi ) )
    {
        printf( "GetConsoleScreenBufferInfo failed: %lu\n", GetLastError() );
        return FALSE;
    }

    // if cursor position is (0,0) then we were launched in a separate console
    return ( ( ! csbi.dwCursorPosition.X ) && ( ! csbi.dwCursorPosition.Y ) );
}

#ifdef ATR_BORLAND
    #define _kbhit kbhit
#endif

// Wait for a keyboard hit
void waitForKeyboardHit()
{
    cout << "press any key to continue" << endl;
    while ( ! _kbhit() )
    {
        Sleep( 100 );
    }
    _getch();
}

#ifdef ATR_BORLAND
    #undef_kbhit
#endif

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void optionEnumerator(uint64 sn, void* user, ftkOptionsInfo* oi)
    {
        cout << "Option " << oi->id << "  " << oi->name << endl;

        switch (oi->component)
        {
        case ftkComponent::FTK_LIBRARY:
            cout << "\tCOMP:  library" << endl;
            break;

        case ftkComponent::FTK_DEVICE:
            cout << "\tCOMP:  device" << endl;
            break;

        case ftkComponent::FTK_DETECTOR:
            cout << "\tCOMP:  detector" << endl;
            break;

        case ftkComponent::FTK_MATCH2D3D:
            cout << "\tCOMP:  matching" << endl;
            break;

        case ftkComponent::FTK_DEVICE_WIRELESS:
            cout << "\tCOMP:  wireless management" << endl;
            break;

        default:
            cout << "\tCOMP:  ????" << endl;
            break;
        }

        cout << "\tDESC:  " << oi->description << endl;
        if (oi->unit)
        {
            cout << "\tUNIT:  " << oi->unit << endl;
        }

        cout << "\tSTAT:  ";
        if (oi->status.read)
        {
            cout << "(READ)";
        }
        if (oi->status.write)
        {
            cout << "(WRITE)";
        }
        cout << endl;

        switch (oi->type)
        {
        case ftkOptionType::FTK_INT32:
            cout << "\tTYPE:  int32" << endl;
            if (strcmp(oi->name, "Challenge result") == 0)
            {
                break;
            }
            if (oi->status.read && oi->status.write)
            {
                int32 out;
                if (ftkGetInt32(lib, sn, oi->id, &out, ftkOptionGetter::FTK_MIN_VAL) != ftkError::FTK_OK)
                {
                    checkError(lib, !isNotFromConsole, false);
                }
                cout << "\tMIN:   " << out << endl;
                if (ftkError::FTK_OK != ftkGetInt32(lib, sn, oi->id, &out, ftkOptionGetter::FTK_MAX_VAL))
                {
                    checkError(lib, !isNotFromConsole, false);
                }
                cout << "\tMAX:   " << out << endl;
                if (ftkError::FTK_OK != ftkGetInt32(lib, sn, oi->id, &out, ftkOptionGetter::FTK_DEF_VAL))
                {
                    checkError(lib, !isNotFromConsole, false);
                }
                cout << "\tDEF:   " << out << endl;
                if (ftkError::FTK_OK != ftkGetInt32(lib, sn, oi->id, &out, ftkOptionGetter::FTK_VALUE))
                {
                    checkError(lib, !isNotFromConsole, false);
                }
                cout << "\tVAL:   " << out << endl;
            }
            else if (oi->status.read)
            {
                int32 out;
                if (ftkError::FTK_OK != ftkGetInt32(lib, sn, oi->id, &out, ftkOptionGetter::FTK_VALUE))
                {
                    checkError(lib, !isNotFromConsole, false);
                }
                cout << "\tVAL:   " << out << endl;
            }
            else if (oi->status.write)
            {
                int32 out;
                if (ftkError::FTK_OK != ftkGetInt32(lib, sn, oi->id, &out, ftkOptionGetter::FTK_MIN_VAL))
                {
                    checkError(lib, !isNotFromConsole, false);
                }
                cout << "\tMIN:   " << out << endl;
                if (ftkError::FTK_OK != ftkGetInt32(lib, sn, oi->id, &out, ftkOptionGetter::FTK_MAX_VAL))
                {
                    checkError(lib, !isNotFromConsole, false);
                }
                cout << "\tMAX:   " << out << endl;
            }
            break;

        case ftkOptionType::FTK_FLOAT32:
            cout << "\tTYPE:  float32" << endl;
            if (oi->status.read && oi->status.write)
            {
                float32 out;
                ftkError err(ftkGetFloat32(lib, sn, oi->id, &out, ftkOptionGetter::FTK_MIN_VAL));
                if (err == ftkError::FTK_OK)
                {
                    cout << "\tMIN:   " << out << endl;
                }
                err = ftkGetFloat32(lib, sn, oi->id, &out, ftkOptionGetter::FTK_MAX_VAL);
                if (err == ftkError::FTK_OK)
                {
                    cout << "\tMAX:   " << out << endl;
                }
                err = ftkGetFloat32(lib, sn, oi->id, &out, ftkOptionGetter::FTK_DEF_VAL);
                if (err == ftkError::FTK_OK)
                {
                    cout << "\tDEF:   " << out << endl;
                }
                if (ftkError::FTK_OK != ftkGetFloat32(lib, sn, oi->id, &out, ftkOptionGetter::FTK_VALUE))
                {
                    checkError(lib, !isNotFromConsole, false);
                }
                cout << "\tVAL:   " << out << endl;
            }
            else if (oi->status.read)
            {
                float32 out;
                if (ftkError::FTK_OK != ftkGetFloat32(lib, sn, oi->id, &out, ftkOptionGetter::FTK_VALUE))
                {
                    checkError(lib, !isNotFromConsole, false);
                }
                cout << "\tVAL:   " << out << endl;
            }
            else if (oi->status.write)
            {
                float32 out;
                if (ftkError::FTK_OK != ftkGetFloat32(lib, sn, oi->id, &out, ftkOptionGetter::FTK_MIN_VAL))
                {
                    checkError(lib, !isNotFromConsole, false);
                }
                cout << "\tMIN:   " << out << endl;
                if (ftkError::FTK_OK != ftkGetFloat32(lib, sn, oi->id, &out, ftkOptionGetter::FTK_MAX_VAL))
                {
                    checkError(lib, !isNotFromConsole, false);
                }
                cout << "\tMAX:   " << out << endl;
            }
            break;

        case ftkOptionType::FTK_DATA:
            cout << "\tTYPE:  data" << endl;
            if (oi->status.read)
            {
                ftkBuffer buffer;
                if (ftkError::FTK_OK != ftkGetData(lib, sn, oi->id, &buffer))
                {
                    checkError(lib, !isNotFromConsole, false);
                }
                cout << "\tVAL:   " << buffer.sData << endl;
            }
            break;

        default:
            cout << "\tTYPE:  ????" << endl;
            break;
        }

        cout << "" << endl;
    }


 void enumerateOptions(ftkLibrary lib, uint64 sn)
{
    ftkError status(ftkEnumerateOptions(lib, sn, optionEnumerator, nullptr));
    if (status != ftkError::FTK_OK && status != ftkError::FTK_WAR_OPT_GLOBAL_ONLY)
    {
        checkError(lib);
    }
}

void setOptionValue(ftkLibrary lib, uint64 sn, uint32 optID, int32 value)
{
    if (ftkSetInt32(lib, sn, optID, value) != ftkError::FTK_OK)
    {
        char message[50];
        sprintf(message, "can not set Option with ID: %d ", optID);
        error(message);
    }
}