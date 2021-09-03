# 21380 IMU tracking firmware

## Installation

Since this project is based on [Zephyr RTOS](https://github.com/zephyrproject-rtos/zephyr),
the Zephyr RTOS must be used alongside its meta build system
'[west](https://github.com/zephyrproject-rtos/west)'. While this seems like a
pain at first, it will save a lot of effort for both you and me.

Installing west is as easy as running:

```
pip3 install west
```

Then, west must be used to create a workspace this project can be cloned into.
First, create a workspace for the west/zephyr project files, it can be called
whatever you need; however, in this example I'm going to call it `west_workspace`.

Inside of this `west_workspace` folder, run the following two commands:

```
west init
```

Followed by:

```
west update
```

At this point, all of the zephyr and west files will be downloaded into the
workspace. Next, clone this repository inside of this workspace. The directory
tree should look something like so:

```
west_workspace/
│
├── 21380_IMU/           # .git/
│   ├── CMakeLists.txt
│   ├── prj.conf
│   ├── src/
│   │   └── main.c
│   └── west.yml
│
├── modules/
│   └── lib/
│       └── tinycbor/    # .git/ project from either the main manifest or some import.
│
├── zephyr/              # .git/ project
│   └── west.yml         # This can be partially imported with lower precedence or ignored.
│                        # Only the 'manifest-rev' version can be imported.
│
└── tools/
```
