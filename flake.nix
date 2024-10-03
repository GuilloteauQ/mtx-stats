{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/23.11";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
  flake-utils.lib.eachDefaultSystem (system:
    let
      pkgs = import nixpkgs { inherit system; };
    in
    {
      packages = rec {
        default = mtx-stats;
        mtx-stats = pkgs.stdenv.mkDerivation rec {
          pname = "mtx-stats";
          version = "0.1";
          src = ./src;
          buildInputs = [
            pkgs.gcc
          ];
          buildPhase = ''
            gcc mtx-stats.c -lm -o ${pname} -Wall -O3 -fopenmp
          '';
          installPhase = ''
            mkdir -p $out/bin
            cp ${pname} $out/bin
          '';
        };
      };
      devShells = {
        default = pkgs.mkShell {
          packages = with pkgs; [
            gcc

          ];
        };
      };

    });
}
