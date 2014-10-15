using System;
using System.Collections.Generic;
using System.Text;
using Springie.Client;
using System.Net;
using System.Xml.Serialization;
using System.IO;

namespace Springie.AutoHostNamespace
{
  class BanList
  {
    public List<BannedUser> Items = new List<BannedUser>();

    public const string BanlistFilename = "banlist.xml";

    private TasClient tas;
    private AutoHost ah;

    public BanList(AutoHost ah, TasClient tas)
    {
      this.tas = tas;
      this.ah = ah;

      tas.BattleUserIpRecieved += new EventHandler<TasEventArgs>(tas_BattleUserIpRecieved);
      tas.BattleUserJoined += new EventHandler<TasEventArgs>(tas_BattleUserJoined);
    }

    void tas_BattleUserIpRecieved(object sender, TasEventArgs e)
    {
      UpdateWithUserIp(e.ServerParams[0], IPAddress.Parse(e.ServerParams[1]));
      BannedUser b;
      if (IsBanned(e.ServerParams[0], out b)) {
        tas.Say(TasClient.SayPlace.Battle, "", e.ServerParams[0] + " (" + b.Name + ") is banned for " + b.Reason, true);
        tas.Kick(e.ServerParams[0]);
      }
    }



    public bool GetByIp(string ip, out BannedUser b)
    {
      b = Items.Find(delegate(BannedUser u) { return u.ipAddresses.Contains(ip); });
      if (b != null) {
        if (b.Expired) {
          Items.Remove(b);
          b = null;
          return false;
        } else return true;
      } else return false;
    }

    public bool GetByName(string name, out BannedUser b)
    {
      b = Items.Find(delegate(BannedUser u) { return u.nickNames.Contains(name); });
      if (b != null) {
        if (b.Expired) { 
          Items.Remove(b);
          b = null;
          return false;
        } else return true;
      } else return false;
    }

    public bool GetByNameOrIp(string name, string ip, out BannedUser b)
    {
      b = Items.Find(delegate(BannedUser bu) { return bu.nickNames.Contains(name) || bu.ipAddresses.Contains(ip); });
      if (b != null) {
        if (b.Expired) {
          Items.Remove(b);
          b = null;
          return false;
        } else return true;
      } else return false;
    }

    private void UpdateWithUserIp(string name, IPAddress ip)
    {
      BannedUser b;
      if (GetByIp(ip.ToString(), out b)) if (!b.nickNames.Contains(name)) b.nickNames.Add(name);
      if (GetByName(name, out b)) if (ip != IPAddress.None && !b.ipAddresses.Contains(ip.ToString())) b.ipAddresses.Add(ip.ToString());
      if (b != null) {
        if (b.Expired) Items.Remove(b);
        Save();
      }
    }

    public bool IsBanned(string name, out BannedUser b)
    {
      Battle bat = tas.GetBattle();
      if (GetByName(name, out b)) return true;
      if (bat!=null) {
        UserBattleStatus ubs;
        if (bat.ContainsUser(name, out ubs)) {
          string ip = ubs.ip.ToString();
          if (GetByIp(ip, out b)) return true;
        }
      }
      return false;
    }

    public bool IsBanned(string name)
    {
      BannedUser b;
      return IsBanned(name, out b);
    }


    void tas_BattleUserJoined(object sender, TasEventArgs e)
    {
      BannedUser b;
      string usname = e.ServerParams[0];

      if (IsBanned(usname, out b)) {
        tas.Say(TasClient.SayPlace.Battle, "", usname + " (" + b.Name + ") is banned for " + b.Reason, true);
        tas.Kick(usname);
      }
    }



    public void ComBan(TasSayEventArgs e, string[] words)
    {
      if (words.Length == 0) {
        ah.Respond(e, "this command needs at least 1 argument - exact user name");
        return;
      }
      
      int duration = 0;
      if (words.Length > 1) {
        if (!int.TryParse(words[1], out duration)) {
          ah.Respond(e, "second argument must be a number - ban time in minutes or 0 - forever");
          return;
        }
      }

      if (IsBanned(words[0])) {
        ah.Respond(e, "this user is already banned");
        return;
      }

      if (duration < 0) duration = 0;
      TimeSpan dur;
      if (duration == 0) dur = TimeSpan.FromDays(365 * 1000); else dur = TimeSpan.FromMinutes(duration);

      BannedUser b = new BannedUser(words[0]);
      b.Duration = dur;
      b.Reason = Utils.Glue(words, 2);

      Battle battle = tas.GetBattle();
      UserBattleStatus ubs;
      if (battle.ContainsUser(b.Name, out ubs)) {
        if (ubs.ip != IPAddress.None) b.ipAddresses.Add(ubs.ip.ToString());
      }
      Items.Add(b);
      tas.Say(TasClient.SayPlace.Battle, "", b.Name + " banned - " + b.Reason, true);
      tas.Kick(b.Name);
      Save();
    }


    public void ComUnban(TasSayEventArgs e, string[] words) {
      if (words.Length ==0) {
        ah.Respond(e, "this command needs 1 argument - user name");
        return;
      }
      BannedUser b;
      if (IsBanned(words[0], out b)) {
        Items.Remove(b);
        ah.Respond(e, b.Name + " removed from banlist");
        Save();
      } else {
        ah.Respond(e, "no such user in banlist");
      }
    }


    public void ComListBans(TasSayEventArgs e, string[] words) 
    {
      Items.RemoveAll(delegate(BannedUser b) { return b.Expired; });
      tas.Say(TasClient.SayPlace.User, e.UserName, "---", false);
      foreach (BannedUser b in Items) {
        string text=  String.Format("{0} --> {1} (banned from:{2} to:{3} (GMT))",b.Name, b.Reason, b.Started.ToUniversalTime(), b.Started.Add(b.Duration).ToUniversalTime() );
        tas.Say(TasClient.SayPlace.User, e.UserName, text, false);
      }
      tas.Say(TasClient.SayPlace.User, e.UserName, "---", false);
    }


    public void Save() {
      Items.RemoveAll(delegate(BannedUser b) { return b.Expired; });
      Items.Sort(delegate(BannedUser a, BannedUser b) { return a.Name.CompareTo(b.Name); });
      XmlSerializer s = new XmlSerializer(Items.GetType());
      FileStream f = File.OpenWrite(BanlistFilename);
      f.SetLength(0);
      s.Serialize(f, Items);
      f.Close();
    }

    public void Load() {
      if (File.Exists(BanlistFilename)) {
        XmlSerializer s = new XmlSerializer(Items.GetType());
        StreamReader r = File.OpenText(BanlistFilename);
        Items= (List<BannedUser>)s.Deserialize(r);
        r.Close();
      }
    }



    /// <summary>
    /// Disconnects event handlers
    /// </summary>
    public void Close()
    {
      tas.BattleUserJoined -= tas_BattleUserJoined;
      tas.BattleUserIpRecieved -= tas_BattleUserIpRecieved;
    }
  }
}
