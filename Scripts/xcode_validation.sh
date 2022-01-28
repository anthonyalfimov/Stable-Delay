if [ "${CONFIGURATION}" = "Release" ] && { [ "${TARGET_NAME}" = "${PRODUCT_NAME} - VST3" ] || [ "${TARGET_NAME}" = "${PRODUCT_NAME} - AU" ]; }
then
    echo "Running plugin validation.."

    PLUGINVAL_PATH="$SRCROOT"
    PLUGINVAL_BIN="${PLUGINVAL_PATH}/pluginval.app/Contents/MacOS/pluginval"

    if [ ! -e "${PLUGINVAL_PATH}/pluginval.app" ]
    then
        echo "Downloading pluginval to ${PLUGINVAL_PATH} .."
        # -s (--silent) and -S (--show-error)
        curl -sS -L "https://github.com/Tracktion/pluginval/releases/latest/download/pluginval_macOS.zip" -o "${PLUGINVAL_PATH}/pluginval.zip"
        unzip -q "${PLUGINVAL_PATH}/pluginval"
        rm "${PLUGINVAL_PATH}/pluginval.zip"
    else
        echo "Found pluginval at ${PLUGINVAL_PATH}"
    fi

    echo ""

    # Test that downloading and unzipping worked and pluginval binary can be found
    if [ -f "$PLUGINVAL_BIN" ]
    then
        ${PLUGINVAL_PATH}/pluginval.app/Contents/MacOS/pluginval --strictness-level 10 --validate "${INSTALL_PATH}/${PRODUCT_NAME}.${WRAPPER_EXTENSION}"
    else
        echo "ERROR: Failed to locate validator binary. Aborting validation"
    fi
else
    echo "Skipping plugin validation.."
fi
