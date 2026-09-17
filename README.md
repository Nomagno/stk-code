# SuperTuxKart Tyre Mod Edition by Nomagno
### + Command Manager (CM) by Kimden
#### Or in short, STK 2.Xue TME + CM :P
SuperTuxKart is a free kart racing game. The SuperTuxKart homepage can be found at <https://supertuxkart.net/>.

## Installing
Grab the latest binary for your system [here](https://github.com/Nomagno/stk-code/releases/latest)

## Building from source

Building instructions can be found in [`INSTALL.md`](/INSTALL.md)

To compile SuperTuxKart TME, you will need to install custom assets. They are found at <https://github.com/Nomagno/stk-assets>.

## A summarized version of most info below can be found in a new "*Tyres*" tab in the in-game help menu.

![STK Tyre Mod Edition](/tme_banner.png)

## License
The software is released under the GNU General Public License (GPL) which can be found in the file [`COPYING`](/COPYING) in the same directory as this file.

---
## What is SuperTuxKart Tyre Mod Edition (TME)?

STK TME is a SuperTuxKart mod by Nomagno that improves event hosting and introduces some limited race simulation elements, namely arcade versions of tyre degradation and fuel systems. Its overall aim is to be as configurable as possible, and to improve long (>>>>6 laps) races and provide more opportunities for tournament formats and friendly competitions. It is not completely concerned with casual players in its default configuration, though it makes sure not to make many casual-unfriendly choices, and they can be disabled easily enough through configuration files or the in game UI (UI is a work-in-progress but much improved over past versions).

It incorporates [kimden's code](https://github.com/kimden/stk-code) that greatly enhances server hosting and online play capabilities. It also takes some features from [the STK 2.0 Alpha](https://github.com/SuperTuxKart/stk-code/tree/BalanceSTK2).

Currently, TME is a hard fork. It stopped tracking any other STK fork months ago, and vows to specifically retain compatibility with content made for STK 1.0 all the way through STK 1.5.

# Original features:

## Base features
- (Experimental but works well for almost all tested cases): the server can change the `powerup.xml` (item probability distribution) and `kart_characteristics.xml` (kart physics and such) files, and the clients do not need to update them, they will be synced over the network at the start of each race. This immensely simplifies event hosting.
- Almost every single kart parameter you can think of has been un-hardcoded and made configurable through `kart_characteristics.xml`. If you have a need/wish for a *specific* sort of custom kart physics, or a *specific* sort of competition format, and it is at the moment impossible, I encourage you to open an issue, as I attempt to provide maximal coverage for users (as long as it feasible).
- The tyres: the main tyres are:
   - Soft: Starts out VERY fast, but also degrades very fast, *specially* by high-speed turning (in Hacienda, one of the least demanding tracks on tyre degradation, it can be useful for around 5 laps only if you don't manage it)
   - Medium: Starts out at the kart's normal speed, and degrades at a normal rate.
   - Hard: Starts out at a lowish speed, gets slightly faster as it warms up, and then degrades very slowly.
   - CHEAT: It does degrade but it has no effect in the kart's speed, meaning it effectively is the same as disabling tyre degradation.
   - New tyre types can be easily created in `kart_characteristics.xml`.
- Tracks that support pit-stops are showed with priority in the track list, and have a clearly visible wheel icon on their track thumbnail to distinguish them.
- The tyre overlay database (`data/track_overlay_database.xml`) ensures that a large number of popular tracks support pit-stops, even those that they don't come packaged with the game. Additionally, TME packages all standard STK 1.0 tracks, plus modified versions of ~20 popular FOSS add-on tracks. All of them also support pit stops.
- UI: the tyre compound name (or PIT if pitting) is shown on the leaderboard on the top left, inside the kart icon. The tyre life in bar form and the fuel are shown in the bottom right in. These bars/meters will show. The fuel bar becomes a 'fuel selection' bar to choose how much to refill during fuel pit stops.
- The default configuration is meant to enhance fairness: items are the same for all positions, and no items that affect players far away exist, only boosts, shield and manually aimed projectiles.
- Two new (optional) kart classes: Hyperkart (prototype), and Monoposto (single-seater). They are slower than the default classes, and are balanced to race with racing simulator-style physics while remaining fun in all existing STK tracks. They reside in the `FrankfurtEndurance` kart category.
- Initial tyre compounds (the ones you have when you start the race) are configured by selecting them in the kart selection screen. Online, it's accessed by double clicking your username, same way as you access the handicap level.
- Fuel can be configured offline from the race setup menu, and online from the wrench icon in the server lobby. Initial fuel can not be configured on a per-player basis yet.
- Revised handicap: it works more like an engine limiting system, where each handicap step reduces base topspeed, acceleration and braking power by 0.5% from no handicap. There are 48 handicap levels, from 0% to 24% penalty in 0.5% steps. As a quick reference, in tracks with a laptime of 50 seconds each handicap step might result in 2-4 tenths of a second lost per lap, though experienced players might be able to partially compensate for small handicap levels as it ever so slightly improves ease of cornering.
- Backplunger patch: In STK 2.0 and 1.0, the plunger item, when shot backwards, blocks the hit player's viewport. This sucks. It has been changed to instead remove the items of the hit player. It hence changes from an annoying item to a disarment item if the player behind takes too long to aim their cake or bowl.
- Full online support. Obviously, though, the server must be compiled from this codebase as well.
- Global powerups: can be configured before the race. It will show the item contained inside each item box as an icon floating above it, instead of it being a surprise, so that the player can choose which they like best.
- Tyre allocation: During a race, the current tyre allocation will be showed as a table of compounds `S M H` and numbers `XXX XXX XXX`, above the tyre life and fuel bars. It can be configured before the race. The allocation has the following meaning for each compound: `-1` is the same as `INF`, and it means that any amount of tyres can be used. `0` means that no tyres of that type can be used anymore for the remainder of the race. `X` where X is a number greater than 0 means that the tyre can be used X more times. When you use a tyre changer, the allocation of the corresponding compound will be reduced by 1. If you try to pit for a new tyre of a specific compound when you have none left, you will receive a 50% tyre life tyre, which is basically useless, so be careful! Can also be configured by the server host with the wrench icon, along with the fuel settings.
- Tyre changers: they will, when passed over, change your tyres to a fresh set. They have the model of a tyre stack. They can be added in the same place where the items and nitro are in each track's `scene.xml`, in the form `<tyre-changer compound="1" stop-time="30" id="tc0" x="234.35" y="44.3" z="24.5" h="20">` where the parameters configure respectively the compound ID (see `kart_characteristics.xml`, by default SOFT=2 MEDIUM=3 HARD=4, and FUEL=123), the pit stop time penalty applied to the kart, the unique object id (must be unique and different from the other nitro and items!), the xyz coordinates and the height from the ground (I think!). You can replace existing items to place them, or instead press the pause button (whenever the pause button is pressed, a copy-pasteable tyre changer at the current position will be logged to the terminal).
- Fuel pumps: tyre stacks that say FUEL above them ( in the scene.xml, ID 123, which is specially reserved to indicate a fuel pump). When going over a fuel pump, a fixed time penalty will be applied. During this time penalty, in the UI your fuel will instead be replaced by the fuel you wish to put into the kart. You can hold the nitro button to increase this amount, though careful you don't run out of time to input your desired number! The amount of liters will be put into the car at a fixed pump speed, so will still be slow until the tank is full!
- Fuel can be enabled/disabled in the race setup menu along with allocation. Heavier classes have more fuel tank capacity. With fuel mode on WEIGHTLESS, fuel doesn't impact performance. With fuel mode on "ON", a full fuel tank slows the kart and increases degradation.
- Gum boost: driving over a popped bubblegum on the ground while using a bubblegum shield now collects it, and gives a minor shield duration and speed boost.
- There are no plans for temperature or grip simulation at the moment, but it wouldn't be particularly hard to add, just impractical for the wide variety of STK tracks and its physics.

## Dynamic race rules (Item Policy)

- Item policy is a way of specifying rules that changed over the course of the race.
- Offline, before clicking on the button to start the race, there is a spinner that allows you to select the current item policy file. You can also create a new one and edit it. The GUI is decently intuitive, just know that the first section MUST start at lap 0.
- In servers, it can be changed from a stored config with `/itempolicypreset <name of config>` when in admin mode (activate with `/power tyre`).
- Major feature list (there's a few more not described here):
    - Every single one of these described features can change over the course of the race. A race is divided into 'sections', each lasting from the section's lap number to the start of the next section. Example: `section 0 starts at lap 0, section 1 starts at lap 5. So section 0 lasts from lap 0 to lap 4, and section 1 lasts from lap 5 to the end of the race`.
    - Ghost mode (if at the start of the race, all players start from the same grid slot): when active, players can not interact with each other physically nor hit each other. They can still collect items, but any items they collect will not despawn (so the person in front doesn't 'steal' nitro bottles from the person behind).
    - Give items to players each new lap with a bunch of different options: either a fixed amount or variable, from a pool of items with probability weights or just one specific item, with caps to prevent stockpiling or not...
    - Forbid specific types of collectibles from spawning: item boxes, nitro, and banana obstacles.
    - Change the probability distribution of item boxes.
    - Refill the nitro meter every lap.
    - Forbid players on different laps from throwing items at each other.
    - Changing how long pit stops take.
    - Forbidding specific inputs (drifting, braking, using nitro, looking backwards...) completely.
    - Changing how fast tyres degrades and fuel is consumed.
    - 'Virtual Pace Car' system: can be used to bunch up the cars together at a specific lap, and also to release the cars at specific intervals once they're bunched up to create artificial gap (rolling start with e.g. 3 sec gaps), or released at the same time (to create close restarts, nascar-style).

## Improvements over STK 1.0 taken from the 2.0 alpha

- New item: Mini-wish. It gives a choice between a weaker version of the cake, zipper and bubblegum powerups
- New item: Electro-shield. It gives a multiplication to the engine force, gives a slight top speed boost, does not shield from bubblegums on the floor, but otherwise acts as a perfect shield.
- Bubblegum. The bubblegum shield now doesn't stack infinitely when deployed on top of an existing bubblegum shield, instead it only adds half its duration
- New "feature": Boosts make the slowdown from going offtrack be reduced, allowing players to freestyle their own cuts. **THIS WAS REMOVED FOR TME BECAUSE IT'S A TERRIBLE FEATURE**
- New feature: Purple skids
- Gradual braking: Basically, now braking will be applied, even with a keyboard, gradually instead of as an on/off deceleration force.
- Bug fix: This seemingly small issue is actually a huge deal: Item boosts and boostpads do not override each other now. Why: In SuperTuxKart 1.0 you can't use item boosts on tracks that have big, long-lasting ground boostpad sections, or you will override the boost. With the proliferation of track ports from racing simulators such as STK's Italian, open-source older brother TORCS/Speed Dreams. These tracks use boost pads heavily.
- Karts now do not rescue on top of each other.

# For developers

- Tyre compounds, configurable from `kart_characteristics.xml`. Each compound has several parameters:
   - Traction tyre life: degrades by two factors, the current speed being a minor factor and the current center of mass (or `centerofmassY = mass*acceleration`) being a major factor. The acceleration used is the *smallest* acceleration from the current time instant to the latest past 6 speed snapshots taken 0.3s apart. In spite of this, changing speed passively by using too many boosts without spacing them out adequately, or braking too hard, can still affect the degradation noticeably. Going offroad and braking multiply the degradation rate of the tyre life by a configurable factor. Additionally bumping into track obstacles reduces the tyre life by a fixed percentage as a penalty for wall riding.
   - Turning tyre life: degrades faster based onthe the center of mass left-right of the kart, (`centerofmass X = mass*(speed^2)/turnRadius`). Multiplied by a configurable factor when skidding. If the track bumping penalty is enabled, it will also be reduced by the same percentage as the traction life.
   - Transfer: turning limiting transfer is how much the traction tyre life is degraded for each 1% of the turning degradation, when `turningLife < tractionLife`. The regular transfer is the same, but for when `turningLife > tractionLife`. Same for the traction limiting and turning transfers.
   - Topspeed, traction, and turning response curves: each compound can react differently at each percentage of its tyre life by reducing/increasing the kart's traction and topspeed (both use the same traction tyre life) and reducing/increasing how much the kart turns. These curves can be seen as functions affecting the kart stats dynamically `finalStat(Percentage, baseStat) = baseStat*curve(Percentage)` or `finalStat(Percentage, baseStat) = baseStat - curve(Percentage)`. It can be configured if the numbers in these curves will be substracted from the base stat, or multiplied by the base stat.
