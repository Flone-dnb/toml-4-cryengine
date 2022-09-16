# TOML 4 CRYENGINE

![](nodes.png?raw=true)

This plugin adds new Flow Graph nodes and C++ functions for working with TOML files.

# Building

Generate CRYENGINE solution, select `Toml4Cryengine` target and compile it.

# How to use Flow Graph nodes

- Build the plugin (see previous section) or download it from the [Releases](https://github.com/Flone-dnb/toml-4-cryengine/releases) section.
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

# How to use C++ functions

- Copy directory `TomlManager` from `Code` directory to your project's `Code` directory.
- Regenerate CRYENGINE solution.
- You can now create `CTomlManager` object and use its functions.