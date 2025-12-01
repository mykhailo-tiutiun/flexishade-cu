if [ -z "$ESP_IDF_VERSION"]; then
    . ~/.local/share/esp-idf/export.sh
fi

idf.py "$*"
