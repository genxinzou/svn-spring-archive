// Copyright Hugh Perkins 2006
// hughperkins@gmail.com http://manageddreams.com
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURVector3E. See the GNU General Public License for
//  more details.
//
// You should have received a copy of the GNU General Public License along
// with this program in the file licence.txt; if not, write to the
// Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-
// 1307 USA
// You can find the licence also on the web at:
// http://www.opensource.org/licenses/gpl-license.php
//
// ======================================================================================
//

using System;
using System.IO;
using System.Collections;
using System.Xml;

namespace CSharpAI
{
    // implemented by units that want to be called back by factories etc to check if they need any particular units
    public interface IUnitRequester
    {
        // return priority indicating how badly you need some units
        // they can ask factory what it is able to build
        double DoYouNeedSomeUnits( IFactory factory );
            
        // Ask what unit they need building
        IUnitDef WhatUnitDoYouNeed( IFactory factory );
            
        // signal to requester that we granted its request
        void WeAreBuildingYouA( IUnitDef unitwearebuilding );
            
        // your request got destroyed; new what-to-build negotations round
        void YourRequestWasDestroyedDuringBuilding_Sorry( IUnitDef unitwearebuilding );
    }
}
