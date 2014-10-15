#region using

using System;
using System.ComponentModel;
using System.Xml.Serialization;
using Springie.Client;

#endregion

namespace Springie.autohost
{
	public class CommandConfig
	{
		#region Fields

		private TasSayEventArgs.Places[] listenTo = new[] {TasSayEventArgs.Places.Battle, TasSayEventArgs.Places.Normal};

		#endregion

		#region Properties

		[Category("Texts")]
		[Description("Help text to be displayed in !help listings.")]
		public string HelpText { get; set; }

		[XmlIgnore]
		public DateTime lastCall = DateTime.Now;

		[Category("Command")]
		[Description("Rights level. If user's rights level is higher or equal to rights level of command - user has rights to use this command.")]
		public int Level { get; set; }

		[Category("Command")]
		[Description("From which places can you use this command. Normal = PM to server, Battle = battle lobby, Game = from running game.")]
		public TasSayEventArgs.Places[] ListenTo
		{
			get { return listenTo; }
			set { listenTo = value; }
		}

		[ReadOnly(true)]
		[Category("Command")]
		public string Name { get; set; }

		[Category("Command")]
		[Description("How often can this command be executed (in seconds). 0 = no throttling, can execute at any time.")]
		public int Throttling { get; set; }

		#endregion

		#region Constructors

		public CommandConfig() {}


		public CommandConfig(string name, int level, string helpText, int throttling, TasSayEventArgs.Places[] listenTo) : this(name, level, helpText, throttling)
		{
			this.listenTo = listenTo;
		}

		public CommandConfig(string name, int level, string helpText, int throttling) : this(name, level, helpText)
		{
			this.Throttling = throttling;
		}

		public CommandConfig(string name, int level, string helpText)
		{
			this.Name = name;
			this.Level = level;
			this.HelpText = helpText;
		}

		#endregion
	} ;
}