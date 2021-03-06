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
    public class TankRushPlayStyle : PlayStyle
    {
        static TankRushPlayStyle instance = new TankRushPlayStyle();
        public static TankRushPlayStyle GetInstance(){ return instance; }

        TankRushPlayStyle()
        {
            logfile.WriteLine( "Tankrushplaystyle constructor >>>");
            controllers.Add( new ScoutControllerRandomSearch( this ) );
            controllers.Add( new TankController( this ) );
            controllers.Add( new RadarController( this ) );
            controllers.Add( new FactoryController( this ) );
            controllers.Add( new ConstructorController( this ) );
            
            logfile.WriteLine( "Tankrushplaystyle constructor <<<");
        }
        
        public override string GetName(){ return "tankrush"; }
        public override string GetDescription(){ return "Rushes with lots of little tanks.  Quite effective."; }        
    }
}
