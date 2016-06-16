//==============================================================================
//          � Copyright Common Ground Software Solutions 2014
//          chris.snyder@commongroundss.com
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>
//
// File Name:       scTimeSpan.cpp
// Created By:      Christopher A. Snyder
// Creation Date:   6/1/2014
// $Id: scTimeSpan.cpp $
//
//==============================================================================


#include "scTimeSpan.h"

namespace SharedCore
{

/// <summary>
/// Assignment operator
/// </summary>
/// <param name="source"></param>
scTimeSpan& scTimeSpan::operator=( const scTimeSpan& source )
{
	if ( this != &source )
	{
		m_timeSpan = source.m_timeSpan;
	}
	return *this;
}

}
