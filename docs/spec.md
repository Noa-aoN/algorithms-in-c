# algorithms-in-c spec

テーマごとの設計メモ。1 テーマ 1 項目、新しいものを下に足す。

[English](#english)

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

## heap_sort

- 下から半分の節を `sift_down` して堆を作り、先頭と末尾を入れ替えながら縮める。再帰なし、追加領域なし。
- quick_sort と違い最悪でも O(n log n)。入力に敵対的なケースがあり得る場所 (外部から来るデータ) 向け。
- 不安定。

## crc32

- 反射型 CRC-32 (多項式 0xEDB88320、初期値・最終 XOR とも 0xFFFFFFFF)。zlib / PNG / Ethernet と同じ。
- `crc32_update` / `crc32_finish` の逐次 API で、受信バッファを分割して処理できる。
- 256 エントリのテーブルは初回呼び出し時に生成する (ROM に置きたいときは定数配列に差し替える)。
  初期化はスレッド安全ではないので、マルチスレッド環境では起動時に一度呼んでおく。
- ビット単位版 `crc32_bitwise` はテーブル版の検算用で、ROM 1 KiB を節約したいときの代替にもなる。

## cobs

- Consistent Overhead Byte Stuffing。フレーム中の 0x00 を取り除き、0x00 をフレームの区切りに使えるようにする。
  UART のような区切りのないバイト列でフレームを切るための定番で、SLIP より手数が少なく、
  オーバーヘッドは 254 バイトにつき最大 1 バイトと一定。
- `cobs_encode` / `cobs_decode` は出力容量を受け取り、足りなければ 0 を返す。デコードは
  コード 0x00、途中に 0x00 が混じる、長さが足りない、の各不正入力も 0 で拒否する。
  受信側は信用できないデータを扱うので、ここだけは入口の検査を厚くしている。
- `COBS_ENCODE_MAX(n)` は符号化後の最大長 (n が 0 なら 1、それ以外は n + ceil(n / 254))。
  受信バッファの確保量をコンパイル時に決めるためのマクロ。
- 254 バイト目の境界 (コード 0xFF の後ろに暗黙の 0 を入れない) はテストで両側から確認している。

## fixed_point

- Q16.16 (`int32_t`、整数部 16 ビット、小数部 16 ビット)。FPU のないマイコンでセンサ値やフィルタ係数を
  扱うときの定番で、ここでは浮動小数点を一切使わずに書いています。
- 演算はすべて飽和 (wrap しない) で、丸めは最近接・同値は 0 から遠い側。DSP の作法に合わせ、
  オーバーフローで符号が反転する事故を避けます。乗算・除算は 64 ビット中間値で計算します。
- 負の値の右シフトは C では処理系定義なので、丸めは切り捨て除算 + 剰余の補正で書いています。
  定数除算なのでコンパイラがシフトに落とし、速度は変わりません。
- `fx_sqrt` は `v << 16` の整数平方根を 1 桁ずつ求める方式で、結果がそのまま Q16.16 になります。
  ループは 24 回固定、結果は床。負の入力は 0。
- 0 除算は符号に合わせて飽和値を返します。テストは既知値に加え、乗除算を 2 万組の乱数で
  「厳密な有理数との誤差が 0.5 ulp 以内」という整数だけの判定で確かめています (浮動小数点を
  オラクルにしない)。

## gcd

- `gcd_u32` は Stein の二進 GCD。シフトと減算だけで、除算器のないコア (Cortex-M0 など) でも
  ユークリッド法より速い。共通の 2 の冪を先にくくり出し、最後に戻す。
- `lcm_u32` は `a / gcd * b` を 64 ビットで計算し、32 ビットに収まらなければ `false`。
  0 を含むときは 0。
- `gcd_ext_i64` は拡張ユークリッド。`a*x + b*y = g` の係数を返し、g は常に非負に正規化する。
  負の入力でも等式が成り立つことをテストで確認している。
- `mod_inverse_u32` は拡張ユークリッドの x を法で正規化したもの。互いに素でなければ `false`。

## bit_tricks

- `__builtin_popcount` などの組み込み関数を使わない素の C。結果がコンパイラに依らないので、
  組み込み関数を使う側の検算にも、組み込み関数のないコンパイラの代替にもなる。
- popcount はビット並列 (2, 4, 8 ビット単位で加算し、最後に乗算で合計)。ctz は `v & -v` で最下位ビットを
  取り出して `popcount(lsb - 1)`。clz は上位へ塗りつぶしてから popcount。
- `bt_reverse` はマスク交換 (1, 2, 4 ビット) の後にバイト swap。`bt_next_pow2` は 32 ビットに
  収まらないとき 0 を返す (`0x80000001` 以上)。
- テストは各関数を 1 ビットずつ回す素朴な実装と乱数 10 万件で突き合わせる。

## ring_buffer

- 容量は 2 の冪に限定し、添字の折り返しを剰余ではなくマスクで行う。
- `head` / `tail` はフリーランカウンタ。`count = head - tail` は符号なしの回り込みでも正しく、
  スロットを 1 つ余らせない (1 スロット空け方式の満杯 / 空の曖昧さが生じない)。
- 生産者が `head` だけ、消費者が `tail` だけを書くので、`size_t` の store がアトミックな
  ターゲットでは単一生産者 / 単一消費者でロック不要。ただしここでは証明しておらず、
  テストは単一スレッド。

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

- Bottom-up (merge adjacent runs of width 1, 2, 4, ...), so there is no recursion.
- The scratch area (`n` elements) is passed in by the caller. The no-`malloc` rule is expressed
  in the interface rather than hidden inside.
- The merge takes from the right run only when right < left; on ties the left element goes
  first, which keeps the sort stable.

### quick_sort

- Hoare partition with median-of-three pivot selection. Sorted, reversed and all-equal inputs
  stay at O(n log n).
- Recurse on the smaller side only and loop on the larger one, which bounds the stack depth
  at O(log n).
- Not stable. Use merge_sort when stability matters.

### heap_sort

- Build the heap by `sift_down` from the lower half, then shrink it while swapping the root
  with the last element. No recursion, no extra storage.
- Unlike quick_sort the worst case is O(n log n), which matters where the input can be
  adversarial (data from outside).
- Not stable.

### crc32

- Reflected CRC-32 (polynomial 0xEDB88320, initial value and final XOR both 0xFFFFFFFF), the
  same as zlib, PNG and Ethernet.
- `crc32_update` / `crc32_finish` form a streaming API so a receive buffer can be processed in
  chunks.
- The 256-entry table is generated on first use (swap in a constant array to keep it in ROM).
  The initialisation is not thread safe, so call it once at startup in a multithreaded system.
- `crc32_bitwise` is the bitwise variant, used to cross-check the table and as a 1 KiB ROM
  saving alternative.

### cobs

- Consistent Overhead Byte Stuffing removes every 0x00 from a frame so that 0x00 can delimit
  frames. The usual choice for framing on a UART-style byte stream: fewer steps than SLIP and a
  fixed overhead of at most one byte per 254.
- `cobs_encode` / `cobs_decode` take the output capacity and return 0 when it is too small.
  Decode also returns 0 for a zero code byte, an embedded 0x00 and a truncated block. The
  receiver handles untrusted data, so this is the one place where input checks are deliberately
  thorough.
- `COBS_ENCODE_MAX(n)` is the maximum encoded length (1 for n = 0, otherwise
  n + ceil(n / 254)), for sizing receive buffers at compile time.
- The 254-byte boundary (no implicit zero after a 0xFF code) is tested from both sides.

### fixed_point

- Q16.16 in an `int32_t`. The usual format for sensor values and filter coefficients on an MCU
  without an FPU; written here with no floating point at all.
- Every operation saturates rather than wraps, and rounds to nearest with ties away from zero,
  as DSP libraries do, so an overflow cannot flip the sign. Multiply and divide use 64-bit
  intermediates.
- A right shift of a negative value is implementation-defined in C, so rounding is written as
  truncating division plus a remainder correction. The divisor is a constant, so the compiler
  still emits a shift.
- `fx_sqrt` is a digit-by-digit integer square root of `v << 16`, which lands directly in
  Q16.16. Fixed 24 iterations, floor result, 0 for negative input.
- Division by zero saturates by sign. Besides known values, the tests check 20,000 random
  multiply/divide pairs against the exact rational with an integer-only "within 0.5 ulp" test,
  so floating point is never the oracle.

### gcd

- `gcd_u32` is Stein's binary GCD: shifts and subtractions only, so it beats Euclid on a core
  without a hardware divider (Cortex-M0 and the like). Common powers of two are factored out
  first and restored at the end.
- `lcm_u32` computes `a / gcd * b` in 64 bits and returns `false` when the result does not fit
  in 32 bits; 0 if either input is 0.
- `gcd_ext_i64` is the extended Euclid. It returns coefficients with `a*x + b*y = g` and
  normalises g to be non-negative; the identity is tested with negative inputs too.
- `mod_inverse_u32` reduces the x from the extended Euclid modulo m, `false` unless coprime.

### bit_tricks

- Plain C with no compiler builtins, so results do not depend on the toolchain; usable both as a
  cross-check for the intrinsics and as a fallback where they do not exist.
- popcount is the bit-parallel form (add in 2, 4, 8-bit lanes, then a multiply to sum). ctz
  isolates the lowest bit with `v & -v` and takes `popcount(lsb - 1)`; clz smears upward and
  counts.
- `bt_reverse` swaps in 1, 2, 4-bit masks and then byte-swaps. `bt_next_pow2` returns 0 when
  the result does not fit in 32 bits (input above `0x80000000`).
- Each function is checked against a naive one-bit-at-a-time loop on 100,000 random values.

### ring_buffer


- Capacity must be a power of two so the index wrap is a mask, not a modulo.
- `head` and `tail` are free-running counters; `count = head - tail` is well defined under
  unsigned wraparound, and no slot is wasted (the full/empty ambiguity of the one-slot-free
  scheme does not arise).
- Single-producer single-consumer safe without locks on targets where `size_t` stores are atomic,
  because the producer only writes `head` and the consumer only writes `tail`. Not proven here;
  the tests are single-threaded.
