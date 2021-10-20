#include <term/term.hpp>
#include <sys/msg.hpp>

static int cmd_start(const struct shell *shell, size_t argc, char **argv)
{
    printf("Staring capture...\n");

    sys::msg::dispatch(sys::msg::SENSOR_CAPTURE_START);

    return 0;
}

static int cmd_stop(const struct shell *shell, size_t argc, char **argv)
{
    printf("Stopping capture... \n");

    sys::msg::dispatch(sys::msg::SENSOR_CAPTURE_STOP);

    return 0;
}

static int cmd_time(const struct shell *shell, size_t argc, char **argv)
{
    printf("Sync time\n");
    sys::msg::dispatch(sys::msg::TIME_SYNC);

    return 0;
}

static int cmd_stream(const struct shell *shell, size_t argc, char **argv)
{
    printf("Data streaming toggled\n");
    sys::msg::dispatch(sys::msg::SENSOR_CAPTURE_STREAM);

    return 0;
}

SHELL_CMD_REGISTER(start,  NULL, "Start IMU Recording", cmd_start);
SHELL_CMD_REGISTER(stop,   NULL, "End IMU Recording",   cmd_stop);
SHELL_CMD_REGISTER(time,   NULL, "Set System Time",     cmd_time);
SHELL_CMD_REGISTER(stream, NULL, "Stream IMU data",     cmd_stream);
