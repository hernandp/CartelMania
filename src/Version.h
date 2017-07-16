#pragma once
/*****************************************************************************

    CartelMania
	Copyright(C) 2017 Hernán Di Pietro

	This program is free software : you can redistribute it and / or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.If not, see < http://www.gnu.org/licenses/>.

*****************************************************************************/

/** Version Data -- this file is processed by VerStamp.ps1 Pre-build step 
    on release configurations by stamping data on _REVPLACEHOLDER and 
	_BUILDPLACEHOLDER definitions. Do not mess this!	
	
	The script should stamp "revision" field with year/month/day. Month
	in hex (0-C), e.g: for 2017-11-30:  17B30.

	Build number will be set to the minute of the day in which the build 
	was made (0-1440).

	See verstamp.ps1 script in pre-build step for details.

**/

#define X_TO_STR(A) #A
#define TO_STR(A) X_TO_STR(A)

#define _REVPLACEHOLDER   9999
#define _BUILDPLACEHOLDER 9999

// ---------------------------------------------------------------------------
#define CMANIA_VERSION_MAJOR 0
#define CMANIA_VERSION_MINOR 5
#define CMANIA_VERSION_REV   _REVPLACEHOLDER
#define CMANIA_VERSION_BUILD _BUILDPLACEHOLDER

#define CMANIA_VERSION_STRING TO_STR(CMANIA_VERSION_MAJOR) L"." TO_STR(CMANIA_VERSION_MINOR) L"." TO_STR(CMANIA_VERSION_REV) L"." TO_STR(CMANIA_VERSION_BUILD)
// ---------------------------------------------------------------------------

