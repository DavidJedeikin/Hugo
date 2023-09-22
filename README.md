# Hugo

## Setup 

### Steps for enabled clangd extension 
1. Disable the cpp intelliSense engine

The two cannot co-exists, but PlatformIo depends on the extension for dependency management. You can include the following in your settings.json
```
  "C_Cpp.intelliSenseEngine": "disabled"
```

2. Ensure you have the Python compile commands scripts located in: `scripts`

3. Ensure the scripts are enabled in `platformio.ini`, something like: 
```
extra_scripts = hook_compile_commands.py, pre:generateCompileCommands.py
```