/********************************************************************************
* FaceTrackNoIR		This program is a private project of some enthusiastic		*
*					gamers from Holland, who don't like to pay much for			*
*					head-tracking.												*
*																				*
* Copyright (C) 2012	Wim Vriend (Developing)									*
*						Ron Hendriks (Researching and Testing)					*
*																				*
* Homepage																		*
*																				*
* This program is free software; you can redistribute it and/or modify it		*
* under the terms of the GNU General Public License as published by the			*
* Free Software Foundation; either version 3 of the License, or (at your		*
* option) any later version.													*
*																				*
* This program is distributed in the hope that it will be useful, but			*
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY	*
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for	*
* more details.																	*
*																				*
* You should have received a copy of the GNU General Public License along		*
* with this program; if not, see <http://www.gnu.org/licenses/>.				*
*																				*
********************************************************************************/
#include "ftnoir_tracker_sm.h"
#include <QDebug>
#include "facetracknoir/global-settings.h"

FTNoIR_TrackerDll::FTNoIR_TrackerDll() {
	//populate the description strings
	trackerFullName = "faceAPI V3.2.6";
	trackerShortName = "faceAPI";
	trackerDescription = "SeeingMachines faceAPI V3.2.6";
}

FTNoIR_TrackerDll::~FTNoIR_TrackerDll()
{

}
void FTNoIR_TrackerDll::getFullName(QString *strToBeFilled)
{
	*strToBeFilled = trackerFullName;
};

void FTNoIR_TrackerDll::getShortName(QString *strToBeFilled)
{
	*strToBeFilled = trackerShortName;
};

void FTNoIR_TrackerDll::getDescription(QString *strToBeFilled)
{
	*strToBeFilled = trackerDescription;
};

void FTNoIR_TrackerDll::getIcon(QIcon *icon)
{
    *icon = QIcon(":/images/sm.png");
};

////////////////////////////////////////////////////////////////////////////////
// Factory function that creates instances if the Tracker object.

// Export both decorated and undecorated names.
//   GetTrackerDll     - Undecorated name, which can be easily used with GetProcAddress
//						Win32 API function.
//   _GetTrackerDll@0  - Common name decoration for __stdcall functions in C language.
//#pragma comment(linker, "/export:GetTrackerDll=_GetTrackerDll@0")

extern "C" FTNOIR_TRACKER_BASE_EXPORT Metadata* CALLING_CONVENTION GetMetadata()
{
	return new FTNoIR_TrackerDll;
}
