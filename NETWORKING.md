# Online networking games for STK

A reminder: you are looking at NETWORKING.md of kimden's Command Manager fork (in this case, as part of Nomagno's Tyre Mod Edition), not the one of the standard code.

## Hosting server
You **should** compile STK with `-DSERVER_ONLY=ON`. This option produces a GUI-less STK binary optimized for size and memory usage, useful for situation like in VPS. As this fork is server-side (at least for now), this option is enabled **always** during the testing. You can compile the code without this option, and most likely nothing bad will happen, but it's not an intended way to run this fork.

The other option(s) you should consider using are `-DUSE_SQLITE3=ON`, if you want to use a database for storing data.

A typical cmake command looks like
```bash
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo -DSERVER_ONLY=ON -DUSE_SQLITE3=ON
```

The dependencies for RHEL/CentOS 7 are installed with:
```bash
yum install wget; cd /tmp; wget https://dl.fedoraproject.org/pub/epel/7/x86_64/Packages/e/epel-release-7-12.noarch.rpm; rpm -Uvh epel-release*rpm
yum install gcc-c++ cmake openssl-devel libcurl-devel zlib-devel enet
```
### Hosting WAN (public internet) server
You are required to have an stk online account first, go [here](https://online.supertuxkart.net/register.php) for registration.

It is recommended you have a saved user in your computer to allow hosting multiple servers simultaneously with the same account, if you have a fresh STK installation, first run:

If you intend to keep your server always on (24x7) you are required to implement port forward / direct connection with NAT penetration in your network, we will regularly remove any servers not following this rule.

`supertuxkart --init-user --login=your_registered_name --password=your_password`

After that you should see `Done saving user, leaving` in terminal if it successfully logged in.

Than you can just run:

`supertuxkart --server-config=your_config.xml --network-console`

It will create that xml configuration file if not found in current directory, you can type `quit` in terminal, than you can edit that file for further configuration as required.
` --network-console` should not be used if you run supertuxkart server later with systemd service, see issue [#4299](https://github.com/supertuxkart/stk-code/issues/4299).

#### Server configuration

A typical current server configuration xml that fits the current code version is provided below. The values listed allow to create a "generic" server (and so, they are not always corresponding to the default values), that is, you can safely use them — but at the same time, the config below doesn't provide an example how to use some advanced settings.

```xml
<?xml version="1.0"?>
<server-config version="6" >

    <!-- Name of server, encode in XML if you want to use unicode characters. -->
    <server-name value="STK Server" />

    <!-- Port used in server, if you specify 0, it will use the server port specified in stk_config.xml. If you wish to use a random port, set random-server-port to '1' in user config. STK will automatically switch to a random port if the port you specify fails to be bound. -->
    <server-port value="0" />

    <!-- Game mode in server, 0 is normal race (grand prix), 1 is time trial (grand prix), 3 is normal race, 4 time trial, 6 is soccer, 7 is free-for-all and 8 is capture the flag. -->
    <server-mode value="3" />

    <!-- Difficulty in server, 0 is beginner, 1 is intermediate, 2 is expert and 3 is supertux (the most difficult). -->
    <server-difficulty value="3" />

    <!-- Number of grand prix tracks per game (If grand prix enabled). -->
    <gp-track-count value="3" />

    <!-- Use goal target in soccer. -->
    <soccer-goal-target value="true" />

    <!-- Enable wan server, which requires you to have an stk-addons account with a saved session. Check init-user command for details. -->
    <wan-server value="true" />

    <!-- Enable network console, which can do for example kickban. -->
    <enable-console value="true" />

    <!-- Maximum number of players on the server, setting this to a value greater than 8 can cause performance degradation. -->
    <server-max-players value="8" />

    <!-- Maximum number of players in the game, all other players on the server are spectators. Specify 0 to allow all players on the server to play. -->
    <max-players-in-game value="8" />

    <!-- Password for private server, leave empty for a public server. -->
    <private-server-password value="" />

    <!-- Message of today shown in lobby, you can enter here encoded XML words or a .txt file and let STK load it. -->
    <motd value="" />

    <!-- Help message shown after sending /help. You can enter here the message or a .txt file and let STK load it. -->
    <help value="" />

    <!-- If this value is set to false, the server will ignore chat messages from all players. -->
    <chat value="true" />

    <!-- If client sends more than chat-consecutive-interval / 2 chats within this value (read in seconds), it will be ignore, negative value to disable. -->
    <chat-consecutive-interval value="8" />

    <!-- Allow players to vote for which track to play. If this value is set to false, the server will randomly pick the next track to play. -->
    <track-voting value="true" />

    <!-- Timeout in seconds for selecting karts and (or) voting tracks in server, you may want to use a lower value if you have track-voting off. -->
    <voting-timeout value="30" />

    <!-- Timeout in seconds for validation of clients in wan, currently STK will use the stk-addons server to share AES key between the client and server. -->
    <validation-timeout value="20" />

    <!-- By default WAN server will always validate player and LAN will not, disable it to allow non-validated player in WAN. -->
    <validating-player value="true" />

    <!-- Disable it to turn off all stun related code in server, it allows for saving of server resources if your server is not behind a firewall. -->
    <firewalled-server value="true" />

    <!-- Enable to allow IPv6 connection if you have a public IPv6 address. STK currently uses dual-stack mode which requires server to have both IPv4 and IPv6 and listen to same port. If STK detects your server has no public IPv6 address or port differs between IPv4 and IPv6 then it will use IPv4 only socket. For system which doesn't support dual-stack socket (like OpenBSD) you may fail to be connected by IPv4 clients. You can override the detection in config.xml at supertuxkart config-0.10 folder, with default-ip-type option. -->
    <ipv6-connection value="true" />

    <!-- No server owner in lobby which can control the starting of game or kick any players. -->
    <owner-less value="false" />

    <!-- Time to wait before entering kart selection screen if satisfied min-start-game-players below for owner less or ranked server. Negative to disable the timer, you will need to use /start then instead of the button. -->
    <start-game-counter value="60" />

    <!-- Clients below this value will be rejected from joining this server. It's determined by number of official karts in client / number of official karts in server -->
    <official-karts-threshold value="1" />

    <!-- Clients below this value will be rejected from joining this server. It's determined by number of official tracks in client / number of official tracks in server, setting this value too high will prevent android players from playing on this server, because STK android apk has some official tracks removed. -->
    <official-tracks-threshold value="0.9" />

    <!-- Clients below this value will be rejected from playing on this server. It's determined by number of official karts in client / number of official karts in server. -->
    <official-karts-play-threshold value="0" />

    <!-- Clients below this value will be rejected from playing on this server. It's determined by number of official tracks in client / number of official tracks in server, setting this value too high will prevent android players from playing on this server, because STK android apk has some official tracks removed. -->
    <official-tracks-play-threshold value="0" />

    <!-- Clients below this value will be rejected from joining this server. It's determined by number of addon karts in client -->
    <addon-karts-join-threshold value="0" />

    <!-- Clients below this value will be rejected from joining this server. It's determined by number of addon tracks in client -->
    <addon-tracks-join-threshold value="0" />

    <!-- Clients below this value will be rejected from joining this server. It's determined by number of addon arenas in client -->
    <addon-arenas-join-threshold value="0" />

    <!-- Clients below this value will be rejected from joining this server. It's determined by number of addon soccer fields in client -->
    <addon-soccers-join-threshold value="0" />

    <!-- Clients below this value will be rejected from playing games. It's determined by number of addon karts in client -->
    <addon-karts-play-threshold value="0" />

    <!-- Clients below this value will be rejected from playing games. It's determined by number of addon tracks in client -->
    <addon-tracks-play-threshold value="0" />

    <!-- Clients below this value will be rejected from playing games. It's determined by number of addon arenas in client -->
    <addon-arenas-play-threshold value="0" />

    <!-- Clients below this value will be rejected from playing games. It's determined by number of addon soccer fields in client -->
    <addon-soccers-play-threshold value="0" />

    <!-- Tracks needed to enter the server, leave empty for no restriction. -->
    <must-have-tracks value="" />

    <!-- Tracks needed to be able to play, leave empty for no restriction. -->
    <play-requirement-tracks value="" />

    <!-- List of karts that can be played on a server, leave empty for no restriction or put 'not' before the list to name tracks that cannot be played. It is not guaranteed to work with addons. -->
    <only-played-karts value="" />

    <!-- List of tracks that can be played on a server, leave empty for no restriction or put 'not' before the list to name tracks that cannot be played. -->
    <only-played-tracks value="" />

    <!-- Use fixed lap count, negative or zero to disable. -->
    <fixed-lap-count value="-1" />

    <!-- Use fixed direction (0 for forward or 1 for reverse), -1 to disable. -->
    <fixed-direction value="-1" />

    <!-- If this value is set to true, players and the server must have at least one common official track. -->
    <official-tracks-needed value="true" />

    <!-- If this value is set to true, warn and kick players who troll others. -->
    <use-anti-troll-system value="false" />

    <!-- This message is shown as warning. -->
    <troll-warn-message value="WARNING: Stop trolling!" />

    <!-- After this time (in sec) of backwards movement or stopping a warning is issued. -->
    <troll-warning-time value="10" />

    <!-- After this time (in sec) of backwards movement or stopping the player is kicked. -->
    <troll-kick-time value="15" />

    <!-- Minimum speed in correct direction to decrease wron_way timer. -->
    <troll-min-normal-speed value="12" />

    <!-- A player going slower than this is considered stopping. Negative speed allows to move with it in any direction. -->
    <troll-max-stop-speed value="5" />

    <!-- If this value is set to true, a message is send to everybody if a player makes a teammate explode. -->
    <show-teammate-hits value="true" />

    <!-- Punish teammate hits? If set to true players get an anvil when they hit teammates. -->
    <teammate-hit-mode value="true" />

    <!-- This string is the prefix for the messages if players hit teammates. (Make sure it ends with a whitespace if non-empty.) -->
    <teammate-hit-msg-prefix value="Oops, " />

    <!-- Only auto start kart selection when number of connected player is larger than or equals this value, for owner less or ranked server, after start-game-counter reaches 0. -->
    <min-start-game-players value="1" />

    <!-- Automatically end linear race game after 1st player finished for some time (currently his finished time * 0.25 + 15.0). -->
    <auto-end value="true" />

    <!-- Enable team choosing in lobby in team game (soccer and CTF). If owner-less is enabled and live-spectate is not enabled, than this option is always disabled. -->
    <team-choosing value="true" />

    <!-- If strict-players is on, no duplicated online id or split screen players are allowed, which can prevent someone using more than 1 network AI with this server. -->
    <strict-players value="false" />

    <!-- Server will submit ranking to stk-addons server for linear race games, you require permission for that. validating-player, auto-end, strict-player and owner-less will be turned on. -->
    <ranked value="false" />

    <!-- If true, the server owner can config the difficulty and game mode in the GUI of lobby. This option cannot be used with owner-less or grand prix server, and will be automatically turned on if the server was created using the in-game GUI. The changed difficulty and game mode will not be saved in this config file. -->
    <server-configurable value="true" />

    <!-- Description of modes and difficulties that can be set on a configurable server. Doesn't affect initial mode and doesn't affect unconfigurable servers. Use the format "d0123 m012345678". -->
    <available-modes value="d0123 m012345678" />

    <!-- If true, players can live join or spectate the in-progress game. Currently live joining is only available if the current game mode used in server is FFA, CTF or soccer, also official-karts-threshold will be made 1.0. If false addon karts will use their original hitbox other than tux, all players having it restriction applies. -->
    <live-spectate value="true" />

    <!-- If true, server will send its addon karts real physics (kart size, length, type, etc) to client. If false or client chooses an addon kart which server is missing, tux's kart physics and kart type of the original addon is sent. -->
    <real-addon-karts value="true" />

    <!-- Time in seconds when a flag is dropped a by player in CTF returning to its own base. -->
    <flag-return-timeout value="20" />

    <!-- Time in seconds to deactivate a flag when it's captured or returned to own base by players. -->
    <flag-deactivated-time value="3" />

    <!-- Hit limit of free for all, zero to disable hit limit. -->
    <hit-limit value="20" />

    <!-- Time limit of free for all in seconds, zero to disable time limit. -->
    <time-limit-ffa value="360" />

    <!-- Capture limit of CTF, zero to disable capture limit. -->
    <capture-limit value="5" />

    <!-- Time limit of CTF in seconds, zero to disable time limit. -->
    <time-limit-ctf value="600" />

    <!-- Value used by server to automatically estimate each game time. For races, it decides the lap of each race in network game, if more than 0.0f, the number of lap of each track vote in linear race will be determined by max(1.0f, auto-game-time-ratio * default lap of that track). For soccer if more than 0.0f, for time limit game it will be auto-game-time-ratio * soccer-time-limit in UserConfig, for goal limit game it will be auto-game-time-ratio * numgoals in UserConfig, -1 to disable for all. -->
    <auto-game-time-ratio value="-1" />

    <!-- Maximum ping allowed for a player (in ms), it's recommended to use default value if live-spectate is on. -->
    <max-ping value="300" />

    <!-- Tolerance of jitter in network allowed (in ms), it's recommended to use default value if live-spectate is on. -->
    <jitter-tolerance value="100" />

    <!-- Kick players whose ping is above max-ping. -->
    <kick-high-ping-players value="false" />

    <!-- Allow players exceeding max-ping to have a playable game, if enabled kick-high-ping-players will be disabled, please also use a default value for max-ping and jitter-tolerance with it. -->
    <high-ping-workaround value="true" />

    <!-- Kick idle player which has no network activity to server for more than some seconds during game, unless he has finished the race. Negative value to disable, and this option will always be disabled for LAN server. -->
    <kick-idle-player-seconds value="60" />

    <!-- Kick idle player which has no network activity to server for more than some seconds, while in the lobby. Duration also includes the period after the player finishes and waits for others to finish. Be careful using it. Negative value to disable, and this option will always be disabled for LAN server. -->
    <kick-idle-lobby-player-seconds value="-1" />

    <!-- Set how many states the server will send per second, the higher this value, the more bandwidth requires, also each client will trigger more rewind, which clients with slow device may have problem playing this server, use the default value is recommended. -->
    <state-frequency value="10" />

    <!-- Use sql database for handling server stats and maintenance, STK needs to be compiled with sqlite3 supported. -->
    <sql-management value="false" />

    <!-- Database filename for sqlite to use, it can be shared for all servers created in this machine, and STK will create specific table for each server. You need to create the database yourself first, see NETWORKING.md for details -->
    <database-file value="stkservers.db" />

    <!-- Specified in millisecond for maximum time waiting in sqlite3_busy_handler. You may need a higher value if your database is shared by many servers or having a slow hard disk. -->
    <database-timeout value="1000" />

    <!-- IPv4 ban list table name, you need to create the table first, see NETWORKING.md for details, empty to disable. This table can be shared for all servers if you use the same name. STK can auto kick active peer from ban list (update per minute) whichallows live kicking peer by inserting record to database. -->
    <ip-ban-table value="ip_ban" />

    <!-- IPv6 ban list table name, you need to create the table first, see NETWORKING.md for details, empty to disable. This table can be shared for all servers if you use the same name. STK can auto kick active peer from ban list (update per minute) which allows live kicking peer by inserting record to database. -->
    <ipv6-ban-table value="ipv6_ban" />

    <!-- Online ID ban list table name, you need to create the table first, see NETWORKING.md for details, empty to disable. This table can be shared for all servers if you use the same name. STK can auto kick active peer from ban list (update per minute) which allows live kicking peer by inserting record to database. -->
    <online-id-ban-table value="online_id_ban" />

    <!-- Player reports table name, which will be written when a player reports player in the network user dialog, you need to create the table first, see NETWORKING.md for details, empty to disable. This table can be shared for all servers if you use the same name. -->
    <player-reports-table value="player_reports" />

    <!-- Days to keep player reports, older than that will be auto cleared, 0 to keep them forever. -->
    <player-reports-expired-days value="0" />

    <!-- IP geolocation table, you only need this table if you want to geolocate IP from non-stk-addons connection, as all validated players connecting from stk-addons will provide the location info, you need to create the table first, see NETWORKING.md for details, empty to disable. This table can be shared for all servers if you use the same name. -->
    <ip-geolocation-table value="ip_mapping" />

    <!-- IPv6 geolocation table, you only need this table if you want to geolocate IP from non-stk-addons connection, as all validated players connecting from stk-addons will provide the location info, you need to create the table first, see NETWORKING.md for details, empty to disable. This table can be shared for all servers if you use the same name. -->
    <ipv6-geolocation-table value="ipv6_mapping" />

    <!-- If true this server will auto add / remove AI connected with network-ai=x, which will kick N - 1 bot(s) where N is the number of human players. Only use this for non-GP racing server. -->
    <ai-handling value="true" />

    <!-- If true no one can start a race and everyone should use the server for sleeping/chatting only. -->
    <sleeping-server value="false" />

    <!-- IP that should be used to connect to a server, set 0 for a default IP. You need this only if the server's IP shouldn't be used for some reason (e.g. blocking) and there is another IP ('good') that points to the same server. In this case, you should write a good IP here as a number. Only IPv4 is supported. -->
    <false-ip value="0" />

    <!-- When true, stores race results in a separate table for each server. -->
    <store-results value="false" />

    <!-- When non-empty, server is telling whether a player has beaten a server record, records are taken from the table specified in this field. So it can be the results table for this server or for all servers hosted on the machine. -->
    <records-table-name value="" />

    <!-- When true, stores the info about each forced kick in a database (if it exists). -->
    <track-kicks value="false" />

    <!-- When true, the server allows changing teams freely. -->
    <free-teams value="false" />

    <!-- When true, the server has the functionality to host soccer tournaments. Rules may change so better ask STK players about the actual rules. -->
    <soccer-tournament value="false" />

    <!-- List of players and judges. Use the format "R red red red B blue blue blue J judge judge" where the category is preceded by its letter. Categories can be empty or absent and can go in any order. You can use a category (#A) instead of listing all players. -->
    <soccer-tournament-match value="" />

    <!-- List of players with categories (teams, etc.). Use the format #Category1 player1 ... playerN #Category2 ... Use ## to hide the category from the player list. -->
    <categories value="" />

    <!-- A string specifying the match format. -->
    <soccer-tournament-rules value="nochat 10 TTTTG RRBBR;;;not %1;not %1 %2;;;" />

    <!-- A string given to a peer if it has incompatible data, so that it can know why it cannot enter, empty to disable. -->
    <incompatible-advice value="" />

    <!-- A feature to make others only spectate. -->
    <only-host-riding value="false" />

    <!-- If true, normal race and time trial games are recorded into ghost replays. -->
    <record-replays value="false" />

    <!-- If true and no track is selected, then an addon track can be picked. -->
    <random-selects-addons value="true" />

    <!-- If non-empty, these tracks (or track filters if enclosed in curly braces) are played in the order until the list ends. -->
    <tracks-queue value="" />

    <!-- If non-empty, these tracks (or track filters if enclosed in curly braces) are played in the order cyclically, except if something is in the regular tracks queue. -->
    <cyclic-tracks-queue value="" />

    <!-- If non-empty, these karts (or kart filters if enclosed in curly braces) are played in the order until the list ends. -->
    <karts-queue value="" />

    <!-- If non-empty, these karts (or kart filters if enclosed in curly braces) are played in the order cyclically, except if something is in the regular karts queue. -->
    <cyclic-karts-queue value="" />

    <!-- A custom Grand Prix scoring system to be used, should have format 'type int int int int ...', where type is either 'inc' or 'fixed'. -->
    <grand-prix-scoring value="fixed 0 1 10 8 6 5 4 3 2 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0" />

    <!-- For a private server, a list of players who can enter with any password. Works only for online accounts. -->
    <white-list value="" />

    <!-- If true, all mobile peers get a corresponding icon into the name. -->
    <expose-mobile value="true" />

    <!-- Specifies how to count own goals: standard - last touching player is counted, no-own-goals - last touching player of scoring team is counted if existing, advanced - as standard for now. -->
    <soccer-goals-policy value="standard" />

    <!-- Allows server owner (not crowned player!) to go to power mode to kick players using GUI and not be kicked, empty to disable. -->
    <power-password value="" />

    <!-- If true this server will allow AI instance to be connected from anywhere. (other than LAN network only) -->
    <ai-anywhere value="false" />

    <!-- If true, the server owner can kick players, either via the UI button or using /kick command. -->
    <kicks-allowed value="true" />

    <!-- When non-empty, stores input given by /register command in the corresponding table. Otherwise, /register command does nothing -->
    <registration-table-name value="reg" />

    <!-- If true, the GP grid is shuffled before each race, not only before the first one. -->
    <shuffle-grand-prix-grid value="true" />

    <!-- If true, when a player leaves and rejoins the battle server, the score is preserved (works for distinct names only for now). -->
    <preserve-battle-scores value="false" />

    <!-- Whatever specified here wouldn't be reset when all players leave. Possible options are mode, elim, laps, queue, replay, to include several separate them by spaces, empty to include nothing. -->
    <preserve-on-reset value="replay" />

    <!-- Specifies how the server should decide which map vote wins in map selection. 0 corresponds to standard system, 1 - to randomly selecting one of votes. -->
    <map-vote-handling value="0" />

    <!-- Codes of teams initially available on the server. Format is any subset of rbygopcms (the teams currently available). You can change it from the lobby using hammer command(s), and this will be overridden during soccer and ctf modes. -->
    <available-teams value="rbygopcms" />

    <!-- File with commands used for this server. Default file is commands.xml which can be changed with new commits, but you can use any other file from data/ folder, or even include contents of one other commands file using external-commands-file tag inside your file. -->
    <commands-file value="commands.xml" />

    <!-- Whether the game can be started immediately when the server is opened. -->
    <start-allowed value="true" />

    <!-- Timeout in seconds during which you cannot start the next game, regardless of hammers, votes, or buttons pressed.-->
    <lobby-cooldown value="0" />

</server-config>
```

If you use a single configuration file for a server (like the one above), it will be overridden by the game when you close the server. Edit your config file only when your server is down.

#### Nested server configuration

Alternatively, you can use one configuration file as base configuration, and another one to override the base settings with individual server settings. This is useful when you run several servers and don't want to clone the same values everywhere. The base configuration file has the same format as in single file configuration case, and the individual configuration file can include only those settings you need to change for that specific server, plus the `external-config` setting as shown below in an example of individual config file:

```xml
<?xml version="1.0"?>
<server-config version="6" >

    <external-config value="/full/path/to/base/configuration.xml" />

    <!-- Name of server, encode in XML if you want to use unicode characters. -->
    <server-name value="STK Server from Individual Config File" />

    <!-- Port used in server, if you specify 0, it will use the server port specified in stk_config.xml. If you wish to use a random port, set random-server-port to '1' in user config. STK will automatically switch to a random port if the port you specify fails to be bound. -->
    <server-port value="12345" />

    <!-- Maximum number of players on the server, setting this to a value greater than 8 can cause performance degradation. -->
    <server-max-players value="15" />

    <!-- Message of today shown in lobby, you can enter here encoded XML words or a .txt file and let STK load it. -->
    <motd value="motd.txt" />

</server-config>
```

Note, however, that your configuration files will **not** be overridden if you use this kind of nested configuration, as there's no code to overwrite a file with only certain settings. It can also be more convenient to be able to edit files for the next server restart while the server is still running. On the other hand, if you need to format the base configuration properly, you can just run STK once on it.

#### Ports and STUN

At the moment STK has a list of STUN servers for NAT penetration which allows players or servers behind a firewall or router to be able to connect to each other, but in case it doesn't work, you have to manually disable the firewall or port forward the port(s) used by STK.
By default STK servers use port `2759` (UDP). For example, in Ubuntu based distributions, run the following command to disable the firewall on that port:

`sudo ufw allow 2759`

You may also need to handle the server discovery port `2757` (UDP) for connecting your WAN server in LAN / localhost.

Notice: You don't need to make any firewall or router configuration changes if you connect to the recommended servers (marked with ☆★STK★☆).

### Hosting LAN (local internet) server
Everything is basically the same as WAN one, except you don't need an stk online account, just do:

`supertuxkart --server-config=your_config.xml --lan-server=your_server_name --network-console`

For LAN server it is required that the server and server discovery port is connectable by clients directly, no NAT penetration will be done in LAN.

LAN server can be connected too by typing your server public address (with port) in ```Enter server address``` dialog without relying on stk-addons.

------
After the first time configuration, you can just start the server with the command:

`supertuxkart --server-config=your_config.xml`, regardless of whether LAN or WAN server is chosen (of course you need to have a saved user for the WAN one), by default your server logging will be saved to the STK configuration directory with a name of `your_config.log`, given that the server configuration filename is `your_config.xml`.

You can find out that directory location [here (See Where is the configuration stored?)](https://supertuxkart.net/FAQ)

## Testing server
There is a network AI tester in STK which can use AI on player controller for server hosting linear races game mode, which helps automating the testing for servers, to enable it use it on lan server:

`supertuxkart --connect-now=x.x.x.x:y --network-ai=n --no-graphics`

Here, `x.x.x.x` is IP address, `y` is port, and `n` is the number of AIs to connect. Remove `--no-graphics` if you want to see the AI racing. You can also run network AI tester in server-only build of STK.

With the network AI tester, it's easier to for example simulate high-loaded servers or bad networks (ones with high ping and/or packet loss).

Tested on a Raspberry Pi 3 Model B+, if you have 8 players connected to a server hosted on it, the usage of a single CPU core is ~60% and there are ~60MB of memory usage for game with heavy tracks like Cocoa Temple or Candela City on the server, you can use the above figures to estimate how many STK servers can be hosted on the same computer.

For bad network simulation, we recommend `network traffic control` by Linux kernel, see [here](https://wiki.linuxfoundation.org/networking/netem) for details.

You will have the best gaming experience by choosing a server where all players have less than 100ms ping with no packet loss.

## Server management (Since 1.1)

Currently STK uses sqlite (if building with sqlite3 on) for server management with the following functions at the moment:
1. Server statistics
2. IP / online ID ban list
3. Player reports
4. IPv4 and IPv6 geolocation
5. Record tables (if enabled)

You need to create a database in sqlite first, run `sqlite3 stkservers.db` in the folder where (all) your server_config.xml(s) located. If that alone doesn't create the database, you might need to do some actions, for example, `CREATE TABLE a(b);`, then `DROP TABLE a;`, so that database is formed correctly.

A table named `v(server database version)_(your_server_config_filename_without_.xml_extension)_stats` will also be created in your database if one does not exist:
```sql
CREATE TABLE IF NOT EXISTS (table name above)
(
    host_id INTEGER UNSIGNED NOT NULL PRIMARY KEY, -- Unique host id in STKHost of each connection session for a STKPeer
    ip INTEGER UNSIGNED NOT NULL, -- IP decimal of host
    ipv6 TEXT NOT NULL DEFAULT '', -- IPv6 (if exists) in string of host (only created if IPv6 server)
    port INTEGER UNSIGNED NOT NULL, -- Port of host
    online_id INTEGER UNSIGNED NOT NULL, -- Online if of the host (0 for offline account)
    username TEXT NOT NULL, -- First player name in the host (if the host has splitscreen player)
    player_num INTEGER UNSIGNED NOT NULL, -- Number of player(s) from the host, more than 1 if it has splitscreen player
    country_code TEXT NULL DEFAULT NULL, -- 2-letter country code of the host
    version TEXT NOT NULL, -- SuperTuxKart version of the host
    os TEXT NOT NULL, -- Operating system of the host
    connected_time TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP, -- Time when connected
    disconnected_time TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP, -- Time when disconnected (saved when disconnected)
    ping INTEGER UNSIGNED NOT NULL DEFAULT 0, -- Ping of the host
    packet_loss INTEGER NOT NULL DEFAULT 0, -- Mean packet loss count from ENet (saved when disconnected)
    addon_karts_count INTEGER UNSIGNED NOT NULL DEFAULT -1, -- Number of addon karts of the host
    addon_tracks_count INTEGER UNSIGNED NOT NULL DEFAULT -1, -- Number of addon tracks of the host
    addon_arenas_count INTEGER UNSIGNED NOT NULL DEFAULT -1, -- Number of addon arenas of the host
    addon_soccers_count INTEGER UNSIGNED NOT NULL DEFAULT -1 -- Number of addon soccers of the host
) WITHOUT ROWID;
```
Note that unlike in the standard code, there are four additional columns for addon numbers.

STK will also create the following default views from the stats table:

`*_full_stats`
Full stats with ip in human readable format and time played of each players in minutes.

`*_current_players`
Current players in server with ip in human readable format and time played of each players in minutes.

`*_player_stats`
All players with online id and username with their time played stats in this server since creation of this database.
If sqlite supports window functions (since 3.25), it will include last session player info (ip, country, ping...).

If `store-results` is enabled in server configuration file, a table named `v(server database version)_(your_server_config_filename_without_.xml_extension)_results` will be created to store the results of the games. Check [this page](doc/fork/Database specifications.md) for the exact structure of this database. The structure was last updated in August 2024, so you might need to update your database for that, the page also has the instructions how to do it.

If you had a database with a table of results of an old format and attempt to run latest code, the code will not allow the server to start. That is done intentionally, so that server owners are aware of updates and edit the database themselves, as opposed to silently crashing something with the new code. You can disable it with `-DFATAL_WHEN_OLD_RECORDS=OFF`.

An empty table named `v(server database version)_countries` will also be created in your database if not exists:
```sql
CREATE TABLE IF NOT EXISTS (table name above)
(
    country_code TEXT NOT NULL PRIMARY KEY UNIQUE, -- Unique 2-letter country code
    country_flag TEXT NOT NULL, -- Unicode country flag representation of 2-letter country code
    country_name TEXT NOT NULL -- Readable name of this country
) WITHOUT ROWID;
```

If you want to see flags and readable names of countries in the above views, you need to initialize `v(server database version)_countries` table, check [this script](tools/generate-countries-table.py).

For IPv4 and online ID ban list, player reports or IP mapping (in case your server doesn't communicate with STK addons server, which can do IP mapping instead of your server), you need to create one yourself:
```sql
CREATE TABLE ip_ban
(
    ip_start INTEGER UNSIGNED NOT NULL UNIQUE, -- Starting of ip decimal for banning (inclusive)
    ip_end INTEGER UNSIGNED NOT NULL UNIQUE, -- Ending of ip decimal for banning (inclusive)
    starting_time TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP, -- Starting time of this banning entry to be effective
    expired_days REAL NULL DEFAULT NULL, -- Days for this banning to be expired, use NULL for a permanent ban
    reason TEXT NOT NULL DEFAULT '', -- Banned reason shown in user stk menu, can be empty
    description TEXT NOT NULL DEFAULT '', -- Private description for server admin
    trigger_count INTEGER UNSIGNED NOT NULL DEFAULT 0, -- Number of banning triggered by this ban entry
    last_trigger TIMESTAMP NULL DEFAULT NULL -- Latest time this banning entry was triggered
);

CREATE TABLE ipv6_ban
(
    ipv6_cidr TEXT NOT NULL UNIQUE, -- IPv6 CIDR range for banning (for example 2001::/64), use /128 for a specific ip
    starting_time TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP, -- Starting time of this banning entry to be effective
    expired_days REAL NULL DEFAULT NULL, -- Days for this banning to be expired, use NULL for a permanent ban
    reason TEXT NOT NULL DEFAULT '', -- Banned reason shown in user stk menu, can be empty
    description TEXT NOT NULL DEFAULT '', -- Private description for server admin
    trigger_count INTEGER UNSIGNED NOT NULL DEFAULT 0, -- Number of banning triggered by this ban entry
    last_trigger TIMESTAMP NULL DEFAULT NULL -- Latest time this banning entry was triggered
);

CREATE TABLE online_id_ban
(
    online_id INTEGER UNSIGNED NOT NULL UNIQUE, -- Online id from STK addons database for banning
    starting_time TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP, -- Starting time of this banning entry to be effective
    expired_days REAL NULL DEFAULT NULL, -- Days for this banning to be expired, use NULL for a permanent ban
    reason TEXT NOT NULL DEFAULT '', -- Banned reason shown in user stk menu, can be empty
    description TEXT NOT NULL DEFAULT '', -- Private description for server admin
    trigger_count INTEGER UNSIGNED NOT NULL DEFAULT 0, -- Number of banning triggered by this ban entry
    last_trigger TIMESTAMP NULL DEFAULT NULL -- Latest time this banning entry was triggered
);

CREATE TABLE player_reports
(
    server_uid TEXT NOT NULL, -- Report from which server unique id (config filename)
    reporter_ip INTEGER UNSIGNED NOT NULL, -- IP decimal of player who reports
    reporter_ipv6 TEXT NOT NULL DEFAULT '', -- IPv6 (if exists) in string of player who reports (only needed for IPv6 server)
    reporter_online_id INTEGER UNSIGNED NOT NULL, -- Online id of player who reports, 0 for offline player
    reporter_username TEXT NOT NULL, -- Player name who reports
    reported_time TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP, -- Time of reporting
    info TEXT NOT NULL, -- Report info by reporter
    reporting_ip INTEGER UNSIGNED NOT NULL, -- IP decimal of player being reported
    reporting_ipv6 TEXT NOT NULL DEFAULT '', -- IPv6 (if exists) in string of player who reports (only needed for IPv6 server)
    reporting_online_id INTEGER UNSIGNED NOT NULL, -- Online id of player being reported, 0 for offline player
    reporting_username TEXT NOT NULL -- Player name being reported
);

CREATE TABLE ip_mapping
(
    ip_start INTEGER UNSIGNED NOT NULL PRIMARY KEY UNIQUE, -- IP decimal start
    ip_end INTEGER UNSIGNED NOT NULL UNIQUE, -- IP decimal end
    latitude REAL NOT NULL, -- Latitude of this IP range
    longitude REAL NOT NULL, -- Longitude of this IP range
    country_code TEXT NOT NULL -- 2-letter country code
) WITHOUT ROWID;

CREATE TABLE ipv6_mapping
(
    ip_start INTEGER UNSIGNED NOT NULL PRIMARY KEY UNIQUE, -- IP decimal (upper 64bit) start
    ip_end INTEGER UNSIGNED NOT NULL UNIQUE, -- IP decimal (upper 64bit) end
    latitude REAL NOT NULL, -- Latitude of this IP range
    longitude REAL NOT NULL, -- Longitude of this IP range
    country_code TEXT NOT NULL -- 2-letter country code
);
```

For initialization of `ip_mapping` table, check [this script](tools/generate-ip-mappings.py).
