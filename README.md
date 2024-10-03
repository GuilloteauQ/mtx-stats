# `mtx-stats`

Get information about MatrixMarket files.

Informations:

- Dimesions of the matrix

- Number of non-zero elements

- Average Row Length

- Coefficient of Variation of the Row Length

## Usage

```sh
mtx-stats file1.mtx ... fileN.mtx
```

## Install

With Nix:

```sh
nix shell github:Guilloteau/mtx-stats 
```

or

```sh
nix run github:Guilloteau/mtx-stats  -- file1.mtx ... fileN.mtx
```
