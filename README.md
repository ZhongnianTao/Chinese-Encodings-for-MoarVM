# GB2312-Encoding-for-MoarVM

This repository stores all the files used in developing GB2312 encoding for MoarVM.

My blog post describing the work: [Implementing the GB2312 Encoding](https://zhongniantao.wordpress.com/2019/07/15/implementing-the-gb2312-encoding/).

Here are my PRs: [MoarVM (initial commit)](https://github.com/MoarVM/MoarVM/pull/1147), [MoarVM (conversion table compressed)](https://github.com/MoarVM/MoarVM/pull/1150), [Rakudo](https://github.com/rakudo/rakudo/pull/3079), [Roast](https://github.com/perl6/roast/pull/563). 

I'm also implementing GB18030 encoding. By far the 'decode' and 'encode' function pass the tests, while I'm still testing the 'decodestream' function.

