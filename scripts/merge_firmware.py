Import("env")

APP_BIN = "$BUILD_DIR/${PROGNAME}.bin"
MERGED_BIN = "$BUILD_DIR/${PROGNAME}_merged.bin"

board_config = env.BoardConfig()


def get_flash_settings():
    flash_size = board_config.get("upload.flash_size", "4MB")

    flash_freq = board_config.get("build.f_flash", "40000000L")
    flash_freq = str(flash_freq).replace("L", "")
    flash_freq = str(int(int(flash_freq) / 1000000)) + "m"

    flash_mode = board_config.get("build.flash_mode", "dio")
    memory_type = board_config.get("build.arduino.memory_type", "qio_qspi")

    # wichtige Anpassungen (ESP32 Besonderheiten!)
    if flash_mode in ["qio", "qout"]:
        flash_mode = "dio"

    if memory_type in ["opi_opi", "opi_qspi"]:
        flash_mode = "dout"

    return flash_mode, flash_freq, flash_size


def merge_bin(source, target, env):
    flash_mode, flash_freq, flash_size = get_flash_settings()

    chip = board_config.get("build.mcu", "esp32")

    flash_images = env.Flatten(env.get("FLASH_EXTRA_IMAGES", [])) + [
        "$ESP32_APP_OFFSET",
        APP_BIN,
    ]

    cmd = [
        "$PYTHONEXE",
        "$OBJCOPY",  # -> das ist intern esptool.py
        "--chip",
        chip,
        "merge-bin",
        "-o",
        MERGED_BIN,
        "--flash-mode",
        flash_mode,
        "--flash-freq",
        flash_freq,
        "--flash-size",
        flash_size,
    ] + flash_images

    print("Merging firmware with:")
    print(" ".join(cmd))

    env.Execute(" ".join(cmd))


# Nach dem Build ausführen
env.AddPostAction(APP_BIN, merge_bin)


#Upload so patchen, dass merged.bin bei 0x0 geflasht wird
# env.Replace(
#     UPLOADERFLAGS=[
#         f
#         for f in env.get("UPLOADERFLAGS")
#         if f not in env.Flatten(env.get("FLASH_EXTRA_IMAGES"))
#     ]
#     + ["0x0", MERGED_BIN],
#     UPLOADCMD='"$PYTHONEXE" "$UPLOADER" $UPLOADERFLAGS',
# )