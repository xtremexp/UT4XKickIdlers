# UT4X-KickIdlers
Automatically kicks idlers for online Unreal Tournament games.



## Information

### Description

UT4X Kick Idlers mutator is a server side mutator that will automatically kick idle players.

The vanilla auto-kick feature in UT4 only kick during map change.

This mutator allows players to be kicked during match and have some extra settings to tune it.

Since it's a server side mutator, player does not need to download the mutator and no redirect is needed.

### Author

Thomas 'WinterIsComing' P.

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

Default configuration of this mutator can be changed modifying in <Windows/Linux>Server\UnrealTournament\Saved\Config\Mod.ini file.

You have to add these lines if they are not present

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

These values can modified modifying Mod.ini file as seen before.

## Commands

As a logged admin (command: "rconauth mypassword"), you can enable or disabled mutator:

Enable KickIdlers:

`mutate enablekickidlers`

Disable KickIdlers:

`mutate disablekickidlers`