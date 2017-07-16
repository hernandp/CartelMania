#/*****************************************************************************
#
#    CartelMania
#	Copyright(C) 2017 Hernán Di Pietro
#
#	This program is free software : you can redistribute it and / or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	(at your option) any later version.
#
#	This program is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with this program.If not, see < http://www.gnu.org/licenses/>.
#
#****************************************************************************/

# Version stamping script
#
# 
# The script should stamp "revision" field with the number days elapsed from
# 1, January, 2017.
#
# Build number will be set to the minute of the day in which the build 
# was made (0-1440).
#

$timespan = (new-timespan -end (get-date -year 2017 -month 1 -day 1));
$hour = Get-Date -UFormat %H;
$mins = Get-Date -UFormat %M;
$todaymins = $hour.ToInt32($Null) * 60 + $mins.Toint32($Null)

$filetext = Get-Content -Path "..\src\version.h" -ErrorAction Stop;
$build = "#define _BUILDPLACEHOLDER " + $todaymins;
$rev   = "#define _REVPLACEHOLDER   " + -$timespan.Days;
$filetext = $filetext -replace "#define _BUILDPLACEHOLDER [ 0-9]*$", $build;
$filetext = $filetext -replace "#define _REVPLACEHOLDER [ 0-9]*$", $rev;

Set-Content -Path "..\src\version.h" -Value $filetext -ErrorAction Stop;
