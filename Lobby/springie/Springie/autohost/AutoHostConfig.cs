#region using

using System.Collections.Generic;
using System.ComponentModel;
using Springie.Client;
using Springie.SpringNamespace;

#endregion

namespace Springie.autohost
{
	public class AutoHostConfig
	{
		#region Fields

		private int autoLockMinPlayers = 2;
		private BattleDetails battleDetails = new BattleDetails();
		private int defaulRightsLevel = 1;
		private int defaulRightsLevelForLobbyAdmins = 4;
		private string defaultMap = "SmallDivide.smf";
		private string defaultMod = "XTA v8";
		private UnitInfo[] disabledUnits = new UnitInfo[] {};
		private string gameTitle = "AutoHost (%1)";
		private int hostingPort = 8452;
		private string kickSpectatorText = "spectators not allowed here at this time, sorry";
		private string[] mapCycle = new string[] {};
		private int maxPlayers = 10;
		private string password = "*";
		private string welcome = "Hi %1 (rights:%2), welcome to %3, automated host. For help say !help";

		#endregion

		#region Properties

		[Category("Mod and map")]
		[Description("Causes springie to auto rehost to newest version of this mod. Enter: Complete Annihilation STABLE")]
		public string AutoUpdateMod { get; set; }

		[Category("Basic options")]
		[Description("Minimum number of players for autolocking")]
		public int AutoLockMinPlayers
		{
			get { return autoLockMinPlayers; }
			set { autoLockMinPlayers = value; }
		}

		[Category("Default battle settings")]
		[Description("Defines battle details to use by default")]
		public BattleDetails BattleDetails
		{
			get { return battleDetails; }
			set { battleDetails = value; }
		}

		public List<CommandConfig> Commands = new List<CommandConfig>();

		[Category("Rights")]
		[Description("Default rights level for non-privileged users")]
		public int DefaulRightsLevel
		{
			get { return defaulRightsLevel; }
			set { defaulRightsLevel = value; }
		}

		[Category("Rights")]
		[Description("Default rights level for lobby admins (mod admins)")]
		public int DefaulRightsLevelForLobbyAdmins
		{
			get { return defaulRightsLevelForLobbyAdmins; }
			set { defaulRightsLevelForLobbyAdmins = value; }
		}

		[Category("Mod and map")]
		[Description("Default game map")]
		[TypeConverter(typeof (MapConverter))]
		public string DefaultMap
		{
			get { return defaultMap; }
			set { defaultMap = value; }
		}

		[Category("Mod and map")]
		[Description("Default game mod")]
		[TypeConverter(typeof (ModConverter))]
		public string DefaultMod
		{
			get { return defaultMod; }
			set { defaultMod = value; }
		}

		public List<BattleRect> DefaultRectangles = new List<BattleRect>();

		[Category("Rights")]
		[Description("Default rights level for non-privileged users when there is a boss in game")]
		public int DefaultRightsLevelWithBoss { get; set; }

		[Category("Default battle settings")]
		[Description("List of units disabled by default")]
		public UnitInfo[] DisabledUnits
		{
			get { return disabledUnits; }
			set { disabledUnits = value; }
		}


		[Category("Texts")]
		[Description("Game title - appears in open game list, %1 = springie version")]
		public string GameTitle
		{
			get { return gameTitle; }
			set { gameTitle = value; }
		}

		[Category("Basic options")]
		[Description("Hosting port number")]
		public int HostingPort
		{
			get { return hostingPort; }
			set { hostingPort = value; }
		}


		[Category("Basic options")]
		[Description("Should autohost kick people below min rank?")]
		public bool KickMinRank { get; set; }

		[Category("Default battle settings")]
		[Description("Do you want to automatically kick spectators")]
		public bool KickSpectators { get; set; }

		[Category("Texts")]
		[Description("Message used when kicking spectator")]
		public string KickSpectatorText
		{
			get { return kickSpectatorText; }
			set { kickSpectatorText = value; }
		}

		[Category("Basic options")]
		[Description("Should Springie host ladder map? Pick ladder id")]
		public int LadderId { get; set; }


		[Category("Mod and map")]
		[Description("Limit map selection to this list")]
		public string[] LimitMaps { get; set; }

		[Category("Mod and map")]
		[Description("Limit mod selection to this list")]
		public string[] LimitMods { get; set; }


		[Category("Mod and map")]
		[Description("Optional mapcycle - when game ends, another map is from this list is picked. You don't have to specify exact names here, springie is using filtering capabilities to find entered maps.")]
		public string[] MapCycle
		{
			get { return mapCycle; }
			set { mapCycle = value; }
		}

		[Category("Basic options")]
		[Description("Maximum number of players")]
		public int MaxPlayers
		{
			get { return maxPlayers; }
			set { maxPlayers = value; }
		}


		[Category("Default battle settings")]
		[Description("Players with CPU speed (in GHz) below this value will be autokicked - 0 no kicking")]
		public double MinCpuSpeed { get; set; }

		[Category("Basic options")]
		[Description("Minimum rank to be allowed to join")]
		public int MinRank { get; set; }

		[Category("Basic options")]
		[Description("Game password")]
		public string Password
		{
			get { return password; }
			set { password = value; }
		}

		public List<PrivilegedUser> PrivilegedUsers = new List<PrivilegedUser>();

		[Category("Basic options")]
		[Description("Should Springie use hole punching NAT traversal method? - Incompatible with gargamel mode")]
		public bool UseHolePunching { get; set; }


		[Category("Texts")]
		[Description("Welcome message - server says this when users joins. %1 = user name, %2 = user rights level, %3 = springie version")]
		public string Welcome
		{
			get { return welcome; }
			set { welcome = value; }
		}

		#endregion

		#region Public methods

		public void AddMissingCommands()
		{
			var addedCommands = new List<CommandConfig>();

			AddMissing(new CommandConfig("help", 0, " - lists all commands available specifically to you", 5), addedCommands);
			AddMissing(new CommandConfig("setpwserver", 0, " <address> - sets PlanetWars server", 5), addedCommands);

			AddMissing(new CommandConfig("random", 1, "<allycount> - assigns people to <allycount> random alliances, e.g. !random - makes 2 random alliances", 10), addedCommands);

			AddMissing(new CommandConfig("register", 5, "<side> <password> - registers player for PlanetWars, e.g. !register arm secretPassword", 1), addedCommands);

			AddMissing(new CommandConfig("balance", 1, "<allycount> - assigns people to <allycount> rank balanced alliances, e.g. !balance - makes 2 random but balanced alliances", 10), addedCommands);

			AddMissing(new CommandConfig("start", 1, " - starts game", 5), addedCommands);

			AddMissing(new CommandConfig("ring", 0, "[<filters>..] - rings all unready or specific player(s), e.g. !ring - rings unready, !ring icho - rings Licho", 5, new[] {TasSayEventArgs.Places.Normal, TasSayEventArgs.Places.Battle, TasSayEventArgs.Places.Game}), addedCommands);

			AddMissing(new CommandConfig("listmaps", 0, "[<filters>..] - lists maps on server, e.g. !listmaps altor div", 10), addedCommands);

			AddMissing(new CommandConfig("listplanets", 5, "[<filters>..] - lists planets open for attack !listmaps quantumia", 1), addedCommands);
			AddMissing(new CommandConfig("planet", 5, "[<filters>..] - picks a planet (you must be commander-in-chief)", 1), addedCommands);
			AddMissing(new CommandConfig("voteplanet", 5, "[<filters>..] - starts a vote for new planet", 1), addedCommands);
            AddMissing(new CommandConfig("attack", 5, "[<filters>..] - picks a planet (you must be commander-in-chief)", 1), addedCommands);
            AddMissing(new CommandConfig("voteattack", 5, "[<filters>..] - starts a vote for new planet", 1), addedCommands);


			AddMissing(new CommandConfig("listmods", 0, "[<filters>..] - lists mods on server, e.g. !listmods absolute 2.23", 5), addedCommands);
			AddMissing(new CommandConfig("map", 2, "[<filters>..] - changes server map, eg. !map altor div"), addedCommands);

			AddMissing(new CommandConfig("manage", 1, "<minaplayer> [<maxplayers>] - auto manage server for min to max players"), addedCommands);

			AddMissing(new CommandConfig("forcestart", 2, " - starts game forcibly (ignoring warnings)", 5), addedCommands);

			AddMissing(new CommandConfig("say", 0, "<text> - says something in game", 0, new[] {TasSayEventArgs.Places.Normal, TasSayEventArgs.Places.Battle, TasSayEventArgs.Places.Game}), addedCommands);

			AddMissing(new CommandConfig("force", 2, " - forces game start inside game", 8, new[] {TasSayEventArgs.Places.Normal, TasSayEventArgs.Places.Battle, TasSayEventArgs.Places.Game}), addedCommands);
			AddMissing(new CommandConfig("kick", 3, "[<filters>..] - kicks a player", 0, new[] {TasSayEventArgs.Places.Normal, TasSayEventArgs.Places.Battle, TasSayEventArgs.Places.Game}), addedCommands);

			AddMissing(new CommandConfig("split", 1, "<\"h\"/\"v\"> <percent> - draws with given direction and percentual size, e.g. !split h 15"), addedCommands);

			AddMissing(new CommandConfig("corners", 1, "<\"a\"/\"b\"> <percent> - draws corners (a or b mode differ in ordering), e.g. !corners a 15"), addedCommands);

			AddMissing(new CommandConfig("exit", 3, " - exits the game", 0, new[] {TasSayEventArgs.Places.Normal, TasSayEventArgs.Places.Battle, TasSayEventArgs.Places.Game}), addedCommands);

			AddMissing(new CommandConfig("lock", 1, " - locks the game"), addedCommands);

			AddMissing(new CommandConfig("unlock", 1, " - unlocks the game"), addedCommands);

			AddMissing(new CommandConfig("fix", 1, " - fixes teamnumbers", 5), addedCommands);

			AddMissing(new CommandConfig("votemap", 0, "[<mapname>..] - starts vote for new map, e.g. !votemap altored div"), addedCommands);

			AddMissing(new CommandConfig("votekick", 0, "[<mapname>..] - starts vote to kick a player, e.g. !votekick Licho", 0, new[] {TasSayEventArgs.Places.Normal, TasSayEventArgs.Places.Battle, TasSayEventArgs.Places.Game}), addedCommands);

			AddMissing(new CommandConfig("voteforcestart", 0, " - starts vote to force game to start in lobby"), addedCommands);

			AddMissing(new CommandConfig("voteforce", 0, " - starts vote to force game to start from game", 0, new[] {TasSayEventArgs.Places.Normal, TasSayEventArgs.Places.Battle, TasSayEventArgs.Places.Game}), addedCommands);

			AddMissing(new CommandConfig("voteexit", 0, " - starts vote to exit game", 0, new[] {TasSayEventArgs.Places.Normal, TasSayEventArgs.Places.Battle, TasSayEventArgs.Places.Game}), addedCommands);

			AddMissing(new CommandConfig("vote", 0, "<number> - votes for given option (works from battle only), e.g. !vote 1", 0, new[] {TasSayEventArgs.Places.Battle, TasSayEventArgs.Places.Game}), addedCommands);

			AddMissing(new CommandConfig("rehost", 3, "[<modname>..] - rehosts game, e.g. !rehost abosol 2.23 - rehosts AA2.23"), addedCommands);
			AddMissing(new CommandConfig("voterehost", 0, "[<modname>..] - votes to rehost game, e.g. !rehost abosol 2.23 - rehosts AA2.23", 0, new[] {TasSayEventArgs.Places.Normal, TasSayEventArgs.Places.Battle, TasSayEventArgs.Places.Game}), addedCommands);

			AddMissing(new CommandConfig("admins", 0, " - lists admins", 5), addedCommands);

			AddMissing(new CommandConfig("setlevel", 4, "<level> <playername> - set's rights level for player.Setting to 0 deletes players."), addedCommands);

			AddMissing(new CommandConfig("maplink", 0, "[<mapname>..] - looks for maplinks at unknown-files", 5, new[] {TasSayEventArgs.Places.Battle, TasSayEventArgs.Places.Normal}), addedCommands);

			AddMissing(new CommandConfig("modlink", 0, "[<modname>..] - looks for modlinks at unknown-files", 5, new[] {TasSayEventArgs.Places.Battle, TasSayEventArgs.Places.Normal}), addedCommands);


			AddMissing(new CommandConfig("id", 2, "<idnumber> [<playername>..] - forces given player to an id"), addedCommands);

			AddMissing(new CommandConfig("team", 2, "<teamnumber> [<playername>..] - forces given player to a team"), addedCommands);

			AddMissing(new CommandConfig("helpall", 0, "- lists all commands known to Springie (sorted by command level)", 5), addedCommands);

			AddMissing(new CommandConfig("fixcolors", 1, "- changes too similar colors to more different (note that color difference is highly subjective and impossible to model mathematically, so it won't always produce results satisfying for all)", 10), addedCommands);

			AddMissing(new CommandConfig("springie", 0, "- responds with basic springie information", 5, new[] {TasSayEventArgs.Places.Normal, TasSayEventArgs.Places.Battle, TasSayEventArgs.Places.Channel}), addedCommands);

			AddMissing(new CommandConfig("endvote", 2, "- ends current poll"), addedCommands);

			AddMissing(new CommandConfig("addbox", 1, "<left> <top> <width> <height> [<number>] - adds a new box rectangle"), addedCommands);

			AddMissing(new CommandConfig("clearbox", 1, "[<number>] - removes a box (or removes all boxes if number not specified)"), addedCommands);

			AddMissing(new CommandConfig("listpresets", 0, "[<presetname>..] - lists all presets this server has (with name filtering)", 5), addedCommands);

			AddMissing(new CommandConfig("listoptions", 1, " - lists all mod/map options", 5), addedCommands);

			AddMissing(new CommandConfig("setoptions", 1, "<name>=<value>[,<name>=<value>] - applies mod/map options", 0), addedCommands);

			AddMissing(new CommandConfig("votesetoptions", 1, "<name>=<value>[,<name>=<value>] - starts a vote to apply mod/map options", 0), addedCommands);

			AddMissing(new CommandConfig("preset", 2, "[<presetname>..] - applies given preset to current battle"), addedCommands);

			AddMissing(new CommandConfig("votepreset", 0, "[<presetname>..] - starts a vote to apply given preset"), addedCommands);

			AddMissing(new CommandConfig("presetdetails", 0, "[<presetname>..] - shows details of given preset", 2), addedCommands);

			AddMissing(new CommandConfig("cbalance", 1, "[<allycount>] - assigns people to allycount random balanced alliances but attempts to put clanmates to same teams", 10), addedCommands);

			AddMissing(new CommandConfig("ban", 4, "<username> [<duration>] [<reason>...] - bans user username for duration (in minutes) with given reason. Duration 0 = ban for 1000 years", 0), addedCommands);

			AddMissing(new CommandConfig("unban", 4, "<username> - unbans user", 0), addedCommands);

			AddMissing(new CommandConfig("listbans", 0, "- lists currently banned users", 0), addedCommands);

			AddMissing(new CommandConfig("smurfs", 0, "- finds smurfs, use this command to get more help", 5, new[] {TasSayEventArgs.Places.Normal, TasSayEventArgs.Places.Battle, TasSayEventArgs.Places.Channel}), addedCommands);

			AddMissing(new CommandConfig("stats", 0, "- displays statistics, use this command to get more help", 5, new[] {TasSayEventArgs.Places.Normal, TasSayEventArgs.Places.Battle, TasSayEventArgs.Places.Channel}), addedCommands);

			AddMissing(new CommandConfig("kickspec", 2, "[0|1] - enables or disables automatic spectator kicking", 0, new[] {TasSayEventArgs.Places.Battle, TasSayEventArgs.Places.Game, TasSayEventArgs.Places.Normal}), addedCommands);

			AddMissing(new CommandConfig("votekickspec", 0, "- starts a vote to enables or disable automatic spectator kicking", 0, new[] {TasSayEventArgs.Places.Battle, TasSayEventArgs.Places.Game}), addedCommands);

			AddMissing(new CommandConfig("setpassword", 4, "<newpassword> - sets server password (needs !rehost to apply)"), addedCommands);

			AddMissing(new CommandConfig("setminrank", 4, "<minrank> - sets server minimum rank (needs !rehost to apply)"), addedCommands);

			AddMissing(new CommandConfig("setmaxplayers", 4, "<maxplayers> - sets server size (needs !rehost to apply)"), addedCommands);

			AddMissing(new CommandConfig("setgametitle", 4, "<new title> - sets server game title (needs !rehost to apply)"), addedCommands);

			AddMissing(new CommandConfig("mincpuspeed", 4, "<GHz> - sets minimum CPU for this host - players with CPU speed below this value are auto-kicked, 0 = no limit"), addedCommands);

			AddMissing(new CommandConfig("boss", 2, "<name> - sets <name> as a new boss, use without parameter to remove any current boss. If there is a boss on server, other non-admin people have their rights reduced"), addedCommands);

			AddMissing(new CommandConfig("voteboss", 0, "<name> - sets <name> as a new boss, use without parameter to remove any current boss. If there is a boss on server, other non-admin people have their rights reduced", 0, new[] {TasSayEventArgs.Places.Normal, TasSayEventArgs.Places.Battle, TasSayEventArgs.Places.Game}), addedCommands);

			AddMissing(new CommandConfig("autolock", 1, "<players> - sets desired number of players in game. If this number is reached, server will autolock itself, if someone leaves, it will autounlock again. !autolock without parameter disables auto locking"), addedCommands);

			AddMissing(new CommandConfig("spec", 2, "<username> - forces player to become spectator", 0), addedCommands);

			AddMissing(new CommandConfig("specafk", 1, "forces all AFK player to become spectators", 0), addedCommands);

			AddMissing(new CommandConfig("kickminrank", 4, "[0/1] enables or disables automatic kicking of people based upon their rank", 0), addedCommands);

			AddMissing(new CommandConfig("cheats", 3, "enables/disables .cheats in game", 0, new[] {TasSayEventArgs.Places.Normal, TasSayEventArgs.Places.Battle, TasSayEventArgs.Places.Game}), addedCommands);

			AddMissing(new CommandConfig("notify", 0, "springie notifies you when game ends", 0, new[] {TasSayEventArgs.Places.Normal, TasSayEventArgs.Places.Battle, TasSayEventArgs.Places.Game, TasSayEventArgs.Places.Channel}), addedCommands);

            AddMissing(new CommandConfig("resetpassword", 0,"resets planetwars password", 0), addedCommands);

			Commands.RemoveAll(delegate(CommandConfig c) { return !addedCommands.Contains(c); });
		}


		public static int CommandComparer(CommandConfig a, CommandConfig b)
		{
			return a.Name.CompareTo(b.Name);
		}

		public void Defaults()
		{
			DefaultRectangles.Add(new BattleRect(0.0, 0.0, 1.0, 0.15));
			DefaultRectangles.Add(new BattleRect(0.0, 0.85, 1.0, 1.0));
			AddMissingCommands();
		}

		public void SetPrivilegedUser(string name, int level)
		{
			for (int i = 0; i < PrivilegedUsers.Count; ++i) {
				if (PrivilegedUsers[i].Name == name) {
					if (level == 0) {
						PrivilegedUsers.RemoveAt(i);
						return;
					} else {
						PrivilegedUsers[i].Level = level;
						return;
					}
				}
			}
			if (level > 0) PrivilegedUsers.Add(new PrivilegedUser(name, level));
		}

		public static int UserComparer(PrivilegedUser a, PrivilegedUser b)
		{
			return a.Name.CompareTo(b.Name);
		}

		#endregion

		#region Other methods

		private void AddMissing(CommandConfig command, List<CommandConfig> addedCommands)
		{
			foreach (var c in Commands) {
				if (c.Name == command.Name) {
					addedCommands.Add(c);
					return;
				}
			}
			Commands.Add(command);
			addedCommands.Add(command);
		}

		#endregion

		#region Nested type: MapConverter

		public class MapConverter : StringConverter
		{
			#region Overrides

			public override StandardValuesCollection GetStandardValues(ITypeDescriptorContext context)
			{
				var maps = new List<string>();
				foreach (var p in Program.main.Spring.UnitSyncWrapper.MapList) maps.Add(p.Value.Name);
				return new StandardValuesCollection(maps);
			}

			public override bool GetStandardValuesSupported(ITypeDescriptorContext context)
			{
				return true;
			}

			#endregion
		} ;

		#endregion

		#region Nested type: ModConverter

		public class ModConverter : StringConverter
		{
			#region Overrides

			public override StandardValuesCollection GetStandardValues(ITypeDescriptorContext context)
			{
				var mods = new List<string>();
				foreach (var p in Program.main.Spring.UnitSyncWrapper.ModList) mods.Add(p.Value.Name);
				return new StandardValuesCollection(mods);
			}

			public override bool GetStandardValuesSupported(ITypeDescriptorContext context)
			{
				return true;
			}

			#endregion
		} ;

		#endregion
	}
}