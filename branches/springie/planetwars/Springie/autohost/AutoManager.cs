﻿using System;
using System.Collections.Generic;
using System.Threading;
using System.Timers;
using Springie.autohost;
using Springie.Client;
using Springie.SpringNamespace;
using Timer=System.Timers.Timer;

namespace Springie.AutoHostNamespace
{
  public class AutoManager
  {
    private const int KickAfter = 300;
    private const int RingEvery = 60;
    private const int SpecForceAfter = 120;

    private AutoHost ah;
    private int from;
    private DateTime lastRing = DateTime.Now;
    private Spring spring;
    private TasClient tas;
    private Timer timer = new Timer(5000);

    private int to;

    public AutoManager(AutoHost ah, TasClient tas, Spring spring)
    {
      this.ah = ah;
      this.tas = tas;
      this.spring = spring;
      timer.Elapsed += new ElapsedEventHandler(timer_Elapsed);
      timer.Start();
    }

    public bool Enabled
    {
      get { return from > 0; }
    }

    Dictionary<string, DateTime> problemSince = new Dictionary<string, DateTime>();


    private void timer_Elapsed(object sender, ElapsedEventArgs e)
    {
      lock (timer) {
        timer.Stop();
        try {
          if (from > 0 && !spring.IsRunning) {
            Battle b = tas.GetBattle();
            if (b != null) {
              int plrCnt = b.CountPlayers();
              if (plrCnt >= from) {
                List<string> notReady;

                if (!ah.AllUniqueTeams(out notReady)) {
                  ah.ComFix(TasSayEventArgs.Default, new string[] { });
                  return;
                }

                bool isReady = ah.AllReadyAndSynced(out notReady);
                if (plrCnt%2 == 0) {
                  int allyno;
                  if (!ah.BalancedTeams(out allyno)) { //teams are not balanced but even number - fix colors and balance
                    ah.ComFixColors(TasSayEventArgs.Default, new string[] {});
                    ah.BalanceTeams(2, false);
                  }
                }
                if (isReady) {
                  Thread.Sleep(1000);
                  if (!spring.IsRunning) ah.ComStart(TasSayEventArgs.Default, new string[] {});
                } else {
                  DateTime now = DateTime.Now;

                  // we have enough people, but its odd number and server locked = unlock for more
                  if (plrCnt > from && plrCnt%2 == 1 && b.IsLocked) ah.ComAutoLock(TasSayEventArgs.Default, new string[] {(plrCnt + 1).ToString()});


                  if (plrCnt % 2 == 0) { // even number of players
                    
                    if (now.Subtract(lastRing).TotalSeconds > RingEvery) { // we ring them
                      lastRing = now;
                      ah.ComRing(TasSayEventArgs.Default, new string[] {});
                    }

                    DateTime worstTime = DateTime.MaxValue;
                    String worstName = "";
                    foreach (string s in notReady) { // find longest offending player
                      if (!problemSince.ContainsKey(s)) problemSince[s] = DateTime.Now;
                      if (problemSince[s] < worstTime) {
                        worstTime = problemSince[s];
                        worstName = s;
                      }
                    }
                    foreach (string s in new List<string>(problemSince.Keys)) { // delete not offending plaeyrs
                      if (!notReady.Contains(s)) problemSince.Remove(s);
                    }


                    if (now.Subtract(worstTime).TotalSeconds > SpecForceAfter) ah.ComForceSpectator(TasSayEventArgs.Default, new string[] { worstName }); // spec longest offending person
                    if (now.Subtract(worstTime).TotalSeconds > KickAfter) ah.ComKick(TasSayEventArgs.Default, new string[] {worstName}); // kick longest offending
                  } else { // teams are not even delet offender list
                    problemSince.Clear();
                  }
                  
                  
                }
              } else { // not enough players, make sure we unlock and clear offenders
                if (b.IsLocked && plrCnt < from) ah.ComAutoLock(TasSayEventArgs.Default, new string[] {to.ToString()});
                problemSince.Clear();
              }
            }
          } else { // spring running, reset timer and delete offenders
            lastRing = DateTime.Now;
            problemSince.Clear();
          }
        } finally {
          timer.Start();
        }
      }
    }

    public void Manage(int from, int to)
    {
      if (to < from) to = from;
      this.from = from;
      this.to = to;
      timer.Start();
    }

    public void Stop()
    {
      from = 0;
      to = 0;
      lock (timer) {
        timer.Stop();
      }
    }
  }
}