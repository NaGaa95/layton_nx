Run `CC=gcc python3 tests/run_apk_layout_tests.py` with a 64-bit host C compiler.

The test compiles the actual layout selector from `main.c`. It checks the
Japanese and international synchronization addresses, missing and unknown
`movieTex` exports, and different runtime bases. The Japanese layout uses
`movieTex` offset `0x356bbc`, with turnstiles at `0x356af8` and `0x356b20`;
the international offsets remain `0x356b38` and `0x356b60`.
