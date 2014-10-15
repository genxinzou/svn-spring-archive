WARNING:
Upon first login Springie creates file do_not_delete_me.xml. This file contains password to central stats server. If you delete it you won't be able to gather stats anymore (central server won't give a new password to same spring account for security reasons)

Commands:
========
--- HELP ---
!help - lists commands availiable specifically to you

!helpall - lists all commands (sorted by their level)

--- COMMUNICATION ---
!ring [<filters>..]- rings either all unready or specified users

!say - allows you to talk between lobby and game  (no need for it if realy chat enabled)

--- TEAM AND ALLY MANAGEMENT ---
!fix - fix teamnumbers

!fixcolors - fixes team colors (too similar colors are changed)

!random [<allycount>] - assigns people to <allycount> alliances (!random makes 2 alliances with same number of people)

!balance [<allycount>] - scans all combinations and pick randomly some with best ally rank balance (warning,untest for large teams, might take too long)

!cbalance [<allycount>] - assigns people to allycount random balanced alliances but attempts to put clanmates to same teams

!ally <allynum> [<player>..] - forces ally number

!team <teamnum> [<player>..] - forces team number

!spec <username> - forces player to become spectator

!specafk - forces all AFK player to become spectators


--- GAME CONTROL ---
!start - starts game (checks for duplicate team numbers and ally fairness)

!forcestart - overrides ordinary !start checks (duplicate team numbers and ally fairness)

!force - makes spring force start (ctrl+enter)

!exit - exits the game

--- VOTING ---
!voteforcestart - starts vote to execute !forcestart

!voteforce - starts vote to force (in game)

!voteexit - starts vote to exit game

!votekick [<filter>] - starts vote to kick player

!votemap [<filters>] - starts vote for given map

!voterehost [<filter>] - starts the vote to rehost

!votepreset [<presetname>..] - starts a vote to apply given preset

!voteboss <name> - sets <name> as a new boss, use without parameter to remove any current boss. If there is a boss on server, other non-admin people have their rights reduced

!votekickspec - starts a vote to enables or disable automatic spectator kicking

!vote <number> - casts your vote (you must say it in battle window)

!endvote - ends current poll

--- BATTLE LOCKING --
!lock - locks battle

!unlock - unlocks battle

!autlock [<players>] - sets desired number of players in game. If this number is reached, server will autolock itself, if someone leaves, it will autounlock again. !autolock without parameter disables auto locking

--- PRESETS ---
!listpresets [<presetname>..] - lists all presets this server has (with name filtering)

!presetdetails [<presetname>..] - shows details of given preset

!preset [<presetname>..] - applies given preset to current battle


--- BOXES ---
!addbox <left> <top> <width> <height> [<number>] - adds a new box rectangle

!clearbox [<number>] - removes a box (or removes all boxes if number not specified)

!split <"h"/"v"> <percent> - makes 2 boxes in h or v direction

!corners <"a"/"b"> <percent> - makes 4 corners, a/b determines ordering

--- MAPS ---
!listmaps [<filters>] - lists maps (can apply filtering)

!map <filters> - changes map, says new maplink

!maplink - looks for map links on unknown files 

!dlmap <mapname/dllid/url> - downloads map to server. You can either specify map name or map id (from unknown files) or map URL

-- MODS --
!listmods [<filters>] - lists mods (can apply filtering)

!modlink - looks for mod links on unknown files 

!dlmod <modname/dllid/url> - downloads mod to server. You can either specify mod name or mod id (from unknown files) or mod URL

!rehost [<filter>] - rehosts game (optionally with new mod)

--- ADMINISTERING -- 
!admins - list privileged users

!boss <name> - sets <name> as a new boss, use without parameter to remove any current boss. If there is a boss on server, other non-admin people have their rights reduced.

!kick <filters> - kicks specified players from server (both game and lobby)

!setlevel <username> <level> - sets rights level for given player 

!ban <username> [<duration>] [<reason>...] - bans user username for duration (in minutes) with given reason. Duration 0 = ban for 1000 years

!unban <username> - unbans user

!listbans - lists currently banned users

!reload - reloads mod and map list

!kickspec [0|1] - enables or disables automatic spectator kicking

!mincpuspeed <GHz> - sets minimum CPU for this host - players with CPU speed below this value are auto-kicked, 0 = no limit

!setpassword <newpassword> - sets server password (needs !rehost to apply)

!setminrank <minrank> - sets server minimum rank (needs !rehost to apply)

!setmaxplayers <maxplayers> - sets server size (needs !rehost to apply)

!setgametitle <new title> - sets server game title (needs !rehost to apply)

!springie - displays basic springie information

!cheats - enables/disables .cheats in game

--- STATISTICS ---
!stats - lists various stats

!smurfs - lists smurfs


/////////////////////////////////////////////////
CHANGELOG

===============================
SPRINGIE 0.99
===============================
- game now auto-starts when AutoLock limit is reached and all ready/teams ok
- fixed error - autohost with # in title were unable to submit stats
- removed hole punching (was causing more problems than it solved)
- fixed annoying repeating dialog popup asking for correct password
- added automated countdown when asking for new password
- 64bit Vista compatibility fix
- automatic map discovery changed to only auto-download new maps when spring is not running


===============================
SPRINGIE 0.98
===============================
- !setgametitle added
- lobby admins now have separate default rights level (set it in game settings, default level is 4)
- if spectator kicking is enabled, server automatically locks up when full and unlocks when less than full to prevent annoying kicks
- fixed bug in springie that caused some games to be stuck in "in game status". This used to happen when spring crashed during startup.
- !autolock added - this command autolocks/unlocks server when certain number of players is reached. AutoLockMinPlayers config option added to game settings.


===============================
SPRINGIE 0.97
===============================
- !setpassword, !setminrank, !setmaxplayers added
- reduced spams from !ring and !vote commands
- !mincpuspeed added - to auto kick people with lower than given CPU speed (similar options added to battle settings)
- !tempadmin, !votetempadmin commands added. This allows you to put one person as a temporary admin. Tempadmin has his normal rights, but rights of all others (except admins) are reduced to value you can set it game settings (0 by default). Since commands like !balance and !start need rights level 1 by default, this blocks all non-admins from executing otherwise public commands and for example breaking team setup or boxes.
- !modlink and !dlmod added
- finally fixed !cbalance (now for real :-) 
- several minor tweaks and fixes

===============================
SPRINGIE 0.96
===============================
- hole punching added

===============================
SPRINGIE 0.95b
===============================
- added automatic self-update - springie now upgrades itself automatically
- added automatic spectator kicking (configurable in "game" settings)
- added !kickspec [0|1] and !votekickspec to enable and disable automatic spectator kicking
- springie now starts game automatically when server is full, people ready and in teams
- minor bugfix for servers with stats disabled
- it's no longer possible to kick springie


===============================
SPRINGIE 0.94b
===============================

- updated for new lobby protocol version released by Betalord.  Lobby server now doesnt send player IP by default, this means that smurfs service is now less powerfull. Also only servers with Gargamel mode enabled will be able to ban by IP address. Those servers will appear as using NAT traversal method in server list. Gargamel mode servers will still report IP of users to central server for smurfs, but only players who join server.  

- fixed map change bug (springie didnt reflect map change in status bar and didnt advertise new maplink). This fix also fixed missing *locked* and *unlocked* chat messages from springie.

- springie now reports player ranks to central server (it displays it in smurfs command now)


===============================
SPRINGIE 0.93b
===============================

- changed !cbalance logic to give higher priotiy to clans playing together than rank balance


===============================
SPRINGIE 0.92b
===============================

- fixed !cbalance (clan balance command)
- fixed spring start bug and improved !start speed
- fixed bug in stats reporting (some games had both teams as winners :)
- fixed auto-unlock bug
- fixed start empty exploit (it was possible to start empty game and crash spring in host)
- springie now sends more data to central server allowing more interesting stats interaction


===============================
SPRINGIE 0.90b
===============================

- stats didn't register players who disconnected before game start (fixed)


===============================
SPRINGIE 0.89b
===============================

- fixed player status GUI bug

===============================
SPRINGIE 0.88b
===============================

- spring window hiding is now more reliable
- new icons added - systray icon now looks just like game icon in TAS to reflect game status (empty, players in , game running)
- fixed GUI crossthreading bug that caused some random errors with try tooltip and status bar texts


===============================
SPRINGIE 0.87b
===============================

- springie now has optional central server stats gathering (setup it in "main" section)
- springie now has optional gargamel mode (for smurf reporting - needs stats enabled)
- !stats and !smurfs commands added (those servers are server side and params can change, use !stats and !smurfs without parameters to obtain more help)
- springie is now using .kickbynum to properly kick players from game

===============================
SPRINGIE 0.86b
===============================

- banning added (!ban, !unban, !listbans) - banning has optional timelimit and reasons. Bannes users are tracked and their IP and name changes are monitored (=it's hard to come back once you get banned)
- mapcycle added ("game" section of settings). You can specify list of maps and when game ends Springie automatically changes map to next one from this list.


minor tweaks and bugfixes: 
==========================
- fixed unit disabling preset bug (actually workaround lobby server bug) - applying multiple presets with disabled units didnt work properly

===============================
SPRINGIE 0.85b
===============================

- !endvote command added - cancels current poll
- added !addbox and !clearbox commands to manually manage boxes
- added unit disabling
- GUI now contains "current battle" dialog which allows you to modify battle settings (details, map, disabled units) directly from menu
- presets implemented, presets can be defined in Springie settings. Presets can change some of the battle details or disabled units and can be voted for or listed on server.
- !listpresets, !preset, !presetdetail, !votepreset commands added to list, view and apply presets
- !cbalance  added - works just like balance but attempts to group clanmates together

minor tweaks and bugfixes: 
==========================
- fixed GUI default map selection bug
- springie now reports AMD "virtual" CPU speeds rather than real MHz (So Athlon 2500+ will report 2500 MHz instead of it's real 1830MHz)
- voting improved - it will now end automatically if remaining undecided votes are not enough to win
- added seperated options to minimize and hide spring
- default hosting priority now set to normal
- it's now possible to disable automatic maplink display (option in settings)
- unknown sync kicker disabled (no more need for it, lobby server is fixed now)
- it was possible to !start game when server was empty (and cause spring crash), this is now fixed
- fixed long lasting throttling bug (springie sometimes refused to run a command)
- !springie command now displays time intervals instead of GMT time

===============================
SPRINGIE 0.84b
===============================

- !fixcolors command added
- !springie command added (can be used in main or other channel). Reports back basic springie information (name, version, online since, mod, map, players, game state)
- springie now has status bar (and systray icon tooltip) which shows basic information (similar to !springie command)

minor tweaks and bugfixes: 
==========================
- improved startup error handling (missing spring link or failed spring startup)
- fixed admin settings (some buttons were causing errors)

