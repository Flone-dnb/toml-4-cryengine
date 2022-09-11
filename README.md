# TOML 4 CRYENGINE

TODO: add screenshot

This plugin adds new Flow Graph nodes for working with TOML files.

# Building

Generate CRYENGINE solution, select `Toml4Cryengine` target and compile it.

# How to use

- Build plugin (see previous section).
- Copy `Toml4Cryengine.dll` from `bin/win_x64` directory to you project's `bin/win_x64` directory.
- Modify your project's `Game.cryproject` file, add a new entry in `"plugins"` section:

```
{ "guid": "", "type": "EType::Native", "path": "bin/win_x64/Toml4Cryengine.dll" },
```

so your section will look like this:

```
"plugins": [
	{ ... },
	{ "guid": "", "type": "EType::Native", "path": "bin/win_x64/Toml4Cryengine.dll" },
	{ ... }
]
```