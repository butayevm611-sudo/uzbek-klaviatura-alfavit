# Yangi oʻzbek alifbosi klaviaturasi — Uzbek (Alfavit, AltGr)

Ubuntu / GNOME (Wayland va X11) uchun. sudo kerak emas.

**Mualliflar:** Butayev Muhammad, Abdullajonov Azimjon

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
GitHub'dan yuklab olib:

    git clone https://github.com/butayevm611-sudo/uzbek-klaviatura-alfavit.git
    cd uzbek-klaviatura-alfavit
    ./install.sh

Yoki git'siz: sahifadagi **Code → Download ZIP** tugmasini bosing, soʻng:

    cd ~/Downloads
    unzip uzbek-klaviatura-alfavit-main.zip
    cd uzbek-klaviatura-alfavit-main
    ./install.sh
Soʻng tizimdan chiqib qayta kiring va Super+Space bilan tanlang.

## Tekshirish
C dasturi (gcc kerak):

    make
    ./xkbtest                   # AltGr birikmalari jadvali
    ./xkbtest "Çöl ğişt maʼno"  # har bir harf qaysi tugma bilan yoziladi

Yoki Python bilan:

    python3 xkbtest.py

## Oʻchirish
    ./install.sh remove

## Fayllar
- `xkb/symbols/uz` — klaviatura kodi (tizimdagi uz fayli + `alfavit` varianti)
- `xkb/rules/evdev.xml` — GNOME sozlamalari uchun nom
- `xkb/COPYING.xkeyboard-config` — xkeyboard-config litsenziyasi
- `install.sh` — oʻrnatish / oʻchirish
- `xkbtest.c`, `Makefile` — tugmalarni sinash dasturi (C, libxkbcommon)
- `xkbtest.py` — xuddi shu sinov Python’da
- `Uzbek_klaviatura.docx` — toʻliq qoʻllanma (kod, tushuntirish, skrinshotlar)
