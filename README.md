fnv-mysql-udf
=============

FNV (Fowler/Noll/Vo) Hash MySQL User Defined Functions.

Sometimes you only need a 32- or 64-bit hash function.
One of my favorites at Yahoo! and something we used at Fraudwall Technologies/Anchor Intelligence
is the FNV (Fowler/Noll/Vo) Hash.

```
mysql> select FNV1A_64('The quick brown fox jumps over the lazy dog.');
+----------------------------------------------------------+
| FNV1A_64('The quick brown fox jumps over the lazy dog.') |
+----------------------------------------------------------+
| 75c4d4d9092c6c5a                                         |
+----------------------------------------------------------+
1 row in set (0.00 sec)

mysql> select FNV1A_32('The quick brown fox jumps over the lazy dog.');
+----------------------------------------------------------+
| FNV1A_32('The quick brown fox jumps over the lazy dog.') |
+----------------------------------------------------------+
| ecaf981a                                                 |
+----------------------------------------------------------+
1 row in set (0.00 sec)

mysql>
```

The functions behave similarly to the MySQL built-ins
[MD5()](http://dev.mysql.com/doc/refman/5.0/en/encryption-functions.html#function_md5)
and [SHA1()](http://dev.mysql.com/doc/refman/5.0/en/encryption-functions.html#function_sha1)
in the sense that they return hex strings.

The module defines 32- and 64-bit versions of all three variants of the FNV hash: FNV-0, FNV-1, and FNV-1a.

Enjoy.

Copyright
---------
Copyright (c) 2007, Fraudwall Technologies. All rights reserved.
