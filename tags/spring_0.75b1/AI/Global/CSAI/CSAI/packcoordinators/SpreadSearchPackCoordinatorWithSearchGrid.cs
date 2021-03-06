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
    // this carries out a spread search: sends all units around randomly
    // can be used by scoutcontroller, or by tankcontroller, for example
    public class SpreadSearchPackCoordinatorWithSearchGrid : IPackCoordinator
    {
        Hashtable UnitDefListByDeployedId;
        Float3 targetpos;
        
        int recentmeansnumframes = 5000;
        
        int mapwidth;
        int mapheight;
        
        CSAI csai;
        IAICallback aicallback;
        LogFile logfile;

        Random random = new Random();
        
        // can pass in pointer to a hashtable in another class if we want
        // ie other class can directly modify our hashtable
        public SpreadSearchPackCoordinatorWithSearchGrid( Hashtable UnitDefListByDeployedId )
        {
            this.UnitDefListByDeployedId = UnitDefListByDeployedId;
            
            csai = CSAI.GetInstance();
            aicallback = csai.aicallback;
            logfile = LogFile.GetInstance();
            
            mapwidth = aicallback.GetMapWidth();
            mapheight = aicallback.GetMapHeight();
            
            debugon = csai.DebugOn;
            
            csai.TickEvent += new CSAI.TickHandler( this.Tick );
            csai.UnitIdleEvent += new CSAI.UnitIdleHandler( UnitIdle );
        }
        
        bool debugon = false;

        bool activated = false; // not started until Activate or SetTarget is called        

        // does NOT imply Activate()
        public void SetTarget( Float3 newtarget )
        {
            this.targetpos = newtarget;
            //Activate();
        }
        
        public void Activate()
        {
            if( !activated )
            {
                logfile.WriteLine( "SpreadSearchPackCoordinatorWithSearchGrid initiating spreadsearch" );
                activated = true;
                restartedfrompause = true;
                Recoordinate();
            }
        }
        
        public void Disactivate()
        {
            if( activated )
            {
                activated = false;
                logfile.WriteLine( "SpreadSearchPackCoordinatorWithSearchGrid shutting down" );
                
                csai.UnregisterVoiceCommand("dumpsearchgrid" );
            }
        }
        
        bool restartedfrompause = true;
        Float3 lasttargetpos = null;
                
        void Recoordinate()
        {
            if( !activated )
            {
                return;
            }
            
            // just send each unit to random destination
            // in unit onidle, we send each unit to a new place
            foreach( DictionaryEntry de in UnitDefListByDeployedId )
            {
                int deployedid = (int)de.Key;
                IUnitDef unitdef = de.Value as IUnitDef;
                ExploreWith( deployedid );
            }
        }
        
        void ExploreWith( int unitid )
        {
            bool destinationfound = false;
            Float3 currentpos = aicallback.GetUnitPos( unitid );
            MovementMaps movementmaps = MovementMaps.GetInstance();
            IUnitDef unitdef = UnitDefListByDeployedId[ unitid ] as IUnitDef;
            int currentarea = movementmaps.GetArea( unitdef, currentpos );
            LosMap losmap = LosMap.GetInstance();
            if( csai.DebugOn )
            {
                logfile.WriteLine("explorewith unit " + unitid + " " + unitdef.humanName + " area: " + currentarea );
            }
            
            /*
            int numtriesleft = 30; // just try a few times then give up
            // maybe there is a better way to do this?
            while( !destinationfound )
            {
                Float3 destination = GetRandomDestination();
               // logfile.WriteLine( "SpreadSearchWithSearchGrid attempt " + destination.ToString() );
                int mapx = (int)( destination.x / 16 );
                int mapy = (int)( destination.z / 16 );
                if( ( movementmaps.GetArea( unitdef, destination ) == currentarea && 
                    losmap.LastSeenFrameCount[ mapx, mapy ] < recentmeansnumframes || numtriesleft <= 0 ) )
                {
                    logfile.WriteLine( "Looks good. Go. " + numtriesleft + " retriesleft" );
                    if( csai.DebugOn )
                    {
                        aicallback.CreateLineFigure( currentpos, destination,10,true,400,0);
                        aicallback.DrawUnit( "ARMFAV", destination, 0.0f, 400, aicallback.GetMyAllyTeam(), true, true);
                    }
                    aicallback.GiveOrder( unitid, new Command( Command.CMD_MOVE, destination.ToDoubleArray() ) );
                    return;
                }
                numtriesleft--;
            }
             */
            // find nearest, area that hasnt had los recently
            //int maxradius = Math.Max( aicallback.GetMapWidth(), aicallback.GetMapHeight() ) / 2;
            //for( int radius = 
            Float3 nextpoint = LosMap.GetInstance().GetNearestUnseen(currentpos, unitdef, 6000 );
            if (nextpoint == null)
            {
                nextpoint = GetRandomDestination();
            }
            aicallback.GiveOrder(unitid, new Command(Command.CMD_MOVE, nextpoint.ToDoubleArray()));
        }

        Float3 GetRandomDestination()
        {
            Float3 destination = new Float3();
            destination.x = random.Next(0, aicallback.GetMapWidth() * MovementMaps.SQUARE_SIZE );
            destination.z = random.Next( 0, aicallback.GetMapHeight() * MovementMaps.SQUARE_SIZE );
            destination.y = aicallback.GetElevation( destination.x, destination.y );
            return destination;
        }

        public void UnitIdle( int unitid )
        {
            if( activated )
            {
                if( UnitDefListByDeployedId.Contains( unitid ) )
                {
                    ExploreWith( unitid );
                }
            }
        }
        
        int totalticks = 0;
        int ticks = 0;
        public void Tick()
        {
            ticks++;
            totalticks++;
            if( ticks >= 30 )
            {
                //Recoordinate();
                
                ticks = 0;
            }
        }
    }
}
