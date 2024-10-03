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

`mtx-stats` uses OpenMP to parallelize file processing.

You can set the environment variable `OMP_NUM_THREADS` to control the number of threads to use:

```sh
OMP_NUM_THREADS=8 mtx-stats file1.mtx ... fileN.mtx
```

## Install

With Nix:

```sh
nix shell github:GuilloteauQ/mtx-stats 
```

or

```sh
nix run github:GuilloteauQ/mtx-stats  -- file1.mtx ... fileN.mtx
```

