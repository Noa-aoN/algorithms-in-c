# algorithms-in-c spec

テーマごとの設計メモ。1 テーマ 1 項目、新しいものを下に足す。

## 共通方針

- `qsort` 風の汎用インターフェース (`void *base, size_t n, size_t size, cmp`)。マクロなしで任意の
  要素型に使える。比較ごとに関数ポインタ呼び出しが入るが、参照実装としては許容。
- 比較関数は探索が `(key, elem)`、整列が `(a, b)`。`bsearch` / `qsort` と揃える。
- コンテナは確保しない。`init` はストレージと容量を受け取り、不正な組み合わせでは assert ではなく
  `false` を返す。

## insertion_sort

- 一時要素を持たず隣接要素をバイト単位で swap する。バッファ不要で要素サイズの上限もない。
  厳密に大きい隣接要素しか動かさないので安定。

## binary_search

- `lower_bound` / `upper_bound` が基本で、`find` は `lower_bound` + 等値判定 1 回。
  重複時に最左を返すので `find` の結果が決定的。
- `mid = lo + (hi - lo) / 2` で `(lo + hi) / 2` のオーバーフローを避ける。

## merge_sort

- ボトムアップ (幅 1, 2, 4, ... で隣接ランをマージ) なので再帰なし。
- 作業領域 `scratch` (要素 n 個分) は呼び出し側が渡す。`malloc` 禁止の規則をインターフェースで表現している。
- マージで右側を取るのは「右 < 左」のときだけ。等しければ左を先に出すので安定。

## quick_sort

- Hoare 分割 + 中央値 3 点でピボット選択。昇順・降順・全要素同値の入力でも O(n log n) に収まる。
- 小さい側だけ再帰し、大きい側はループで続ける。スタック深さは O(log n) に抑えられる。
- 不安定。安定が必要なら merge_sort。

## crc32

- 反射型 CRC-32 (多項式 0xEDB88320、初期値・最終 XOR とも 0xFFFFFFFF)。zlib / PNG / Ethernet と同じ。
- `crc32_update` / `crc32_finish` の逐次 API で、受信バッファを分割して処理できる。
- 256 エントリのテーブルは初回呼び出し時に生成する (ROM に置きたいときは定数配列に差し替える)。
  初期化はスレッド安全ではないので、マルチスレッド環境では起動時に一度呼んでおく。
- ビット単位版 `crc32_bitwise` はテーブル版の検算用で、ROM 1 KiB を節約したいときの代替にもなる。

## ring_buffer

- 容量は 2 の冪に限定し、添字の折り返しを剰余ではなくマスクで行う。
- `head` / `tail` はフリーランカウンタ。`count = head - tail` は符号なしの回り込みでも正しく、
  スロットを 1 つ余らせない (1 スロット空け方式の満杯 / 空の曖昧さが生じない)。
- 生産者が `head` だけ、消費者が `tail` だけを書くので、`size_t` の store がアトミックな
  ターゲットでは単一生産者 / 単一消費者でロック不要。ただしここでは証明しておらず、
  テストは単一スレッド。

[English](#english)

---

## English

Per-item design notes. One short entry per item, newest at the bottom.


### Conventions

- `qsort`-style generic interface (`void *base, size_t n, size_t size, cmp`) so an item works on
  any element type without macros. The cost is a function-pointer call per comparison, which is
  acceptable for a reference implementation.
- Comparators take `(key, elem)` for searching and `(a, b)` for sorting, matching `bsearch` and
  `qsort`.
- Containers never allocate. `init` takes a storage pointer and capacity and returns `false` on an
  invalid combination instead of asserting.

### insertion_sort

- Adjacent byte-wise swaps instead of a temporary element, so no buffer and no element-size limit.
  Stable because only strictly-greater neighbours are moved.

### binary_search

- `lower_bound` and `upper_bound` are the primitives; `find` is `lower_bound` plus one equality
  check. Returning the leftmost match makes `find` deterministic on duplicates.
- `mid = lo + (hi - lo) / 2` avoids the overflow in `(lo + hi) / 2`.

### merge_sort

- ボトムアップ (幅 1, 2, 4, ... で隣接ランをマージ) なので再帰なし。
- 作業領域 `scratch` (要素 n 個分) は呼び出し側が渡す。`malloc` 禁止の規則をインターフェースで表現している。
- マージで右側を取るのは「右 < 左」のときだけ。等しければ左を先に出すので安定。

## quick_sort

- Hoare 分割 + 中央値 3 点でピボット選択。昇順・降順・全要素同値の入力でも O(n log n) に収まる。
- 小さい側だけ再帰し、大きい側はループで続ける。スタック深さは O(log n) に抑えられる。
- 不安定。安定が必要なら merge_sort。

## crc32

- 反射型 CRC-32 (多項式 0xEDB88320、初期値・最終 XOR とも 0xFFFFFFFF)。zlib / PNG / Ethernet と同じ。
- `crc32_update` / `crc32_finish` の逐次 API で、受信バッファを分割して処理できる。
- 256 エントリのテーブルは初回呼び出し時に生成する (ROM に置きたいときは定数配列に差し替える)。
  初期化はスレッド安全ではないので、マルチスレッド環境では起動時に一度呼んでおく。
- ビット単位版 `crc32_bitwise` はテーブル版の検算用で、ROM 1 KiB を節約したいときの代替にもなる。

## ring_buffer

- Capacity must be a power of two so the index wrap is a mask, not a modulo.
- `head` and `tail` are free-running counters; `count = head - tail` is well defined under
  unsigned wraparound, and no slot is wasted (the full/empty ambiguity of the one-slot-free
  scheme does not arise).
- Single-producer single-consumer safe without locks on targets where `size_t` stores are atomic,
  because the producer only writes `head` and the consumer only writes `tail`. Not proven here;
  the tests are single-threaded.
