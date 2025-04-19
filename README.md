# GPoSTTL

## Enhanced version of Brill's Parts-of-Speech Tagger, with built-in Tokenizer and Lemmatizer

[![License](https://img.shields.io/badge/License-Historical%20Permission%20Notice%20and%20Disclaimer-green.svg)](COPYING)

## Overview

GPoSTTL is an enhanced version of Eric Brill's rule-based Parts-of-Speech Tagger, with built-in Tokenizer and Lemmatizer. 
It reads from FILE or STDIN and writes to STDOUT. It is based on LPost package by Jimmy Lin (jimmylin at umd.edu). 
LPost itself is based on Benjamin Han's ePost package, which is a cleaned-up version of Eric Brill's original code. 
The primary lemma list was taken from e_lemma.txt (Ver.1), compiled by Prof. Yasumasa Someya (someya at someya-net.com),
with permission. Later it has been and being enhanced by hundreds of additional entries.


## Key Features
GPoSTTL contains several new features compared to the original Brill Tagger and these features are:

- **Built-in Tokenizer**: GPoSTTL has built-in Tokenizer that allows one to feed raw English text directly into the tagger. This eliminates the need for pre-tokenization of English text.
- **Built-in Lemmatizer**: Brill tagger's original lexicon has been replaced by a lemmatized lexicon. This allows Lemmatizer of GPoSTTL to print lemma information for each token.
- **Handling of knknown Numerals**: Initialization method for unknown numerals has been added. It ensures unknown cardinal numbers are tagged as "CD" in the start-state-tagger sequence.
- **Verb Tagging Enhancement**: GPoSTTL distinguishes between "be" verbs (B), "have" verbs (H) and other verbs (V). The enhancement is done at last step of tagging procedure as its lexicon contains the original Penn tagset.
- **Compatibility with Penn Tagset**: The default mode of GPoSTTL uses enhanced Penn tagset to make its output compatible with the output of TreeTagger.


## Motivation

GPoSTTL is developed as a crucial component for [Anubadok](http://anubadok.sourceforge.net/) (a GPL'ed machine translator for Bengali). 
It serves as an open-source alternative to [TreeTagger](http://www.ims.uni-stuttgart.de/projekte/corplex/TreeTagger/) which was used earlier for Anubadok system.
GPoSTTL is designed to be a drop-in replacement for TreeTagger in such systems.

## Installation

It has standard autoconf-based installation:

```bash
./configure [--prefix=$HOME for non-root installations]
make
make install
```
PS: If you have downloaded the source codes from the repository then you need to first generate the "configure" script by using the following command:

```bash
autoreconf --install
```




