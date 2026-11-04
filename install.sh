#!/usr/bin/env bash
# Yangi oʻzbek alifbosi klaviaturasi — Uzbek (Alfavit, AltGr) — oʻrnatish (sudo kerak emas).
#   ./install.sh          oʻrnatish
#   ./install.sh remove   oʻchirish
set -euo pipefail
DIR="$(cd "$(dirname "$0")" && pwd)"
XKB="${XDG_CONFIG_HOME:-$HOME/.config}/xkb"
SRC="('xkb', 'uz+alfavit')"

if [[ "${1:-}" == "remove" ]]; then
    rm -f "$XKB/symbols/uz" "$XKB/rules/evdev.xml" "$XKB/COPYING.xkeyboard-config"
    if command -v gsettings >/dev/null; then
        cur=$(gsettings get org.gnome.desktop.input-sources sources)
        new=$(sed -E "s/, *\('xkb', *'uz\+alfavit'\)|\('xkb', *'uz\+alfavit'\), *//g" <<<"$cur")
        gsettings set org.gnome.desktop.input-sources sources "$new"
    fi
    echo "Oʻchirildi. Tizimdan chiqib qayta kiring."
    exit 0
fi

mkdir -p "$XKB/symbols" "$XKB/rules"
for f in symbols/uz rules/evdev.xml COPYING.xkeyboard-config; do
    if [[ -e "$XKB/$f" ]] && ! cmp -s "$DIR/xkb/$f" "$XKB/$f"; then
        cp "$XKB/$f" "$XKB/$f.bak"; echo "Eski fayl saqlandi: $XKB/$f.bak"
    fi
    install -m644 "$DIR/xkb/$f" "$XKB/$f"
done
echo "Fayllar oʻrnatildi: $XKB"

if command -v gsettings >/dev/null; then
    cur=$(gsettings get org.gnome.desktop.input-sources sources)
    if [[ "$cur" != *"'uz+alfavit'"* ]]; then
        if [[ "$cur" == "@a(ss) []" || "$cur" == "[]" ]]; then new="[('xkb', 'us'), $SRC]"
        else new="${cur%]}, $SRC]"; fi
        gsettings set org.gnome.desktop.input-sources sources "$new"
    fi
    echo "GNOME kiritish manbalari: $(gsettings get org.gnome.desktop.input-sources sources)"
fi

echo "Tayyor. Tizimdan chiqib qayta kiring, soʻng Super+Space bilan 'Uzbek (Alfavit, AltGr)' ni tanlang."
echo "Tekshirish: python3 \"$DIR/xkbtest.py\""
