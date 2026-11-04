# Oʻrnatilgan uz(alfavit) klaviaturasini libxkbcommon orqali sinaydi:
# tugma bosishni taqlid qiladi va qaysi belgi chiqishini koʻrsatadi.
import ctypes, sys
x = ctypes.CDLL("libxkbcommon.so.0")
class Names(ctypes.Structure):
    _fields_ = [(n, ctypes.c_char_p) for n in ("rules","model","layout","variant","options")]
x.xkb_context_new.restype = ctypes.c_void_p
x.xkb_keymap_new_from_names.restype = ctypes.c_void_p
x.xkb_keymap_new_from_names.argtypes = [ctypes.c_void_p, ctypes.POINTER(Names), ctypes.c_int]
x.xkb_state_new.restype = ctypes.c_void_p
x.xkb_state_new.argtypes = [ctypes.c_void_p]
x.xkb_state_update_key.argtypes = [ctypes.c_void_p, ctypes.c_uint32, ctypes.c_int]
x.xkb_state_key_get_utf8.argtypes = [ctypes.c_void_p, ctypes.c_uint32, ctypes.c_char_p, ctypes.c_size_t]
x.xkb_keymap_layout_get_name.restype = ctypes.c_char_p
x.xkb_keymap_layout_get_name.argtypes = [ctypes.c_void_p, ctypes.c_uint32]

ctx = x.xkb_context_new(0)  # ~/.config/xkb avtomatik qidiriladi
km = x.xkb_keymap_new_from_names(ctx, ctypes.byref(Names(b"evdev", b"pc105", b"uz", b"alfavit", None)), 0)
if not km: sys.exit("keymap kompilyatsiya qilinmadi")
print("Klaviatura:", x.xkb_keymap_layout_get_name(km, 0).decode())
EV = 8  # evdev -> xkb kod
K = dict(o=24, g=34, s=31, c=46, apos=40, a=30, shift=42, altgr=100)
def press(keys):
    st = x.xkb_state_new(km)
    *mods, key = keys
    for m in mods: x.xkb_state_update_key(st, K[m]+EV, 1)
    buf = ctypes.create_string_buffer(16)
    x.xkb_state_key_get_utf8(st, K[key]+EV, buf, 16)
    return buf.value.decode()
label = dict(altgr="AltGr", shift="Shift", apos="'")
print(f"{'Tugmalar':<22}{'Natija':<8}Unicode")
for combo in [["o"],["altgr","o"],["altgr","shift","o"],["altgr","g"],["altgr","shift","g"],
              ["altgr","s"],["altgr","shift","s"],["altgr","c"],["altgr","shift","c"],["apos"],["altgr","apos"]]:
    ch = press(combo)
    name = " + ".join(label.get(k,k) for k in combo)
    print(f"{name:<22}{ch:<8}U+{ord(ch):04X}")
