/*
 * xkbtest.c — Uzbek (Alfavit, AltGr) klaviaturasini sinash dasturi.
 *
 * Dastur GNOME ishlatadigan libxkbcommon kutubxonasi orqali oʻrnatilgan
 * uz(alfavit) klaviaturasini yuklaydi va tugma bosishni taqlid qiladi.
 *
 *   ./xkbtest              — AltGr birikmalari jadvali
 *   ./xkbtest "Çöl maʼno"  — matndagi har bir harf qaysi tugmalar bilan
 *                            yozilishini topadi
 *
 * Kompilyatsiya:
 *   make
 *   yoki: gcc -O2 -Wall -o xkbtest xkbtest.c -l:libxkbcommon.so.0
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/*
 * libxkbcommon funksiyalari. Bu eʼlonlar <xkbcommon/xkbcommon.h> dagilar
 * bilan bir xil; ular shu yerda yozilgan, shunda dastur libxkbcommon-dev
 * paketisiz (sudo’siz) ham kompilyatsiya qilinadi.
 */
struct xkb_context;
struct xkb_keymap;
struct xkb_state;
struct xkb_rule_names {
    const char *rules, *model, *layout, *variant, *options;
};
struct xkb_context *xkb_context_new(int flags);
struct xkb_keymap *xkb_keymap_new_from_names(struct xkb_context *ctx,
                                             const struct xkb_rule_names *names,
                                             int flags);
struct xkb_state *xkb_state_new(struct xkb_keymap *keymap);
int xkb_state_update_key(struct xkb_state *state, uint32_t key, int direction);
int xkb_state_key_get_utf8(struct xkb_state *state, uint32_t key,
                           char *buffer, size_t size);
const char *xkb_keymap_layout_get_name(struct xkb_keymap *keymap, uint32_t idx);
void xkb_state_unref(struct xkb_state *state);
void xkb_keymap_unref(struct xkb_keymap *keymap);
void xkb_context_unref(struct xkb_context *ctx);

#define KEY_DOWN     1
#define EVDEV_OFFSET 8          /* evdev tugma kodi + 8 = XKB tugma kodi */
#define KEY_SHIFT    42         /* chap Shift (evdev) */
#define KEY_ALTGR    100        /* oʻng Alt (evdev) */

/* Modifikatorlar: 0 = yoʻq, 1 = Shift, 2 = AltGr, 3 = AltGr+Shift */
static const char *mod_name[] = {
    "", "Shift + ", "AltGr + ", "AltGr + Shift + "
};

static struct xkb_keymap *keymap;

/* Berilgan modifikatorlar bosilgan holda tugma qaysi belgini chiqaradi. */
static void press(int mods, uint32_t evdev_key, char *out, size_t size)
{
    struct xkb_state *st = xkb_state_new(keymap);
    if (mods & 2)
        xkb_state_update_key(st, KEY_ALTGR + EVDEV_OFFSET, KEY_DOWN);
    if (mods & 1)
        xkb_state_update_key(st, KEY_SHIFT + EVDEV_OFFSET, KEY_DOWN);
    xkb_state_key_get_utf8(st, evdev_key + EVDEV_OFFSET, out, size);
    xkb_state_unref(st);
}

/* UTF-8 belgining Unicode raqami (U+XXXX). */
static unsigned codepoint(const char *s)
{
    const unsigned char *u = (const unsigned char *)s;
    if (u[0] < 0x80)
        return u[0];
    if ((u[0] & 0xE0) == 0xC0)
        return ((u[0] & 0x1F) << 6) | (u[1] & 0x3F);
    if ((u[0] & 0xF0) == 0xE0)
        return ((u[0] & 0x0F) << 12) | ((u[1] & 0x3F) << 6) | (u[2] & 0x3F);
    return ((u[0] & 0x07) << 18) | ((u[1] & 0x3F) << 12) |
           ((u[2] & 0x3F) << 6) | (u[3] & 0x3F);
}

/* UTF-8 belgi necha baytdan iborat (birinchi bayt boʻyicha). */
static int utf8_len(unsigned char c)
{
    return c < 0x80 ? 1 : (c & 0xE0) == 0xC0 ? 2 : (c & 0xF0) == 0xE0 ? 3 : 4;
}

/* Jadval: har bir yangi harf uchun tugma birikmasi va natija. */
static void print_table(void)
{
    static const struct { int mods; uint32_t key; const char *label; } t[] = {
        { 0, 24, "o" }, { 2, 24, "o" }, { 3, 24, "o" },
        { 2, 34, "g" }, { 3, 34, "g" },
        { 2, 31, "s" }, { 3, 31, "s" },
        { 2, 46, "c" }, { 3, 46, "c" },
        { 0, 40, "'" }, { 2, 40, "'" },
    };
    char buf[8], keys[32];

    printf("%-22s%-8s%s\n", "Tugmalar", "Natija", "Unicode");
    for (size_t i = 0; i < sizeof t / sizeof t[0]; i++) {
        press(t[i].mods, t[i].key, buf, sizeof buf);
        snprintf(keys, sizeof keys, "%s%s", mod_name[t[i].mods], t[i].label);
        /* %-8s baytlarni sanaydi, UTF-8 harf esa 2 bayt boʻlishi mumkin,
           shuning uchun harfdan keyingi 7 ta boʻsh joy qoʻlda yozilgan */
        printf("%-22s%s       U+%04X\n", keys, buf, codepoint(buf));
    }
}

/* Teskari qidiruv: matndagi har bir harfni chiqaradigan tugmani topadi. */
static void how_to_type(const char *text)
{
    char buf[8], base[8];

    for (const char *p = text; *p; ) {
        size_t n = utf8_len((unsigned char)*p), rest = strlen(p);
        char ch[8] = { 0 };
        if (n > rest)                   /* notoʻgʻri UTF-8: satr oxiridan */
            n = rest;                   /* tashqariga chiqmaslik uchun */
        memcpy(ch, p, n);
        p += n;

        int found = 0;
        for (uint32_t key = 1; key < 128 && !found; key++)
            for (int mods = 0; mods < 4 && !found; mods++) {
                press(mods, key, buf, sizeof buf);
                if (strcmp(buf, ch) != 0)
                    continue;
                press(0, key, base, sizeof base);   /* tugmadagi asosiy belgi */
                if (base[0] >= 'a' && base[0] <= 'z')
                    base[0] -= 'a' - 'A';
                printf("%s  →  %s%s\n", ch[0] == ' ' ? "␣" : ch,
                       mod_name[mods], base[0] == ' ' ? "Space" : base);
                found = 1;
            }
        if (!found)
            printf("%s  →  (bu klaviaturada yoʻq)\n", ch);
    }
}

int main(int argc, char **argv)
{
    /* Klaviatura fayllari avval ~/.config/xkb, keyin tizimdan qidiriladi */
    struct xkb_context *ctx = xkb_context_new(0);
    struct xkb_rule_names names = { "evdev", "pc105", "uz", "alfavit", NULL };

    keymap = ctx ? xkb_keymap_new_from_names(ctx, &names, 0) : NULL;
    if (!keymap) {
        fprintf(stderr, "Xato: uz(alfavit) klaviaturasi topilmadi.\n"
                        "Avval ./install.sh ni ishga tushiring.\n");
        return 1;
    }
    printf("Klaviatura: %s\n", xkb_keymap_layout_get_name(keymap, 0));

    if (argc > 1)
        how_to_type(argv[1]);
    else
        print_table();

    xkb_keymap_unref(keymap);
    xkb_context_unref(ctx);
    return 0;
}
