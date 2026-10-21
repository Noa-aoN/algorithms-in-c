# algorithms-in-c

[![ci](https://github.com/Noa-aoN/algorithms-in-c/actions/workflows/ci.yml/badge.svg)](https://github.com/Noa-aoN/algorithms-in-c/actions/workflows/ci.yml)

定番のアルゴリズムとデータ構造を、組み込みで C を書くときの流儀で 1 テーマずつ実装したものです。
ヒープ確保をせず、ストレージは呼び出し側が渡し、実装の隣に必ず単体テストを置いています。

[English](#english)

## 何のためのリポジトリか

自分のための整理と演習の場です。知っているつもりのアルゴリズムをもう一度手で書き、
テストで確かめ、迷ったところを書き残しておきます。

書き方は普段の組み込みの流儀に揃えています。`malloc` をしない、再帰を深くしない、
バッファの大きさは呼び出し側が決める。そうしておけば、ここで書いたものをマイコンに
持っていってもそのまま使えます。兄弟リポジトリの [reversi-c](https://github.com/Noa-aoN/reversi-c) も同じ流儀です。

## 一覧

| 分野 | 項目 | 計算量 | 設計上のポイント |
|---|---|---|---|
| sorting | [insertion_sort](sorting/insertion_sort.c) | O(n^2) | 汎用、安定、一時領域なし |
| sorting | [merge_sort](sorting/merge_sort.c) | O(n log n)、作業領域 O(n) | ボトムアップで再帰なし、作業領域は呼び出し側が渡す |
| sorting | [quick_sort](sorting/quick_sort.c) | 平均 O(n log n) | Hoare 分割、中央値 3 点、小さい側だけ再帰してスタック O(log n) |
| sorting | [heap_sort](sorting/heap_sort.c) | O(n log n)、追加領域なし | 最悪でも O(n log n)、再帰なし |
| searching | [binary_search](searching/binary_search.c) | O(log n) | `lower_bound` / `upper_bound` / `find`、重複時は最左 |
| structures | [ring_buffer](structures/ring_buffer.c) | 1 操作 O(1) | 容量は 2 の冪、フリーランカウンタで満杯と空を区別 |
| structures | [stack](structures/stack.c) | 1 操作 O(1) | 固定容量、任意の要素サイズ、ストレージは呼び出し側 |
| structures | [queue](structures/queue.c) | 1 操作 O(1) | 循環配列、容量は任意 (head + count 方式)、任意の要素サイズ |
| structures | [union_find](structures/union_find.c) | ほぼ O(1) (償却) | 経路半減 + サイズによる併合、再帰なし、配列は呼び出し側 |
| bits | [crc32](bits/crc32.c) | O(n) | zlib 互換、逐次更新 API、テーブル版とビット版 |
| bits | [bit_tricks](bits/bit_tricks.c) | O(1) | popcount / ctz / clz / parity / reverse / bswap / next_pow2 を組み込み関数なしで |
| encoding | [cobs](encoding/cobs.c) | O(n) | 0x00 を区切りに使うためのバイトスタッフィング。容量と不正入力を検査 |
| math | [fixed_point](math/fixed_point.c) | 1 演算 O(1) | Q16.16、浮動小数点なし。飽和演算、最近接丸め、整数平方根 |
| math | [gcd](math/gcd.c) | O(log n) | 二進 GCD (除算なし)、オーバーフロー検査付き LCM、拡張ユークリッド、法逆元 |

予定: linked list、hash table、heap、BST、BFS / DFS / Dijkstra、KMP。

## 書き方の約束

- 1 テーマ = `name.h` + `name.c` + `name_test.c`。テーマ間で共有するコードはありません。
- `-std=c11 -Wall -Wextra -Wpedantic -Werror -Wshadow -Wconversion`。サードパーティコードなし。
- `malloc` を使いません。コンテナは呼び出し側からストレージを受け取り、容量を報告し、勝手に伸びません。
- 汎用のものは `qsort` / `bsearch` と同じ `(base, n, size, cmp)` の形にしています。
  マクロなしでどの要素型にも使えて、呼び出し側が標準関数と同じ感覚で読めるからです。
- 失敗は assert ではなく戻り値で返します (`false` や 0)。組み込みでは止まるより続ける方が良い場面が多いためです。

## ビルドとテスト

```sh
make test       # 全 *_test.c をビルドして実行
make asan       # 同じテストを AddressSanitizer + UBSan で
make cross      # arm-none-eabi-gcc で Cortex-M4 向けにコンパイルしてサイズを表示
make clean
```

CI は gcc / clang それぞれで通常ビルドと ASan/UBSan ビルドを回し、valgrind でも全テストを通します。
静的解析 (cppcheck と clang の scan-build) も毎回かけています。
「マイコンに持っていける」を言葉だけにしないために、Cortex-M4 (Thumb-2) 向けのビルドも CI で毎回通しています。
全 11 テーマ合わせてコードは約 2.5 KB です (`-Os`、CRC テーブルの 1 KB は RAM)。
テストはヘッダ 1 つの自作フレームワーク (`tests/minitest.h`) です。

各テーマの設計判断は [docs/spec.md](docs/spec.md) に残しています。

## ライセンス

MIT

---

## English

A personal workbook: classic algorithms and data structures, each written again by hand, pinned
down with a test, with notes on what was not obvious. The style is the embedded C I write day to
day (no heap allocation, caller-provided storage, bounded recursion), so anything here can go to a
microcontroller as is. Same conventions as the sibling [reversi-c](https://github.com/Noa-aoN/reversi-c).

### Index

| Area | Item | Complexity | Design note |
|---|---|---|---|
| sorting | [insertion_sort](sorting/insertion_sort.c) | O(n^2) | generic, stable, no scratch |
| sorting | [merge_sort](sorting/merge_sort.c) | O(n log n), O(n) scratch | bottom-up, no recursion, caller-provided scratch |
| sorting | [quick_sort](sorting/quick_sort.c) | O(n log n) average | Hoare partition, median of three, recurse on smaller side only |
| sorting | [heap_sort](sorting/heap_sort.c) | O(n log n), in place | worst case O(n log n), no recursion |
| searching | [binary_search](searching/binary_search.c) | O(log n) | `lower_bound` / `upper_bound` / `find`, leftmost on duplicates |
| structures | [ring_buffer](structures/ring_buffer.c) | O(1) per op | power-of-two capacity, free-running indices |
| structures | [stack](structures/stack.c) | O(1) per op | fixed capacity, any element size, caller-provided storage |
| structures | [queue](structures/queue.c) | O(1) per op | circular array, any capacity (head + count), any element size |
| structures | [union_find](structures/union_find.c) | near O(1) amortised | path halving + union by size, no recursion, caller-provided arrays |
| bits | [crc32](bits/crc32.c) | O(n) | zlib-compatible, streaming API, table and bitwise variants |
| bits | [bit_tricks](bits/bit_tricks.c) | O(1) | popcount / ctz / clz / parity / reverse / bswap / next_pow2 without compiler builtins |
| encoding | [cobs](encoding/cobs.c) | O(n) | byte stuffing so 0x00 can delimit frames; checks capacity and malformed input |
| math | [fixed_point](math/fixed_point.c) | O(1) per op | Q16.16 without floating point; saturating, round to nearest, integer square root |
| math | [gcd](math/gcd.c) | O(log n) | binary GCD (no division), overflow-checked LCM, extended Euclid, modular inverse |

Planned: linked list, hash table, heap, BST, BFS / DFS / Dijkstra, KMP.

### Rules

- One item = `name.h` + `name.c` + `name_test.c`, nothing shared between items.
- `-std=c11 -Wall -Wextra -Wpedantic -Werror -Wshadow -Wconversion`, no third-party code.
- No `malloc`. Containers take storage from the caller, report capacity and never grow.
- Generic items use the `qsort` / `bsearch` shape `(base, n, size, cmp)`.
- Failures are return values, not asserts.

### Build and test

```sh
make test
make asan
make cross      # Cortex-M4 build with arm-none-eabi-gcc, prints code size
```

CI runs gcc and clang with and without ASan/UBSan, valgrind, cppcheck and scan-build, and a
Cortex-M4 (Thumb-2) build so the "runs on a microcontroller" claim is checked on every push. All eleven items
together are about 2.5 KB of code at `-Os`. Design notes per item are in `docs/spec.md`.

### License

MIT
