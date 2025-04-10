{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  name = "llvm-dev-shell";

  buildInputs = [
    pkgs.clang-tools_17
    pkgs.llvm_17
    pkgs.lld_17
    pkgs.clang_17
    pkgs.cmake
    pkgs.ninja
  ];

  shellHook = ''
    export CC=clang
    export CXX=clang++
    export LLVM_DIR=${pkgs.llvm_17}/lib/cmake/llvm
    export LLD_DIR=${pkgs.lld_17}/lib/cmake/lld
  '';
}
