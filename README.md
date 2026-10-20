# algorithms-in-c

[![ci](https://github.com/Noa-aoN/algorithms-in-c/actions/workflows/ci.yml/badge.svg)](https://github.com/Noa-aoN/algorithms-in-c/actions/workflows/ci.yml)

Classic algorithms and data structures, one per file, written the way embedded C is written:
C11, no heap allocation, caller-provided storage, and a unit test next to every implementation.

[日本語](#日本語)

## Index

| Area | Item | Complexity | Notes | State |
|---|---|---|---|---|
| sorting | [insertion_sort](sorting/insertion_sort.c) | O(n^2) time, O(1) space | generic, stable, in place | done |
| searching | [binary_search](searching/binary_search.c) | O(log n) | `lower_bound` / `upper_bound` / `find` | done |
| structures | [ring_buffer](structures/ring_buffer.c) | O(1) per op | power-of-two capacity, free-running indices | done |
| sorting | merge / quick / heap sort | | | planned |
| structures | linked list, stack, queue, hash table, heap, BST, union-find | | | planned |
| graph | BFS, DFS, Dijkstra | | | planned |
| strings | KMP | | | planned |
| bits | bit tricks, CRC-32 | | | planned |
| math | fixed-point, gcd | | | planned |

## Rules

- Each item is `name.h` + `name.c` + `name_test.c` in its area directory. No shared runtime.
- `-std=c11 -Wall -Wextra -Wpedantic -Werror -Wshadow -Wconversion`. No third-party code.
- No `malloc`. Containers take storage from the caller and report capacity, never grow.
- Tests use one header, `tests/minitest.h`. CI runs gcc and clang with and without
  ASan/UBSan, plus valgrind.

## Build and test

```sh
make test       # build and run every *_test.c
make asan       # same under AddressSanitizer + UBSan
make clean
```

## License

MIT

---

## 日本語

定番のアルゴリズムとデータ構造を 1 テーマ 1 ファイルで、組み込み C の流儀で書いたもの。
C11、ヒープ確保なし、ストレージは呼び出し側が渡す、実装の隣に必ず単体テスト。

### ルール

- 1 テーマ = `name.h` + `name.c` + `name_test.c`。共通ランタイムを持たない。
- `-std=c11 -Wall -Wextra -Wpedantic -Werror -Wshadow -Wconversion`。サードパーティコードなし。
- `malloc` を使わない。コンテナは呼び出し側からストレージを受け取り、容量を報告し、伸長しない。
- テストはヘッダ 1 つ (`tests/minitest.h`)。CI は gcc / clang × (通常 / ASan+UBSan) と valgrind。

### 進め方

上の Index 表を 1 テーマずつ埋めていく。各テーマの設計判断は `docs/spec.md` に残す。
