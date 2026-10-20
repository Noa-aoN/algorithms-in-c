# algorithms-in-c

[![ci](https://github.com/Noa-aoN/algorithms-in-c/actions/workflows/ci.yml/badge.svg)](https://github.com/Noa-aoN/algorithms-in-c/actions/workflows/ci.yml)

定番のアルゴリズムとデータ構造を、組み込みで C を書くときの流儀で 1 テーマずつ実装したものです。
ヒープ確保をせず、ストレージは呼び出し側が渡し、実装の隣に必ず単体テストを置いています。

[English](#english)

## 何のためのリポジトリか

アルゴリズムの実装集は世の中に山ほどあるので、ここでは「どう書くか」の方を揃えています。
マイコンに持っていってもそのまま動く書き方、つまり `malloc` をしない、再帰の深さを抑える、
バッファの大きさを呼び出し側が決める、という前提で全部書いています。
兄弟リポジトリの [reversi-c](https://github.com/Noa-aoN/reversi-c) と同じ流儀です。

## 一覧

| 分野 | 項目 | 計算量 | 設計上のポイント |
|---|---|---|---|
| sorting | [insertion_sort](sorting/insertion_sort.c) | O(n^2) | 汎用、安定、一時領域なし |
| sorting | [merge_sort](sorting/merge_sort.c) | O(n log n)、作業領域 O(n) | ボトムアップで再帰なし、作業領域は呼び出し側が渡す |
| sorting | [quick_sort](sorting/quick_sort.c) | 平均 O(n log n) | Hoare 分割、中央値 3 点、小さい側だけ再帰してスタック O(log n) |
| sorting | [heap_sort](sorting/heap_sort.c) | O(n log n)、追加領域なし | 最悪でも O(n log n)、再帰なし |
| searching | [binary_search](searching/binary_search.c) | O(log n) | `lower_bound` / `upper_bound` / `find`、重複時は最左 |
| structures | [ring_buffer](structures/ring_buffer.c) | 1 操作 O(1) | 容量は 2 の冪、フリーランカウンタで満杯と空を区別 |
| bits | [crc32](bits/crc32.c) | O(n) | zlib 互換、逐次更新 API、テーブル版とビット版 |
| encoding | [cobs](encoding/cobs.c) | O(n) | 0x00 を区切りに使うためのバイトスタッフィング。容量と不正入力を検査 |

予定: linked list、stack / queue、hash table、heap、BST、union-find、BFS / DFS / Dijkstra、KMP、
bit tricks、fixed-point、gcd。

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
make clean
```

CI は gcc / clang それぞれで通常ビルドと ASan/UBSan ビルドを回し、valgrind でも全テストを通します。
テストはヘッダ 1 つの自作フレームワーク (`tests/minitest.h`) です。

各テーマの設計判断は [docs/spec.md](docs/spec.md) に残しています。

## ライセンス

MIT

---

## English

Classic algorithms and data structures, one per file, written the way embedded C is written:
no heap allocation, caller-provided storage, and a unit test next to every implementation.
Same conventions as the sibling [reversi-c](https://github.com/Noa-aoN/reversi-c).

### Index

| Area | Item | Complexity | Design note |
|---|---|---|---|
| sorting | [insertion_sort](sorting/insertion_sort.c) | O(n^2) | generic, stable, no scratch |
| sorting | [merge_sort](sorting/merge_sort.c) | O(n log n), O(n) scratch | bottom-up, no recursion, caller-provided scratch |
| sorting | [quick_sort](sorting/quick_sort.c) | O(n log n) average | Hoare partition, median of three, recurse on smaller side only |
| sorting | [heap_sort](sorting/heap_sort.c) | O(n log n), in place | worst case O(n log n), no recursion |
| searching | [binary_search](searching/binary_search.c) | O(log n) | `lower_bound` / `upper_bound` / `find`, leftmost on duplicates |
| structures | [ring_buffer](structures/ring_buffer.c) | O(1) per op | power-of-two capacity, free-running indices |
| bits | [crc32](bits/crc32.c) | O(n) | zlib-compatible, streaming API, table and bitwise variants |
| encoding | [cobs](encoding/cobs.c) | O(n) | byte stuffing so 0x00 can delimit frames; checks capacity and malformed input |

Planned: linked list, stack / queue, hash table, heap, BST, union-find, BFS / DFS / Dijkstra, KMP,
bit tricks, fixed-point, gcd.

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
```

CI runs gcc and clang with and without ASan/UBSan, plus valgrind. Design notes per item are in
`docs/spec.md`.

### License

MIT
