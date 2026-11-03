#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static struct { void *load_base, *load_virtbase; } game_mod;
static uintptr_t symbol_address;
static uintptr_t registered[2];

static uintptr_t so_try_find_addr_rx(void *module, const char *name) {
  assert(module == &game_mod && !strcmp(name, "movieTex"));
  return symbol_address;
}
static void register_turnstile_mutexes(void *a, void *b) {
  registered[0] = (uintptr_t)a;
  registered[1] = (uintptr_t)b;
}
static int debugPrintf(const char *format, ...) { (void)format; return 0; }

#include "apk_layout.inc"

int main(void) {
  const struct { uintptr_t movie, a, b; } cases[] = {
    {0x356bbc, 0x356af8, 0x356b20},
    {0x356bfc, 0x356b38, 0x356b60},
    {0,        0x356b38, 0x356b60},
    {0x356bbb, 0x356b38, 0x356b60},
    {0x356bbd, 0x356b38, 0x356b60},
    {0x357bbc, 0x356b38, 0x356b60},
  };
  const uintptr_t bases[] = {0x10000000, 0x7100000000};
  for (unsigned base = 0; base < sizeof(bases) / sizeof(*bases); ++base) {
    game_mod.load_base = (void *)0x8000000; /* writable staging alias differs */
    game_mod.load_virtbase = (void *)bases[base];
    for (unsigned i = 0; i < sizeof(cases) / sizeof(*cases); ++i) {
      symbol_address = cases[i].movie ? bases[base] + cases[i].movie : 0;
      register_game_turnstiles();
      assert(registered[0] == bases[base] + cases[i].a);
      assert(registered[1] == bases[base] + cases[i].b);
    }
  }
  puts("PASS Japanese/international APK layouts, missing/unknown markers and relocated runtime addresses");
  return 0;
}
