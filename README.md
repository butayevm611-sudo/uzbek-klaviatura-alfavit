# Yangi oʻzbek alifbosi klaviaturasi — Uzbek (Alfavit, AltGr)

Ubuntu / GNOME (Wayland va X11) uchun. sudo kerak emas.

| Tugmalar          | Harf |
|-------------------|------|
| AltGr + o         | ö    |
| AltGr + Shift + o | Ö    |
| AltGr + g         | ğ  (Shift bilan Ğ) |
| AltGr + s         | ş  (Shift bilan Ş) |
| AltGr + c         | ç  (Shift bilan Ç) |
| AltGr + '         | ʼ  (tutuq belgisi) |

AltGr = oʻng Alt tugmasi.

## Oʻrnatish
    unzip uzbek-klaviatura-alfavit.zip
    cd uzbek-klaviatura-alfavit
    ./install.sh
Soʻng tizimdan chiqib qayta kiring va Super+Space bilan tanlang.

## Tekshirish
    python3 xkbtest.py

## Oʻchirish
    ./install.sh remove

## Fayllar
- `xkb/symbols/uz` — klaviatura kodi (tizimdagi uz fayli + `alfavit` varianti)
- `xkb/rules/evdev.xml` — GNOME sozlamalari uchun nom
- `xkb/COPYING.xkeyboard-config` — xkeyboard-config litsenziyasi
- `install.sh` — oʻrnatish / oʻchirish
- `xkbtest.py` — tugmalarni sinash skripti
- `Uzbek_klaviatura.docx` — toʻliq qoʻllanma (kod, tushuntirish, skrinshotlar)
