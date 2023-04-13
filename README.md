[![Stand With Ukraine](https://raw.githubusercontent.com/vshymanskyy/StandWithUkraine/main/banner-direct-single.svg)](https://stand-with-ukraine.pp.ua)

# SoulWorker Plugin Loader

## What the...

That's what we're all here for. Dll library for loading plugins into this wonderful game...

## How to

- Unzip the latest available release version archive (not the debug one, if you are not a developer) from the [release](../../releases) page into your game folder. Replace the files if necessary
- To use the loader, you need some kind of launcher. If you don't have one, you can use [this](https://github.com/SoulWorkerResearch/swp-launcher)
- Now you can run the game using steam

## For Developers

- [vcpkg](https://vcpkg.io/) is needed for dependency management. If you don't want to fuck with it (GLHF), you can install dependencies yourself. They are listed in the `vcpkg.json` file under the `dependencies` property

- After clone do `git submodule update --init --recursive` for install submodules

- To build this project I use [![image](https://img.shields.io/badge/Visual_Studio-5C2D91?style=flat-square&logo=visual%20studio&logoColor=white)](https://visualstudio.microsoft.com/vs/preview/) version 17.6

- SDK for plugin development can be found [here](https://github.com/SoulWorkerResearch/swp-sdk)

### Host key verification failed

- [check this](https://github.blog/2023-03-23-we-updated-our-rsa-ssh-host-key/)

- If the error still occurs, try running the command through git bash, it is usually installed with `Git for Windows'. Or run the command in the WSL

- Also, submodules installed via SSH auth

## [Well cum 👌 to discord](http://discord.gg/SequFJP)

[![Discord](https://img.shields.io/discord/606442027873206292?style=flat-square)](http://discord.gg/SequFJP)
