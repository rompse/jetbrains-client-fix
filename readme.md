# JetBrains Gateway Client Fix

Injectable payload that allows modification of the JetBrains Gateway Client's .vmoptions

- Only supports Windows

## More .vmoptions

You can modify `writeFile` to write whichever .vmoptions you would like into the client

## Usage

Inject the compiled fix into gateway64.exe

###### Example using process hacker

![Process Hacker](https://i.imgur.com/GPB18Ch.png)

## Reason

When modifying the .vmoptions of the client it gets overwritten upon each startup<br/>
**JetBrains please fix**