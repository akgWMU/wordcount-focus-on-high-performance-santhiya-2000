**Program 1 -  wordcount.cpp**

# Word Frequency Counter with Histogram

This is a **C++ program** that reads all text files in a given folder, counts the frequency of words, and generates a histogram of the top 50 most frequent words.

## Logic
  => Reads multiple text files from a folder.
  => Cleans words (removes punctuation and converts to lowercase).
  => Ignores common **conjunctions** (like *and, the, in, on*).
  => Counts the frequency of all other words.
  => if a word is found again, its count is incremented.
  => Sorts words by frequency in descending order.
  => Prints word counts to the console.
  => Generates a histogram (`histogram.txt`) showing the top 50 words.

**Techniques Used:**
  => Word Cleaning:
  Removes punctuation by checking isalnum(c) for each character.
  Converts all characters to lowercase for uniform comparison.
  => Stopword Removal:
  Maintains a predefined list of stopwords (like and, or, the, of...).
  Uses **binary_search** on the sorted stopword list for O(log k) lookup (where k is number of stopwords).
  => Efficient Word Storage: Stores words in a vector<pair<string,int>>.
  Words are kept in sorted order at all times.
  => Binary Search for Insertion & Counting:
  => Uses lower_bound to find the word in O(log n).
  => If found, increment count.
  => If not found, insert at the correct position, keeping vector sorted.
  => Sorting for Histogram: After processing the file, a copy of the list is sorted in non-increasing order by frequency using std::sort.
  => Extracts the Top 50 words.
  => Output Writing
  Prints the full alphabetical word count to console.
  Writes the Top 50 results to a file (histogram_top50.txt) for external use (e.g., plotting graphs in Python).

**Time Complexity**
  Word Cleaning & Filtering:O(L + log K) per word, where L = average word length.
  Vector Operations:
  Insertion (with shifting): O(N) worst-case.
  Search (binary search): O(log N).
  Sorting by Count (Introsort - QS + HS)(Top 50): O(N log N).
  Access by index	O(1)
  Where:
  W = total words processed, N = unique words, K = stopwords (small constant), T = threads (if extended to parallelism)

**Space Complexity**
  Word List (vector of pairs): O(N).
  Stopword list: O(K) (negligible, constant).
  Temporary structures (sorting): O(N).
  Overall Space: O(N), where N = number of unique words.

## Executed Output (Console)
  santhiya@wireless39-140 wordcount-focus-on-high-performance-santhiya-2000 % g++ -std=c++17 wordcount.cpp -o wordcount
  santhiya@wireless39-140 wordcount-focus-on-high-performance-santhiya-2000 % ./wordcount                              
  Word Counts:
  solution : 62104
  strategy : 61729
  classification : 31543
  revenue : 31491
  database : 31490
  management : 31480
  assessment : 31480
  system : 31463
  ....so on.
  Histogram saved to histogram.txt
  santhiya@wireless39-140 wordcount-focus-on-high-performance-santhiya-2000 % time ./wordcount  
  ** ./wordcount  7.25s user 0.08s system 95% cpu 7.681 total**

## Requirements
  => C++17 or higher.

## Compilation & Usage
  ```bash
  g++ -std=c++17 wordcount.cpp -o wordcount
  ./wordcount
  time ./wordcount
  ```
**Trade-offs**
  Advantages of vector + binary search:
  => Cache-friendly, better memory locality than map.
  => Deterministic ordering without extra sort.
  => Simpler implementation, fewer pointers and allocations.
  Limitations:
  => Insertions are costly (O(N) worst case due to shifting).
  => Poor scalability when almost every word is unique (e.g., huge vocabulary).
  Alternatives:
  std::map (AVL/Red-Black Tree): Guaranteed O(log N) insertion/search. Higher memory usage due to tree nodes.
  std::unordered_map (Hash Table): Expected O(1) insertion/search except collision cases. Requires extra sorting step → O(N log N). Higher memory overhead.
  *For medium-sized datasets with many duplicate words, vector + binary search performs very well due to cache efficiency.
  *For very large datasets with high vocabulary diversity, unordered_map is typically faster.

**Program 2 - wc_buffer.cpp**

# Word Frequency Counter with Buffer and Multithreading added.

This is a **C++ program** that reads all text files in a given folder using multiple threads, counts the frequency of words, and generates a histogram of the top 50 most frequent words.
Unlike the single-threaded version, this program uses multithreading to improve performance on large datasets.

## Logic
  => Reads all text files from a given folder using std::filesystem.
  => Cleans each word (removes punctuation, keeps only alphabets, converts to lowercase).
  => Ignores common stopwords/conjunctions (like and, the, in, on).
  => Uses multiple threads to process different batches of files concurrently.
  => Each thread maintains its own local word-frequency map.
  => After all threads finish, local maps are merged into a global frequency map.
  => Words are sorted in descending order of frequency.
  => Prints word counts to the console.
  => Generates a histogram (histogram_buf.txt) showing the top 50 words.

## Techniques Used

=> Word Cleaning:
Removes non-alphabetic characters by checking isalpha(c) for each character.
Converts all characters to lowercase for uniform comparison.
=> Stopword Removal:
Maintains a predefined list of stopwords (like and, or, the, of...).
Uses find() on a small vector of stopwords for O(k) lookup (k ≈ 30, negligible cost).
=> Threaded File Processing:
Splits the list of files into nearly equal chunks.
Each thread processes its own chunk and maintains a local frequency map.
This avoids race conditions and reduces locking overhead.
=> Local Counting with Maps:
Each thread uses a std::map<string,int> for counting words.
Provides O(log n) insertion/search per word (n = unique words in thread’s subset).
=> Merging Results:
After all threads finish, local maps are merged into a global map.
For each unique word, counts from all threads are accumulated.
=> Sorting for Histogram:
A copy of the global map is stored in vector<pair<string,int>>.
Sorted in non-increasing order of frequency using std::sort (O(n log n)).
Extracts the Top 50 words for histogram.
=> Output Writing:
Prints the complete frequency list to console (alphabetical order by default from map).
Writes the Top 50 histogram with bars (*) to histogram_buf.txt for visualization.

**Time Complexity**
  Word Cleaning & Filtering: O(L + K) per word.
  Map Operations:
  Insertion: O(log N) (N = unique words).
  Search/Update: O(log N).
  Sorting by Count: O(N log N).
  Threading Impact: Work is split across T threads.
  Sequential: O(M · W log N)
  Parallel: O((M · W log N) / T)
  Where:
  M = number of files, W = average words per file, N = unique words, T = number of threads

**Space Complexity**
  Space Complexity
  std::map: O(N) (unique words stored).
  Thread-local maps: O(T · N) in the worst case (if all words appear in every thread’s subset).
  Sorted vector copy: O(N).
  Overall: O(N + T·N) ≈ O(T·N)

## Executed Output (Console)
  santhiya@wireless39-140 wordcount-focus-on-high-performance-santhiya-2000 % g++ -std=c++17 wc_buffer.cpp -o wc_buffer
  santhiya@wireless39-140 wordcount-focus-on-high-performance-santhiya-2000 % ./wc_buffer 
  solution : 62104
  strategy : 61729
  classification : 31543
  revenue : 31491
  database : 31490
  management : 31480
  assessment : 31480
  ....so on.
  Histogram saved to histogram_buf.txt
  santhiya@wireless39-140 wordcount-focus-on-high-performance-santhiya-2000 % time ./wc_buffer    
  **./wc_buffer  15.08s user 0.15s system 696% cpu 2.186 total**

## Requirements
  => C++17 or higher.

## Compilation & Usage
  ```bash
  g++ -std=c++17 wc_buffer.cpp -o wc_buffer
  ./wc_buffer
  time ./wc_buffer
  ```
## Trade-offs
  Advantages of Multithreading with Local Maps:
  => Exploits multiple CPU cores, reducing runtime significantly on large datasets.
  => Thread-local maps remove the need for heavy synchronization (mutex), avoiding contention.
  => Parallel I/O and parsing can overlap, giving better throughput.
  => Good scalability for workloads with many files.
  Limitations:
  => Extra memory usage: Each thread maintains its own local map → O(T·N) worst case.
  => Thread management overhead: Context switching, thread creation, and joining cost time.
  => Diminishing returns: For small datasets or when I/O is bottlenecked, threading may not speed up execution.
  => Merge overhead: After threads finish, results must be merged into a global map (O(T·N log N)).
  Alternatives:
  => std::map (Balanced Tree: Red-Black/AVL)
  Pros: Guaranteed O(log N) insertion/search.
  Cons: Higher memory usage (pointers, tree nodes), less cache-friendly.
  => std::unordered_map (Hash Table)
  Pros: Expected O(1) insertion/search (amortized).
  Cons: Requires extra sorting step for final output → O(N log N).
  Cons: Higher memory overhead due to hash buckets.
  => std::vector + Binary Search
  Pros: Cache-friendly, better memory locality.
  Pros: Deterministic ordering without extra sort.
  Cons: Insertions are costly (O(N) worst case due to shifting).
  Cons: Performs poorly if vocabulary size is huge (many unique words).
  This gives a balanced trade-off analysis of data structures + multithreading approach, so readers can see why map was chosen here, and what alternatives exist.
