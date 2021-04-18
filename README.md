# UT4X-KickIdlers
Automatically kicks idlers for online Unreal Tournament games.



## Information

### Description

UT4X Kick Idlers mutator is a server side mutator (or plugin) that will automatically kick idle players.

The vanilla auto-kick feature in UT4 only kick during map change.

This mutator allows players to be kicked during match and have some extra settings to tune it.

Since it's a server side mutator, player does not need to download the mutator and no redirect is needed.

### Author

Thomas 'WinterIsComing' P.

### Version History

- 1.1 - 18/04/2021 
  - Fixed elimination gamemode auto-kick while player waiting to respawn
  - Fixed bad config file. Changed to UT4X.ini
  - Added admin only command "mutate enablekickidlers" and "mutate disablekickidlers" to enable/disable plugin
  - Added a few extra logs
- 1.0 - 17/04/2021 - First version

### License

See licence file

## Installation

- Clone repository or download repository from https://github.com/xtremexp/UT4XKickIdlers

- Move UT4XKickIdlers folder <Windows/Linux>Server/UnrealTournament/Plugins

- You should then have <Windows/Linux>Server/UnrealTournament/Plugins/UT4XKickIdler folder

  

## Enable Mutator

### For hubs

#### Globally

This will allow to run for any game either from a ruleset or a custom game

- Edit <Windows/Linux>Serverr\UnrealTournament\Saved\Config\Game.ini file
- Add or edit this section

```ini
[/Script/UnrealTournament.UTGameMode]
ConfigMutators[0]=UT4XKickIdlers
```

#### For a specific ruleset

In your ruleset, in GameOptions, add this:

`?mutator=UT4XKickIdlers`

E.G: "gameOptions": "?mutator=UT4XKickIdlers"

### For dedicated server

Add this in command line:

`?mutator=UT4XKickIdlers`

## Configuration

Default configuration of this mutator can be changed modifying in <Windows/Linux>Server\UnrealTournament\Saved\Config\<Windows/Linux>Server\UT4X.ini file. (this file is automatically created at first start).

If file is not present you can create it with this content (see /Config/DefaultUT4X.ini file in this plugin folder) :

```ini
[/Script/UT4XKickIdlers.UT4XKickIdlers]
KickIdlersEnabled=True
MaxIdlingDuration=90
MinPlayersForKickIdlers=2
KickIdlersInPrivateGamesEnabled=False
```

| Property                        | Default Value | Description                                                  |
| ------------------------------- | ------------- | ------------------------------------------------------------ |
| KickIdlersEnabled               | True          | Activate auto kick idlers.                                   |
| MaxIdlingDuration               | 90            | Max duration in seconds until player is kicked if idling.    |
| MinPlayersForKickIdlers         | 2             | Minimum players                                              |
| KickIdlersInPrivateGamesEnabled | False         | Activate kick idlers if it's a private game (passworded or lan game) |

These values can modified modifying UT4X.ini file as seen before.

## Commands

As a logged admin (command: "rconauth mypassword"), you can enable or disable mutator:

Enable KickIdlers:

`mutate enablekickidlers`

Disable KickIdlers:

`mutate disablekickidlers`



## Build UT4XKickIdlers

### Requirements

- Windows OS
- Microsoft Visual Studio 2015 (Community Edition (free) or paid one)
- Git client (either Git for windows or TortoiseGit)

### Procedure

- clone Unreal Tournament repository from https://github.com/EpicGames/UnrealTournament
- run "Setup.bat" in root UnrealTournament folder (might take a long time since it downloads at UT4 content (textures, maps, ...))
- clone UT4XKickIdlers repository to UnrealTournament\UnrealTournament\Plugins folder
- run "GenerateProjectFiles.bat -2015" in root UnrealTournament folder
- open "UE4.sln" solution file within Visual Studio 2015
- Build plugin (first build can quite a long time since it's building UT4 as well)
  - For Windows select Shipping Server / Win64 / UE4 configuration then "Build -> Build Unreal Tournament" in menu
  - For Linux select Shipping Server / Linux / UE4 configuration then "Build -> Build Unreal Tournament" in menu
- Compiled plugin binaries will then be available within UnrealTournament\UnrealTournament\Plugins\UT4XKickIdlers\Binaries folder

