# 21380_IMU
IMU Module Operating System for Team 21380 @ RIT MSD


# Comands to build/flash/debug

* make (compiles code)
* make flash (compiles if needed and uploads to the board (talks to the jlink all by itself))
## debugging
* make debug-server (starts the debug server for the chip)
* make debug _(in another terminal)_ (connects to the debug server and allows debugging)
    * use q to quit
    * CTRL+c to stop the CPU
    * c to continue operation
