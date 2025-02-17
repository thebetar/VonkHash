# Introduction

I have always been interested in how hashing algorithms work. For this puprose I have created my own hashing tool.

This hashing tool uses a very simple approach by taking each character as input and one by one using each character to create an XOR operation and rotating the result after, this creates quite a random string. This hashing algorithm can be mostly used for creating a checksum.

## Results

This hashing algorithm seems great for checksums, due to it's simplicity it is very fast (the `tests()` ran in less than 0.01s). For security it would not be advisiable however since it is reversible if the source code is obtained.
